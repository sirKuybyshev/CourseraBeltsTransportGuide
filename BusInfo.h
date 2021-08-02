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
    explicit BusInfo(std::string number) : name_(std::move(number)) {
        busStops_.reserve(100);
    }

    size_t NumberOfStops() const {
        return busStops_.size();
    }

    size_t NumberOfUniqueStops();

    [[nodiscard]] std::string GetName() const{
        return name_;
    }

    double GetDistance(const std::unordered_map<std::string, Stop> &stops);

    [[nodiscard]] double GetCurvature() const {
        return routeLength_ / minimalLength_;
    }

    [[nodiscard]] const std::vector<std::string>& GetStops() const {
        return busStops_;
    }

    void AddStop(const std::string &stop);

    void Loop();

private:
    std::string name_;
    size_t stopCount_ = 0;
    std::vector<std::string> busStops_;
    double routeLength_ = 0.0;
    double minimalLength_ = 0.0;

    double CalculateMinimalDistance(const std::unordered_map<std::string, Stop> &stops);

    double CalculateTrueDistance(const std::unordered_map<std::string, Stop> &stops);
};

#endif//MAIN_CPP_BUSINFO_H
