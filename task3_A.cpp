#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

// Struttura per rappresentare una tripla (riga, colonna, valore)
struct Triplet {
    int row;
    int col;
    double val;
};

// Funzione di ordinamento per le triplette (per riga crescente, poi per colonna crescente)
bool compareTriplets(const Triplet& a, const Triplet& b) {
    if (a.row != b.row) return a.row < b.row;
    return a.col < b.col;
}

int main() {
    // 1. Conteggio del numero totale di nodi da coords.txt
    std::ifstream coords_file("coords.txt");
    if (!coords_file.is_open()) 
    {
        std::cerr << "Errore: impossibile aprire coords.txt" << std::endl;
        return 1;
    }

    int total_nodes = 0;
    std::string line;
    while (std::getline(coords_file, line)) 
    {
        if (!line.empty()) total_nodes++;
    }
    coords_file.close();

    std::vector<Triplet> A_orig; // Vettore per memorizzare le triplette della matrice A_orig

    // 2. Diagonale principale: A(i, i) = +4.0
    for (int i = 0; i < total_nodes; ++i) 
    {
        A_orig.push_back({i, i, 4.0});
    }

    // 3. Lettura diretta di connectivity.txt (3 colonne: edge_id, nA, nB)
    std::ifstream conn_file("connectivity.txt");
    if (!conn_file.is_open()) 
    {
        std::cerr << "Errore: impossibile aprire connectivity.txt" << std::endl;
        return 1;
    }
    //
    int edge_id, nA, nB;
    while (conn_file >> edge_id >> nA >> nB) 
    {
        // Inserimento diretto nella matrice 
        A_orig.push_back({nA, nB, -1.0});
        A_orig.push_back({nB, nA, -1.0});
    }
    conn_file.close();

    // 4. Ordinamento delle triplette
    std::sort(A_orig.begin(), A_orig.end(), compareTriplets);

    // 5. Scrittura del file di output A_orig.txt
    std::ofstream out_orig("A_orig.txt");
    for (const auto& t : A_orig) {
        out_orig << t.row << " " << t.col << " " << t.val << "\n";
    }
    out_orig.close();

    std::cout << "Matrice A_orig.txt generata con successo! Nodi totali: " 
              << total_nodes << std::endl;

    return 0;
}