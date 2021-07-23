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
    std::unordered_map<std::string_view, Stop> stops_;
    std::unordered_map<int, BusInfo> buses_;
    std::unordered_set<std::string> stopsVault;

    void AddStop(std::string&& name, double latitude, double longitude);

    void ReadStop(std::istream &is);

    void ReadBus(std::istream &is);

    void ProcessBusRequest(int number, std::ostream& os);
};

#endif//MAIN_CPP_ROUTSDICIONARY_H
