/*DSA HW1- Matrix Class*/ 

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <new>
#include "matrix.h"
using namespace std;

/*mind that this is an incomplete matrix class
  due to the row exchange part of Gauss-Jordan process
  and may cause error while using matrix.inverse()*/
  
/*version 1.0.0*/

void row_reduction(int elements, double multiplier, double* by, double* to);
void row_multiplication(int, double, double*);

/*A main function for testing*/


/*int main()
{
  Matrix M1(3, 3);
  Matrix M2(3, 3);
  for(int i = 0; i < M1.row; i++)
  {
    for(int j = 0; j < M1.col; j++)
    {
      cin >> M1.array[i][j];
	}
  }
  
  for(int i = 0; i < M2.row; i++)
  {
    for(int j = 0; j < M2.col; j++)
    {
      cin >> M2.array[i][j];
	}
  }
  M2 = M2 / M2;
  
  for(int i = 0; i < M2.row; i++)
  {
    for(int j = 0; j < M2.col; j++)
    {
      cout << M2[i][j] << " ";
	}
	cout << "\n";
  }
  return 0;
}*/

Matrix::Matrix(const int& r, const int& c) //constructor 
{
  row = r;
  col = c;
  array = new double*[row];
  for(int i = 0; i < row; i++)
  {
    array[i] = new double[col];
  }

}

Matrix::Matrix(const Matrix& rhs) //copy constructor
{
  row = rhs.row;
  col = rhs.col;
  array = new double*[row];
  for(int i = 0; i < row; i++)
  {
    array[i] = new double[col];
    for(int j = 0; j < col; j++)
    {
      array[i][j] = rhs.array[i][j];
	}
  }
}

Matrix::~Matrix() //destructor
{
  for(int i = 0; i < row; i++)
  {
    delete [] array[i];
  }
  
  delete [] array;
}

double* & Matrix::operator [](const int& idx) const	// Enable the use of A[i][j] instead of A.array[i][j]
{
	return array[idx];
}

Matrix Matrix::operator =(const Matrix& rhs) // assignment operator
{
  if (this != &rhs)
  {
    for(int i = 0; i < this->row; i++)
    {
      delete [] array[i];
	}
	delete [] array;
	
	row = rhs.row;
	col = rhs.col;
	array = new double*[row];
    for(int i = 0; i < row; i++)
    {
      array[i] = new double[col];
      for(int j = 0; j < col; j++)
      {
        array[i][j] = rhs.array[i][j];
	  }
    }
    

  }
  
  return *this;
}

Matrix Matrix::operator -() const
{
  Matrix Result = *this;
  for(int i = 0; i < Result.row; i++)
  {
    for(int j = 0; j < Result.col; j++)
    {
      Result[i][j] = -this->array[i][j];
	}
  }
  return Result;
}

Matrix Matrix::operator +() const
{
  return Matrix(*this); //待確認 
}

Matrix Matrix::operator -(const Matrix& rhs) const
{
  Matrix Result(rhs.row, rhs.col);
  
  for(int i = 0; i < Result.row; i++)
  {
    for(int j = 0; j < Result.col; j++)
    {
      Result[i][j] = this->array[i][j] - rhs[i][j]; 
	}
  }
  
  return Result;
  
}

Matrix Matrix::operator +(const Matrix& rhs) const
{
  Matrix Result(rhs.row, rhs.col);
  
  for(int i = 0; i < Result.row; i++)
  {
    for(int j = 0; j < Result.col; j++)
    {
      Result[i][j] = this->array[i][j] + rhs[i][j]; 
	}
  }
  
  return Result;
}

Matrix Matrix::operator *(const Matrix& rhs) const
{
  Matrix Result(this->row, rhs.col);
  
  for(int i = 0; i < Result.row; i++)
  {
    for(int j = 0; j < Result.col; j++)
    {
      double total = 0;
      for(int k = 0; k < this->col; k++)
      {
        total += (this->array[i][k] * rhs[k][j]);
	  }
	  Result[i][j] = total;
	}
  }
  
  return Result;
}

Matrix Matrix::operator /(const Matrix& rhs) const
{
  Matrix Result(1, 1);
  Result = *this * rhs.inverse();
  return Result;
}

Matrix Matrix::inverse() const
{
  Matrix Inverse(this->row, this->col);
  Matrix Original(*this);
  for(int i = 0; i < Inverse.row; i++) // initializing inverse matrix
  {
    for(int j = 0; j < Inverse.col; j++)
    {
      Inverse[i][j] = 0;
	}
  }
  
  for(int i = 0; i < Inverse.row; i++)
  {
    Inverse[i][i] = 1; // turning inverse to an identity matrix
  }
  
  for(int j = 0; j < Inverse.row; j++)
  {
    //work on pivots
    if(Original[j][j] != 1)
    {
      row_multiplication(Inverse.row, 1 / Original[j][j], Inverse[j]); //下面一行會改到matrix的值，故此行需先執行 
      row_multiplication(Original.row, 1 / Original[j][j], Original[j]);
    }
    
    for(int i = 0; i < Inverse.row; i++)
    {
      if(i == j)
        continue;

      if(Original[i][j] != 0)
      {
        row_reduction(Inverse.row, -Original[i][j] / Original[j][j], Inverse[j], Inverse[i]); //下面一行會改到matrix的值，故需先改inverse 
        row_reduction(Original.row, -Original[i][j] / Original[j][j], Original[j], Original[i]);
	  }
	}
  }
  return Inverse;
}

void Matrix::read(const char* fn)
{
	int r, c;
	FILE *fp = fopen(fn, "r");
	if(fp == NULL){
		printf("read file [%s] error\n", fn);
		exit(0);
	}
	fscanf(fp, "%d%d", &r, &c);
	Matrix tmp(r, c);
	for(int i = 0 ; i < r ; i++)
		for(int j = 0 ; j < c ; j++)
			fscanf(fp, "%lf", &tmp.array[i][j]);
	fclose(fp);
	*this = tmp;
}

void Matrix::write(const char* fn)
{
	FILE *fp = fopen(fn, "w");
	if(fp == NULL){
		printf("write file [%s] error\n", fn);
		exit(0);
	}
	fprintf(fp, "%d %d\n", row, col);
	for(int i = 0 ; i < row ; i++)
		for(int j = 0 ; j < col ; j++)
			fprintf(fp, "%lf%c", array[i][j], " \n"[j==col-1]);
	fclose(fp);
}

void Matrix::print() const
{
	for(int i = 0 ; i < row ; i++)
		for(int j = 0 ; j < col ; j++)
			printf("%lf%c", array[i][j], " \n"[j==col-1]);
}

void row_reduction(int elements, double multiplier, double* by, double* to)
{
	for(int i = 0; i < elements; i++)
	{
    	to[i] += multiplier * by[i];
	}
}

void row_multiplication(int elements, double multiplier, double* to)
{
  for(int i = 0; i < elements; i++)
  {
    to[i] *= multiplier;
  }
}
