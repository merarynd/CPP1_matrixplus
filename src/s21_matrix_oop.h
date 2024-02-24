#ifndef S21_MATRIX_OOP_H_
#define S21_MATRIX_OOP_H_

#include <cmath>
#include <iostream>
#include <stdexcept>

class S21Matrix {
 public:
  //// Конструкторы и деструктор:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  ~S21Matrix();
  void Memory() {
    matrix_ = new double*[rows_];
    for (int i = 0; i < rows_; ++i) {
      matrix_[i] = new double[cols_];
    }
  }
  // Операции над матрицами:
  bool EqMatrix(const S21Matrix& other) const;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(double num) noexcept;
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  // Операторы :
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other) noexcept;
  bool operator==(const S21Matrix& other) const;
  S21Matrix operator+(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix operator*(double num) const noexcept;
  // Предусмотрен случай, где число умножается на матрицу: num * matrix
  S21Matrix operator*(const S21Matrix& other);
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(double num);
  S21Matrix& operator*=(const S21Matrix& other);
  double& operator()(int i, int j);
  double operator()(int i, int j) const;

  // Доп. функции:
  void PrintMatrix();
  void FillMatrix(double value);
  int GetRows() const noexcept;
  int GetCols() const noexcept;
  void SetRows(int rows);
  void SetCols(int cols);

 private:
  // Доп. функции:
  void Allocate();
  void Deallocate();
  S21Matrix Minor(int rows, int cols);
  void DelMatrix(double** matrix);
  void AlocMatrix(double*** matrix, int rows, int cols);

  int rows_ = {0};
  int cols_ = {0};
  double** matrix_ = nullptr;

  bool SquareMatrix() { return rows_ == cols_; }

  bool EqualColsRowsOfTwoMatrix(const S21Matrix& other) {
    return cols_ == other.rows_;
  }

  bool EqualSizeMatrix(const S21Matrix& other) const {
    return (cols_ == other.cols_ && rows_ == other.rows_);
  }
};

S21Matrix operator*(const double num, const S21Matrix& other) noexcept;

#endif  // S21_MATRIX_OOP_H_
