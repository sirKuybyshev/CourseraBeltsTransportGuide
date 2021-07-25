//
// Created by timofey on 7/23/21.
//

#include <iomanip>

#include "RoutsDicionary.h"

using namespace std;

void RoutsDictionary::ReadQueries(istream &is) {
    int count;
    is >> count;
    std::string query;
    for (int i = 0; i < count; ++i) {
        is >> query;
        if (query == "Stop") {
            while (is.peek() == ' ') {
                is.get();
            }
            ReadStop(is);
        } else if (query == "Bus") {
            while (is.peek() == ' ') {
                is.get();
            }
            ReadBus(is);
        } else {
            throw std::invalid_argument("Unknown query");
        }
    }
}

std::ostream &RoutsDictionary::ProcessRequests(ostream &os, istream &is) {
    int count;
    is >> count;
    for (int i = 0; i < count; ++i) {
        std::string request;
        is >> request;
        if (request == "Bus") {
            string busNumber;
            while (is.peek() == ' ') {
                is.get();
            }
            getline(is, busNumber, '\n');
            ProcessBusRequest(busNumber, os);
        } else if (request == "Stop") {
            string stop;
            while (is.peek() == ' ') {
                is.get();
            }
            getline(is, stop, '\n');
            ProcessStopRequest(stop, os);
        } else {
            throw invalid_argument("Unknown Request");
        }
    }
    return os;
}

void RoutsDictionary::ReadBus(istream &is) {
    string busNumber;
    getline(is, busNumber, ':');
    buses_.emplace(busNumber, busNumber);
    BusInfo &bus = buses_.at(busNumber);
    std::string stop;
    char delim;
    while (is.peek() != '\n') {
        stop = "";
        while (is.peek() == ' ') {
            is.get();
        };
        while (is.peek() != '>' && is.peek() != '-' && is.peek() != '\n') {
            stop += static_cast<char>(is.get());
        }
        if (is.peek() != '\n') {
            stop.pop_back();
            is >> delim;
        }
        stops_[stop].AddBus(bus.GetName());
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
    getline(is, name, ':');
    is >> latitude >> dummy >> longitude;
    stops_[name].SetCoords(latitude, longitude);
    if (is.peek() != '\n') {
        is.get();
    }
    Stop &stop = stops_.at(name);
    while (is.peek() != '\n') {
        string dum;
        int distance;
        string otherStop;
        is >> distance;
        is >> dummy >> dum;
        while (is.peek() == ' ') {
            is.get();
        }
        while (is.peek() != ',' && is.peek() != '\n') {
            otherStop += static_cast<char>(is.get());
        }
        if (is.peek() != '\n') {
            is.get();
        }
        stop.AddVertex(otherStop, distance);
    }
}


void RoutsDictionary::ProcessBusRequest(const string &number, ostream &os) {
    if (buses_.find(number) != buses_.end()) {
        auto &bus = buses_.at(number);
        os << "Bus " << number << ": " << bus.NumberOfStops() << " stops on route, ";
        os << bus.NumberOfUniqueStops() << " unique stops, ";
        os << setprecision(7) << bus.GetDistance(stops_) << " route length, ";
        os << setprecision(7) << bus.GetCurvature() << " curvature\n";
    } else {
        os << "Bus " << number << ": not found\n";
    }
}

void RoutsDictionary::ProcessStopRequest(const string &stop, ostream &os) {
    if (stops_.find(stop) != stops_.end()) {
        const auto &stopInfo = stops_.at(stop);
        if (!stopInfo.GetBuses().empty()) {
            os << "Stop " << stop << ": buses";
            for (const auto &item : stopInfo.GetBuses()) {
                os << ' ' << item;
            }
            os << '\n';
        } else {
            os << "Stop " << stop << ": no buses\n";
        }
    } else {
        os << "Stop " << stop << ": not found\n";
    }
}
