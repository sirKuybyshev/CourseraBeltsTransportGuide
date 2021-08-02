//
// Created by timofey on 7/31/21.
//

#ifndef MAIN_CPP_EDGEINFO_H
#define MAIN_CPP_EDGEINFO_H

#include <string>

struct EdgeInfo {
    std::string type;
    std::string name;
    double time = 0;
    long distanceInStops = 0;
};


#endif//MAIN_CPP_EDGEINFO_H
