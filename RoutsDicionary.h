//
// Created by timofey on 7/23/21.
//

#ifndef MAIN_CPP_ROUTSDICIONARY_H
#define MAIN_CPP_ROUTSDICIONARY_H

#include "BusInfo.h"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class RoutsDictionary {
public:
    void ReadQueries(std::istream &is = std::cin);

    std::ostream &ProcessRequests(std::ostream &os = std::cout, std::istream &is = std::cin);

private:
    std::unordered_map<std::string, Stop> stops_;
    std::unordered_map<int, BusInfo> buses_;

    void AddStop(const std::string& name, double latitude, double longitude);

    void ReadStop(std::istream &is);

    void ReadBus(std::istream &is);

    std::string RequestBus(int number);
    ;
};

#endif//MAIN_CPP_ROUTSDICIONARY_H
