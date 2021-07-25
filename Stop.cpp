//
// Created by timofey on 7/23/21.
//

#include "Stop.h"

double CalculateSphereDistance(Point a, Point b) {
    static const int EARTH_RADIUS = 6'371'000;
    double cos_d = sin(a.latitude) * sin(b.latitude) +
                   cos(a.latitude) * cos(b.latitude) *
                           cos(a.longitude - b.longitude);//TrigonometryMagic
    return acos(cos_d) * EARTH_RADIUS;
}
void Stop::AddVertex(const std::string& stop, int distance) {
    distances_[stop] = distance;
}
