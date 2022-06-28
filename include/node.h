#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "edge.h"

using namespace std;

// Node class
class Node
{
private:
    Edge *first_edge;
    Edge *last_edge;
    int id;
    int position;
    int in_degree;
    int out_degree;
    int degree;
    float weight;
    Node *next_node;
    bool visited; // Atributo para definir se o nó foi visitado ou não.
    bool assigned;

public:
    // Constructor and destructor
    Node(int id);
    ~Node();

    // Getters
    Edge *getFirstEdge();
    Edge *getLastEdge();
    int getId();
    int getInDegree();
    int getOutDegree();
    int getDegree();
    float getWeight();
    Node *getNextNode();
    bool getVisited();
    int getPosition();
    bool getAssigned();
    Edge *getEdge(int target_id);

    // Setters
    void setNextNode(Node *node);
    void setWeight(float weight);
    void setVisited(bool visited);
    void setPosition(int position);
    void setAssigned(bool inserted);

    // Other methods
    bool searchEdge(int target_id);
    void insertEdge(int target_id, int target_position, float edge_weight);
    void removeAllEdges();
    int removeEdge(int id, bool directed, Node *target_node);
    void incrementOutDegree();
    void decrementOutDegree();
    void incrementInDegree();
    void decrementInDegree();
};

#endif // NODE_H_INCLUDED