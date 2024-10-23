#pragma once
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

class BigInt {
 public:
  BigInt();
  BigInt(int64_t number);
  BigInt(const std::string& str_to_copy);
  BigInt(const BigInt& src);
  friend bool ModuloLess(const BigInt& left, const BigInt& right);
  friend bool ModuloEqual(const BigInt& left, const BigInt& right);
  BigInt& operator=(const BigInt& src);
  friend bool operator<(const BigInt& first, const BigInt& second);
  friend bool operator>(const BigInt& left, const BigInt& right);
  friend bool operator<=(const BigInt& left, const BigInt& right);
  friend bool operator>=(const BigInt& left, const BigInt& right);
  friend bool operator==(const BigInt& left, const BigInt& right);
  friend bool operator!=(const BigInt& left, const BigInt& right);
  friend BigInt operator+(const BigInt& left, const BigInt& right);
  friend BigInt operator-(const BigInt& left, const BigInt& right);
  friend BigInt operator*(const BigInt& left, const BigInt& right);
  friend BigInt operator/(const BigInt& left, BigInt& right);
  friend BigInt operator%(const BigInt& left, BigInt& right);
  BigInt& operator+=(const BigInt& src);
  BigInt& operator-=(const BigInt& src);
  BigInt& operator*=(const BigInt& src);
  BigInt& operator/=(BigInt& src);
  BigInt& operator%=(BigInt& src);
  BigInt& operator--();
  BigInt& operator++();
  BigInt operator--(int);
  BigInt operator++(int);
  friend BigInt& operator-(BigInt& dst);
  void ShiftToRight();
  friend int64_t BinSearch(BigInt& right_copy, BigInt& current);
  friend BigInt SumOfPositive(const BigInt& left, const BigInt& right);
  friend BigInt DifferenceOfPositive(const BigInt& left, const BigInt& right);
  friend bool SmallerSize(const BigInt& left, const BigInt& right);
  friend std::istream& operator>>(std::istream& is, BigInt& number);
  friend std::ostream& operator<<(std::ostream& os, const BigInt& number);
  void MinusZero();
  void RemovingZeroes();
  friend bool ModuloSmaller(const BigInt& left, const BigInt& right);

 private:
  std::vector<int64_t> big_int_;
  int16_t sign_;
  const int16_t kUnitSize = 9;
  const int kMod = 1e9;
};
