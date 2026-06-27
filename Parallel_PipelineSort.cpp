#include <iostream>
#include <vector>
#include <thread>

using namespace std;

// ============================================================================
// ALGORITMOS SECUENCIALES BASE 
// ============================================================================

int particion(vector<int>& arr, int bajo, int alto) {
    int pivote = arr[alto];
    int i = bajo - 1;
    for (int j = bajo; j <= alto - 1; j++) {
        if (arr[j] < pivote) {
            i++;
            int aux = arr[i];
            arr[i] = arr[j];
            arr[j] = aux;
        }
    }
    int aux = arr[i + 1];
    arr[i + 1] = arr[alto];
    arr[alto] = aux;

    return i + 1;
}

void quicksortSecuencial(vector<int>& arr, int bajo, int alto) {
    if (bajo < alto) {
        int pi = particion(arr, bajo, alto);
        quicksortSecuencial(arr, bajo, pi - 1);
        quicksortSecuencial(arr, pi + 1, alto);
    }
}

void merge(vector<int>& arr, int inicio, int medio, int fin) {
    vector<int> temp;
    int i = inicio;
    int j = medio + 1;

    while (i <= medio && j <= fin) {
        if (arr[i] < arr[j]) {
            temp.push_back(arr[i]);
            i++;
        } else {
            temp.push_back(arr[j]);
            j++;
        }
    }
    while (i <= medio) { temp.push_back(arr[i]); i++; }
    while (j <= fin) { temp.push_back(arr[j]); j++; }

    for (size_t k = 0; k < temp.size(); k++) {
        arr[inicio + k] = temp[k];
    }
}

void mergesortSecuencial(vector<int>& arr, int bajo, int alto) {
    if (bajo < alto) {
        int medio = bajo + (alto - bajo) / 2;
        mergesortSecuencial(arr, bajo, medio);
        mergesortSecuencial(arr, medio + 1, alto);
        merge(arr, bajo, medio, alto);
    }
}

// ============================================================================
// FUNCION DE REDUCCIÓN 
// ============================================================================
void fusionar_bloques_paralelo(vector<int>& datos, int chunk) {
    int n = datos.size();
    vector<thread> hilos;
    int bloque_actual = chunk;
    
    // Ejecuta las fases sucesivas de mezcla hasta reconstruir el arreglo global
    while (bloque_actual < n) {
        for (int inicio = 0; inicio < n; inicio += 2 * bloque_actual) {
            int medio = inicio + bloque_actual - 1;
            
            int limite_teorico = inicio + 2 * bloque_actual - 1;
            int fin = (limite_teorico < n - 1) ? limite_teorico : (n - 1);

            if (medio < fin) {
                hilos.push_back(thread(merge, ref(datos), inicio, medio, fin));
            }
        }

        // Barrera de sincronización por nivel
        for (auto &h : hilos) h.join();
        hilos.clear();

        bloque_actual *= 2;
    }
}

// ============================================================================
// PROCESO DE SEGMENTACIÓN PRINCIPAL
// ============================================================================
void pipeline_sort_paralelo(vector<int>& datos) {
    int n = datos.size();
    
    int nt = thread::hardware_concurrency();
    if (nt == 0) nt = 4; 

    cout << "-> Iniciando ordenamiento en pipeline con " << nt << " bloques paralelos.\n";

    int chunk = n / nt;
    vector<thread> hilos;

    // --- FASE 1: ORDENAMIENTO EN PARALELO DE LOS 'nt' BLOQUES ---
    for (int i = 0; i < nt; i++) {
        int inicio = i * chunk;
        int fin = (i == nt - 1) ? (n - 1) : (inicio + chunk - 1);

        if (i < nt / 2) {
            hilos.push_back(thread(quicksortSecuencial, ref(datos), inicio, fin));
        } else {
            hilos.push_back(thread(mergesortSecuencial, ref(datos), inicio, fin));
        }
    }

    for (auto &h : hilos) h.join();
    hilos.clear();

    fusionar_bloques_paralelo(datos, chunk);
}


int main() {
    vector<int> datos = {
        100, 4, 18, 25, 87, 63, 11, 9, 32, 54, 76, 2, 88, 91, 14, 22,
        45, 12, 89, 33, 71, 90, 6, 19, 55, 67, 34, 13, 8, 99, 21, 44,
        76, 54, 23, 65, 87, 10, 5, 38, 60, 41, 19, 82, 30, 77, 3, 50,
        15, 93, 27, 62, 49, 81, 70, 12, 36, 42, 58, 66, 84, 5, 17, 29
    };

    cout << "Arreglo antes de ordenar:\n";
    for (int x : datos) cout << x << " ";
    cout << "\n\n";

    pipeline_sort_paralelo(datos);

    cout << "\nArreglo completamente ordenado:\n";
    for (int x : datos) cout << x << " ";
    cout << "\n";

    return 0;
}