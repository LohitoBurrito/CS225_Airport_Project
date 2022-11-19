#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <json/json.h>
#include "Airports.h"

using namespace std;

int main() {
    Airports system = Airports();
    
    double currLong_;
    double currLat_;

    cout << "Enter your current latitude: ";
    cin >> currLat_;
    cout << "Enter your current longitude: ";
    cin >> currLong_;



    for (int i = 0; i < system.smallAirports.size(); i++) {
        if (system.smallAirports[i]->type == "closed") {
            std::cout << "Bruh" << " small" << "\n";
            return 0;
        }
    }
    std::cout << "Checked Small Airports"<< system.smallAirports.size() << "\n";
    for (int i = 0; i < system.medAirports.size(); i++) {
        if (system.medAirports[i]->type == "closed") {
            std::cout << "Bruh" << " mediums" << "\n";
            return 0;
        }
    }
    std::cout << "Checked Medium Airports" << system.medAirports.size() << "\n";
    for (int i = 0; i < system.largeAirports.size(); i++) {
        if (system.largeAirports[i]->type == "closed") {
            std::cout << "Bruh" << " large" << "\n";
            return 0;
        }
    }
    std::cout << "Checked Large Airports" << system.largeAirports.size()<< "\n";

    //Haversine test cases

    //Calc Cost test cases



    return 0;    
}