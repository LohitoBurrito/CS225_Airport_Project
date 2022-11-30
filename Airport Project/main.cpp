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

    //Closed Airport Check
    for (int i = 0; i < system.smallAirports.size(); i++) {
        if (system.smallAirports[i]->type == "closed") {
            std::cout << "Bruh" << " small" << "\n";
            return 0;
        }
    }
    std::cout << "Checked " << system.smallAirports.size() << " Small Airports" << "\n";
    for (int i = 0; i < system.medAirports.size(); i++) {
        if (system.medAirports[i]->type == "closed") {
            std::cout << "Bruh" << " mediums" << "\n";
            return 0;
        }
    }
    std::cout << "Checked "<<system.medAirports.size()<<" Medium Airports" << "\n";
    for (int i = 0; i < system.largeAirports.size(); i++) {
        if (system.largeAirports[i]->type == "closed") {
            std::cout << "Bruh" << " large" << "\n";
            return 0;
        }
    }
    std::cout << "Checked " << system.largeAirports.size() << " Large Airports" << "\n";

    //Chicago -> Doha
    try {
        double dist = system.calcDistance(41.97859955, -87.90480042, 25.273056, 51.608056);
        if (abs(dist - 11479) > 50) {
            throw (dist);
        }
        else {
            cout << "CHICAGO -> DOHA Distance is valid\n";
        }
    }
    catch(int num) {
        cout << "CHICAGO -> DOHA distance in km:\nCalculated Distance: "<<num<<"km\nEstimated Distance: 11479km\n";
        cout << "Haversine Distance too far out\n";
    }
    try {
        double cost = system.calcCost(41.97859955, -87.90480042, 25.273056, 51.608056, "large_airport");
        double expediaCost = 2778.38 / 2;
        if (abs(cost - expediaCost) > 400) {
            throw(cost);
        }
        else {
            cout << "CHICAGO -> DOHA Cost is valid\n";
        }
    }
    catch (int num) {
        cout << "CHICAGO -> DOHA cost ($):\nCalculated Cost: $" << num << "\nEstimated Cost: $"<<2778.38/2<<"\n";
        cout << "Calculated cost is a little too far off\n";
    }

    //Chicago -> Frankfurt
    try {
        double dist = system.calcDistance(41.97859955, -87.90480042, 50.026402, 8.54313);
        if (abs(dist - 6989) > 50) {
            throw (dist);
        }
        else {
            cout << "CHICAGO -> FRANKFURT Distance is valid\n";
        }
    }
    catch (int num) {
        cout << "CHICAGO -> FRANKFURT distance in km:\nCalculated Distance: " << num << "km\nEstimated Distance: 6989km\n";
        cout << "Haversine Distance too far out\n";
    }
    try {
        double cost = system.calcCost(41.97859955, -87.90480042, 50.026402, 8.54313, "large_airport");
        double expediaCost = 2276 / 2;
        if (abs(cost - expediaCost) > 400) {
            throw(cost);
        }
        else {
            cout << "CHICAGO -> FRANKFURT Cost is valid\n";
        }
    }
    catch (int num) {
        cout << "CHICAGO -> FRANKFURT cost ($):\nCalculated Cost: $" << num << "\nEstimated Cost: $" << 2276 / 2 << "\n";
        cout << "Calculated cost is a little too far off\n";
    }

    return 0;    
}
