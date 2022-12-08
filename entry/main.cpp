//
// Created by lohit on 12/5/2022.
//
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
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

    Airports system = Airports(currLat_, currLong_, destLat_, destLong_, (int) baggageAmount, "Airports.json");

    system.BFS(system.medAirports[0]);
    if (system.getDeparture() != system.getDestination()) {
        system.Djistrka();
    } else {
        system.Kosaraju(system.getDeparture());
    }
    cout << "\nPath: ";
    for (size_t i = 0; i < system.getSolution().size(); i++) {
        if (i == system.getSolution().size() - 1) {
            cout << system.getSolution()[i] -> name <<"\n\n";
        } else {
            cout << system.getSolution()[i] -> name << " -> ";
        }
    }
    return 0;
}