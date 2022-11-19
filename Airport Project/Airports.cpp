#pragma once
#include "Airports.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <json/json.h>
#include <math.h>

//Constructor and Destructor
Airports::Airports() {
    parseData();
    createGraph();
    currLat = 0.0;
    currLong = 0.0;
}
Airports::Airports(double lat_, double long_) {
    parseData();
    createGraph();
    currLat = lat_;
    currLong = long_;
}
Airports::~Airports() { destroyGraph(); }


//Parse and Creating Graph
void Airports::parseData() {
    ifstream file("Airports.json"); //JSON data
    Json::Value jsonData; //contains JSON Data
    Json::Reader reader; //The reader

    reader.parse(file, jsonData); //the reader will parse the Json data and place it into JSON data

    /*
    //We can print out the JSON data
        cout << "total Json Data: " << jsonData << "\n";

    //Or we can print out individual components of the JSON data
        cout << "type: " << jsonData[0]["type"] << "\n";
        cout << "name: " << jsonData[0]["name"] << "\n";
        cout << "latitude_deg: " << jsonData[0]["latitude_deg"] << "\n";
        cout << "longitude_deg: " << jsonData[0]["longitude_deg"] << "\n";
    */

    for (int i = 0; i < jsonData.size(); i++) {
        //Place each airport in the specified type private vector
        //Note if the airport is "closed," it will be discarded
        if (jsonData[i]["type"].asString() != "closed") {
            Airport* airport = new Airport();
            airport->type = jsonData[i]["type"].asString();
            airport->name = jsonData[i]["name"].asString();
            airport->latitude = jsonData[i]["latitude_deg"].asDouble();
            airport->longitude = jsonData[i]["latitude_deg"].asDouble();

            if (jsonData[i]["type"].asString() == "small_airport") {
                smallAirports.push_back(airport);
            }
            else if (jsonData[i]["type"].asString() == "medium_airport") {
                medAirports.push_back(airport);
            }
            else if (jsonData[i]["type"].asString() == "large_airport") {
                largeAirports.push_back(airport);
            }
        }
    }
}
void Airports::createGraph() {
    //3 departure assumptions:
    //Small airports -> closest medium airport
    //medium airports -> all airports
    //large airports -> medium and large airports
    
    //Step 1: small -> med, med -> small
    for (int i = 0; i < smallAirports.size(); i++) {
        Airport* nearestAirport;
        double minCost = 100000000000;
        double cost;
        for (int j = 0; j < medAirports.size(); j++) {
            double lat1 = smallAirports[i]->latitude;
            double lat2 = medAirports[j]->latitude;
            double long1 = smallAirports[i]->longitude;
            double long2 = medAirports[j]->longitude;
            cost = calcCost(lat1, long1, lat2, long2);
            if (minCost < cost) {
                minCost = cost;
                nearestAirport = medAirports[j];
            }
            medAirports[j]->connections.insert({ smallAirports[i], calcCost(lat2, long2, lat1, long1) });
        }
        smallAirports[i]->connections.insert({ nearestAirport, cost });
    }
    std::cout << "finished Small Airport" << "\n";
    //Step 2: med -> large, large-> med, med -> med
    for (int i = 0; i < medAirports.size(); i++) {
        double lat1 = medAirports[i]->latitude;
        double long1 = medAirports[i]->longitude;

        for (int k = 0; k < largeAirports.size(); k++) {
            double lat2 = largeAirports[k]->latitude;
            double long2 = largeAirports[k]->longitude;
            medAirports[i]->connections.insert({ largeAirports[k], calcCost(lat1, long1, lat2, long2) });
            largeAirports[k]->connections.insert({ medAirports[i], calcCost(lat2, long2, lat1, long1) });
        }
        for (int k = 0; k < medAirports.size(); k++) {
            if (medAirports[i] != medAirports[k]) {
                double lat2 = medAirports[k]->latitude;
                double long2 = medAirports[k]->longitude;
                medAirports[i]->connections.insert({ medAirports[k], calcCost(lat1, long1, lat2, long2) });
            }
        }
    }
    std::cout << "finished Medium airports" << "\n";
    //Step 3: large -> large
    for (int i = 0; i < largeAirports.size(); i++) {
        double lat1 = largeAirports[i]->latitude;
        double long1 = largeAirports[i]->longitude;

        for (int k = 0; k < largeAirports.size(); k++) {
            double lat2 = largeAirports[k]->latitude;
            double long2 = largeAirports[k]->longitude;
            largeAirports[i]->connections.insert({ medAirports[k], calcCost(lat1, long1, lat2, long2) });
        }
    }
    std::cout << "finished Large airports" << "\n";

}
void Airports::destroyGraph() {
    for (int i = 0; i < smallAirports.size(); i++) {
        delete smallAirports[i];
    }
    std::cout << "Destroyed Small airports" << "\n";

    for (int i = 0; i < medAirports.size(); i++) {
        delete medAirports[i];
    }

    std::cout << "Destroyed Medium airports" << "\n";

    for (int i = 0; i < largeAirports.size(); i++) {
        delete largeAirports[i];
    }

    std::cout << "Destroyed Large airports" << "\n";
}
//Algorithm
vector<Airports::Airport*> Airports::BFS(Airport* startPoint) {
	return vector<Airports::Airport*>();
}
vector<Airports::Airport*> Airports::Kosaraju(int num, Airport* startPoint) {
	return vector<Airports::Airport*>();
}
vector<Airports::Airport*> Airports::Djistrka(map<Airport*, double>, Airport*) {
	return vector<Airports::Airport*>();
}

//Helpers
double Airports::calcDistance(double lat1, double long1, double lat2, double long2) {
    //We are going to compute distances using Haversine Formula
    double deltaLatRad = (lat2 - lat1) * ((3.14159265358979323846) / 180);
    double deltaLongRad = (long2 - long1) * ((3.14159265358979323846) / 180);

    double lat1Rad = lat1 * ((3.14159265358979323846) / 180);
    double lat2Rad = lat2 * ((3.14159265358979323846) / 180);

    double a = pow(sin(deltaLatRad / 2), 2) + cos(lat1Rad) * cos(lat2Rad) * pow(deltaLongRad / 2, 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = 6371 * 2;

    //d returns the distance in kilometers
	return d;
    //Source: https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
}
double Airports::calcCost(double lat1, double long1, double lat2, double long2) {
    //We are going to make assumptions on the cost of travelling
    double distance = calcDistance(lat1, long1, lat2, long2);

    // Costs are determined by:
    // 1) Base fare
    // 2) Taxes and airport fees
    // 3) Fuel surcharge
    // 4) Service fee to issue
    // 5) Food
    // 6) Seat selection
    // 7) Baggage
    // Source: https://flightfox.com/tradecraft/how-do-airlines-set-prices


        
	return 0.0;
}