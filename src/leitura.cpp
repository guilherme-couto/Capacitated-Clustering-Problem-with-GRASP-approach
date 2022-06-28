#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "graph.h"
#include "leitura.h"

using namespace std;

void leRanRealSparse(Graph *graph, string &input_file_name)
{
    // Abrindo arquivo de entrada
    ifstream input_file;
    input_file.open(input_file_name, ios::in);

    cout << "\nLendo o arquivo..." << endl;

    string first_line;
    getline(input_file, first_line);
    stringstream ss_first_line(first_line);

    vector<string> first_line_infos;
    string aux;

    while (getline(ss_first_line, aux, ' '))
        first_line_infos.push_back(aux);

    int order = stoi(first_line_infos[0]);
    int num_clusters = stoi(first_line_infos[1]);
    graph->setMaxClusters(num_clusters);
    string cluster_type = first_line_infos[2];
    graph->setClusterType(cluster_type);

    for (int i = 0; i < num_clusters; i++)
    {
        int lower = stoi(first_line_infos[3 + (2 * i)]);
        int upper = stoi(first_line_infos[4 + (2 * i)]);
        graph->insertCluster(lower, upper);
    }

    int nodes_index = 4 + (2 * (num_clusters - 1)) + 2; // o +2 no final é para ignorar o W que separa

    for (int i = 0; i < order; i++)
    {
        float node_weight = stoi(first_line_infos[nodes_index + i]);
        graph->insertNode(i, node_weight);
    }

    string edge;
    while (getline(input_file, edge))
    {
        stringstream ss_edge(edge);
        vector<string> edge_infos;

        while (getline(ss_edge, aux, ' '))
            edge_infos.push_back(aux);

        int source_id = stoi(edge_infos[0]);
        int target_id = stoi(edge_infos[1]);
        float edge_weight = stof(edge_infos[2]);
        float source_weight = graph->getNode(source_id)->getWeight();
        float target_weight = graph->getNode(target_id)->getWeight();

        graph->insertEdge(source_id, source_weight, target_id, target_weight, edge_weight);
    }

    // Fechando arquivo de entrada
    input_file.close();
}

void leHandover(Graph *graph, string &input_file_name)
{
    // Abrindo arquivo de entrada
    ifstream input_file;
    input_file.open(input_file_name, ios::in);

    cout << "\nLendo o arquivo..." << endl;

    string line;

    getline(input_file, line);
    int order = stoi(line);

    getline(input_file, line);
    int num_clusters = stoi(line);
    graph->setMaxClusters(num_clusters);

    getline(input_file, line);
    float capacity = stof(line);

    for (int i = 0; i < num_clusters; i++)
        graph->insertCluster(0.0, capacity);

    for (int i = 0; i < order; i++)
    {
        getline(input_file, line);
        float node_weight = stof(line);
        graph->insertNode(i, node_weight);
    }

    getline(input_file, line);
    stringstream ss_line(line);
    vector<string> matrix;
    string aux;

    while (getline(ss_line, aux, ' '))
        matrix.push_back(aux);

    int matrix_size = matrix.size();
    for (int k = 0; k < matrix_size; k++)
    {
        if (stoi(matrix[k]) != 0)
        {
            int edge_weight = stoi(matrix[k]);
            int source_id = k / order;
            int target_id = k % order;
            float source_weight = graph->getNode(source_id)->getWeight();
            float target_weight = graph->getNode(target_id)->getWeight();

            graph->insertEdge(source_id, source_weight, target_id, target_weight, edge_weight);
        }
    }

    // Fechando arquivo de entrada
    input_file.close();
}