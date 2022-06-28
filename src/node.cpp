#include "edge.h"
#include "node.h"

using namespace std;

// Constructor
Node::Node(int id)
{
    this->id = id;
    this->in_degree = 0;
    this->out_degree = 0;
    this->weight = 0.0;
    this->first_edge = nullptr;
    this->last_edge = nullptr;
    this->next_node = nullptr;
    this->visited = false;
    this->assigned = false;
};

// Destructor
Node::~Node()
{
    Edge *next_edge = this->first_edge;

    while (next_edge != nullptr)
    {
        Edge *aux_edge = next_edge->getNextEdge();
        delete next_edge;
        next_edge = aux_edge;
    }
};

// Getters
Edge *Node::getFirstEdge()
{
    return this->first_edge;
}

Edge *Node::getLastEdge()
{
    return this->last_edge;
}

int Node::getId()
{
    return this->id;
}

int Node::getPosition()
{
    return this->position;
}

int Node::getInDegree()
{
    return this->in_degree;
}

int Node::getOutDegree()
{
    return this->out_degree;
}

int Node::getDegree()
{
    return this->degree;
}

float Node::getWeight()
{
    return this->weight;
}

Node *Node::getNextNode()
{
    return this->next_node;
}

bool Node::getVisited()
{
    return this->visited;
}

bool Node::getAssigned()
{
    return this->assigned;
}

Edge *Node::getEdge(int target_id)
{
    Edge *edge = this->first_edge;

    while(edge != nullptr)
    {
        if(edge->getTargetId() == target_id)
            return edge;
        edge = edge->getNextEdge();
    }
    return nullptr;
}

// Setters
void Node::setNextNode(Node *next_node)
{
    this->next_node = next_node;
}

void Node::setWeight(float weight)
{
    this->weight = weight;
}

void Node::setPosition(int position)
{
    this->position = position;
}

void Node::setVisited(bool v)
{
    this->visited = v;
}

void Node::setAssigned(bool assigned)
{
    this->assigned = assigned;
}

// Other methods
void Node::insertEdge(int target_id, int target_position, float edge_weight)
{
    // Verifica se há pelo menos uma aresta no nó
    if (this->first_edge != nullptr)
    {
        // Alocando a nova aresta e mantendo a integridade da lista de aresta
        Edge *edge = new Edge(target_id, target_position);
        edge->setWeight(edge_weight);
        this->last_edge->setNextEdge(edge);
        this->last_edge = edge;
    }
    else
    {
        // Alocando a nova aresta e mantendo a integridade da lista de aresta
        this->first_edge = new Edge(target_id, target_position);
        this->first_edge->setWeight(edge_weight);
        this->last_edge = this->first_edge;
    }
}

void Node::removeAllEdges()
{
    // Verifies whether there are at least one edge in the node
    if (this->first_edge != nullptr)
    {
        Edge *next = this->first_edge;;
        Edge *aux = nullptr;
        // Removing all edges of the node
        while (next != nullptr)
        {
            aux = next;
            next = next->getNextEdge();
            delete aux;
        }
    }
    this->first_edge = this->last_edge = nullptr;
}

int Node::removeEdge(int id, bool directed, Node *target_node)
{
    // Verifies whether the edge to remove is in the node
    if (this->searchEdge(id))
    {
        Edge *aux = this->first_edge;
        Edge *previous = nullptr;
        // Searching for the edge to be removed
        while (aux->getTargetId() != id)
        {
            previous = aux;
            aux = aux->getNextEdge();
        }
        // Keeping the integrity of the edge list
        if (previous != nullptr)
            previous->setNextEdge(aux->getNextEdge());

        else
            this->first_edge = aux->getNextEdge();

        if (aux == this->last_edge)
            this->last_edge = previous;

        if (aux->getNextEdge() == this->last_edge)
            this->last_edge = aux->getNextEdge();

        delete aux;
        // Verifies whether the graph is directed
        if (directed)
            this->decrementOutDegree();

        else
        {
            this->decrementInDegree();
            target_node->decrementInDegree();
        }
        return 1;
    }
    return 0;
}

bool Node::searchEdge(int target_id)
{
    // Verifica se há uma Aresta no Nó para o nó de target_id
    if (this->first_edge != nullptr)
    {
        // Procurando por uma Aresta especifica do id informado
        for (Edge *aux = this->first_edge; aux != nullptr; aux = aux->getNextEdge())
            if (aux->getTargetId() == target_id)
                return true;
    }
    return false;
}

void Node::incrementInDegree()
{
    this->in_degree++;
}

void Node::incrementOutDegree()
{
    this->out_degree++;
}

void Node::decrementInDegree()
{
    this->in_degree--;
}

void Node::decrementOutDegree()
{
    this->out_degree--;
}
