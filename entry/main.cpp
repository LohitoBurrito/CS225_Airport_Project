//
// Created by lohit on 12/5/2022.
//
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "Airports.h"

using namespace std;

void TestClosedAirports(Airports& system) {
    //Closed Airport Check
    try {
        for (int i = 0; i < (int) system.smallAirports.size(); i++) {
            if (system.smallAirports[i]->type == "closed" || system.smallAirports[i]->type == "heliport" || system.smallAirports[i]->type == "seaplane_base") {
                std::cout << "Bruh" << " small" << "\n";
                throw(0);
            }
        }
        std::cout << "Checked " << system.smallAirports.size() << " Small Airports" << "\n";
        for (int i = 0; i < (int) system.medAirports.size(); i++) {
            if (system.medAirports[i]->type == "closed" || system.medAirports[i]->type == "heliport" || system.medAirports[i]->type == "seaplane_base") {
                std::cout << "Bruh" << " mediums" << "\n";
                throw(1);
            }
        }
        std::cout << "Checked " << system.medAirports.size() << " Medium Airports" << "\n";
        for (int i = 0; i < (int) system.largeAirports.size(); i++) {
            if (system.largeAirports[i]->type == "closed" || system.largeAirports[i]->type == "heliport" || system.largeAirports[i]->type == "seaplane_base") {
                std::cout << "Bruh" << " large" << "\n";
                throw(2);
            }
        }
        std::cout << "Checked " << system.largeAirports.size() << " Large Airports" << "\n";
    }
    catch (int num) {
        cout << "You have an unwanted airport in: " << ((num == 0) ? "Small Airports" : ((num == 1) ? "Medium Airports" : "Large Airports")) << "\n";
    }
}
void TestCalcAlgos(Airports& system) {
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
    catch (int num) {
        cout << "CHICAGO -> DOHA distance in km:\nCalculated Distance: " << num << "km\nEstimated Distance: 11479km\n";
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
        cout << "CHICAGO -> DOHA cost ($):\nCalculated Cost: $" << num << "\nEstimated Cost: $" << 2778.38 / 2 << "\n";
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
        double expediaCost = 2276.0 / 2;
        if (abs(cost - expediaCost) > 400) {
            throw(cost);
        }
        else {
            cout << "CHICAGO -> FRANKFURT Cost is valid\n";
        }
    }
    catch (double num) {
        cout << "CHICAGO -> FRANKFURT cost ($):\nCalculated Cost: $" << num << "\nEstimated Cost: $" << 2276 / 2 << "\n";
        cout << "Calculated cost is a little too far off\n";
    }
}
void TestBFSAndDikstras(Airports& system) {
    try {
        //CHICAGO -> DOHA
        double cLat = system.getCurrLat();
        double cLong = system.getCurrLong();
        double dLat = system.getDestLat();
        double dLong = system.getDestLong();
        system.setCurrLat(41.97859955);
        system.setCurrLong(-87.90480042);
        system.setDestLat(25.273056);
        system.setDestLong(51.608056);
        system.BFS(system.medAirports[0]);
        if (system.getDestination()->name != "Hamad International Airport") {
            throw(1);
        }
        if (system.getDeparture()->name != "Chicago O'Hare International Airport") {
            throw(0);
        }
        cout << "FIRST BFS WORKS!" << "\n";
        system.Djistrka();
        system.setCurrLat(cLat);
        system.setCurrLong(cLong);
        system.setDestLat(dLat);
        system.setDestLong(dLong);
    }
    catch (int num) {
        if (num == 1) {
            cout << "Wrong destination" << "\n";
        }
        else {
            cout << "Wrong departure" << "\n";
        }
    }
    try {
        //AURORA -> DHAKLA
        double cLat = system.getCurrLat();
        double cLong = system.getCurrLong();
        double dLat = system.getDestLat();
        double dLong = system.getDestLong();
        system.setCurrLat(41.77190018);
        system.setCurrLong(-88.47570038);
        system.setDestLat(23.71829987);
        system.setDestLong(-15.93200016);
        system.BFS(system.medAirports[0]);

        if (system.getDestination()->name != "Dakhla Airport") {
            throw(1);
        }
        if (system.getDeparture()->name != "Aurora Municipal Airport") {
            throw(0);
        }
        cout << "SECOND BFS WORKS!" << "\n";
        system.Djistrka();

        system.setCurrLat(cLat);
        system.setCurrLong(cLong);
        system.setDestLat(dLat);
        system.setDestLong(dLong);
    }
    catch (int num) {
        if (num == 1) {
            cout << "Wrong destination" << "\n";
        }
        else {
            cout << "Wrong departure" << "\n";
        }
    }

}
void TestBFSAndKosuraju(Airports& system) {
    try {

    }
    catch (int num) {

    }
}

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

    TestClosedAirports(system);
    TestCalcAlgos(system);
    TestBFSAndDikstras(system);
    //TestBFSAndKosuraju(system);

    return 0;
}