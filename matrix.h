#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <string.h>
#include <stdio.h>
#include <malloc.h>  //sudo apt-get install valgrind
#include <iostream>
#include <stdlib.h>
using namespace std;

class CMatrix
{
public:
    class Cref1;
    class Cref2;
	class IndexOutOfRange{};
	class WrongDim{};
    Cref1 operator[] (int);
    CMatrix(int, int, double c=0, double d=0);       //d=parametr domyslny
    ~CMatrix();
    friend ostream& operator<<(ostream&, const CMatrix&);
    CMatrix& operator=(const CMatrix&);
    friend CMatrix operator*(const CMatrix&, const CMatrix&);
	double read(unsigned int, unsigned int) const;
	void write(unsigned int i, unsigned int j, double c);
private:
    struct rcmatrix;
	rcmatrix* data;
};

struct CMatrix::rcmatrix
{
	
    double **matrix;
    int wiersz;
    int kolumna;
    unsigned int n;

    rcmatrix(int a, int b, double c, double d)
{
    int k=0;
    int j=0;
    n=1;
    wiersz=a;
    kolumna=b;
    matrix=alloc(wiersz, kolumna);
    for(k=0;k<wiersz;k++) //wypelnianie wierszy i kolumn
    {
        for(j=0;j<kolumna;j++)
        {
            if(k!=j)matrix[k][j]=d;
            else matrix[k][j]=c;
        }
    }
}

	rcmatrix(const rcmatrix& s)
	{
		int i=0;
		int j=0;
		n=1;
		wiersz=s.wiersz;
		kolumna=s.kolumna;
		for(i=0; i<s.wiersz;i++)
		{
			for(j=0;j<s.kolumna;j++)
			{
				matrix[i][j]=s.matrix[i][j];
			}
		}
	}

double** alloc(int wiersz, int kolumna)
{
    int k=0;
    double **matrix=new double*[wiersz]; //alokaja wierszy
    for(k=0;k<wiersz;k++)
    {
        matrix[k]=new double[kolumna]; //alokacja kolumn
    }
    return matrix;
}

~rcmatrix()
{
    int i=0;
    for(i=0; i<wiersz; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

  rcmatrix* detach()
  {
    if(n==1)
      return this;
    rcmatrix* t=new rcmatrix(*this);
    n--;
    return t;
  };

};

CMatrix::CMatrix(int a, int b, double c, double d) //tworze macierz a=wiersze, b=kolumny
{
	data=new rcmatrix(a, b, c, d);
}


class CMatrix::Cref2
{
	friend class CMatrix;
	CMatrix& s;
	int i;	
	int j;


public:

Cref2(CMatrix& _s, int _i, int _j): s(_s), i(_i), j(_j) {};

	Cref2& operator = (double d)
	{
		s.write(i,j,d);
		return *this;
	};

 	operator double() const
	{
		return s.read(i,j);
	}
};

class CMatrix::Cref1
{
	friend class CMatrix;
	CMatrix& s;
	int i;

public:
	Cref1(CMatrix& _s, int _i): s(_s), i(_i) {};

	Cref2 operator[](int j)
	{
		if(j>s.data->kolumna) throw IndexOutOfRange();
		return Cref2(s, i, j);
	}
};

CMatrix::Cref1 CMatrix::operator[](int z)
{
	if(z>data->wiersz) throw IndexOutOfRange();
	return Cref1(*this, z);
}


CMatrix::~CMatrix() //destruktor zwalniajacy pamiec
{
	if(--data->n==0)
	delete data;
}


ostream& operator << (ostream& out, const CMatrix& x) //wyswietlanie macierzy
{
	int i=0;
	int j=0;
	for(i=0; i<x.data->wiersz; i++)
	{
		for(j=0; j<x.data->kolumna; j++)
		{
			out << x.read(i, j) << " "; 
		}
		out << endl; 
	}
	return out;
}

CMatrix& CMatrix::operator=(const CMatrix& x) //operator przypisania
{
	int i=0;
	int j=0;
	for(i=0; i<x.data->wiersz;i++)
	{
		for(j=0;j<x.data->kolumna;j++)
		{
			data->matrix[i][j]=x.data->matrix[i][j];
		}
	}
	return *this;
}

inline CMatrix operator * (const CMatrix& s1, const CMatrix& s2)
{
if(s1.data->kolumna!=s2.data->wiersz) throw CMatrix::WrongDim();
		CMatrix temp(s1.data->wiersz, s2.data->kolumna);
		for(int k = 0; k < s1.data->wiersz; k++)
		{
			for(int l = 0; l < s2.data->kolumna; l++)
				{
					for(int m = 0; m < s1.data->kolumna; m++)
					{
							temp.data->matrix[k][l] += s1.data->matrix[k][m] * s2.data->matrix[m][l];		
					} 
				}
		}
		return temp;
}

//fstream

inline double CMatrix::read(unsigned int i, unsigned int j) const
{
	return data->matrix[i][j];
}

inline void CMatrix::write(unsigned int i, unsigned int j, double c)
{
  data = data->detach();
  data->matrix[i][j] = c;
}


#endif /* __MATRIX_H__ */
