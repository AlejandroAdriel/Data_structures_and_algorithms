/******************************************************************************

Sparse Matrix

*******************************************************************************/

#include <iostream>

struct MatrixElement
{
	int val;
	int row, col;
	MatrixElement* right;
	MatrixElement* down;

	MatrixElement(int VAL, int x = 0, int y = 0)
	{
		val = VAL;
		row = x;
		col = y;
		right = down = nullptr;
	}
};

struct SparseMatrix
{
	MatrixElement** vh;
	MatrixElement** vv;

	int nrows, ncolumns;

	SparseMatrix(int rows, int columns)
	{
		nrows = rows;
		ncolumns = columns;


		vh = new MatrixElement * [nrows];
		vv = new MatrixElement * [ncolumns];

		for (MatrixElement** i = vh; i < vh + rows; i++) *(i) = nullptr;
		for (MatrixElement** i = vv; i < vv + columns; i++) *(i) = nullptr;
	}

	~SparseMatrix()
	{
		clear();
	}

	void clear() 
	{
		for (MatrixElement** i = vh; i < vh + nrows; i++)
		{
			MatrixElement* curr = *i;
			while (curr)
			{
				MatrixElement* t = curr->right;
				delete curr;
				curr = t;
			}
		}

		delete[] vh;
		delete[] vv;
	}

	SparseMatrix& operator=(const SparseMatrix& other)
	{
		if (this == &other) return *this;

		clear();

		nrows = other.nrows;
		ncolumns = other.ncolumns;

		vh = new MatrixElement * [nrows];
		vv = new MatrixElement * [ncolumns];

		for (MatrixElement** i = vh; i < vh + nrows; i++) *(i) = nullptr;
		for (MatrixElement** i = vv; i < vv + ncolumns; i++) *(i) = nullptr;

		for (MatrixElement** i = other.vh; i < other.vh + other.nrows; i++)
		{
			MatrixElement* curr = *i;
			while (curr)
			{
				set(curr->row, curr->col, curr->val);
				curr = curr->right;
			}
		}

		return *this; 
	}

	bool findh(int x, int y, MatrixElement**& h)
	{
		h = (vh + x);

		while (*h && (*h)->col < y)
		{
			h = &((*h)->right);
		}

		return (*h && (*h)->col == y);
	}

	bool findv(int x, int y, MatrixElement**& v)
	{
		v = (vv + y);

		while (*v && (*v)->row < x)
		{
			v = &((*v)->down); 
		}

		return (*v && (*v)->row == x);
	}

	void ins(int x, int y, MatrixElement** h, MatrixElement** v, int val)
	{
		MatrixElement* t = new MatrixElement(val, x, y);

		t->right = *h;
		*h = t;

		t->down = *v;
		*v = t;

		return;
	}

	bool rem(int x, int y, MatrixElement** h, MatrixElement** v)
	{
		MatrixElement* t = *h;

		*h = t->right;

		*v = t->down;

		delete t;

		return true;
	}

	int get(int x, int y)
	{
		if (x < 0 || x >= nrows || y < 0 || y >= ncolumns) return 0;

		MatrixElement** h = nullptr;

		if (findh(x, y, h))
		{
			return (*h)->val;
		}

		return 0;
	}

	void set(int x, int y, int val)
	{
		if (x < 0 || x >= nrows || y < 0 || y >= ncolumns) return;

		MatrixElement** h = nullptr;
		MatrixElement** v = nullptr;

		bool flag_h = findh(x, y, h);
		bool flag_v = findv(x, y, v);

		if (flag_h)
		{
			if (val == 0)
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
			if (val != 0)
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

		void operator = (int k)
		{
			m->set(x, y, k);
		}

		operator int() {
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
				std::cout << get(i, j) << "\t";
			}
			std::cout << std::endl;
		}
	}

};

int main()
{
	SparseMatrix mat1(4, 4);

	std::cout << "\n Insertando..." << std::endl;
	mat1(3, 3) = 40; 
	mat1(0, 0) = 10;
	mat1(2, 2) = 30; 
	mat1(1, 1) = 20; 
	mat1(0, 3) = 15; 
	mat1(3, 0) = 35; 
	mat1.print();

	std::cout << "\n Modificando..." << std::endl;
	mat1(0, 0) = 99;
	mat1(3, 3) = 11;
	mat1.print();

	std::cout << "\n Eliminando..." << std::endl;
	mat1(1, 1) = 0; 
	mat1(0, 0) = 0; 
	mat1.print();

	std::cout << "\n Operador de Asignacion (mat2 = mat1)..." << std::endl;
	SparseMatrix mat2(2, 2); 
	mat2(0, 0) = 1;
	mat2(1, 1) = 2;

	mat2 = mat1; 
	std::cout << "Matriz 2 :" << std::endl;
	mat2.print();

	std::cout << "\n Modificando Matriz 2..." << std::endl;
	mat2(2, 2) = 555;
	std::cout << "Matriz 1 original :" << std::endl;
	mat1.print();
	std::cout << "Matriz 2 modificada :" << std::endl;
	mat2.print();

	std::cout << "\n Sometiendo a auto-asignacion (mat1 = mat1)..." << std::endl;
	mat1 = mat1;
	mat1.print();

	std::cout << "\n Verificacion de limites..." << std::endl;
	mat1(10, 10) = 100; 
	int fuera = mat1(-1, 2); 
	std::cout << "Lectura en (-1,2): " << fuera << " (Esperado: 0)" << std::endl;

	return 0;
}