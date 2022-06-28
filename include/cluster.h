#ifndef CLUSTER_H_INCLUDED
#define CLUSTER_H_INCLUDED

#include <vector>

#include "cluster.h"
#include "edge.h"
#include "node.h"

using namespace std;

// Cluster class
class Cluster
{
private:
    float lower;
    float upper;
    int size;
    float nodes_weight;
    float load;
    vector<Node *> nodes;
    float lightest_node_weight;
    float heaviest_node_weight;

public:
    // Constructor and destructor
    Cluster(float lower_bound, float upper_bound);
    ~Cluster();

    // Getters
    float getLowerBound();
    float getUpperBound();
    int getSize();
    Node *getNode(int id);
    Node *getNodePosition(int position);
    float getNodesWeight();
    int getNodesSize();
    float getLoad();
    float getLightestNodeWeight();
    float getHeaviestNodeWeight();

    // Other methods
    void insertNode(Node *node);
    void removeNode(Node *node);
    bool searchNode(int id);
    float incrementGain(Node *node);
    float removalCost(Node *node);
};

#endif // CLUSTER_H_INCLUDED