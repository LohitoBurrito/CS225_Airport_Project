#pragma once
#include <vector>
#include <map>
#include <string>

using namespace std;

class Airports {
	private:
		struct Airport {
			string type;
			string name;
			double latitude;
			double longitude;
			map<Airport*, double> connections;
		};

	public:
		//Airports Constructor
		Airports();
		Airports(double lat_, double long_);
		~Airports();

		//Parse and Creating Graph
		void parseData();
		void createGraph();
		void destroyGraph();

		//Algorithm
		vector<Airport*> BFS(Airport* startPoint);
		vector<Airport*> Kosaraju(int num, Airport* startPoint);
		vector<Airport*> Djistrka(map<Airport*, double>, Airport*);

		//Helpers
		double calcDistance(double lat1, double long1, double lat2, double long2);
		double calcCost(double lat1, double long1, double lat2, double long2);

		//Public Variables
		vector<Airport*> smallAirports;
		vector<Airport*> medAirports;
		vector<Airport*> largeAirports;

	private:
		double currLong;
		double currLat;
};