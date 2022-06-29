#include <algorithm>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <math.h>
#include <sys/time.h>

#include "cluster.h"
#include "edge.h"
#include "graph.h"
#include "node.h"
#include "random/Xoshiro256plus.h"

using namespace std;

float INF = 99999999.9;

// Constructor
Graph::Graph()
{
    this->order = this->edges_count = 0;
    this->first_node = this->last_node = nullptr;
    this->clusters.clear();
    this->max_clusters = 0;
    this->lightest_node_weight = 0.0;
}

// Destructor
Graph::~Graph()
{
    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters

int Graph::getOrder()
{
    return this->order;
}

int Graph::getNumberEdges()
{
    return this->edges_count;
}

Node *Graph::getFirstNode()
{
    return this->first_node;
}

Node *Graph::getLastNode()
{
    return this->last_node;
}

Node *Graph::getNode(int id)
{
    // cria ponteiro para percorrer a lista de nodes
    Node *node = this->first_node;

    // encontra o node com o id desejado
    while (node != nullptr)
    {
        if (node->getId() == id)
            return node;
        node = node->getNextNode();
    }

    // retorna o node ou null caso nao encontre
    return nullptr;
}

Node *Graph::getNodePosition(int position)
{
    // cria ponteiro para percorrer a lista de nodes
    Node *node = this->first_node;

    // encontra o node com o id desejado
    while (node != nullptr)
    {
        if (node->getPosition() == position)
            return node;

        node = node->getNextNode();
    }
    // retorna o node ou null caso nao encontre
    return nullptr;
}

Cluster *Graph::getCluster(int position)
{
    return &(this->clusters[position]);
}

int Graph::getNumberClusters()
{
    return this->clusters.size();
}

string Graph::getClusterType()
{
    return this->cluster_type;
}

// Others methods
void Graph::insertNode(int id, float weight)
{
    // confere se o grafo tem nodes
    if (this->first_node != nullptr)
    {
        // caso tenha, cria um novo node, aponta o ultimo pro novo e o novo se torna o ultimo
        Node *new_node = new Node(id);
        new_node->setPosition(this->order);
        new_node->setWeight(weight);
        this->last_node->setNextNode(new_node);
        this->last_node = new_node;
        if (weight < this->lightest_node_weight)
            this->lightest_node_weight = weight;
    }
    else
    {
        // caso nao tenha, cria um novo node e ele se torna o ultimo e o primeiro
        Node *new_node = new Node(id);
        new_node->setPosition(this->order);
        new_node->setWeight(weight);
        this->first_node = new_node;
        this->last_node = new_node;
        this->lightest_node_weight = weight;
    }
    this->order = this->order + 1;
}

void Graph::insertEdge(int source_id, float source_weight, int target_id, float target_weight, float edge_weight)
{
    // cria um ponteiro para o node desejado e um auxiliar para o node alvo da aresta
    Node *node = getNode(source_id);
    Node *aux = getNode(target_id);

    if (node == nullptr)
    {
        this->insertNode(source_id, source_weight);
        node = this->last_node;
    }
    if (aux == nullptr)
    {
        this->insertNode(target_id, target_weight);
        aux = this->last_node;
    }
    // confere se os nodes existem
    if (node != nullptr && aux != nullptr)
    {
        // confere se a aresta já existe
        if (!node->searchEdge(target_id))
        {
            // caso o node exista mas a aresta nao, insere a aresta
            node->insertEdge(target_id, aux->getPosition(), edge_weight);
            this->edges_count++;

            // se o grafo for nao-direcionado e nao houver aresta de volta
            if (!aux->searchEdge(source_id))
            {
                // insere a aresta de volta
                aux->insertEdge(source_id, node->getPosition(), edge_weight);
            }
        }
    }
}

void Graph::removeNode(int id)
{
    // cria um ponteiro para o node desejado
    Node *node = getNode(id);

    // retorna caso nao exista o node desejado
    if (node == nullptr)
        return;
    else if (node == this->first_node) // se o node é o primeiro, apenas faz o proximo ser o primeiro
        this->first_node = node->getNextNode();
    else
    {
        // caso seja um node do meio ou o ultimo, cria um ponteiro auxiliar
        Node *aux = this->first_node;

        // encontra o node anterior ao node desejado
        while (aux->getNextNode() != node)
            aux = aux->getNextNode();

        // se o no desejado for o ultimo, atualiza o ultimo para o anterior
        if (node == this->last_node)
            this->last_node = aux;

        // seta o proximo de anterior para o proximo do desejado
        aux->setNextNode(node->getNextNode());
    }

    // deleta o node desejado
    delete node;
    this->order--;
}

bool Graph::searchNode(int id)
{
    // cria um ponteiro para o node desejado
    Node *node = getNode(id);

    // retorna falso caso nao encontre e verdadeiro caso encontre
    if (node == nullptr)
        return false;
    else
        return true;
}

void Graph::setAllNonVisited()
{
    /**
     * @brief Função para definir todos os nós do grafo como não visitados.
     */

    Node *node = this->first_node; // Ponteiro que armazena o endereço de memória do primeiro nó do grafo.

    // Realiza a operação para todos os nós do grafo.
    while (node != nullptr)
    {
        node->setVisited(false);    // Define o nó como não visitado.
        node = node->getNextNode(); // Ponteiro passa a apontar para o próximo nó do grafo.
    }
}

void Graph::insertCluster(int lower, int upper)
{
    Cluster *cluster = new Cluster(lower, upper);
    this->clusters.push_back(*cluster);
}

void Graph::setMaxClusters(int max_number)
{
    this->max_clusters = max_number;
}

void Graph::setClusterType(string cluster_type)
{
    this->cluster_type = cluster_type;
}

bool Graph::isNodeUnassigned()
{
    Node *node = this->first_node;

    while (node != nullptr)
    {
        if (!node->getAssigned())
            return true;
        node = node->getNextNode();
    }
    return false;
}

// Imprimir clusters e seus pesos e tamanho -> Cluster X: tamanho | peso (lower, upper) | load
void Graph::printClusters()
{
    int assigned_nodes = 0;
    for (int k = 0; k < this->clusters.size(); k++)
    {
        cout << "Cluster " << k << ": " << this->clusters[k].getSize() << " | ";
        cout << this->clusters[k].getNodesWeight() << " (" << this->clusters[k].getLowerBound() << ", ";
        cout << this->clusters[k].getUpperBound() << ") | ";
        cout << this->clusters[k].getLoad() << endl;

        assigned_nodes += this->clusters[k].getSize();
    }
    cout << "Total assigned nodes: " << assigned_nodes << endl;
    float total = this->totalClustersLoads();
    cout << "Total loads: " << total << endl;
}

void Graph::printBestSolution()
{
    cout << "S = { ";
    for (int k = 0; k < this->clusters.size(); k++)
    {
        cout << "{";
        for (int i = 0; i < this->clusters[k].getSize(); i++)
        {
            if (i + 1 < this->clusters[k].getSize())
                cout << this->clusters[k].getNodePosition(i)->getId() << ", ";
            else
                cout << this->clusters[k].getNodePosition(i)->getId();
        }
        if (k + 1 < this->clusters.size())
            cout << "}, ";
        else
            cout << "} ";
    }
    cout << "}" << endl;
}

float Graph::totalClustersLoads()
{
    float sum = 0.0;
    for (int k = 0; k < this->clusters.size(); k++)
        sum += this->clusters[k].getLoad() * 1.0;
    return sum;
}

// Metaheuristic methods
void Graph::greedyRandom()
{
    Xoshiro256plus rand(time(NULL));

    // 1a parte: acrescentar, em cada cluster, um nó aleatório
    int random_number;
    Node *node;
    for (int k = 0; k < this->clusters.size(); k++)
    {
        // busca um nó aleatorio ainda nao inserido em algum cluster
        do
        {
            random_number = rand.next() % this->order; // gera um inteiro entre 0 e order-1
            node = this->getNode(random_number);
        } while (node->getAssigned());
        // insere o nó no cluster i e marca ele como inserido
        if (!node->getAssigned())
        {
            this->clusters[k].insertNode(node);
            node->setAssigned(true);
        }
    }

    // 2a parte: montar lista de candidatos CL com os nós não inseridos e seus valores de incremento nos clusters
    vector<pair<float, pair<int, int>>> candidate_list; // {valor do incremento, {node_id=i, cluster_id=k}}
    candidate_list.clear();
    float max_increment = -INF;

    for (int k = 0; k < this->clusters.size(); k++)
    {
        node = this->first_node;
        Node *cluster_node = this->clusters[k].getNodePosition(0);
        while (node != nullptr)
        {
            if (!node->getAssigned())
            {
                Edge *edge = node->getEdge(cluster_node->getId());
                if (edge != nullptr)
                {
                    float increment = edge->getWeight();
                    candidate_list.push_back({increment, {node->getId(), k}});
                    if (increment > max_increment)
                        max_increment = increment;
                }
            }
            node = node->getNextNode(); // Ponteiro passa a apontar para o próximo nó do grafo
        }
    }

    // 3a parte: montar uma lista restrita de candidatos para cada cluster e acrescentar nós até atingir o limite inferior de cada cluster
    float alfa = 0.6; // valor encontrado na literatura
    float increment_percentage = alfa * max_increment;
    for (int k = 0; k < this->clusters.size(); k++)
    {
        vector<int> rcl; // restricted candidate list
        rcl.clear();
        vector<pair<float, int>> cluster_cl; // cl sem restrição de increment % {increment, node_id}
        cluster_cl.clear();
        for (int c = 0; c < candidate_list.size(); c++)
        {
            if (candidate_list[c].second.second == k)
            {                                                        // candidate_list[c].second.second = cluster_id=k
                if (candidate_list[c].first >= increment_percentage) // candidate_list[c].first = valor do incremento
                    rcl.push_back(candidate_list[c].second.first);   // adiciona o id do nó na rcl
                cluster_cl.push_back({candidate_list[c].first, candidate_list[c].second.first});
            }
        }

        bool loop_flag = false;
        if (rcl.size() > 0)
        {
            int loop_counter = 0;
            // acrescenta aleatoriamente nós da rcl no cluster k até o limite inferior ser atingido
            while (this->clusters[k].getNodesWeight() < this->clusters[k].getLowerBound())
            {
                if (loop_counter > rcl.size() * rcl.size()) // preso em um loop. atrasando a execução
                {
                    loop_flag = true;
                    break;
                }

                random_number = rand.next() % rcl.size(); // gera um inteiro entre 0 e rcl.size
                node = this->getNode(rcl[random_number]);
                if (!node->getAssigned())
                {
                    this->clusters[k].insertNode(node);
                    node->setAssigned(true);
                }
                loop_counter++;
            }
        }
        if (rcl.size() <= 0 || loop_flag == true) // caso não tenha sido possível construir uma rcl com o valor de increment_percentage
        {
            sort(cluster_cl.rbegin(), cluster_cl.rend()); // ordena do maior para o menor incremento
            int index = 0;
            while (this->clusters[k].getNodesWeight() < this->clusters[k].getLowerBound())
            {
                node = this->getNode(cluster_cl[index].second); // pega o id do node com maior incremento
                if (!node->getAssigned())
                {
                    this->clusters[k].insertNode(node);
                    node->setAssigned(true);
                }
                index++; // insere um por um
            }
        }
    }

    gettimeofday(&this->end, 0);
    long seconds = this->end.tv_sec - this->begin.tv_sec;
    if (seconds > 60)
        return;

    // 4a parte: montar nova lista de candidatos com os nós que estão faltando
    while (this->isNodeUnassigned())
    {
        vector<pair<float, pair<int, int>>> new_cl; // {valor do incremento, {node_id=i, cluster_id=k}}
        new_cl.clear();
        max_increment = -INF;
        node = this->first_node;

        while (node != nullptr)
        {
            // checar se o nó ainda nao foi inserido para colocá-lo na new_cl
            if (!node->getAssigned())
            {
                // avaliar o incremento da inserção do nó em cada cluster
                for (int k = 0; k < this->clusters.size(); k++)
                {
                    // checa se a inserção do nó ainda faria o cluster ficar dentro do limite superior
                    if (node->getWeight() + this->clusters[k].getNodesWeight() <= this->clusters[k].getUpperBound())
                    {
                        float I = this->clusters[k].incrementGain(node);
                        if (I >= 0.0) // se retornar 0.0 é pq o nó não tem arestas que o liguem aos demais nós do cluster ou a aresta vale 0
                        {
                            new_cl.push_back({I, {node->getId(), k}});
                            if (I > max_increment)
                                max_increment = I;
                        }
                    }
                }
            }
            node = node->getNextNode();
        }

        gettimeofday(&this->end, 0);
        long seconds = this->end.tv_sec - this->begin.tv_sec;
        if (seconds > 60)
            return;
            
        // 5a parte: montar uma restricted candidate list new_rcl a partir da new_cl
        vector<pair<int, int>> new_rcl; // pair = {node_id, cluster_id}
        new_rcl.clear();
        vector<pair<int, int>> alternative_cl; // pair = {node_id, cluster_id}
        alternative_cl.clear();
        increment_percentage = alfa * max_increment;

        for (int c = 0; c < new_cl.size(); c++)
        {
            if (new_cl[c].first > increment_percentage)
                new_rcl.push_back(new_cl[c].second);
            else
                alternative_cl.push_back(new_cl[c].second);
        }

        gettimeofday(&this->end, 0);
        seconds = this->end.tv_sec - this->begin.tv_sec;
        if (seconds > 60)
            return;

        // 6a parte: acrescentar aleatoriamente os nós da new_rcl nos clusters
        if (new_rcl.size() > 0)
        {
            random_number = rand.next() % new_rcl.size(); // gera um inteiro entre 0 e new_rcl.size
            int node_id = new_rcl[random_number].first;
            node = this->getNode(node_id);

            if (!node->getAssigned())
            {
                int cluster_id = new_rcl[random_number].second;
                if (!node->getAssigned())
                {
                    this->clusters[cluster_id].insertNode(node);
                    node->setAssigned(true);
                }
            }
        }
        else if (alternative_cl.size() > 0) // caso nao tenha sido possível construir a rcl
        {
            random_number = rand.next() % alternative_cl.size(); // gera um inteiro entre 0 e new_rcl.size
            int node_id = alternative_cl[random_number].first;
            node = this->getNode(node_id);

            if (!node->getAssigned())
            {
                int cluster_id = alternative_cl[random_number].second;
                if (!node->getAssigned())
                {
                    this->clusters[cluster_id].insertNode(node);
                    node->setAssigned(true);
                }
            }
        }
        else // caso os nós a serem inseridos extrapolem o bound
        {
            node = this->first_node;

            while (node != nullptr)
            {
                if (!node->getAssigned())
                {
                    float lightest_cluster_weight = this->clusters[0].getNodesWeight();
                    int lightest_cluster_id = 0;
                    for (int k = 1; k < this->clusters.size(); k++) // procura o cluster mais leve
                    {
                        if (this->clusters[k].getNodesWeight() < lightest_cluster_weight)
                        {
                            lightest_cluster_weight = this->clusters[k].getNodesWeight();
                            lightest_cluster_id = k;
                        }
                    }
                    this->clusters[lightest_cluster_id].insertNode(node);
                    node->setAssigned(true);
                }
                node = node->getNextNode();
            }
        }
    }
}

float Graph::insertionMvmt()
{
    // criar uma rcl dos clusters capazes de ceder um nó e uma rcl dos cluster capazes de receber
    vector<int> source_rcl;
    vector<int> dest_rcl;
    source_rcl.clear();
    dest_rcl.clear();

    for (int k = 0; k < this->clusters.size(); k++)
    {
        Cluster cluster = this->clusters[k];
        if (cluster.getNodesWeight() - cluster.getLightestNodeWeight() >= cluster.getLowerBound())
            source_rcl.push_back(k);
        if (cluster.getNodesWeight() + this->lightest_node_weight <= cluster.getUpperBound())
            dest_rcl.push_back(k);
    }

    // criar todos os vizinhos possíveis da vizinhança
    int insertions = source_rcl.size() * dest_rcl.size();

    Node *node = nullptr;
    Node *best_node = nullptr;
    int best_from, best_to;
    best_from = best_to = -1;
    float mvmt_cost = 0.0;
    float best_neighbour_cost = 0.0;

    // percorre cada cluster em source_rcl
    for (int k = 0; k < source_rcl.size(); k++)
    {
        gettimeofday(&this->end, 0);
        long seconds = this->end.tv_sec - this->begin.tv_sec;
        if (seconds > this->timeLimit)
            return -INF;

        int from_index = source_rcl[k];
        Cluster source_cluster = this->clusters[from_index];

        for (int i = 0; i < source_cluster.getSize(); i++) // para cada nó do cluster de origem
        {
            gettimeofday(&this->end, 0);
            long seconds = this->end.tv_sec - this->begin.tv_sec;
            if (seconds > this->timeLimit)
                return -INF;

            node = source_cluster.getNodePosition(i);

            // checa se o source_cluster pode perder o node
            if (source_cluster.getNodesWeight() - node->getWeight() >= source_cluster.getLowerBound())
            {
                for (int j = 0; j < dest_rcl.size(); j++) // avaliar a inserção do nó em cada cluster na rcl destino
                {
                    gettimeofday(&this->end, 0);
                    long seconds = this->end.tv_sec - this->begin.tv_sec;
                    if (seconds > this->timeLimit)
                        return -INF;

                    int to_index = dest_rcl[j];

                    if (from_index != to_index)
                    {
                        Cluster dest_cluster = this->clusters[to_index];

                        // checa se o dest_cluster pode receber o node
                        if (dest_cluster.getNodesWeight() + node->getWeight() <= dest_cluster.getUpperBound())
                        {
                            // avalia o custo da nova solução (vizinho) e atualiza as variaveis best
                            // se mvmt_cost for negativo ou igual a zero, é sinal de que não houve melhora com relação à solução de partida
                            mvmt_cost = source_cluster.removalCost(node) + dest_cluster.incrementGain(node);
                            if (mvmt_cost > best_neighbour_cost)
                            {
                                best_neighbour_cost = mvmt_cost;
                                best_node = node;
                                best_from = from_index;
                                best_to = to_index;
                            }
                        }
                    }
                }
            }
        }
    }

    // atualiza a solução com o melhor vizinho da vizinhança, caso ele exista
    if (best_neighbour_cost > 0.0)
    {
        this->clusters[best_from].removeNode(best_node);
        this->clusters[best_to].insertNode(best_node);
    }
    return best_neighbour_cost;
}

float Graph::swapMvmt()
{
    Node *best_node1 = nullptr;
    Node *best_node2 = nullptr;
    int best_k1, best_k2;
    best_k1 = best_k2 = -1;
    float mvmt_cost = 0.0;
    float best_neighbour_cost = 0.0;

    // percorrer todos os clusters avaliando suas possíveis combinações de swap
    for (int k1 = 0; k1 < this->clusters.size(); k1++)
    {
        gettimeofday(&this->end, 0);
        long seconds = this->end.tv_sec - this->begin.tv_sec;
        if (seconds > this->timeLimit)
            return -INF;

        // para cada cluster, avaliar suas trocas de nodes com os demais clusters
        for (int k2 = k1 + 1; k2 < this->clusters.size(); k2++)
        {
            gettimeofday(&this->end, 0);
            long seconds = this->end.tv_sec - this->begin.tv_sec;
            if (seconds > this->timeLimit)
                return -INF;

            // avaliar, para cada nó do cluster k1, quais nós do cluster k2 podem ser trocados
            for (int i1 = 0; i1 < this->clusters[k1].getSize(); i1++)
            {
                gettimeofday(&this->end, 0);
                long seconds = this->end.tv_sec - this->begin.tv_sec;
                if (seconds > this->timeLimit)
                    return -INF;

                Node *node1 = this->clusters[k1].getNodePosition(i1);
                float node1_weight = node1->getWeight();

                for (int i2 = 0; i2 < this->clusters[k2].getSize(); i2++)
                {
                    gettimeofday(&this->end, 0);
                    long seconds = this->end.tv_sec - this->begin.tv_sec;
                    if (seconds > this->timeLimit)
                        return -INF;

                    Node *node2 = this->clusters[k2].getNodePosition(i2);
                    float node2_weight = node2->getWeight();

                    // avalia se a troca vai manter os clusters dentro dos bounds
                    float cluster1_balance = node2_weight - node1_weight; // ganhando node2, perdendo node1
                    float cluster2_balance = node1_weight - node2_weight; // ganhando node1, perdendo node2
                    bool mvmt_flag = false;

                    // se for diminuir o weight do cluster1 e aumentar o do cluster2
                    if (cluster1_balance < 0.0)
                        if (this->clusters[k1].getNodesWeight() + cluster1_balance >= this->clusters[k1].getLowerBound())
                            if (this->clusters[k2].getNodesWeight() + cluster2_balance <= this->clusters[k2].getUpperBound())
                                mvmt_flag = true;

                            // se for diminuir o weight do cluster2 e aumentar o do cluster1
                            else if (cluster2_balance < 0.0)
                                if (this->clusters[k2].getNodesWeight() + cluster2_balance >= this->clusters[k2].getLowerBound())
                                    if (this->clusters[k1].getNodesWeight() + cluster1_balance <= this->clusters[k1].getUpperBound())
                                        mvmt_flag = true;

                                    // caso não afete o weight de nenhum dos clusters. balances = 0
                                    else
                                        mvmt_flag = true;

                    // calcula o custo do movimento, a qualidade
                    if (mvmt_flag)
                    {
                        mvmt_cost = this->clusters[k1].removalCost(node1) + this->clusters[k2].incrementGain(node1);
                        mvmt_cost += this->clusters[k2].removalCost(node2) + this->clusters[k1].incrementGain(node2);

                        gettimeofday(&this->end, 0);
                        long seconds = this->end.tv_sec - this->begin.tv_sec;
                        if (seconds > this->timeLimit)
                            return -INF;

                        // avalia se o mvmt_cost é melhor do que o melhor até entao
                        if (mvmt_cost > best_neighbour_cost)
                        {
                            best_neighbour_cost = mvmt_cost;
                            best_node1 = node1;
                            best_node2 = node2;
                            best_k1 = k1;
                            best_k2 = k2;
                        }
                    }
                }
            }
        }
    }

    // atualiza a solução com o melhor vizinho da vizinhança, caso ele exista
    if (best_neighbour_cost > 0.0)
    {
        this->clusters[best_k1].removeNode(best_node1);
        this->clusters[best_k2].removeNode(best_node2);
        this->clusters[best_k1].insertNode(best_node2);
        this->clusters[best_k2].insertNode(best_node1);
    }
    return best_neighbour_cost;
}

void Graph::localSearch()
{
    // VND strategy
    bool improved1, improved2;
    float best_solution = this->totalClustersLoads();
    float result = 0.0;
    do
    {
        do
        {
            improved1 = false;
            result = this->insertionMvmt();
            if (result == -INF) // time's up
                return;
            if (result > 0.0)
            {
                improved1 = true;
                best_solution += result;
            }
        } while (improved1);

        gettimeofday(&this->end, 0);
        long seconds = this->end.tv_sec - this->begin.tv_sec;
        if (seconds > this->timeLimit)
            return;

        improved2 = false;
        result = this->swapMvmt();
        if (result == -INF) // time's up
            return;
        if (result > 0.0)
        {
            improved2 = true;
            best_solution += result;
        }
    } while (improved2);
}

void Graph::GRASP()
{
    this->timeLimit = this->getOrder() / 4.0;
    gettimeofday(&this->begin, 0);

    // first phase - greedy construction
    this->greedyRandom();
    /*
    cout << "GREEDY DONE" << endl;
    this->printClusters();
    this->printBestSolution();
    */

    gettimeofday(&this->end, 0);
    long seconds = this->end.tv_sec - this->begin.tv_sec;
    if (seconds > this->timeLimit)
        return;

    // second phase - local search
    this->localSearch();

    gettimeofday(&this->end, 0);
    seconds = this->end.tv_sec - this->begin.tv_sec;
    if (seconds > this->timeLimit)
        cout << "Time's up (" << seconds << " seconds)" << endl;
    
    this->printClusters();
    this->printBestSolution();
}