//
// Created by timofey on 7/23/21.
//

#include "RoutsDicionary.h"
using namespace std;

string RoutsDictionary::RequestBus(int number) {
    std::stringstream result;
    if (buses_.find(number) != buses_.end()) {
        auto &bus = buses_.at(number);
        result << "Bus " << number << ": "
               << bus.GetDistance(stops_) << ' '
               << bus.NumberOfStops() << ' ' << bus.NumberOfUniqueStops() << std::endl;
    } else {
        result << "Bus " << number << " not found";
    }
    return result.str();
}
void RoutsDictionary::ReadBus(istream &is) {
    int busNumber;
    is >> busNumber;
    buses_.emplace(busNumber, busNumber);
    BusInfo &bus = buses_.at(busNumber);
    std::string stop;
    char delim;
    while (is.peek() != '\n') {
        is >> stop;//todo check last stop
        if (is.peek() != '\n') {
            is >> delim;
        }
        bus.AddStop(move(stop));
    }
    if (delim == '-') {
        bus.Loop();
    }
}
void RoutsDictionary::ReadStop(istream &is) {
    std::string name;
    double latitude, longitude;
    getline(is, name, ':');
    is >> latitude >> longitude;
    name.pop_back();
    AddStop(move(name), latitude, longitude);
}
void RoutsDictionary::AddStop(string &&name, double latitude, double longitude) {
    stops_[name] = {name, {latitude, longitude}};
}
std::ostream &RoutsDictionary::ProcessRequests(ostream &os, istream &is) {
    int count;
    is >> count;
    for (int i = 0; i < count; ++i) {
        std::string request;
        is >> request;
        if (request == "Bus") {
            int busNumber;
            is >> busNumber;
            os << RequestBus(busNumber);
        }
    }
    return os;
}
void RoutsDictionary::ReadQueries(istream &is) {
    int count;
    is >> count;
    std::string query;
    for (int i = 0; i < count; ++i) {
        is >> query;
        if (query == "Stop") {
            ReadStop(is);
        } else if (query == "Bus") {
            ReadBus(is);
        } else {
            throw std::invalid_argument("Unknown query");
        }
    }
}
