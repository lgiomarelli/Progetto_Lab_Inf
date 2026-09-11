#include <iostream>
#include <fstream>

int main() {
    int N;
    std::cout << "Inserisci il valore di N: ";
    if (!(std::cin >> N) || N <= 0) 
    {
        std::cerr << "Valore di N non valido!" << std::endl;
        return 1;
    }
    
    // Calcolo del passo h. Usiamo 1.0 per forzare la divisione in virgola mobile
    double h = 1.0 / (N + 1.0);
    int n = 0; // Indice progressivo che parte da 0
    int edge_count = 0; //Contatore archi che parte da 0
    
    // Apro il file in scrittura per i nodi
    std::ofstream coords_file("coords.txt");
    // Apro il file in scrittura per gli archi
    std::ofstream conn_file("connectivity.txt");
    // Controllo se i file sono stati aperti correttamente
    if (!coords_file.is_open()) {
        std::cerr << "Errore nell'apertura del file!" << std::endl;
        return 1;
    }
    if (!conn_file.is_open()) {
        std::cerr << "Errore nell'apertura del file!" << std::endl;
        return 1;
    }
    // Doppio ciclo per i punti interni (da 1 a N); per come ho ordinato i cicli for sto di fatto tenendo ferme le colonne e scorrendo le righe per ogni colonna
    for (int i = 1; i <= N; ++i) 
    {
        for (int j = 1; j <= N; ++j) 
        {
            
            // Calcolo le coordinate spaziali
            double x = i * h;
            double y = j * h;
            
            // Scrivo la riga nel file txt nel formato richiesto: n i j x y
            coords_file << n << " " << i << " " << j << " " << x << " " << y << "\n";
            
            // Incremento l'indice progressivo
            //n++; errore: non incrementare n qui, altrimenti non corrisponde all'indice progressivo dei nodi

            // Genero le connessioni
            // Connessioni orizzontali (verso destra)
            if (i < N)
            {
                int n_right = n + N;
                conn_file << edge_count << " " << n << " " << n_right << "\n";
                edge_count++;
            }
            // Connessioni verticali (verso l'alto)
            if (j < N)
            {
                int n_up = n + 1;
                conn_file << edge_count << " " << n << " " << n_up << "\n";
                edge_count++;
            }
            n++; // Incremento l'indice progressivo alla fine del ciclo interno, così che corrisponda all'indice del nodo corrente
        }
    }
    
    coords_file.close();
    conn_file.close();

    std::cout << "File coords.txt generato con successo. Nodi totali: " << n << std::endl;
    std::cout << "File connectivity.txt generato con successo. Archi totali: " << edge_count << std::endl;
    
    return 0;
}
