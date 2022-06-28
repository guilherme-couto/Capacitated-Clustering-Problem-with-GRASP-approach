#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <string>
#include <vector>

#include "cluster.h"
#include "node.h"

using namespace std;

class Graph
{
private:
    int order;
    int edges_count;
    Node *first_node;
    Node *last_node;
    vector<Cluster> clusters;
    int max_clusters;
    string cluster_type;
    float lightest_node_weight;
    struct timeval begin, end;
    long timeLimit;

public:
    // Constructor and destructor
    Graph();
    ~Graph();

    // Getters
    int getOrder();
    int getNumberEdges();
    Node *getFirstNode();
    Node *getLastNode();
    Node *getNode(int id);
    Node *getNodePosition(int position); // Pega um node apartir de sua posição de inserção
    Cluster *getCluster(int position);
    int getNumberClusters();
    string getClusterType();

    // Other methods
    void insertNode(int id, float weight);
    void insertEdge(int source_id, float source_weight, int target_id, float target_weight, float edge_weight);
    void removeNode(int id);
    bool searchNode(int id);
    void setAllNonVisited(); // Define todos os nós como não visitados.
    void insertCluster(int lower, int upper);
    void setMaxClusters(int max_number);
    void setClusterType(string cluster_type);
    bool isNodeUnassigned();

    // Metahueristic methods
    void printClusters();
    void greedyRandom();
    float totalClustersLoads();
    float insertionMvmt();
    float swapMvmt();
    void localSearch();
    void GRASP();
    void printBestSolution();
};

#endif // GRAPH_H_INCLUDED