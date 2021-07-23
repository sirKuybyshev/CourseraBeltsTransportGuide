//
// Created by timofey on 7/23/21.
//

#ifndef MAIN_CPP_STOP_H
#define MAIN_CPP_STOP_H

#include <cmath>
#include <string>

struct Point {
    constexpr Point(double lat, double lon) : latitude(lat / 360 * 2 * M_PI), longitude(lon / 360 * 2 * M_PI){};

private:
    double latitude;
    double longitude;

    friend double CalculateSphereDistance(Point a, Point b);
};

struct Stop {
    std::string_view name;
    Point coords = {0, 0};
};

bool operator==(const Stop &lhs, const Stop &rhs) {
    return lhs.name == rhs.name;
}

namespace std {
    template<>
    struct [[maybe_unused]] hash<Stop> {
        size_t operator()(const Stop &x) const {
            return hash<string_view>()(x.name);
        }
    };
}// namespace std



#endif//MAIN_CPP_STOP_H
