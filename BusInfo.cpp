//
// Created by timofey on 7/23/21.
//

#include "BusInfo.h"

void BusInfo::AddStop(std::string &&stop) {
    busStops_.push_back(stop);
    if (!uniqueBusStops_.count(busStops_.back())) {
        uniqueBusStops_.insert(busStops_.back());
        stopCount_++;
    }
}

double BusInfo::GetDistance(const std::unordered_map<std::string, Stop> &stops) {
    if (routeLength_ == 0.0) {
        routeLength_ = CalculateDistance(stops);
    }
    return routeLength_;
}

double BusInfo::CalculateDistance(const std::unordered_map<std::string, Stop> &stops) {
    double result = 0.0;
    auto stop = busStops_.begin();
    while (stop != --busStops_.end()) {
        result += CalculateSphereDistance(stops.at(*stop).coords, stops.at(*stop++).coords);
    }
    return result;
}

void BusInfo::Loop() {
    for (auto item = --busStops_.rbegin(); item != busStops_.rend(); item++) {
        busStops_.push_back(*item);
    }
}
