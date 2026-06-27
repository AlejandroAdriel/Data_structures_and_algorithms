#include <iostream>
#include <vector>
#include <thread>

using namespace std;

// Partición clásica (Lomuto) sin ninguna modificación
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quicksort con inyección directa de hilos en cada recursión
void threaded_quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // Creamos un hilo exclusivo para la mitad izquierda
        thread t_left(threaded_quicksort, ref(arr), low, pi - 1);
        
        // El hilo actual resuelve la mitad derecha
        threaded_quicksort(arr, pi + 1, high);

        // Esperamos obligatoriamente a que el hilo de la izquierda termine
        t_left.join();
    }
}

int main() {
    // Un arreglo mediano (si pones millones aquí, colapsará por falta de memoria para tantos hilos)
    vector<int> arr = {14, 3, 22, 8, 10, 5, 2, 9, 25, 11, 4, 1, 18, 7, 12, 6};
    
    cout << "Quicksort con hilos en cada recursion..." << endl;
    threaded_quicksort(arr, 0, arr.size() - 1);

    for (int x : arr) cout << x << " ";
    cout << "\n-- completado --" << endl;

    return 0;
}