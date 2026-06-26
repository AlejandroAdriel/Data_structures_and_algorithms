/******************************************************************************

Hash table

*******************************************************************************/

#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <string>
#include <cmath>

struct fn_djb2 {
    int operator()(const std::string& x) const {
        unsigned int hash = 5381;
        for (char c : x) hash = hash * 33 + c;
        return hash;
    }
};

struct fn_fnv1a {
    int operator()(const std::string& x) const {
        unsigned int hash = 2166136261U;
        for (char c : x) {
            hash ^= c;
            hash *= 16777619;
        }
        return hash;
    }
};

struct fn_simple_sum {
    int operator()(const std::string& x) const {
        unsigned int hash = 0;
        for (char c : x) hash += c;
        return hash;
    }
};

template<class T>
class xlist 
{
private:
    std::list<T> data;
public:
    bool find(T x) 
    {
        for (T i : data)  
        {
            if (i == x) return true;
        }
        return false;
    }

    void ins(T x) 
    {
        data.push_back(x);
    }
    void rem(T x) 
    {
        data.remove(x);
    }

    void print() const
    {
        for ( T e : data)
        {
            std::cout << e << " -> ";
        }
        std::cout << '\n';
    }

    unsigned int size() const 
    { 
        return data.size(); 
    }
};

template<class T>
class xvector
{
private:
    std::vector<T> data;
public:
    bool find(const T& x) const
    {
        for (const T& i : data)
        {
            if (i == x) return true;
        }
        return false;
    }

    void ins(const T& x)
    {
        data.push_back(x);
    }

    void rem(const T& x)
    {
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            if (*it == x)
            {
                data.erase(it);
                break;
            }
        }
    }

    void print() const
    {
        for (const T& e : data)
        {
            std::cout << e << " -> ";
        }
        std::cout << '\n';
    }

    unsigned int size() const
    {
        return data.size();
    }
};

template<class T>
class xset
{
private:
    std::set<T> data;
public:
    bool find(const T& x) const
    {
        return data.find(x) != data.end();
    }

    void ins(const T& x)
    {
        data.insert(x);
    }

    void rem(const T& x)
    {
        data.erase(x); 
    }

    void print() const
    {
        for (const T& e : data)
        {
            std::cout << e << " -> ";
        }
        std::cout << '\n';
    }

    unsigned int size() const
    {
        return data.size();
    }
};

/* T es el tipo de dato, S es la estructura de datos donde se almacena las colisiones,
   F es un function-obtject que contiene a la función de dispersión, y Z es el tamaño de la tabla hash */
template<class T, class S, class F, unsigned int Z>
class CHashTable 
{
private:
    S* data;
    F hash_fn;
public:
    CHashTable() 
    {
        data = new S[Z];
    }

    ~CHashTable() 
    {
        delete[] data;
    }

    bool find(T x) 
    {
        unsigned int idx = hash_fn(x) % Z;
        return data[idx].find(x);
    }

    void ins(T x) 
    {
        unsigned int idx = hash_fn(x) % Z;
        data[idx].ins(x);
    }

    void rem(T x) {
        unsigned int idx = hash_fn(x) % Z;
        data[idx].rem(x);
    }

    void print() const 
    {
        for (unsigned int i = 0; i < Z; i++) 
        {
            std::cout << "[" << i << "]: ";
            data[i].print();
        }
    }

    double analizar_homogeneidad() const {
        double total_elementos = 0;
        std::cout << "\nConteo de colisiones por bucket:\n";

        for (unsigned int i = 0; i < Z; i++) {
            unsigned int colisiones = data[i].size();
            total_elementos += colisiones;
            std::cout << "  Bucket [" << i << "]: " << colisiones << " elementos.\n";
        }

        double media = total_elementos / Z;
        double suma_varianza = 0;

        for (unsigned int i = 0; i < Z; i++) {
            suma_varianza += std::pow(data[i].size() - media, 2);
        }

        return std::sqrt(suma_varianza / Z);
    }
};

int main() {

    std::string names[] = {
        "JAIME GABRIEL ANDRADE RIOS",
        "SERGIO DERZU APAZA CALIZAYA",
        "GABRIEL ALEJANDRO BACA FLORES",
        "OLGA ANGELICA BARREROS RODRIGUEZ",
        "RUTH MERY BENIQUE HUAMAN",
        "RODRIGO DIOGO EMANUEL BRAVO RIVERA",
        "GUSTAVO RAUL BUSTAMANTE MAMANI",
        "FERNANDO JOSE CAMAÑA CHAMPI",
        "SAMIR FABRIZIO CARRERA ALFARO",
        "ANA CECILIA CONCHA CASTRO",
        "MIGUEL ANGEL CONCHACALLA QUISPE",
        "FERNANDA RAQUEL CRUZ PONCE",
        "ALEX JESUS CUADROS VARGAS",
        "GUSTAVO GERARDO DELGADO UGARTE",
        "ARIANA CAMILA FAILOC CASTRO",
        "LUCIANA MILAGROS FARFAN BEGAZO",
        "IVAN ALEXANDROVICH GONZALES BEGAZO",
        "VICTOR SANTIAGO HUAMAN AMANQUI",
        "ANIBAL LAJO ATAYUPANQUI",
        "LEONARDO ANDRE LOAYZA PAUCAR",
        "MARICIELO KAREN LUNA CARPIO",
        "CAMILA ANDREA YANELA MANSILLA LUJAN",
        "KAEL ALEXANDER MAZZA CORDOVA",
        "ESTEBAN ANDRES MEDINA CHINO",
        "EDUARDO SEBASTIAN MENDOZA MAQUE",
        "JEAN PAUL MOLLAPAZA APAZA",
        "NAYEMY MURILLO RIVERA",
        "KAROLINE MISHELL PAREDES HALLASI",
        "ALVARO ALEJANDRO PATIÑO DELGADO",
        "MAURICIO JERSON PAYEHUANCA DIAZ",
        "ALYSON GISELY PEREZ FLORES",
        "MARY ANGELA PEREZ VEGA",
        "JULIO EDUARDO PINO CHARUN",
        "ÁNGEL FERNANDO PONCE MOLINA",
        "PIERO ESTEFANO PRADO MANRIQUE",
        "ALEJANDRO ADRIEL QUISPE MONTES",
        "ANDRE JAFETH REVILLA CALDERON",
        "NICOL FERNANDA ROCHA ALVIS",
        "EDGARD EDWARD RODRIGUEZ VILCAPAZA",
        "GIANELLA ARIANA ROSAS LIPA",
        "CRISTHIAN DANIEL ROSPIGLIOSI ESCALANTE",
        "ALEJANDRO ALFREDO SALAZAR GUILLEN",
        "CHRISTIAN GABRIEL SANA FLORES",
        "ILENNE DAMARIS SARAVIA APAZA",
        "RODRIGO ADOLFO SIERRA HUARACHA",
        "FABRICIO HECTOR STELMAN MELO",
        "BRYAN ARON VARGAS LLERENA",
        "GIANCARLO VILLACHICA CUENTAS",
        "ERICK DARVIS VILLACIS ESPINOZA",
        "MAURICIO SEBASTIAN VIRRUETA MARQUEZ"
    };

    const unsigned int size = sizeof(names) / sizeof(names[0]);

    CHashTable<std::string, xlist<std::string>, fn_djb2, size> names_table;
    //CHashTable<std::string, xvector<std::string>, fn_djb2, size> names_table;
    //CHashTable<std::string, xset<std::string>, fn_djb2, size> names_table;

    for (unsigned int i = 0; i < size; i++) {
        names_table.ins(names[i]);
    }

    names_table.print();

    //evaluar homogeneidad

    //calculamos otro size para ir probando
    const unsigned int table_size = 50;

    CHashTable<std::string, xlist<std::string>, fn_djb2, table_size> t_djb2;
    CHashTable<std::string, xlist<std::string>, fn_fnv1a, table_size> t_fnv1a;
    CHashTable<std::string, xlist<std::string>, fn_simple_sum, table_size> t_simple;

    for (unsigned int i = 0; i < size; i++) {
        t_djb2.ins(names[i]);
        t_fnv1a.ins(names[i]);
        t_simple.ins(names[i]);
    }

    std::cout << std::endl;
    std::cout << "EVALUANDO FUNCION 1: DJB2\n";
    double ds_djb2 = t_djb2.analizar_homogeneidad();
    std::cout << std::endl;
    std::cout << ">> Desviacion Estandar DJB2: " << ds_djb2 << "\n\n";

    std::cout << std::endl;
    std::cout << "EVALUANDO FUNCION 2: FNV-1a\n";
    double ds_fnv1a = t_fnv1a.analizar_homogeneidad();
    std::cout << std::endl;
    std::cout << ">> Desviacion Estandar FNV-1a: " << ds_fnv1a << "\n\n";
    
    std::cout << std::endl;
    std::cout << "EVALUANDO FUNCIÓN 3: SUMA SIMPLE\n";
    double ds_simple = t_simple.analizar_homogeneidad();
    std::cout << std::endl;
    std::cout << ">> Desviacion Estandar Suma Simple: " << ds_simple << "\n\n";

    if (ds_djb2 <= ds_fnv1a && ds_djb2 <= ds_simple) {
        std::cout << "La funcion DJB2 tiene la distribucion mas homogenea.\n";
    }
    else if (ds_fnv1a <= ds_djb2 && ds_fnv1a <= ds_simple) {
        std::cout << "La funcion FNV-1a tiene la distribucion mas homogenea.\n";
    }
    else {
        std::cout << "La funcion Suma Simple tiene la distribucion mas homogenea.\n";
    }

    return 0;
}