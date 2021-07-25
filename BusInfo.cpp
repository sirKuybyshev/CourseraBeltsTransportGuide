//
// Created by timofey on 7/23/21.
//

#include "BusInfo.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>


using namespace std;

void BusInfo::AddStop(string &&stop) {
    busStops_.push_back(stop);
}

double BusInfo::GetDistance(const unordered_map<string, Stop> &stops) {
    if (minimalLength_ == 0.0) {
        minimalLength_ = CalculateMinimalDistance(stops);
    }
    if (routeLength_ == 0.0) {
        routeLength_ = CalculateTrueDistance(stops);
    }
    return routeLength_;
}

double BusInfo::CalculateMinimalDistance(const unordered_map<string, Stop> &stops) {
    double result = 0.0;
    for (auto stop = busStops_.begin(); stop < --busStops_.end(); stop++) {
        result += CalculateSphereDistance(stops.at(*stop).GetCoords(), stops.at(*next(stop)).GetCoords());
    }
    return result;
}

void BusInfo::Loop() {
    if (!stopCount_) {
        unordered_set<string> unique;
        unique.reserve(busStops_.size());
        for (const auto &item : busStops_) {
            unique.insert(item);
        }
        stopCount_ = unique.size();
    }
    busStops_.insert(busStops_.end(), ++busStops_.rbegin(), busStops_.rend());
}
size_t BusInfo::NumberOfUniqueStops() {
    if (!stopCount_) {
        unordered_set<string> unique;
        unique.reserve(busStops_.size());
        for (const auto &item : busStops_) {
            unique.insert(item);
        }
        stopCount_ = unique.size();
    }
    return stopCount_;
}
double BusInfo::CalculateTrueDistance(const unordered_map<std::string, Stop> &stops) {
    double result = 0.0;
    for (auto stop = busStops_.begin(); stop != --busStops_.end(); stop++) {
        if (stops.at(*stop).GetDistances().count(*next(stop))) {
            result += stops.at(*stop).GetDistances()[*next(stop)];
        } else if (stops.at(*next(stop)).GetDistances().count(*stop)) {
            result += stops.at(*next(stop)).GetDistances()[*stop];
        } else {
            cerr << *stop << ' ' << *next(stop);
            throw invalid_argument("No trip between");
        }
    }
    return result;
}
