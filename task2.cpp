#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

//struttura per rappresentare un nodo con i suoi attributi
struct Node {
    int n;       // ID originale del nodo
    int i, j;    // Indici della griglia
    double x, y; // Coordinate spaziali
};

// creo una funzione per calcolare la mediana delle coordinate x o y di un sottoinsieme di nodi
double get_median_coord(const std::vector<Node>& subset, bool split_x) 
{
    std::vector<double> coords;
    for (const auto& node : subset) //ciclo for per estrarre le coordinate x o y in base al valore di split_x
    {
        coords.push_back(split_x ? node.x : node.y); 
    }
    std::sort(coords.begin(), coords.end()); //ordino le coordinate in ordine crescente
    coords.erase(std::unique(coords.begin(), coords.end(), [](double a, double b) //funzione lambda per rimuovere i duplicati considerando una tolleranza di 1e-9
    {
        return std::abs(a - b) < 1e-9;
    }), coords.end());
    // ritorno la mediana delle coordinate ordinate
    return coords[coords.size() / 2];
}
// creo una funzione ricorsiva per eseguire la nested dissection
void compute_nested_dissection(const std::vector<Node>& subset, bool split_x, std::vector<int>& ordering) 
{
    if (subset.empty()) return; // caso base: se il sottoinsieme è vuoto, ritorno

    if (subset.size() == 1) //  se il sottoinsieme contiene un solo nodo, aggiungo il suo ID all'ordinamento e ritorno
    {
        ordering.push_back(subset[0].n);
        return;
    }
    // calcolo la mediana delle coordinate x o y in base al valore di split_x
    double hat = get_median_coord(subset, split_x);
    std::vector<Node> V1, V2, VS; // creo tre vettori per contenere i nodi a sinistra, a destra e sulla mediana
    
    // divido i nodi in tre insiemi: V1 , V2  e VS, in base alla posizione rispetto alla mediana 
    for (const auto& node : subset) 
    {
        double val = split_x ? node.x : node.y; // se split_x è true, uso la coordinata x del nodo, altrimenti uso la coordinata y
        //a questo punto metto a confronto la coordinata del nodo con la mediana
        if (std::abs(val - hat) < 1e-9) 
        {
            VS.push_back(node);
        } else if (val < hat) {
            V1.push_back(node);
        } else {
            V2.push_back(node);
        }
    }
    // richiamo ricorsivamente la funzione per i sottoinsiemi V1 e V2, invertendo il valore di split_x
    compute_nested_dissection(V1, !split_x, ordering);
    compute_nested_dissection(V2, !split_x, ordering);
    
    // aggiungo gli ID dei nodi sulla mediana all'ordinamento (i valori della mediana sono aggiunti per ultimi)
    for (const auto& node : VS) 
    {
        ordering.push_back(node.n);
    }
}
int main() 
{
    // Apro il file in lettura per i nodi
    std::ifstream coords_in("coords.txt");
    if (!coords_in.is_open()) return 1;
    
    // Leggo i nodi dal file e li memorizzo in un vettore
    std::vector<Node> nodes;
    Node temp; 
    // ciclo while per leggere i dati dal file coords.txt e riempire il vettore nodes
    while (coords_in >> temp.n >> temp.i >> temp.j >> temp.x >> temp.y) {
        nodes.push_back(temp);
    }
    coords_in.close();
    
    // creo un vettore per memorizzare l'ordinamento dei nodi e richiamo l'algoritmo di riordinamento
    std::vector<int> ordering;
    compute_nested_dissection(nodes, true, ordering);

    // Apro il file in scrittura per l'ordinamento dei nodi
    std::ofstream out("ordering.txt");
    // ciclo for per scrivere l'ordinamento dei nodi nel file ordering.txt
    for (int m = 0; m < static_cast<int>(ordering.size()); ++m) 
    {
        out << m << " " << ordering[m] << "\n";
    }
    out.close();

    return 0;
}