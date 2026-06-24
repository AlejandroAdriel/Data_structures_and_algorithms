/******************************************************************************

Sparse Matrix with templates

*******************************************************************************/

#include <iostream>

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

template <typename T, T obs>
struct SparseMatrix
{
	MatrixElement<T>** vh;
	MatrixElement<T>** vv;

	int nrows, ncolumns;

	SparseMatrix(int rows, int columns)
	{
	    
		nrows = rows;
		ncolumns = columns;

		vh = new MatrixElement<T>* [nrows];
		vv = new MatrixElement<T>* [ncolumns];

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

	SparseMatrix& operator=(const SparseMatrix& other)
	{
		if (this == &other) return *this;

		clear();

		nrows = other.nrows;
		ncolumns = other.ncolumns;

		vh = new MatrixElement<T>* [nrows];
		vv = new MatrixElement<T>* [ncolumns];

		for (MatrixElement<T>** i = vh; i < vh + nrows; i++) *(i) = nullptr;
		for (MatrixElement<T>** i = vv; i < vv + ncolumns; i++) *(i) = nullptr;

		for (MatrixElement<T>** i = other.vh; i < other.vh + other.nrows; i++)
		{
			MatrixElement<T>* curr = *i;
			while (curr)
			{
				set(curr->row, curr->col, curr->val);
				curr = curr->right;
			}
		}

		return *this; 
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
		if (x < 0 || x >= nrows || y < 0 || y >= ncolumns) return obs;

		MatrixElement<T>** h = nullptr;

		if (findh(x, y, h))
		{
			return (*h)->val;
		}

		return obs;
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
			if (val == obs)
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
			if (val != obs)
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
				std::cout << get(i, j) << "\t";
			}
			std::cout << std::endl;
		}
	}

};

int main()
{
    SparseMatrix<int, 1> Matrix(5, 5);
    
    Matrix(1,2) = 0;
    Matrix(1,4) = 1;
    Matrix.print();
    
	return 0;
}