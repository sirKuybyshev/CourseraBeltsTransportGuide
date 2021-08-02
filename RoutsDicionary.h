#ifndef MAIN_CPP_ROUTSDICIONARY_H
#define MAIN_CPP_ROUTSDICIONARY_H

#include "BusInfo.h"
#include "router.h"
#include "Json.h"
#include "EdgeInfo.h"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class RoutsDictionary {
public:
    std::istream &Process(std::istream &is = std::cin, std::ostream &os = std::cout);

    void ReadQueries(const std::vector<Json::Node> &queries);

    std::ostream &ProcessRequests(const std::vector<Json::Node> &, std::ostream &os = std::cout);

private:
    struct Settings {
        double STOP_WAITING_TIME;
        double BUSES_VELOCITY;
    };
    Settings settings_;
    std::unordered_map<std::string, Stop> stops_;
    std::unordered_map<std::string, BusInfo> buses_;
    Graph::Router<double> router_;
    std::unordered_map<Graph::EdgeId, EdgeInfo> edgesInfo_;

    void SetSettings(const std::map<std::string, Json::Node> &info);

    void ReadStop(const Json::Node &query);

    void ReadBus(const Json::Node &query);

    void BuildGraph();

    Json::Node ProcessBusRequest(const std::string &number, int id);

    Json::Node ProcessStopRequest(const std::string &stop, int id);

    Json::Node ProcessTypeRequest(const std::string &from, const std::string &to, int id);

    static Json::Node PrintRouteElement(const EdgeInfo &info);
};

#endif//MAIN_CPP_ROUTSDICIONARY_H
