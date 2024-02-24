#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() : rows_(1), cols_(1) {
  rows_ = 0;
  cols_ = 0;
  matrix_ = nullptr;
}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  Allocate();
}

S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  this->Allocate();
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

S21Matrix::S21Matrix(S21Matrix &&other) noexcept {
  matrix_ = other.matrix_;
  rows_ = other.rows_;
  cols_ = other.cols_;
  other.matrix_ = nullptr;
  other.cols_ = 0;
  other.rows_ = 0;
}

S21Matrix::~S21Matrix() { Deallocate(); }

bool S21Matrix::EqMatrix(const S21Matrix &other) const {
  bool res = true;
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    res = false;
  } else {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        if (std::fabs(matrix_[i][j] - other.matrix_[i][j]) >= 1e-7) {
          res = false;
        }
      }
    }
  }
  return res;
}

void S21Matrix::SumMatrix(const S21Matrix &other) {
  if (EqualSizeMatrix(other)) {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        matrix_[i][j] += other.matrix_[i][j];
      }
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix &other) {
  if (EqualSizeMatrix(other)) {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        matrix_[i][j] -= other.matrix_[i][j];
      }
    }
  }
}

void S21Matrix::MulNumber(double num) noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (EqualColsRowsOfTwoMatrix(other)) {
    S21Matrix res(rows_, other.cols_);
    for (int i = 0; i < res.rows_; ++i) {
      for (int j = 0; j < res.cols_; ++j) {
        for (int k = 0; k < cols_; ++k) {
          res.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
        }
      }
    }
    *this = std::move(res);
  }
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix result(cols_, rows_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      result.matrix_[j][i] = matrix_[i][j];
    }
  }
  return result;
}

S21Matrix S21Matrix::CalcComplements() {
  S21Matrix result(rows_, cols_);
  if (SquareMatrix()) {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j != cols_; ++j) {
        S21Matrix minor_matrix = Minor(i, j);
        result.matrix_[i][j] =
            std::pow((-1), i + j) * minor_matrix.Determinant();
      }
    }
  }
  return result;
}

double S21Matrix::Determinant() {
  double result = 0.0;
  if (SquareMatrix()) {
    if (rows_ == 1) {
      result = matrix_[0][0];
    } else if (rows_ == 2) {
      result = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
    } else {
      for (int j = 0; j < cols_; ++j) {
        S21Matrix minor_matrix = Minor(0, j);
        result += matrix_[0][j] * pow(-1, j) * minor_matrix.Determinant();
      }
    }
  }
  return result;
}

S21Matrix S21Matrix::Minor(int row, int col) {
  S21Matrix result(rows_ - 1, cols_ - 1);
  for (int i = 0, min_i = 0; min_i < result.rows_; ++min_i) {
    if (row == i) ++i;
    for (int j = 0, min_j = 0; min_j < result.cols_; ++min_j) {
      if (col == j) ++j;
      result.matrix_[min_i][min_j] = matrix_[i][j];
      ++j;
    }
    ++i;
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() {
  double det = Determinant();
  if (std::fabs(det) < 1e-7) {
    throw std::invalid_argument("Matrix determinant must be > 0.");
  }
  S21Matrix result(rows_, cols_);
  if (rows_ == 1) {
    result(0, 0) = 1 / matrix_[0][0];
  } else {
    S21Matrix temp = CalcComplements();
    result = temp.Transpose();
    result.MulNumber(1 / det);
  }
  return result;
}

// Операторы :

S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this != &other) {
    Deallocate();
    rows_ = other.rows_;
    cols_ = other.cols_;
    this->Allocate();
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
  return *this;
}

S21Matrix &S21Matrix::operator=(S21Matrix &&other) noexcept {
  if (this != &other) {
    Deallocate();
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
    std::swap(matrix_, other.matrix_);
  }
  return *this;
}

bool S21Matrix::operator==(const S21Matrix &other) const {
  return EqMatrix(other);
}

S21Matrix S21Matrix::operator+(const S21Matrix &other) {
  S21Matrix resultMatrix(*this);
  resultMatrix.SumMatrix(other);
  return resultMatrix;
}

S21Matrix S21Matrix::operator-(const S21Matrix &other) {
  S21Matrix resultMatrix(*this);
  resultMatrix.SubMatrix(other);
  return resultMatrix;
}

S21Matrix S21Matrix::operator*(double num) const noexcept {
  S21Matrix resultMatrix(*this);
  resultMatrix.MulNumber(num);
  return resultMatrix;
}

S21Matrix operator*(const double num, const S21Matrix &other) noexcept {
  return other * num;
}

S21Matrix S21Matrix::operator*(const S21Matrix &other) {
  S21Matrix resultMatrix(*this);
  resultMatrix.MulMatrix(other);
  return resultMatrix;
}

S21Matrix &S21Matrix::operator+=(const S21Matrix &other) {
  SumMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator-=(const S21Matrix &other) {
  SubMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator*=(double num) {
  MulNumber(num);
  return *this;
}

S21Matrix &S21Matrix::operator*=(const S21Matrix &other) {
  MulMatrix(other);
  return *this;
}

double &S21Matrix::operator()(int i, int j) {
  if (i < 0 || j < 0 || i >= rows_ || j >= cols_) {
    throw std::out_of_range("Index is out of the matrix range");
  }
  return matrix_[i][j];
}

double S21Matrix::operator()(int i, int j) const {
  if (i < 0 || j < 0 || i >= rows_ || j >= cols_) {
    throw std::out_of_range("Index is out of the matrix range");
  }
  return matrix_[i][j];
}

// Доп. функции:

int S21Matrix::GetRows() const noexcept { return rows_; }

int S21Matrix::GetCols() const noexcept { return cols_; }

void S21Matrix::SetRows(int rows) {
  double **newMatrix;
  AlocMatrix(&newMatrix, rows, cols_);
  for (int i = 0; i < rows && i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      newMatrix[i][j] = (*this)(i, j);
    }
  }
  DelMatrix(matrix_);
  rows_ = rows;
  matrix_ = newMatrix;
}

void S21Matrix::SetCols(int cols) {
  double **newMatrix;
  AlocMatrix(&newMatrix, rows_, cols);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols && j < cols_; j++) {
      newMatrix[i][j] = (*this)(i, j);
    }
  }
  DelMatrix(matrix_);
  cols_ = cols;
  matrix_ = newMatrix;
}

void S21Matrix::AlocMatrix(double ***matrix, int rows, int cols) {
  // if (rows == 1 && cols == 1) {
  //   throw std::invalid_argument("Invalid number of rows " +
  //                               std::to_string(rows));
  // }
  if (rows < 0 || cols < 0) {
    throw std::invalid_argument("Invalid number of cols " +
                                std::to_string(cols));
  }
  *matrix = new double *[rows]();
  for (int i = 0; i < rows; i++) {
    (*matrix)[i] = new double[cols]();
  }
}

void S21Matrix::Allocate() {
  if (rows_ < 1 || cols_ < 1) {
    throw std::invalid_argument("Matrix size cannot be less than 1x1");
  }
  matrix_ = new double *[rows_]();
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_]();
  }
}

void S21Matrix::Deallocate() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; ++i) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
    rows_ = 0;
    cols_ = 0;
  }
}

void S21Matrix::DelMatrix(double **matrix) {
  for (int i = 0; i < rows_; i++) {
    delete[] matrix[i];
  }
  delete[] matrix;
}
