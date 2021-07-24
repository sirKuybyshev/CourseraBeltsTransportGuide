//
// Created by timofey on 7/23/21.
//

#ifndef MAIN_CPP_STOP_H
#define MAIN_CPP_STOP_H

#include <cmath>
#include <string>
#include <set>

struct Point {
    constexpr Point(double lat, double lon) : latitude(lat / 360 * 2 * M_PI), longitude(lon / 360 * 2 * M_PI){};
    [[nodiscard]] double GetSum() const {
        return latitude + longitude;
    }

private:
    double latitude;
    double longitude;

    friend double CalculateSphereDistance(Point a, Point b);
};

class Stop {
    Point coords = {0, 0};
    std::set<std::string> buses;

public:
    void SetCoords(double lat, double lon) {
        coords = {lat, lon};
    }
    void AddBus(const std::string& busName) {
        buses.insert(busName);
    }
    [[nodiscard]] std::set<std::string> GetBuses() const {
        return buses;
    }
    [[nodiscard]] Point GetCoords() const {
        return coords;
    }
};

namespace std {
    template<>
    struct [[maybe_unused]] hash<Point> {
        size_t operator()(const Point &x) const {
            return hash<double>()(x.GetSum());
        }
    };
    template<>
    struct [[maybe_unused]] hash<Stop> {
        size_t operator()(const Stop &x) const {
            return hash<Point>()(x.GetCoords());
        }
    };
}// namespace std


#endif//MAIN_CPP_STOP_H
