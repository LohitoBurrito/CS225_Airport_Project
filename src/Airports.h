#pragma once
#include <vector>
#include <map>
#include <string>
#include <list>
#include <stack>
#include <cfloat>

using namespace std;

class Airports {
public:
    //How each Airports Node is structured
    struct Airport {
        string type;
        string name;
        double latitude;
        double longitude;
        int visited = 0;
        double fullCost = DBL_MAX;
        Airport* previous = nullptr;
        map<Airport*, double> connections;
    };

public:
    //Airports Constructor: Calls createGraph(), parseData(), and sets private variables,
    //and the public vectors (Airport Node type) smallAirports, medAirports, and largeAirports
    Airports();
    Airports(double lat_, double long_, double lat2_, double long2_, int baggageAmount, string filename);
    ~Airports();

    //Parse and Creating Graph
    void parseData();
    void createGraph();
    void destroyGraph();

    //Algorithm
    //Finds the closest start location based off of currLat and currLong, and closest end location based off of
    //destLat and destLong
    bool BFS(Airport* startPoint);
    vector<vector<Airport*>> Kosaraju(Airport* startPoint);
    //Find the most cost effective path between the start location and end location found in BFS()
    //The optimal airport path is stored in private vector "solution"
    void Djistrka();

    //Helpers
    static double calcDistance(double lat1, double long1, double lat2, double long2);
    double calcCost(double lat1, double long1, double lat2, double long2, const string& startAirport);
    void DFS(Airport* vertex, std::list<Airport*>& visited, std::map<Airport*, vector<Airport*>> graph);
    void DFS(Airport* vertex, std::list<Airport*>& visited, std::list<Airport*>& finished, std::stack<Airport*>& S);
    void transposeGraph(Airport* vertex, list<Airport*>& visited, std::map<Airport*, vector<Airport*>> transposed);
    bool contains(list<Airport*> list, Airport* vertex);

    //Setters
    void setCurrLong(double val);
    void setCurrLat(double val);
    void setDestLong(double val);
    void setDestLat(double val);
    void setBaggage(int val);
    void setDeparture(Airport* val);
    void setDestination(Airport* val);
    //Getters
    double getCurrLong() const;
    double getCurrLat() const;
    double getDestLong() const;
    double getDestLat() const;
    Airport* getDeparture();
    Airport* getDestination();
    vector<Airport*> getSolution();

    //Public Variables
    vector<Airport*> smallAirports;
    vector<Airport*> medAirports;
    vector<Airport*> largeAirports;

    // { "Aircraft Model" -> "Max Distance", "Fuel Burn Rates (kg/km)", "Seats" }
    // Source: https://en.wikipedia.org/wiki/Fuel_economy_in_aircraft
    // Source 2: https://www.airliners.net/forum/viewtopic.php?t=1355819
    // Models include: A320 class
    map<string, vector<double>> mediumAirport = {
            {"A321neo", vector<double> {7400,3.30,244}},
            {"A319ceo", vector<double> {6950,2.93,156}},
            {"A320neo", vector<double> {6300,2.79,194}},
            {"A320ceo", vector<double> {6200,3.13,180}},
            {"A321ceo", vector<double> {5950,3.61,220}},
            {"A319neo", vector<double> {6850,2.40,160}},
            {"Boeing 737 MAX 8", vector<double> {4085,3.04,170}},
            {"Boeing 737-700", vector<double> {6380,3.21,143}},
            {"Boeing 737-800", vector<double> {3695,3.59,162}}
    };

    // Models include: A350 class, A330 class, A320 class
    map<string, vector<double>> largeAirport = {
            {"A321neo", vector<double> {7400,3.30,244}},
            {"A319ceo", vector<double> {6950,2.93,156}},
            {"A320neo", vector<double> {6300,2.79,194}},
            {"A320ceo", vector<double> {6200,3.13,180}},
            {"A321ceo", vector<double> {5950,3.61,220}},
            {"A319neo", vector<double> {6850,2.40,160}},
            {"A330-900", vector<double> {13334,5.94,460}},
            {"A330-800", vector<double> {15094,5.45,406}},
            {"A330-200", vector<double> {15094,6.20,406}},
            {"A330-300", vector<double> {11750,6.53,440}},
            {"A350-900", vector<double> {15372,6.27,440}},
            {"A350-1000", vector<double> {16112,6.03,480}},
            {"Boeing 777-300ER", vector<double> {13650,8.58,396}},
            {"Boeing 737 MAX 8", vector<double> {4085,3.04,170}},
            {"Boeing 787-9", vector<double> {14140,6.40,246}},
            {"Boeing 737-700", vector<double> {6380,3.21,143}},
            {"Boeing 737-800", vector<double> {3695,3.59,162}}
    };

    // Fuel Burn Rate (kg / hr) * 1/Avg Speed (hr / km) = Fuel Burn Rate (kg / km)
    map<string, vector<double>> smallAirport = {
            {"Beechcraft King Air 350", vector<double> {2798,0.79,8}},
            {"BN2 Islander", vector<double> {1400,0.36,9}},
            {"Cessna Caravan", vector<double> {1982,0.77,14}},
            {"Pilatus PC-12", vector<double> {3339,0.53,8}},
            {"British Aerospace Jetstream 41", vector<double> {1433,0.77,29}},
            {"Beechcraft King Air 260", vector<double> {3185,0.53,9}}
    };


private:
    double currLong;
    double currLat;
    double destLat;
    double destLong;
    int baggage;
    string fileName;
    Airport* departure = nullptr;
    Airport* destination = nullptr;
    vector<Airport*> solution;
};
