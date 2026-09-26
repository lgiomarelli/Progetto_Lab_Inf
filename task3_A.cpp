#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <iomanip>

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

int main() 
{
    //Apertura del file e conteggio del numero totale di nodi da coords.txt
    std::ifstream coords_file("coords.txt");
    if (!coords_file.is_open()) 
    {
        std::cerr << "Errore: impossibile aprire coords.txt" << std::endl;
        return 1;
    }

    int total_nodes = 0;
    std::string line;
    while (std::getline(coords_file, line)) // Leggo ogni riga del file e conto le righe non vuote
    {
        if (!line.empty()) total_nodes++;
    }
    coords_file.close();

    // Calcolo di N, h e dei coefficienti della matrice
    int N = static_cast<int>(std::round(std::sqrt(total_nodes)));
    double h = 1.0 / (N + 1.0);
    double kappa = 0.01;

    double diag_val = -4.0 * kappa / (h * h);
    double off_val  =  1.0 * kappa / (h * h);

    // Lettura di ordering.txt (mappa perm[n] = m)
    std::ifstream ordering_file("ordering.txt");
    if (!ordering_file.is_open()) 
    {
        std::cerr << "Errore: impossibile aprire ordering.txt" << std::endl;
        return 1;
    }
    // Creo un vettore perm per memorizzare la mappatura degli indici
    std::vector<int> perm(total_nodes);
    int m, n;
    while (ordering_file >> m >> n) 
    {
        if (n >= 0 && n < total_nodes)
        {
            perm[n] = m;
        }
    }
    ordering_file.close();

    std::vector<Triplet> A_orig; // Vettore per memorizzare le triplette della matrice A_orig
    std::vector<Triplet> A_reordered; // Vettore per memorizzare le triplette della matrice A_reordered

    // Diagonale principale: A(i, i) = +4.0
    for (int i = 0; i < total_nodes; ++i) 
    {
        A_orig.push_back({i, i, diag_val});

        // Matrice riordinata con indici mappati da perm
        int m_i = perm[i];
        A_reordered.push_back({m_i, m_i, diag_val});
    }

    // Lettura diretta di connectivity.txt (3 colonne: edge_id, nA, nB)
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
        // Inserimento diretto nella matrice (matrice originale)
        A_orig.push_back({nA, nB, off_val});
        A_orig.push_back({nB, nA, off_val});

        // Matrice riordinata con indici mappati da perm (ma e mb sono i nuovi nodi adiacenti)
        int mA = perm[nA];
        int mB = perm[nB];
        A_reordered.push_back({mA, mB, off_val});
        A_reordered.push_back({mB, mA, off_val});
    }
    conn_file.close();

    // Ordinamento delle triplette
    std::sort(A_orig.begin(), A_orig.end(), compareTriplets);
    std::sort(A_reordered.begin(), A_reordered.end(), compareTriplets);

    // Scrittura del file di output A_orig.txt
    std::ofstream out_orig("A_orig.txt");
    for (const auto& t : A_orig) 
    {
        out_orig << t.row << " " << t.col << " " << t.val << "\n";
    }
    out_orig.close();
    // Scrittura del file di output A_reordered.txt
    std::ofstream out_reordered("A_reordered.txt");
    for (const auto& t : A_reordered) 
    {
        out_reordered << t.row << " " << t.col << " " << t.val << "\n";
    }
    out_reordered.close();

   std::cout << "Matrici A_orig.txt e A_reordered.txt generate con successo! Nodi totali: " 
              << total_nodes << std::endl;

    return 0;
}