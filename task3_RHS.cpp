#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

//Struttura per rappresentare un nodo con i suoi attributi
struct Node 
{
    int n; 
    int i, j;
    double x, y;
};


// Sorgente termica : f(x, y) = exp(-10 * (x^2 + y^2))
double eval_f(double x, double y) 
{
    return std::exp(-10.0 * (x * x + y * y));
}


// Condizione al contorno unica applicata uniformemente a tutti i bordi
double eval_u_boundary(double x, double y, bool is_zero, double const_val) {
    if (is_zero) 
    {
        return 0.0; // Bordo omogeneo (u = 0 dappertutto)
    }
    return const_val; // Stesso valore costante applicato a tutti i bordi
}

int main() {
    // 1. Lettura di coords.txt con controllo dell'apertura del file
    std::ifstream coords_file("coords.txt");
    if (!coords_file.is_open()) 
    {
        std::cerr << "Errore: impossibile aprire coords.txt" << std::endl;
        return 1;
    }
    // Leggo i nodi dal file coords.txt e li memorizzo in un vettore temporaneo
    std::vector<Node> nodes;
    Node temp;
    while (coords_file >> temp.n >> temp.i >> temp.j >> temp.x >> temp.y) 
    {
        nodes.push_back(temp);
    }
    coords_file.close();
    // Calcolo il numero totale di nodi
    int total_nodes = nodes.size();
    if (total_nodes == 0) return 1;

    // Parametri griglia e fisici
    int N = static_cast<int>(std::round(std::sqrt(total_nodes))); //Calcolo N come radice quadrata del numero totale di nodi
    double h = 1.0 / (N + 1.0);
    double k = 0.01;            // Conducibilita termica
    double factor = (h * h) / k; // Coefficiente h^2 / k

    // Lettura di ordering.txt per la permutazione
    std::ifstream ordering_file("ordering.txt");
    if (!ordering_file.is_open()) 
    {
        std::cerr << "Errore: impossibile aprire ordering.txt" << std::endl;
        return 1;
    }
    // Creo un vettore perm per memorizzare la mappatura degli indici
    std::vector<int> perm(total_nodes);
    int m, n;
    while (ordering_file >> m >> n) {
        if (n >= 0 && n < total_nodes) {
            perm[n] = m;
        }
    }
    ordering_file.close();

    // Selezione Condizioni al Contorno dell'utente
    std::cout << "=== Condizioni al Contorno (Bordi) ===\n";
    std::cout << "Vuoi impostare u = 0 su tutti i bordi? (1: Si, 0: No): ";
    int choice = 1;
    std::cin >> choice;
    
    bool is_zero = (choice == 1);
    double const_val = 0.0;

    // Se l'utente sceglie di non impostare u = 0, chiediamo il valore costante da applicare a tutti i bordi
    if (!is_zero) {
        std::cout << "Inserisci il valore uniforme di u da applicare a tutti i bordi: ";
        std::cin >> const_val;
    }

    // Assemblaggio vettori b_orig e b_reordered
    std::vector<double> b_orig(total_nodes, 0.0);
    std::vector<double> b_reordered(total_nodes, 0.0);

    //  Calcolo dei contributi della sorgente termica e dei bordi per ogni nodo
    for (const auto& node : nodes) 
    {
        int idx = node.n;

        // Contributo della sorgente termica: (h^2 / k) * f(x, y)
        b_orig[idx] = factor * eval_f(node.x, node.y);

        // Aggiunta contributi dei bordi vicini
        if (node.i == 1) 
        { // Bordo Sinistro (x = 0)
            b_orig[idx] += eval_u_boundary(0.0, node.y, is_zero, const_val);
        }
        if (node.i == N) 
        { // Bordo Destro (x = 1)
            b_orig[idx] += eval_u_boundary(1.0, node.y, is_zero, const_val);
        }
        if (node.j == 1) 
        { // Bordo Inferiore (y = 0)
            b_orig[idx] += eval_u_boundary(node.x, 0.0, is_zero, const_val);
        }
        if (node.j == N) 
        { // Bordo Superiore (y = 1)
            b_orig[idx] += eval_u_boundary(node.x, 1.0, is_zero, const_val);
        }

        // Mappatura sul vettore riordinato
        int m_idx = perm[idx];
        b_reordered[m_idx] = b_orig[idx];
    }

    // Scrittura su file
    std::ofstream out_orig("rhs_orig.txt");
    for (int i = 0; i < total_nodes; ++i) {
        out_orig << i << " " << b_orig[i] << "\n";
    }
    out_orig.close();

    std::ofstream out_reordered("rhs_reordered.txt");
    for (int i = 0; i < total_nodes; ++i) {
        out_reordered << i << " " << b_reordered[i] << "\n";
    }
    out_reordered.close();

    std::cout << "\nVettori rhs_orig.txt e rhs_reordered.txt generati con successo!\n";

    return 0;
}