#include <iostream>
#include <string>

#include "graph.h"
#include "leitura.h"

using namespace std;

int main(int argc, char const *argv[])
{
    // Verificação se todos os parâmetros do programa foram entrados
    if (argc != 2)
    {
        cout << "ERRO: Esperado: ./<program_name> <input_file>" << endl;
        return 1;
    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    cout << "Running " << program_name << " with instance " << input_file_name << " ... " << endl;

    float avg = 0.0;
    float quality;
    float best = 0.0;
    for (int i = 0; i < 10; i++)
    {
        Graph *graph = new Graph();

        if (input_file_name[0] >= '2' && input_file_name[0] <= '4')
        {
            leHandover(graph, input_file_name);
        }
        else if (input_file_name[0] >= 'R' || input_file_name[0] <= 'S')
        {
            leRanRealSparse(graph, input_file_name);
        }
        else
        {
            cout << "Nao foi possivel abrir o arquivo " << input_file_name;
            return 1;
        }

        cout << "\nDados sobre o grafo criado" << endl;
        cout << "Ordem: " << graph->getOrder() << endl;
        cout << "Nº arestas inseridas: " << graph->getNumberEdges() << endl;
        cout << "Nº clusters: " << graph->getNumberClusters() << endl;

        graph->GRASP();
        quality = graph->totalClustersLoads();
        if(quality > best)
            best = quality;
        avg += quality;
        cout << "Qualidade = " << quality << endl;
        cout << endl;
        cout << "-------------------  " << i << "  ---------------------" << endl;
    }

    cout << "BEST = " << best << endl;
    cout << "AVG = " << avg / 10.0 << endl;
    
    return 0;
}
