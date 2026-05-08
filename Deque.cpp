/******************************************************************************

DEQUE

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

int main() {

    return 0;
}