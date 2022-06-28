#include <iostream>
#include <vector>

#include "cluster.h"
#include "edge.h"
#include "node.h"

using namespace std;

// Constructor
Cluster::Cluster(float lower_bound, float upper_bound)
{
    this->lower = lower_bound;
    this->upper = upper_bound;
    this->size = 0;
    this->nodes_weight = this->load = 0.0;
    nodes.clear();
    this->lightest_node_weight = this->heaviest_node_weight = 0.0;
}

// Destructor
Cluster::~Cluster()
{
    nodes.clear();
}

// Getters
float Cluster::getLowerBound()
{
    return this->lower;
}

float Cluster::getUpperBound()
{
    return this->upper;
}

int Cluster::getSize()
{
    return this->size;
}

Node *Cluster::getNode(int id)
{
    for (int i = 0; i < this->nodes.size(); i++)
    {
        if (this->nodes[i]->getId() == id)
            return this->nodes[i];
    }
    // retorna o node ou null caso nao encontre
    return nullptr;
}

Node *Cluster::getNodePosition(int position)
{
    return this->nodes[position];
}

float Cluster::getNodesWeight()
{
    return this->nodes_weight;
}

int Cluster::getNodesSize()
{
    return this->nodes.size();
}

float Cluster::getLoad()
{
    return this->load;
}

float Cluster::getLightestNodeWeight()
{
    return this->lightest_node_weight;
}

float Cluster::getHeaviestNodeWeight()
{
    return this->heaviest_node_weight;
}

// Other methods
void Cluster::insertNode(Node *node)
{
    // caso seja o primeiro nó a ser acrescentado
    if (this->nodes.size() == 0)
        this->lightest_node_weight = this->heaviest_node_weight = node->getWeight();

    this->nodes.push_back(node);
    this->size++;
    this->nodes_weight += node->getWeight();

    if (node->getWeight() < this->lightest_node_weight)
        this->lightest_node_weight = node->getWeight();
    else if (node->getWeight() > heaviest_node_weight)
        this->heaviest_node_weight = node->getWeight();

    for (int i = 0; i < this->nodes.size() - 1; i++)
    {
        Edge *edge = node->getEdge(this->nodes[i]->getId());
        if (edge != nullptr)
            this->load += edge->getWeight();
    }
}

void Cluster::removeNode(Node *node)
{
    this->size--;
    this->nodes_weight -= node->getWeight();

    int index = -1;
    for (int i = 0; i < this->nodes.size(); i++)
    {
        if (this->nodes[i]->getId() != node->getId())
        {
            Edge *edge = node->getEdge(this->nodes[i]->getId());
            if (edge != nullptr)
                this->load -= edge->getWeight();
        }
        else
        {
            index = i;
        }
    }
    this->nodes.erase(this->nodes.begin() + index);

    // função para atualizar o peso do nó mais leve e mais pesado
    float light, heavy;
    light = heavy = this->nodes[0]->getWeight();
    for (int i = 1; i < this->size; i++)
    {
        float node_weight = this->nodes[i]->getWeight();
        if (node_weight < light)
            this->lightest_node_weight = node_weight;
        if (node_weight > heavy)
            this->heaviest_node_weight = node_weight;
    }
}

bool Cluster::searchNode(int id)
{
    // cria um ponteiro para o node desejado
    Node *node = getNode(id);

    // retorna falso caso nao encontre e verdadeiro caso encontre
    if (node == nullptr)
        return false;
    else
        return true;
}

float Cluster::incrementGain(Node *node)
{
    float gain = 0.0;
    for (int i = 0; i < this->nodes.size(); i++)
    {
        Edge *edge = node->getEdge(this->nodes[i]->getId());
        if (edge != nullptr)
            gain += edge->getWeight();
    }
    return gain;
}

float Cluster::removalCost(Node *node)
{
    float cost = 0.0;
    for (int i = 0; i < this->nodes.size(); i++)
    {
        if (this->nodes[i]->getId() != node->getId())
        {
            Edge *edge = node->getEdge(this->nodes[i]->getId());
            if (edge != nullptr)
                cost -= edge->getWeight();
        }
    }
    return cost;
}