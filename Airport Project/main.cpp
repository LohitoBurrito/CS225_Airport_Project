#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <json/json.h>
#include "Airports.h"

using namespace std;

int main() {  
    double currLong_;
    double currLat_;
    double destLong_;
    double destLat_;
    double baggageAmount;

    cout << "Enter your current latitude: ";
    cin >> currLat_;
    cout << "Enter your current longitude: ";
    cin >> currLong_;
    cout << "Enter your destination latitude: ";
    cin >> destLat_;
    cout << "Enter your destination longitude: ";
    cin >> destLong_;
    cout << "How much bags are you travelling with: ";
    cin >> baggageAmount;

    Airports system = Airports(currLat_, currLong_, destLat_, destLong_, baggageAmount);

    //Check if there is a closed airport
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
    cout << system.calcDistance(41.97859955, -87.90480042, 25.273056, 51.608056) << "\n"; //CHICAGO -> DOHA
    //Calc Cost test cases
    cout << system.calcCost(41.97859955, -87.90480042, 25.273056, 51.608056, "large_airport") << "\n"; //CHICAGO -> DOHA
    return 0;    
}
