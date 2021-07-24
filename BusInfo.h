//
// Created by timofey on 7/23/21.
//

#ifndef MAIN_CPP_BUSINFO_H
#define MAIN_CPP_BUSINFO_H

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Stop.h"

class BusInfo {
public:
    explicit BusInfo(std::string number) : number_(std::move(number)) {
        busStops_.reserve(100);
    }

    size_t NumberOfStops() {
        return busStops_.size();
    }

    size_t NumberOfUniqueStops();

    double GetDistance(const std::unordered_map<std::string, Stop> &stops);

    void AddStop(std::string &&stop);

    void Loop();

private:
    std::string number_;
    int stopCount_ = 0;
    std::vector<std::string> busStops_;
    double routeLength_ = 0.0;

    double CalculateDistance(const std::unordered_map<std::string, Stop> &stops);
};

#endif//MAIN_CPP_BUSINFO_H
