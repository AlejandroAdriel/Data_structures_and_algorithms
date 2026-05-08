/******************************************************************************

NOTACION POLACA

hacerlo con un deque
usar una cola y una pila para armar el result
el result meterlo en una pila
operar con una pila

https://www.algorithmroom.com/dsa/infix-prefix-and-postfix-conversion-in-stack

3+4*2/(1-5)
barrido de izquierda a derecha

result[3,4,2,*,1]

stack[+,1,(,-,5,]

result[3,4,2,*,1,5,-,7,+]

transformar en notacion polaca luego meterlo en una pila

+ , - , *, /, (, )

reglas:

If the character is an operand (number or variable):
-Add it directly to the result list.

If the character is an opening parenthesis (:
-Push it onto the stack.

If the character is a closing parenthesis ):
-Pop from the stack and add to the result list until an opening parenthesis ( is encountered. Discard the opening parenthesis.

If the character is an operator (e.g., +, -, *, /):
-While the operator at the top of the stack has greater precedence or the same precedence (and is left-associative), pop it from the stack and add to the result.
-Push the current operator onto the stack.

*******************************************************************************/
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

template <typename T>
struct cDeque{
    unsigned long NELEM_MAPA;
    unsigned long NELEM;
    unsigned long SIZE_MAP;
    unsigned long SIZE_PAGE;
    T** MAP;
    T** mHEAD;
    T** mTAIL;
    T* pHEAD;
    T* pTAIL;

    cDeque(unsigned long mSIZE, unsigned long pSIZE);
    ~cDeque();

    T& operator[](unsigned long INDICE);
    bool push_back(T VAL);
    bool pop_back();
    bool push_front(T VAL);
    bool pop_front();
    bool expand_map();
    void print();
};

template <typename T>
cDeque<T>::cDeque(unsigned long mSIZE, unsigned long pSIZE) {
    NELEM = 0;
    NELEM_MAPA = 0;
    SIZE_MAP = mSIZE;
    SIZE_PAGE = pSIZE;
    MAP = new T * [SIZE_MAP];
    mHEAD = mTAIL = nullptr;
    pHEAD = pTAIL = nullptr;
}

template <typename T>
cDeque<T>::~cDeque() {
    if (mHEAD != nullptr) {
        T** p = mHEAD;
        while (p < mTAIL) {
            delete[](*p);
            p++;
        }
    }
    delete[] MAP;
}

template <typename T>
T& cDeque<T>::operator [](unsigned long INDICE) {
    assert(INDICE < NELEM);

    unsigned long offset = (pHEAD - *mHEAD) + INDICE;
    T** p = mHEAD + (offset / SIZE_PAGE);

    return *(*p + (offset % SIZE_PAGE));
}


template <typename T>
bool cDeque<T>::expand_map() {
    unsigned long new_size = (SIZE_MAP * 2) + 1;
    T** new_map = new T * [new_size];
    unsigned long num_paginas = mTAIL - mHEAD;

    unsigned long off_h = pHEAD - *mHEAD;
    unsigned long off_t = pTAIL - *(mTAIL - 1);

    T** n_mHEAD = new_map + (new_size - num_paginas) / 2;

    T** p = mHEAD;
    T** q = n_mHEAD;
    while (p < mTAIL) {
        *q = *p;
        p++;
        q++;
    }

    delete[] MAP;
    MAP = new_map;
    SIZE_MAP = new_size;
    mHEAD = n_mHEAD;
    mTAIL = n_mHEAD + num_paginas;


    return true;
}

template <typename T>
bool cDeque<T>::push_back(T VAL) {
    if (!mHEAD) {
        mHEAD = mTAIL = MAP + (SIZE_MAP / 2);
        *mTAIL = new T[SIZE_PAGE];
        mTAIL++;
        NELEM_MAPA++;
        pHEAD = pTAIL = (*(mTAIL - 1)) + (SIZE_PAGE / 2);
        *pTAIL = VAL;
        pTAIL++;
    }
    else if (pTAIL == *(mTAIL - 1) + SIZE_PAGE) {
        if (mTAIL == MAP + SIZE_MAP) expand_map();
        *mTAIL = new T[SIZE_PAGE];
        pTAIL = *mTAIL;
        *pTAIL = VAL;
        pTAIL++;
        mTAIL++;
        NELEM_MAPA++;
    }
    else {
        *pTAIL = VAL;
        pTAIL++;
    }
    NELEM++;
    return true;
}

template <typename T>
bool cDeque<T>::push_front(T VAL) {
    if (!mHEAD) {
        mHEAD = mTAIL = MAP + (SIZE_MAP / 2);
        *mHEAD = new T[SIZE_PAGE];
        mTAIL++;
        NELEM_MAPA++;
        pHEAD = *mHEAD + (SIZE_PAGE / 2);
        pTAIL = pHEAD + 1;
        *pHEAD = VAL;
    }
    else if (pHEAD == *mHEAD) {
        if (mHEAD == MAP) expand_map();
        mHEAD--;
        *mHEAD = new T[SIZE_PAGE];
        NELEM_MAPA++;
        pHEAD = *mHEAD + (SIZE_PAGE - 1);
        *pHEAD = VAL;
    }
    else {
        pHEAD--;
        *pHEAD = VAL;
    }
    NELEM++;
    return true;
}

template <typename T>
bool cDeque<T>::pop_back() {
    if (NELEM == 0) return false;

    pTAIL--;
    NELEM--;

    if (NELEM > 0 && pTAIL == *(mTAIL - 1)) {
        mTAIL--;
        delete[](*mTAIL);
        pTAIL = *(mTAIL - 1) + SIZE_PAGE;
        NELEM_MAPA--;
    }
    else if (NELEM == 0) {
        delete[](*mHEAD);
        mHEAD = mTAIL = nullptr;
        pHEAD = pTAIL = nullptr;
    }
    return true;
}

template <typename T>
bool cDeque<T>::pop_front() {
    if (NELEM == 0) return false;

    pHEAD++;
    NELEM--;

    if (NELEM > 0 && pHEAD == *mHEAD + SIZE_PAGE) {
        T* temp = *mHEAD;
        mHEAD++;
        pHEAD = *mHEAD;
        delete[] temp;
        NELEM_MAPA--;
    }
    else if (NELEM == 0) {
        delete[](*mHEAD);
        mHEAD = mTAIL = nullptr;
        pHEAD = pTAIL = nullptr;
    }

    return true;
}

template <typename T>
void cDeque<T>::print() {
    if (NELEM == 0) {
        std::cout << "vacio" << std::endl;
        return;
    }
    T** m = mHEAD;
    T* p = pHEAD;
    std::cout << "[ ";
    while (m < mTAIL) {
        T* page_end = (m == (mTAIL - 1)) ? pTAIL : (*m + SIZE_PAGE);
        while (p < page_end) {
            std::cout << *p << " ";
            p++;
        }
        m++;
        if (m < mTAIL) p = *m;
    }
    std::cout << "]" << std::endl;
}

template <typename T>
struct cStack {
    cDeque<T> d;

    cStack() : d(5, 10) {}

    void push(T val) {
        d.push_back(val);
    }

    void pop() {
        d.pop_back();
    }

    T& top() {
        return d[d.NELEM - 1];
    }

    bool empty() {
        return d.NELEM == 0;
    }

    unsigned long size() {
        return d.NELEM;
    }
};

template <typename T>
struct cQueue {
    cDeque<T> d;

    cQueue() : d(5, 10) {}

    void push(T val) {
        d.push_back(val);
    }

    void pop() {
        d.pop_front();
    }

    T& front() {
        return d[0];
    }

    T& back() {
        return d[d.NELEM - 1];
    }

    bool empty() {
        return d.NELEM == 0;
    }

    unsigned long size() {
        return d.NELEM;
    }
};

struct Notacion_Polaca {

    string ORIGINAL;
    string CONVERSION;
    int RESULTADO; 

    Notacion_Polaca() : ORIGINAL(""), CONVERSION(""), RESULTADO(0)
    {
        cout << "Ingrese su operacion: ";
        getline(cin, ORIGINAL);
    }

    void convertir();
    void operar();

    bool isOperator(string s) {
        if (s.length() != 1) return false;

        char c = s[0];
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    bool isOperand(string s) {
        return (s[0] >= '0' && s[0] <= '9');
    }

    int precedence(string s) {
        if (s.length() != 1) return 0;

        char c = s[0];
        if (c == '*' || c == '/') return 2;
        if (c == '+' || c == '-') return 1;
        return 0;
    }

    bool isCharDigit(char c) {
        return (c >= '0' && c <= '9');
    }

};

void Notacion_Polaca::convertir() {

    cQueue<string> ToConvert;

    for (int i = 0; i < ORIGINAL.length(); i++) {
        if (ORIGINAL[i] == ' ') continue;

        if (isCharDigit(ORIGINAL[i])) {
            string numero = "";
            while (i < ORIGINAL.length() && isCharDigit(ORIGINAL[i])) {
                numero += ORIGINAL[i];
                i++;
            }
            ToConvert.push(numero);
            i--;
        }
        else {
            string op(1, ORIGINAL[i]);
            ToConvert.push(op);
        }
    }

    cStack<string> Stack;
    cQueue<string> Result;

    while (!ToConvert.empty()) {
        string token = ToConvert.front();
        ToConvert.pop();

        if (isOperand(token)) {
            Result.push(token);

        }

        else if (isOperator(token)) {
            while (!Stack.empty() && precedence(Stack.top()) >= precedence(token) && Stack.top() != "(" ) {
                Result.push(Stack.top());
                Stack.pop();
            }
      
            Stack.push(token);
        }

        else if (token == "(") {
            Stack.push(token);
        }

        else if (token == ")") { 
            while (Stack.top() != "(") {
                Result.push(Stack.top());
                Stack.pop();
            }

            Stack.pop();
        }
    }

    while (!Stack.empty()) {
        Result.push(Stack.top());
        Stack.pop();
    }

    while (!Result.empty()) {
        CONVERSION += Result.front() + " ";
        Result.pop();
    }
    cout << "Notacion Polaca: " << CONVERSION << endl;
}

void Notacion_Polaca::operar() {
    cStack<int> Result;

    for (int i = 0; i < CONVERSION.length(); i++) {
        if (CONVERSION[i] == ' ') continue;

        if (isCharDigit(CONVERSION[i])) {
            string numero = "";
            while (i < CONVERSION.length() && isCharDigit(CONVERSION[i])) {
                numero += CONVERSION[i];
                i++;
            }
            Result.push(stoi(numero));
            i--;
        }
        else {
            string op(1, CONVERSION[i]);
            if (isOperator(op)) {
                int n2 = Result.top(); Result.pop();
                int n1 = Result.top(); Result.pop();
                
                char o = op[0];
                
                switch (o) {
                case '+': Result.push(n1 + n2); break;
                case '-': Result.push(n1 - n2); break;
                case '*': Result.push(n1 * n2); break;
                case '/':
                    if (n2 != 0) Result.push(n1 / n2);
                    else cout << "Error";
                    break;
                }
            }
        }
    }

    if (!Result.empty()) {
        RESULTADO = Result.top();
        cout << "Resultado de la operacion: " << RESULTADO << endl;
    }
}

int main()
{
    Notacion_Polaca p;
    p.convertir();
    p.operar();

    return 0;
}