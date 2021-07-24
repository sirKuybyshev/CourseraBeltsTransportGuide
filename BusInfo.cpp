//
// Created by timofey on 7/23/21.
//

#include "BusInfo.h"

#include <algorithm>

using namespace std;

void BusInfo::AddStop(string &&stop) {
    busStops_.push_back(stop);
}

double BusInfo::GetDistance(const unordered_map<string, Stop> &stops) {
    if (routeLength_ == 0.0) {
        routeLength_ = CalculateDistance(stops);
    }
    return routeLength_;
}

double BusInfo::CalculateDistance(const unordered_map<string, Stop> &stops) {
    double result = 0.0;
    for (auto stop = busStops_.begin(); stop < --busStops_.end(); stop++) {
        result += CalculateSphereDistance(stops.at(*stop).coords, stops.at(*next(stop)).coords);
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
