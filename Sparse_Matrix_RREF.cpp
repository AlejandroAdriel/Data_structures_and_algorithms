/******************************************************************************

Programa donde ingrese los coeficientes de x , de y y de z y que haga un SEL,
que lo resuelva por eliminacion gaussiana, hacerlo en una matriz
que sea una sparce matrix. sin sfml solo imprimir xd hacer un print para mostrar los elemenos de la sparse matrix

*******************************************************************************/
#include <iostream>
#include <cmath>

template <typename T>
struct MatrixElement
{
	T val;
	int row, col;
	MatrixElement* right;
	MatrixElement* down;

	MatrixElement(T VAL, int x = 0, int y = 0)
	{
		val = VAL;
		row = x;
		col = y;
		right = down = nullptr;
	}
};

template <typename T>
struct SparseMatrix
{
	MatrixElement<T>** vh;
	MatrixElement<T>** vv;

	int nrows, ncolumns;

	SparseMatrix(int rows, int columns)
	{
		nrows = rows;
		ncolumns = columns;

		vh = new MatrixElement<T>*[nrows];
		vv = new MatrixElement<T>*[ncolumns];

		for (MatrixElement<T>** i = vh; i < vh + rows; i++) *(i) = nullptr;
		for (MatrixElement<T>** i = vv; i < vv + columns; i++) *(i) = nullptr;
	}

	~SparseMatrix()
	{
		clear();
	}

	void clear()
	{
		for (MatrixElement<T>** i = vh; i < vh + nrows; i++)
		{
			MatrixElement<T>* curr = *i;
			while (curr)
			{
				MatrixElement<T>* t = curr->right;
				delete curr;
				curr = t;
			}
		}

		delete[] vh;
		delete[] vv;
	}

	bool findh(int x, int y, MatrixElement<T>**& h)
	{
		h = (vh + x);

		while (*h && (*h)->col < y)
		{
			h = &((*h)->right);
		}

		return (*h && (*h)->col == y);
	}

	bool findv(int x, int y, MatrixElement<T>**& v)
	{
		v = (vv + y);

		while (*v && (*v)->row < x)
		{
			v = &((*v)->down);
		}

		return (*v && (*v)->row == x);
	}

	void ins(int x, int y, MatrixElement<T>** h, MatrixElement<T>** v, T val)
	{
		MatrixElement<T>* t = new MatrixElement<T>(val, x, y);

		t->right = *h;
		*h = t;

		t->down = *v;
		*v = t;

		return;
	}

	bool rem(int x, int y, MatrixElement<T>** h, MatrixElement<T>** v)
	{
		MatrixElement<T>* t = *h;

		*h = t->right;

		*v = t->down;

		delete t;

		return true;
	}

	T get(int x, int y)
	{
		if (x < 0 || x >= nrows || y < 0 || y >= ncolumns) return T(0);

		MatrixElement<T>** h = nullptr;

		if (findh(x, y, h))
		{
			return (*h)->val;
		}

		return T(0);
	}

	void set(int x, int y, T val)
	{
		if (x < 0 || x >= nrows || y < 0 || y >= ncolumns) return;

		MatrixElement<T>** h = nullptr;
		MatrixElement<T>** v = nullptr;

		bool flag_h = findh(x, y, h);
		bool flag_v = findv(x, y, v);

		if (flag_h)
		{
			if (val == T(0))
			{
				rem(x, y, h, v);
			}
			else
			{
				(*h)->val = val;
			}
		}
		else
		{
			if (val != T(0))
			{
				ins(x, y, h, v, val);
			}
		}
	}

	struct objx
	{
		SparseMatrix* m;
		int x, y;

		objx(SparseMatrix* tm, int tx, int ty)
		{
			m = tm;
			x = tx;
			y = ty;
		}

		void operator = (T k)
		{
			m->set(x, y, k);
		}

		operator T() {
			return m->get(x, y);
		}
	};

	objx operator()(int x, int y)
	{
		return objx(this, x, y);
	}

	void print()
	{
		std::cout << std::endl;
		for (int i = 0; i < nrows; i++)
		{
			for (int j = 0; j < ncolumns; j++)
			{
				if (j == ncolumns - 1) std::cout << "  | ";
				std::cout << get(i, j) << "\t";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

};

void eliminacionGaussiana(SparseMatrix<double>& matrix)
{
	int n = matrix.nrows;

	std::cout << "\n--- MATRIZ INICIAL ---" << std::endl;
	matrix.print();

	for (int i = 0; i < n; i++)
	{
		if (matrix.get(i, i) == 0.0)
		{
			for (int k = i + 1; k < n; k++)
			{
				if (matrix.get(k, i) != 0.0)
				{
					for (int j = 0; j < matrix.ncolumns; j++)
					{
						double temp = matrix.get(i, j);
						matrix(i, j) = matrix.get(k, j);
						matrix(k, j) = temp;
					}
					std::cout << "-> Intercambio Fila " << i + 1 << " con Fila " << k + 1 << std::endl;
					matrix.print();
					break;
				}
			}
		}

		double pivote = matrix.get(i, i);
		if (pivote == 0.0) continue;

		for (int k = i + 1; k < n; k++)
		{
			double factor = matrix.get(k, i) / pivote;
			if (factor != 0.0)
			{
				std::cout << "-> Operacion: F" << k + 1 << " = F" << k + 1 << " - (" << factor << ") * F" << i + 1 << std::endl;
				for (int j = i; j < matrix.ncolumns; j++)
				{
					matrix(k, j) = matrix.get(k, j) - factor * matrix.get(i, j);
				}
				matrix.print();
			}
		}
	}

	double* soluciones = new double[n];
	for (int i = 0; i < n; i++) soluciones[i] = 0.0;

	for (int i = n - 1; i >= 0; i--)
	{
		double suma = matrix.get(i, matrix.ncolumns - 1);
		for (int j = i + 1; j < n; j++)
		{
			suma = suma - matrix.get(i, j) * soluciones[j];
		}
		if (matrix.get(i, i) == 0.0)
		{
			std::cout << "Sistema sin solucion unica." << std::endl;
			delete[] soluciones;
			return;
		}
		soluciones[i] = suma / matrix.get(i, i);
	}

	std::cout << "--- RESULTADOS ---" << std::endl;
	std::cout << std::endl;

	char variables[3] = { 'x', 'y', 'z' };

	for (int i = 0; i < n && i < 3; i++)
	{
		std::cout << variables[i] << " = " << soluciones[i] << std::endl;
	}

	delete[] soluciones;
}

int main()
{
	SparseMatrix<double> matrix(3, 4);


//		       X                     Y						 Z           |          R

	matrix(0, 0) = 3.0;		matrix(0, 1) = 2.0;		matrix(0, 2) = 9.0;		matrix(0, 3) = 11.0;
	matrix(1, 0) = 2.0;		matrix(1, 1) = 11.0;	matrix(1, 2) = 8.0;		matrix(1, 3) = 9.0;
	matrix(2, 0) = 4.0;		matrix(2, 1) = 8.0;		matrix(2, 2) = 16.0;	matrix(2, 3) = -2.0;

	eliminacionGaussiana(matrix);

	return 0;
}