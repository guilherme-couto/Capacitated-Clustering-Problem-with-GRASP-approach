#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

using namespace std;

// Edge class
class Edge
{
private:
    int target_id;
    int target_position;
    Edge *next_edge;
    float weight;

public:
    // Constructor and destructor
    Edge(int target_id, int target_position);
    ~Edge();

    // Getters
    int getTargetId();
    int getTargetPosition();
    Edge *getNextEdge();
    float getWeight();

    // Setters
    void setNextEdge(Edge *edge);
    void setWeight(float weight);
};

#endif // EDGE_H_INCLUDED