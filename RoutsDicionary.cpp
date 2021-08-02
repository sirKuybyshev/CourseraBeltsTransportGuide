#include <iomanip>

#include "Json.h"
#include "RoutsDicionary.h"

using namespace std;

Graph::EdgeId Stop::lastId_ = 0;

istream &RoutsDictionary::Process(istream &is, ostream &os) {
    auto input = Json::Load(is).GetRoot().AsMap();
    SetSettings(input.at("routing_settings"));
    ReadQueries(input.at("base_requests").AsArray());
    BuildGraph();
    ProcessRequests(input.at("stat_requests").AsArray(), os);
    return is;
}

void RoutsDictionary::SetSettings(const Json::Node &info) {
    settings_ = {info.AsMap().at("bus_wait_time").AsDouble(),
                 info.AsMap().at("bus_velocity").AsDouble()};
}

void RoutsDictionary::ReadQueries(const vector<Json::Node> &queries) {
    for (auto &query : queries) {
        if (query.AsMap().at("type").AsString() == "Stop") {
            ReadStop(query);
        } else if (query.AsMap().at("type").AsString() == "Bus") {
            ReadBus(query);
        } else {
            throw std::invalid_argument("Unknown query");
        }
    }
}

void RoutsDictionary::BuildGraph() {
    Graph::DirectedWeightedGraph<double> graph(Stop::lastId_ * 2 + 1);
    for (const auto &bus : buses_) {
        for (auto stop = bus.second.GetStops().begin(); stop != bus.second.GetStops().end(); stop++) {
            double distance = 0.0;
            for (auto otherStop = next(stop); otherStop != bus.second.GetStops().end(); otherStop++) {
                distance += stops_.at(*prev(otherStop)).GetDistances()[*otherStop];
                EdgeInfo edge = {"Bus", bus.second.GetName(),
                                 distance / 1000.0 / settings_.BUSES_VELOCITY * 60, otherStop - stop};
                auto id = graph.AddEdge({stops_.at(*stop).id_ + Stop::lastId_,
                                         stops_.at(*otherStop).id_,
                                         edge.time});
                edgesInfo_.insert({id, edge});
            }
        }
    }
    for (const auto &stop : stops_) {
        {
            EdgeInfo edge{"Stop", stop.first, 0};
            auto id = graph.AddEdge({stop.second.id_ + Stop::lastId_,
                                     stop.second.id_, edge.time});
            edgesInfo_.emplace(id, edge);
        }
        {
            EdgeInfo edge = {"Stop", stop.first, settings_.STOP_WAITING_TIME};
            auto id = graph.AddEdge({stop.second.id_,
                                     stop.second.id_ + Stop::lastId_, edge.time});
            edgesInfo_.emplace(id, edge);
        }
    }
    router_ = Graph::Router<double>(move(graph));
}

std::ostream &RoutsDictionary::ProcessRequests(const std::vector<Json::Node> &requests, ostream &os) {
    vector<Json::Node> responses;
    for (const auto &request : requests) {
        const auto &rMap = request.AsMap();
        if (rMap.at("type").AsString() == "Bus") {
            responses.push_back(ProcessBusRequest(rMap.at("name").AsString(), rMap.at("id").AsInt()));
        } else if (rMap.at("type").AsString() == "Stop") {
            responses.push_back(ProcessStopRequest(rMap.at("name").AsString(), rMap.at("id").AsInt()));
        } else if (rMap.at("type").AsString() == "Route") {
            responses.push_back(ProcessTypeRequest(rMap.at("from").AsString(),
                                                   rMap.at("to").AsString(), rMap.at("id").AsInt()));
        } else {
            throw invalid_argument("Unknown Request");
        }
    }
    os << Json::Document(move(responses)) << '\n';
    return os;
}
void RoutsDictionary::ReadBus(const Json::Node &query) {
    const auto &qMap = query.AsMap();
    const auto &busName = qMap.at("name").AsString();
    buses_.emplace(busName, busName);
    BusInfo &bus = buses_.at(busName);
    for (const auto &stop : qMap.at("stops").AsArray()) {
        stops_[stop.AsString()].AddBus(busName);
        bus.AddStop(stop.AsString());
    }
    if (!qMap.at("is_roundtrip").AsBool()) {
        bus.Loop();
    }
}


void RoutsDictionary::ReadStop(const Json::Node &query) {
    const auto &qMap = query.AsMap();
    stops_[qMap.at("name").AsString()].SetCoords(qMap.at("latitude").AsDouble(), qMap.at("longitude").AsDouble());
    stops_[qMap.at("name").AsString()].id_ = ++Stop::lastId_;
    const auto &reachableStops = qMap.at("road_distances").AsMap();
    for (const auto &stop : reachableStops) {
        stops_[qMap.at("name").AsString()].AddVertex(stop.first, stop.second.AsInt());
    }
}

Json::Node RoutsDictionary::ProcessBusRequest(const string &number, int id) {
    map<string, Json::Node> response;
    if (buses_.count(number)) {
        auto &bus = buses_.at(number);
        response.emplace("request_id", id);
        response.emplace("route_length", bus.GetDistance(stops_));
        response.emplace("curvature", bus.GetCurvature());
        response.emplace("stop_count", static_cast<int>(bus.NumberOfStops()));
        response.emplace("unique_stop_count", static_cast<int>(bus.NumberOfUniqueStops()));
    } else {
        response.emplace("request_id", id);
        response.emplace("error_message", "not found");
    }
    return response;
}

Json::Node RoutsDictionary::ProcessStopRequest(const string &stop, int id) {
    map<string, Json::Node> response;
    if (stops_.count(stop)) {
        const auto &stopInfo = stops_.at(stop);
        vector<Json::Node> busesOutput;
        if (!stopInfo.GetBuses().empty()) {
            const auto &buses = stopInfo.GetBuses();
            for (const auto &item : buses) {
                busesOutput.emplace_back(item);
            }
        }
        response.emplace("buses", busesOutput);
        response.emplace("request_id", id);
    } else {
        response.emplace("request_id", id);
        response.emplace("error_message", "not found");
    }
    return response;
}

Json::Node RoutsDictionary::ProcessTypeRequest(const string &from, const string &to, int id) {
    auto route = router_.BuildRoute(stops_.at(from).id_, stops_.at(to).id_);
    map<string, Json::Node> response;
    if (route) {
        response.emplace("request_id", id);
        response.emplace("total_time", route->weight);
        vector<Json::Node> items;
        items.reserve(route->edge_count);
        for (int i = 0; i < route->edge_count; i++) {
            items.push_back(PrintRouteElement(edgesInfo_[router_.GetRouteEdge(route->id, i)]));
        }
        response.emplace("items", items);
    } else {
        response.emplace("request_id", id);
        response.emplace("error_message", "not found");
    }
    return response;
}

Json::Node RoutsDictionary::PrintRouteElement(const EdgeInfo &info) {
    map<string, Json::Node> response;
    if (info.type == "Stop") {
        if (info.time != 0.0) {
            response.emplace("type", "Wait");
            response.emplace("stop_name", info.name);
            response.emplace("time", info.time);
        }
    } else if (info.type == "Bus") {
        response.emplace("type", "Bus");
        response.emplace("bus", info.name);
        response.emplace("time", info.time);
        response.emplace("span_count", static_cast<int>(info.distanceInStops));
    }
    return response;
}
