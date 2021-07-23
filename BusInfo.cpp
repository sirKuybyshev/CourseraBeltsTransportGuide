//
// Created by timofey on 7/23/21.
//

#include "BusInfo.h"

using namespace std;

void BusInfo::AddStop(string_view stop) {
    busStops_.push_back(stop);
    if (!uniqueBusStops_.count(stop)) {
        uniqueBusStops_.insert(stop);
        stopCount_++;
    }
}

double BusInfo::GetDistance(const std::unordered_map<std::string_view, Stop> &stops) {
    if (routeLength_ == 0.0) {
        routeLength_ = CalculateDistance(stops);
    }
    return routeLength_;
}

double BusInfo::CalculateDistance(const unordered_map<string_view, Stop> &stops) {
    double result = 0.0;
    for (auto stop = busStops_.begin(); stop < --busStops_.end(); stop++) {
        result += CalculateSphereDistance(stops.at(*stop).coords, stops.at(*next(stop)).coords);
    }
    return result;
}

void BusInfo::Loop() {
    busStops_.insert(busStops_.end(), ++busStops_.rbegin(), busStops_.rend());
}
