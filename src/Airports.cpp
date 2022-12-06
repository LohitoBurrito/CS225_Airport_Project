#include "Airports.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <queue>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;
// Constructor and Destructor
Airports::Airports() {
    parseData();
    currLat = 0.0;
    currLong = 0.0;
    destLat = 0.0;
    destLong = 0.0;
    baggage = 0;
    createGraph();
}
Airports::Airports(double lat_, double long_, double lat2_, double long2_, int baggageAmount) {
    parseData();
    cout << "data parsed" << "\n";
    currLat = lat_;
    currLong = long_;
    destLat = lat2_;
    destLong = long2_;
    baggage = baggageAmount;
    createGraph();
}
Airports::~Airports() { destroyGraph(); }


//Parse and Creating Graph
void Airports::parseData() {
    ifstream file("../tests/data/Airports.json"); // JSON data
    Json::Value jsonData; // contains JSON Data
    file >> jsonData;

    /*
    // We can print out the JSON data
        cout << "total Json Data: " << jsonData << "\n";
    // Or we can print out individual components of the JSON data
        cout << "type: " << jsonData[0]["type"] << "\n";
        cout << "name: " << jsonData[0]["name"] << "\n";
        cout << "latitude_deg: " << jsonData[0]["latitude_deg"] << "\n";
        cout << "longitude_deg: " << jsonData[0]["longitude_deg"] << "\n";
    */

    cout << jsonData.size() << "\n";
    for (int i = 0; i < (int) jsonData.size(); i++) {
        //Place each airport in the specified type private vector
        //Note if the airport is "closed," it will be discarded
        if (jsonData[i]["type"].asString() != "closed") {
            Airport* airport = new Airport();
            airport->type = jsonData[i]["type"].asString();
            airport->name = jsonData[i]["name"].asString();
            airport->latitude = jsonData[i]["latitude_deg"].asDouble();
            airport->longitude = jsonData[i]["longitude_deg"].asDouble();

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
    // 3 departure assumptions:
    // Small airports -> closest medium airport
    // medium airports -> all airports
    // large airports -> medium and large airports

    // Step 1: small -> closest med, med -> small
    for (int i = 0; i < (int) smallAirports.size(); i++) {
        Airport* nearestAirport;
        double minCost = 100000000000;
        double cost;
        for (int j = 0; j < (int) medAirports.size(); j++) {
            double lat1 = smallAirports[i]->latitude;
            double lat2 = medAirports[j]->latitude;
            double long1 = smallAirports[i]->longitude;
            double long2 = medAirports[j]->longitude;
            cost = calcCost(lat1, long1, lat2, long2, "small_airport");
            if (minCost > cost) {
                minCost = cost;
                nearestAirport = medAirports[j];
            }
            medAirports[j]->connections.insert({ smallAirports[i], calcCost(lat2, long2, lat1, long1, "medium_airport") });
        }
        smallAirports[i]->connections.insert({ nearestAirport, minCost });
    }
    std::cout << "finished Small Airport" << "\n";
    // Step 2: med -> large, large-> med, med -> med
    for (int i = 0; i < (int) medAirports.size(); i++) {
        double lat1 = medAirports[i]->latitude;
        double long1 = medAirports[i]->longitude;

        for (int k = 0; k < (int) largeAirports.size(); k++) {
            double lat2 = largeAirports[k]->latitude;
            double long2 = largeAirports[k]->longitude;
            medAirports[i]->connections.insert({ largeAirports[k], calcCost(lat1, long1, lat2, long2, "medium_airport") });
            largeAirports[k]->connections.insert({ medAirports[i], calcCost(lat2, long2, lat1, long1, "large_airport") });
        }
        for (int k = 0; k < (int) medAirports.size(); k++) {
            if (medAirports[i] != medAirports[k]) {
                double lat2 = medAirports[k]->latitude;
                double long2 = medAirports[k]->longitude;
                medAirports[i]->connections.insert({ medAirports[k], calcCost(lat1, long1, lat2, long2, "medium_airport") });
            }
        }
    }
    std::cout << "finished Medium airports" << "\n";
    // Step 3: large -> large
    for (int i = 0; i < (int) largeAirports.size(); i++) {
        double lat1 = largeAirports[i]->latitude;
        double long1 = largeAirports[i]->longitude;

        for (int k = 0; k < (int) largeAirports.size(); k++) {
            double lat2 = largeAirports[k]->latitude;
            double long2 = largeAirports[k]->longitude;
            largeAirports[i]->connections.insert({ largeAirports[k], calcCost(lat1, long1, lat2, long2, "large_airport") });
        }
    }
    std::cout << "finished Large airports" << "\n";
}
void Airports::destroyGraph() {
    for (int i = 0; i < (int) smallAirports.size(); i++) {
        delete smallAirports[i];
    }
    std::cout << "Destroyed Small airports" << "\n";

    for (int i = 0; i < (int) medAirports.size(); i++) {
        delete medAirports[i];
    }

    std::cout << "Destroyed Medium airports" << "\n";

    for (int i = 0; i < (int) largeAirports.size(); i++) {
        delete largeAirports[i];
    }

    std::cout << "Destroyed Large airports" << "\n";
}
//Algorithm
void Airports::BFS(Airport* startPoint)
{
    queue<Airport*> q;
    startPoint->visited = 1;
    q.push(startPoint);
    departure = startPoint;
    destination = startPoint;
    while (!q.empty())
    {
        Airport* front = q.front();
        q.pop();
        double currDistancetoDeparture = calcDistance(departure->latitude, departure->longitude, currLat, currLong);
        double airportDistancetoDeparture = calcDistance(currLat, currLong, front->latitude, front->longitude);
        if (abs(currDistancetoDeparture) > abs(airportDistancetoDeparture))
            departure = front;
        double currDistancetoDestination = calcDistance(destination->latitude, destination->longitude, destLat, destLong);
        double airportDistancetoDestination = calcDistance(destLat, destLong, front->latitude, front->longitude);
        if (abs(currDistancetoDestination) > abs(airportDistancetoDestination))
            destination = front;
        for (auto i : front -> connections)
        {
            if (i.first->visited != 1)
            {
                q.push(i.first);
                i.first->visited = 1;
            }
        }
    }
    for (int i = 0; i < (int) medAirports.size(); i++) { medAirports[i]->visited = 0; }
    for (int i = 0; i < (int) largeAirports.size(); i++) { largeAirports[i]->visited = 0; }
    for (int i = 0; i < (int) smallAirports.size(); i++) { smallAirports[i]->visited = 0; }
}
vector<vector<Airports::Airport*>> Airports::Kosaraju(int num, Airport* startPoint) {
    std::list<Airports::Airport*> visited;
    std::list<Airports::Airport*> finished;
    std::stack<Airports::Airport*> S;

    for (std::pair<Airport*, double> v : startPoint->connections) {
        if (!contains(visited, v.first)) {
            DFS(v.first, visited, finished, S);
        }
    }
    std::map<Airport*, vector<Airport*>> t_graph;
    std::list<Airport*> r_visited;
    transposeGraph(startPoint, r_visited, t_graph);
    visited = list<Airport*>();

    vector<vector<Airport*>> sc_components;

    while (!S.empty()) {

        if (!contains(visited, S.top())) {
            DFS(S.top(), visited, t_graph);
        }
        S.pop();

        if (!visited.empty()) {
            vector<Airport*> component;
            for (Airport* a : visited) {
                component.push_back(a);
            }
            sc_components.push_back(component);
        }
    }
    return sc_components;
}
//Helpers
void Airports::DFS(Airport* vertex, std::list<Airport*> visited, std::map<Airport*, vector<Airport*>> graph) {
    visited.push_back(vertex);
    for (Airport* a : graph.at(vertex)) {
        if (contains(visited, a)) {
            DFS(a, visited, graph);
        }
    }
}

void Airports::DFS(Airports::Airport* vertex, std::list<Airports::Airport*> visited, std::list<Airports::Airport*> finished,
                   std::stack<Airports::Airport*> S) {
    visited.push_back(vertex);
    for (std::pair<Airport*, double> v : vertex->connections) {
        if (!contains(visited, v.first)) {
            DFS(v.first, visited, finished, S);
        }
    }
    finished.push_back(vertex);
    S.push(vertex);
}
void Airports::transposeGraph(Airports::Airport* vertex, list<Airport*> visited, std::map<Airport*, vector<Airport*>> transposed) {
    visited.push_back(vertex);
    for (std::pair<Airport*, double> v : vertex->connections) {
        if (transposed.count(v.first) > 0) {
            transposed.at(v.first).push_back(vertex);
        }
        else {
            vector<Airport*> vt;
            vt.push_back(vertex);
            transposed.emplace(v.first, vt);
        }
    }
}

bool Airports::contains(list<Airport*> list, Airport* vertex) {
    for (Airport* a : list) {
        if (a == vertex) {
            return true;
        }
    }
    return false;
}
void Airports::Djistrka() {
    departure->fullCost = 0;
    vector<Airport*> allAirports;
    for (int i = 0; i < (int) smallAirports.size(); i++) {
        allAirports.push_back(smallAirports[i]);
    }
    for (int i = 0; i < (int) medAirports.size(); i++) {
        allAirports.push_back(medAirports[i]);
    }
    for (int i = 0; i < (int) largeAirports.size(); i++) {
        allAirports.push_back(largeAirports[i]);
    }
    size_t size = allAirports.size();
    for (int i = 0; i < (int) size; i++) {
        if (allAirports[i]->fullCost < allAirports[allAirports.size() - 1]->fullCost) {
            Airport* temp = allAirports[i];
            allAirports[i] = allAirports[size - 1];
            allAirports[size - 1] = temp;
        }
    }
    while (!allAirports.empty()) {
        Airport* currentAirport = allAirports.back();
        allAirports.pop_back();
        currentAirport->visited = 1;
        for (auto i = currentAirport->connections.begin(); i != currentAirport->connections.end(); ++i) {
            if (i->first->visited == 0) {
                if (i->second + currentAirport->fullCost < i->first->fullCost) {
                    i->first->fullCost = i->second + currentAirport->fullCost;
                    i->first->previous = currentAirport;
                }
            }
        }
        size = allAirports.size();
        for (int i = 0; i < (int) size; i++) {
            if (allAirports[i]->fullCost < allAirports[allAirports.size() - 1]->fullCost) {
                Airport* temp = allAirports[i];
                allAirports[i] = allAirports[size - 1];
                allAirports[size - 1] = temp;
            }
        }
    }
    Airport* val = destination;
    do {
        solution.push_back(val);
        cout << val->name << "\n";
        val = val->previous;
    } while (val != departure);
    cout << val->name << "\n";
    solution.push_back(val);
    reverse(solution.begin(), solution.end());
    for (int i = 0; i < (int) smallAirports.size(); i++) {
        smallAirports[i]->previous = nullptr;
        smallAirports[i]->fullCost = DBL_MAX;
        smallAirports[i]->visited = 0;
    }
    for (int i = 0; i < (int) medAirports.size(); i++) {
        medAirports[i]->previous = nullptr;
        medAirports[i]->fullCost = DBL_MAX;
        medAirports[i]->visited = 0;
    }
    for (int i = 0; i < (int) largeAirports.size(); i++) {
        largeAirports[i]->previous = nullptr;
        largeAirports[i]->fullCost = DBL_MAX;
        largeAirports[i]->visited = 0;
    }
}

//Helpers
double Airports::calcDistance(double lat1, double long1, double lat2, double long2) {
    // We are going to compute distances using Haversine Formula
    double deltaLatRad = (lat2 - lat1) * ((3.14159265358979323846) / 180.0);
    double deltaLongRad = (long2 - long1) * ((3.14159265358979323846) / 180.0);

    double lat1Rad = lat1 * ((3.14159265358979323846) / 180.0);
    double lat2Rad = lat2 * ((3.14159265358979323846) / 180.0);

    double a = pow(sin(deltaLatRad / 2), 2) + cos(lat1Rad) * cos(lat2Rad) * pow(sin(deltaLongRad / 2), 2);
    double c = 2 * asin(sqrt(a));
    double d = 6371 * c;

    // d returns the distance in kilometers
    return d;
    // Source: https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
}
double Airports::calcCost(double lat1, double long1, double lat2, double long2, const string& startAirport) {
    // We are going to make assumptions on the cost of travelling
    double distance = calcDistance(lat1, long1, lat2, long2);
    // Lets assume that distance is large if it is greater than 6500 km
    bool largeDistance = distance > 6500;
    // Costs are determined by:
    // 1) Base fare (https://www.tripsavvy.com/what-is-a-base-fare-468261)
    // 2) Taxes and airport fees (need to check)
    // 3) Fuel surcharge (depends on plane model)
    // 4) Service fee to issue (need to check)
    // 5) Food (fixed)
    // 6) Seat selection (assume economy)
    // 7) Baggage (assume no baggages)
    // Source: https://flightfox.com/tradecraft/how-do-airlines-set-prices

    // On average, fuels costs are $7 per gallon (best case scenario) 
    // of fuel for commercial use so we will use this https://executiveflyers.com/how-much-does-jet-fuel-cost/

    // Base Fare (Ballpark Estimate after experimenting with Orbitz and Expedia and looking at different prices for international and domestic travels):
    double BaseFare;
    if (startAirport == "medium_airport") {
        BaseFare = 350;
    }
    else if (startAirport == "large_airport") {
        BaseFare = 750;
    }
    else {
        BaseFare = 100;
    }
    // Ticket cost = Base Fare (original cost of flying) + Fees (Federal and other)
    // Source: https://www.tripsavvy.com/what-is-a-base-fare-468261
    double FederalFees;
    if (largeDistance) {
        FederalFees = 1.17 * (4.10 + 5.60 + ((4.50 + 18) / 2));
    }
    else {
        FederalFees = 1.17 * (4.10 + 5.60 + ((4.50 + 18) / 2) + 200);
    }

    // Other Fees = Food ($9) + Snacks ($4) + Baggage (Ballpark Estimate Large Distance: 1 -> $0, 2 -> $100 , 3 -> $200)
    // (Ballpark Estimate Smaller Distance: 1 -> $30, 2 -> $40 , 3 -> $125)
    // Source: https://www.airfarewatchdog.com/blog/3802275/in-flight-drink-snack-prices/
    // Source: https://thepointsguy.com/guide/airline-baggage-fees/
    double OtherFees;
    if (largeDistance) {
        switch (baggage) {
            case 2:
                OtherFees = 9 + 4 + 100;
            case 3:
                OtherFees = 9 + 4 + 200;
            default:
                OtherFees = 9 + 4 + 0;
        };
    }
    else {
        switch (baggage) {
            case 0:
                OtherFees = 9 + 4 + 0;
            case 1:
                OtherFees = 9 + 4 + 30;
            case 2:
                OtherFees = 9 + 4 + 40;
            default:
                OtherFees = 9 + 4 + 125;
        };
    }

    // Note: Fuel Cost = (kg / km) * 0.26 (gal / kg) * 7 ($ / gal) * distance (km) 
    double MinFuelCosts = DBL_MAX;
    if (startAirport == "large_airport") {
        //Traverse every aircraft in large_airport
        for (auto i = largeAirport.begin(); i != largeAirport.end(); ++i) {
            if (distance < (i->second)[0]) {
                double FuelCost = ((i->second)[1] * 0.26 * 7 * distance) / (i->second)[2];
                MinFuelCosts = (FuelCost < MinFuelCosts) ? FuelCost : MinFuelCosts;
            }
        }
    }
    else if (startAirport == "medium_airport") {
        //Traverse every aircraft in mediums_airport
        for (auto i = mediumAirport.begin(); i != mediumAirport.end(); ++i) {
            if (distance < (i->second)[0]) {
                double FuelCost = ((i->second)[1] * 0.26 * 7 * distance) / (i->second)[2];
                MinFuelCosts = (FuelCost < MinFuelCosts) ? FuelCost : MinFuelCosts;
            }
        }
    }
    else {
        //Traverse every aircraft in small_airport
        for (auto i = smallAirport.begin(); i != smallAirport.end(); ++i) {
            if (distance < (i->second)[0]) {
                double FuelCost = ((i->second)[1] * 0.26 * 7 * distance) / (i->second)[2];
                MinFuelCosts = (FuelCost < MinFuelCosts) ? FuelCost : MinFuelCosts;
            }
        }
    }
    return FederalFees + OtherFees + MinFuelCosts + BaseFare;
}


//Getters and Setters
void Airports::setCurrLong(double val) { currLong = val; }
void Airports::setCurrLat(double val) { currLat = val; }
void Airports::setDestLong(double val) { destLong = val; }
void Airports::setDestLat(double val) { destLat = val; }
void Airports::setBaggage(int val) { baggage = val; }
void Airports::setDeparture(Airports::Airport* val) { departure = val; }
void Airports::setDestination(Airports::Airport* val) { destination = val; }
double Airports::getCurrLong() const { return currLong; }
double Airports::getCurrLat() const { return currLat; }
double Airports::getDestLong() const { return destLong; }
double Airports::getDestLat() const { return destLat; }
Airports::Airport* Airports::getDeparture() { return departure; }
Airports::Airport* Airports::getDestination() { return destination; }
vector<Airports::Airport*>Airports::getSolution() { return solution; }
