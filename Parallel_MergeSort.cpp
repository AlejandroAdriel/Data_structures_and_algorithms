#include <iostream>
#include <vector>
#include <thread>

using namespace std;

// Función de mezcla (Merge) clásica
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i]; i++;
        } else {
            arr[k] = R[j]; j++;
        }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

// Mergesort con inyección directa de hilos en cada división
void threaded_mergesort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // Se lanza un hilo nuevo para la mitad izquierda
        thread t_left(threaded_mergesort, ref(arr), l, m);
        
        // El hilo actual procesa la mitad derecha
        threaded_mergesort(arr, m + 1, r);

        // Sincronización estricta: No podemos hacer el merge hasta que t_left termine
        t_left.join();

        // Mezclar las dos mitades ya ordenadas
        merge(arr, l, m, r);
    }
}

int main() {
    vector<int> arr = {38, 27, 43, 3, 9, 82, 10, 15, 22, 4, 19, 90, 56, 1, 12, 8};

    cout << "Mergesort con hilos en cada recursion..." << endl;
    threaded_mergesort(arr, 0, arr.size() - 1);

    for (int x : arr) cout << x << " ";
    cout << "\n-- completado --" << endl;

    return 0;
}