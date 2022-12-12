## Leading Question 

The goal of this project is to find the most cost effective path from going from one airport to another airport. Essentially, if we look at the distances/time it takes to go from airport A to airport B, the shortest path is to go from airport A to airport B, but sometimes it is not the most cost effective path or even a path at all. Sometimes people may need to take connecting flights to have the most cost effective flight. Therefore, the question we are trying to solve is which path will lead to the lowest money wastage while trying to optimize distances? We will accomplish this using 3 algorithms, Breadth First Search for traversal for traversing a graph, Dijkstra’s algorithm to determine the shortest and most cost-effective path, and Kosaraju’s algorithm for an edge case involving connected components.

## Dataset Acquisition and Processing

Current Data Set: https://raw.githubusercontent.com/unseen1980/awesome-travel/master/Airports/airports_array.json

Github for dataset:
https://github.com/unseen1980/awesome-travel/tree/master/Airports

Essentially, we found this dataset online. This dataset is open source and available to anyone for usage. The data contains a list of 8978 different airports in JSON format, with categorization which will be discussed in Data Format.

## Data Format

The data inputs are as follows: 

 {
    "type": "either small_airport, medium_airport, large_airport",
    "name": "Airport_Name",
    "latitude_deg": latitude_number,
    "longitude_deg": longitude_number,
    "elevation_ft": 50,
    "continent": "Continent_Name",
    "municipality": "Municipality_Name",
    "iata_code": "Iata_code_data"
  }

We essentially have 8978 different JSON data in one large JSON file where every piece of data represents data and description of a certain airport. We will not be using every single piece of data in each piece of JSON data. The main pieces of data we will be using are “type”, “name”, “latitude_deg”, and “longitude_deg.” If we do happen to need more parts from each piece of data, we will change our plan of action by using them.

### Data Correction

Essentially, we will be using an external library called “jsoncpp” to fetch the JSON data. Vcpkg is an open source package manager that allows people to download many different libraries, and it happened to include a “jsoncpp” which is a json library allowing us to read and write to JSON. The JSON data that we will fetch will be placed into a map. The map will contain an “airport name” and a vector containing the rest of the data. Now, to correct our JSON data, some of the airports contain nodes which have a “type” called closed. This means that the airport is currently shut down and we do not want to travel to these airports. We will want to filter out these airports in our map before we make a graph out of these airports. 

### Data Storage

Essentially, we will approach this problem using a class based implementation. 

In our class, we will have a private struct that will contain the properties of a certain airport: “type”,”name”,”latitude”, and “longitude” and a map. Each pair in our map will contain 2 pieces of data: an airport that is connected to our initial airport and the cost of travel. This is just like an adjacency list which stores neighbor (airport) data as well as edge data (cost of travel). If we want to look at more different information like distances and others, we can change our map of airports and cost to a map of airports and a vector, where each vector inside the map will contain the connecting airport, cost of travel, distances, etc. The question still arises, how do we know which airports are connected? We will be making a certain assumption in this project to answer this: Small airports can only make a connection to the closest medium airport, medium airports can make connections to every airport, and large airports can make connections to both medium and large airports. This also brings another important question. How do we receive costs since there are no costs in our JSON data. The costs will be calculated based on the research on different popular aircraft models. Different aircraft models use different amounts of fuels and will cover different distances, and these aircrafts will have an average cost of use. We will find these pieces of information, and formulate a way to create a method of assigning costs.

Our public will contain necessary functions to implement our algorithms to find shortest/cost effective pathing. 

We will then have another private which will contain 3 vectors just for accessibility purposes, and to build our graph. Each vector will contain airports for a different specified type [small airports, medium airports, and large airports]. We will also create the map described in “Data Correction” here.

In summary, to store airport nodes, we will allocate memory. To store edges/connections, we will be using a vector of airport connections. To add weights to these edges, the vector of airport connections will store different data on costs initially and distances. Note that our algorithm will be O(n) since we are allocating “n” nodes to memory using a singular loop. We will be filling our vectors with a singular loop making it O(n) as well. O(n) + O(n) is technically O(n) making our algorithm O(n) in terms of space complexity. We will not use our vectors to access data, but rather our graph will be used to access certain nodes. Therefore, accessing data in our graph depends on how we create our graph. We are using a map implementation of our graph which contains pairs of connecting airport nodes and their cost of travel. Therefore, we have to use a BFS traversal to find other nodes and access them. This will take O(n) time to access the worst case scenario since the worst case scenario includes that we traverse through all of the nodes to get to the node we are locating. Although finding the node in our graph takes O(n) runtime, accessing it only requires O(1) runtime. The airport node is built off of a struct as described before. Therefore we can access other data for our airport. We can also access cost without running a loop or traversing.

## Graph Algorithms

In terms of traversing the graph we allocated in memory, we will traverse it using a BFS search algorithm. To attain the most cost effective path, we will be using Dijkstra's algorithm. The map will store a string data representing the name of the airport that has a vector of the previous airport and smallest cost from the starting airport. All of these costs will be set to 10000000 or some high number initially except the starting airport which will be 0 because the cost to go from the starting airport to the starting airport is 0. We will go from the starting airport to the connected airports, and then update their costs to A. Every airport we visit will be removed from the vector we created. If we ever reach a point where these costs can be minimized, we will update those costs. We will keep doing this until we get through every node, and our final airport will contain the best cost, and previous airport. We will use a backtracking algorithm to find the entire path to be displayed with the costs in the terminal. In summary, we will use a BFS to add every airport name to the map with our secondary vector value initialized with our starting values. 

We have one massive problem with this project: What happens if our start and destination locations are within the radius of the closest. Therefore, our user wants to just fly a plane for no apparent reason at all. We will then proceed to ask the user, how many airports he wants to travel to. We will be using Kosaraju’s algorithms to solve this problem and determine the best connected component to minimize cost.

### Function Inputs 

For our BFS algorithm, we will take in our initial airport Node, we will then look at our children airport nodes, and then traverse to them and each airport until all of the airports are added to a map. This is the input for our Dijkstra’s algorithm. The input of the Dijkstra’s algorithm will contain a map of vectors storing the previous airport to its cost of travel (which is initially set to a really high number 10000000), a departure node, and a destination node. These costs of travel will be updated as we go from the start node to the next node until we reach the destination which will contain the total minimized cost. 

Kosaraju's algorithm will take in an int number of airports to travel, our graph itself, as well as our start node. 

### Function Outputs

Our BFS algorithm will output a map of airport names to vectors. This will be the input of our Djistrka’s algorithm. The output of our Djikstra’s algorithm will be the most cost effective shortest path in a form of the vector which will be printed in our terminal. The output of Kosaraju’s algorithm will be a vector containing the most efficient connected component for a looped journey.

### Function Efficiency

Our BFS algorithm will run in O(V) or O(2E) where V represents the number of vertices and E represents the number of edges. Worst case scenario, it will travel all of the nodes making it O(n). Since we are traveling to every node in BFS, we have to account for each node we go to. For Djistrkas, we also travel to every node, therefore it will also be O(V) or O(2E). Kosaraju’s algorithm takes O(V + E) time complexity. In this algorithm, we need to perform a transpose on the graph which will also take O(V + E). Essentially, we have O(V + E) + O(V + E) which ends up becoming an overall O(V + E) in terms of runtime for Kosaraju’s algorithm.







## Timeline

**Week 1**

__Objective: __

Parse Data
Start creating algorithms to filter out data and BFS algorithm

To-Dos:

Parse Data
Start Creating Class/Struct to hold observation data
Start creating our BFS algorithm

**Week 2**

Objective:

Continue working on building algorithms

To-Dos:

Finish up on algorithm from week 1
Create Dijkstra's algorithm

**Week 3**

Objective:

We need to be able to finish our algorithms and start creating test cases

To-Dos:

Finish algorithms from Week 2
Create test cases and test out implementation

**Week 4**

Objective:

Have viable product ready

To-Dos:

Presentation 
Make PPT & video
