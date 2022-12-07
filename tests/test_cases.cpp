#include <catch2/catch_test_macros.hpp>
#include "Airports.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <queue>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <cmath>

using namespace std;

TEST_CASE( "Test Closed/Heliport/Seabase Airports", "[closed Airports]") {
    //Closed Airport Check
    Airports system = Airports(0, 0, 0, 0, 0, "Airports.json");
    for (int i = 0; i < (int) system.smallAirports.size(); i++) {
        REQUIRE(system.smallAirports[i]->type != "closed");
        REQUIRE(system.smallAirports[i]->type != "heliport");
        REQUIRE(system.smallAirports[i]->type != "seaplane_base");
    }
    for (int i = 0; i < (int) system.medAirports.size(); i++) {
        REQUIRE(system.medAirports[i]->type != "closed");
        REQUIRE(system.medAirports[i]->type != "heliport");
        REQUIRE(system.medAirports[i]->type != "seaplane_base");
    }
    for (int i = 0; i < (int) system.largeAirports.size(); i++) {
        REQUIRE(system.largeAirports[i]->type != "closed");
        REQUIRE(system.largeAirports[i]->type != "heliport");
        REQUIRE(system.largeAirports[i]->type != "seaplane_base");
    }
    system.destroyGraph();
}
TEST_CASE( "Test CalcDist and CalcCost Algos 1", "[calcAlgos]"){
    //CHICAGO->DOHA
    Airports system = Airports(0, 0, 0, 0, 0, "Airports.json");
    double dist = system.calcDistance(41.97859955, -87.90480042, 25.273056, 51.608056);
    REQUIRE(abs(dist - 11479) > 50);
    double cost = system.calcCost(41.97859955, -87.90480042, 25.273056, 51.608056, "large_airport");
    double expediaCost = 2778.38 / 2;
    REQUIRE(abs(cost - expediaCost) > 400);
}
TEST_CASE( "Test CalcDist and CalcCost Algos 2", "[calcAlgos]"){
    //CHICAGO->FRANKFURT
    Airports system = Airports(0, 0, 0, 0, 0, "Airports.json");
    double dist = system.calcDistance(41.97859955, -87.90480042, 50.026402, 8.54313);
    REQUIRE(abs(dist - 6989) > 50);
    double cost = system.calcCost(41.97859955, -87.90480042, 50.026402, 8.54313, "large_airport");
    double expediaCost = 2276.0 / 2;
    REQUIRE(abs(cost - expediaCost) > 400);
}
TEST_CASE( "Test BFS + Dijkstra algorithm 1", "[bfs && dijkstra]" ){
    Airports system = Airports(41.97859955, -87.90480042, 25.273056, 51.608056, 2, "test2.json");
    system.BFS(system.medAirports[0]);
    REQUIRE(system.getDestination()->name == "Hamad International Airport");
    REQUIRE(system.getDeparture()->name == "Chicago O'Hare International Airport");
    system.Djistrka();
    REQUIRE(system.getSolution().size() == 2);
}
TEST_CASE( "Test BFS + Dijkstra algorithm 2", "[bfs && dijkstra]" ){
    Airports system = Airports(41.77190018, -88.47570038, 23.71829987, -15.93200016, 2, "test.json");
    system.BFS(system.medAirports[0]);
    REQUIRE(system.getDestination()->name == "Dakhla Airport");
    REQUIRE(system.getDeparture()->name == "Aurora Municipal Airport");
    system.Djistrka();
    REQUIRE(system.getSolution().size() == 3);
}
TEST_CASE( "Test BFS + Kosaraju algorithm 1", "[bfs && dijkstra]" ){
    //im about to morb
    Airports system = Airports(41.77190018, -88.47570038, 41.77190018, -88.47570038, 2, "test3.json");
    system.BFS(system.medAirports[0]);
    REQUIRE(system.getDestination() == system.getDeparture());
    vector<vector<Airports::Airport*>> connectedComponents = system.Kosaraju(system.getDeparture());
    for (int i = 0; i < (int) connectedComponents.size(); i++) {
        for (int j = 0; j < (int) connectedComponents[i].size(); j++) {
            cout<<connectedComponents[i][j]<<" ";
        }
        cout<<"\n";
    }
}