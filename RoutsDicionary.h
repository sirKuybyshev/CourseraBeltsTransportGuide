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
    std::unordered_map<std::string, BusInfo> buses_;

    void ReadStop(std::istream &is);

    void ReadBus(std::istream &is);

    void ProcessBusRequest(const std::string &number, std::ostream &os);

    void ProcessStopRequest(const std::string &stop, std::ostream &os);
};

#endif//MAIN_CPP_ROUTSDICIONARY_H
