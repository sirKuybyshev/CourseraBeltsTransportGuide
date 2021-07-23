//
// Created by timofey on 7/23/21.
//

#include <iomanip>

#include "RoutsDicionary.h"
using namespace std;

void RoutsDictionary::ProcessBusRequest(int number, ostream& os) {
    if (buses_.find(number) != buses_.end()) {
        auto &bus = buses_.at(number);
        os << "Bus " << number << ": " << bus.NumberOfStops() << " stops on route, ";
        os << bus.NumberOfUniqueStops() << " unique stops, ";
        os << setprecision(6) << bus.GetDistance(stops_) << " route length\n";
    } else {
        os << "Bus " << number << ": not found\n";
    }
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
        while (isalpha(is.peek()) || isdigit(is.peek())) {
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
        string_view sw = *(stopsVault.insert(move(stop)).first);
        bus.AddStop(sw);
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

void RoutsDictionary::AddStop(string&& name, double latitude, double longitude) {
    string_view sw = *(stopsVault.insert(name).first);
    stops_[sw] = {sw, {latitude, longitude}};
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
            ProcessBusRequest(busNumber, os);
        } else {
            throw invalid_argument("Unknown Request");
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
