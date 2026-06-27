#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

// ============================================================================
// VARIABLES GLOBALES (De t3.cpp)
// ============================================================================
mutex m;
long global_sum = 0;

// ============================================================================
// FUNCIONES DE TRABAJO (WORKERS)
// ============================================================================

// Usada en Experimento 1 y 2 (t1.cpp y t2.cpp)
void F_print_chars(char c, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << c;
    }
}

// Usada en Experimento 3 (t3.cpp)
void F_race_condition(int x, int n)
{
    for (int i = 0; i < n; i++)
    {
        // Quita los comentarios de abajo para resolver la condición de carrera
        // m.lock();
        global_sum += x;
        // m.unlock();
    }
}

// Usada en Experimento 4 (t4.cpp)
void F_partial_sum(const int* ar, int n, int* s)
{
    for (int i = 0; i < n; i++)
    {
        *s += ar[i];
    }
}

// Usada en Experimento 5 (t5.cpp)
void F_heavy_work(int n)
{
    int local_sum = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            local_sum += 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;
        }
    }
    // Evita que el compilador optimice y elimine el bucle al no usar local_sum
    (void)local_sum; 
}

// ============================================================================
// MAIN: EJECUCIÓN DE LOS EXPERIMENTOS
// ============================================================================
int main()
{
    int nt = thread::hardware_concurrency();
    cout << "=== Hilos de hardware disponibles: " << nt << " ===\n\n";

    // ------------------------------------------------------------------------
    // REPASO DE T1.CPP: Creación y sincronización básica de un único hilo
    // ------------------------------------------------------------------------
    cout << "--- [t1.cpp] Creando un solo hilo asignado a una tarea ---\n";
    thread t1(F_print_chars, 'c', 50);
    t1.join();
    cout << "\n\n";

    // ------------------------------------------------------------------------
    // REPASO DE T2.CPP: Concurrencia masiva imprimiendo caracteres mezclados
    // ------------------------------------------------------------------------
    cout << "--- [t2.cpp] Lanzando " << nt << " hilos concurrentes de impresion ---\n";
    vector<thread> ths2(nt);
    for (int i = 0; i < nt; i++)
        ths2[i] = thread(F_print_chars, char('a' + i), 100);

    for (int i = 0; i < nt; i++)
        ths2[i].join();
    cout << "\n\n";

    // ------------------------------------------------------------------------
    // REPASO DE T3.CPP: Condición de carrera sobre variable global compartida
    // ------------------------------------------------------------------------
    cout << "--- [t3.cpp] Condicion de carrera (Race Condition) ---\n";
    cout << " n-threads: " << nt;
    
    vector<thread> ths3(nt);
    for (int i = 0; i < nt; i++)
        ths3[i] = thread(F_race_condition, i, 1000);

    for (int i = 0; i < nt; i++)
        ths3[i].join();
    
    cout << "\n Suma global compartida resulta en: " << global_sum;
    cout << "\n (Nota: Si da un valor aleatorio o inconsistente, es por falta de Mutex)\n\n";

    // ------------------------------------------------------------------------
    // REPASO DE T4.CPP: Procesamiento paralelo de sub-arreglos y reducción
    // ------------------------------------------------------------------------
    cout << "--- [t4.cpp] Suma de bloques de un arreglo en paralelo ---\n";
    int ar[16] = {2, 6, 3, 1, 7, 8, 9, 6, 4, 3, 2, 8, 9, 11, 13, 1};
    int xsum[4] = {0, 0, 0, 0};
    int total_array_sum = 0;
    int nt_fixed = 4;
    
    vector<thread> ths4(nt_fixed);
    for (int i = 0; i < nt_fixed; i++)
        ths4[i] = thread(F_partial_sum, &ar[i * 4], 4, &xsum[i]);

    for (int i = 0; i < nt_fixed; i++)
        ths4[i].join();
    
    for (int i = 0; i < nt_fixed; i++)
        total_array_sum += xsum[i];
    
    cout << " Suma total calculada por partes: " << total_array_sum << "\n\n";

    // ------------------------------------------------------------------------
    // REPASO DE T5.CPP: Estrés de CPU utilizando variables locales
    // ------------------------------------------------------------------------
    cout << "--- [t5.cpp] Carga de trabajo pesada e independiente (CPU Stress) ---\n";
    vector<thread> ths5(nt);
    
    for (int i = 0; i < nt; i++)
        ths5[i] = thread(F_heavy_work, 1000);

    for (int i = 0; i < nt; i++)
        ths5[i].join();
    
    cout << " -- Fin del procesamiento pesado --\n\n";

    // Fin del programa unificado
    cout << "Presiona Enter para terminar...";
    getchar();
    return 0;
}