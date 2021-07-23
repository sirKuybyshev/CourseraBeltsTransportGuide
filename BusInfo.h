//
// Created by timofey on 7/23/21.
//

#ifndef MAIN_CPP_BUSINFO_H
#define MAIN_CPP_BUSINFO_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Stop.h"

class BusInfo {
public:
    explicit BusInfo(int number) : number_(number) {}

    size_t NumberOfStops() {
        return busStops_.size();
    }

    size_t NumberOfUniqueStops() {
        return uniqueBusStops_.size();
    }

    double GetDistance(const std::unordered_map<std::string_view, Stop> &stops);

    void AddStop(std::string_view stop);

    void Loop();

private:
    int number_;
    int stopCount_ = 0;
    std::vector<std::string_view> busStops_;
    std::unordered_set<std::string_view> uniqueBusStops_;
    double routeLength_ = 0.0;

    double CalculateDistance(const std::unordered_map<std::string_view, Stop> &stops);
};

#endif//MAIN_CPP_BUSINFO_H
