//
// Created by timofey on 7/23/21.
//

#include <iomanip>

#include "RoutsDicionary.h"
using namespace std;

string RoutsDictionary::RequestBus(int number) {
    std::stringstream result;
    if (buses_.find(number) != buses_.end()) {
        auto &bus = buses_.at(number);
        result << "Bus " << number << ": " << bus.NumberOfStops() << " stops on route, ";
        result << bus.NumberOfUniqueStops() << " unique stops, ";
        result << setprecision(6) << bus.GetDistance(stops_) << " route length" << std::endl;
    } else {
        result << "Bus " << number << ": not found" << endl;
    }
    return result.str();
}
void RoutsDictionary::ReadBus(istream &is) {
    int busNumber;
    char delim;
    is >> busNumber >> delim;
    buses_.emplace(busNumber, busNumber);
    BusInfo &bus = buses_.at(busNumber);
    std::string stop;
    while (is.peek() != '\n') {
        stop = "";
        string word;
        while (is.peek() == ' ') {
            is.get();
        }
        while (isalpha(is.peek())) {
            is >> word;
            if (!stop.empty()) {
                stop += ' ';
            }
            stop += word;
            while (is.peek() == ' ') {
                is.get();
            }
        }
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
    char dummy;
    while (is.peek() == ' ') {
        is.get();
    }
    getline(is, name, ':');
    is >> latitude >> dummy >> longitude;
    AddStop(move(name), latitude, longitude);
}

void RoutsDictionary::AddStop(const string& name, double latitude, double longitude) {
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
