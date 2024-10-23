#include "big_integer.hpp"

bool SmallerSize(const BigInt& left, const BigInt& right) {
  return left.big_int_.size() < right.big_int_.size();
}

bool ModuloLess(const BigInt& left, const BigInt& right) {
  for (int i = static_cast<int>(left.big_int_.size()) - 1; i >= 0; --i) {
    if (left.big_int_[i] < right.big_int_[i]) {
      return true;
    }
    if (left.big_int_[i] > right.big_int_[i]) {
      return false;
    }
  }
  return false;
}

bool ModuloEqual(const BigInt& left, const BigInt& right) {
  for (int i = static_cast<int>(left.big_int_.size()) - 1; i >= 0; --i) {
    if (left.big_int_[i] < right.big_int_[i]) {
      return false;
    }
    if (left.big_int_[i] > right.big_int_[i]) {
      return false;
    }
  }
  return true;
}

bool operator<(const BigInt& first, const BigInt& second) {
  BigInt left = first;
  BigInt right = second;
  left.MinusZero();
  right.MinusZero();
  if (left.sign_ != right.sign_) {
    return left.sign_ < right.sign_;
  }
  if (SmallerSize(left, right)) {
    return true;
  }
  if (left.big_int_.size() == right.big_int_.size()) {
    if (left.sign_ == -1) {
      if (ModuloEqual(left, right)) {
        return false;
      }
    }
    return left.sign_ == -1 ? !ModuloLess(left, right)
                            : ModuloLess(left, right);
  }
  return false;
}

bool operator>(const BigInt& left, const BigInt& right) { return right < left; }

bool operator<=(const BigInt& left, const BigInt& right) {
  return !(left > right);
}

bool operator>=(const BigInt& left, const BigInt& right) {
  return !(left < right);
}

bool operator==(const BigInt& left, const BigInt& right) {
  return !(left > right) && !(left < right);
}

bool operator!=(const BigInt& left, const BigInt& right) {
  return (left > right) || (left < right);
}

BigInt::BigInt() { sign_ = 1; }

BigInt::BigInt(int64_t number) {
  if (number < 0) {
    sign_ = -1;
    number = -number;
  } else {
    sign_ = 1;
  }
  if (number == 0) {
    big_int_.push_back(number);
  }
  while (number != 0) {
    int64_t unit = llabs(number % kMod);
    number /= kMod;
    big_int_.push_back(unit);
  }
}

BigInt::BigInt(const std::string& str_to_copy) {
  if (str_to_copy.empty()) {
    big_int_.push_back(0);
    sign_ = 1;
    return;
  }
  std::string str = str_to_copy;
  sign_ = str[0] == '-' ? -1 : 1;
  while (static_cast<int16_t>(str.size()) > kUnitSize) {
    int64_t unit_index = static_cast<int64_t>(str.size()) - kUnitSize;
    big_int_.push_back(std::stoi(str.substr(unit_index, kUnitSize)));
    str.resize(str.size() - kUnitSize);
  }
  if ((str[0] == '-' || str[0] == '+') && str.size() > 1) {
    big_int_.push_back(std::stoi(str.substr(1)));
  } else if (str[0] != '-' && str[0] != '+') {
    big_int_.push_back(std::stoi(str));
  }
}

BigInt::BigInt(const BigInt& src) : sign_(src.sign_), big_int_(src.big_int_) {}

BigInt& BigInt::operator=(const BigInt& src) {
  BigInt tmp(src);
  sign_ = src.sign_;
  big_int_.swap(tmp.big_int_);
  return *this;
}

void BigInt::MinusZero() {
  if (big_int_.size() == 1 && big_int_[0] == 0) {
    sign_ = 1;
  }
}

BigInt SumOfPositive(const BigInt& left, const BigInt& right) {
  BigInt answer;
  int64_t remainder = 0;
  int64_t smallest_size = static_cast<int64_t>(
      std::min(left.big_int_.size(), right.big_int_.size()));
  int64_t biggest_size = static_cast<int64_t>(
      std::max(left.big_int_.size(), right.big_int_.size()));
  BigInt biggest = left < right ? right : left;
  for (int64_t i = 0; i < smallest_size; ++i) {
    answer.big_int_.push_back(
        (left.big_int_[i] + right.big_int_[i] + remainder) % answer.kMod);
    remainder =
        (left.big_int_[i] + right.big_int_[i] + remainder) / answer.kMod;
  }
  for (int64_t i = smallest_size; i < biggest_size; ++i) {
    answer.big_int_.push_back((biggest.big_int_[i] + remainder) % answer.kMod);
    remainder = (biggest.big_int_[i] + remainder) / answer.kMod;
  }
  if (remainder != 0) {
    answer.big_int_.push_back(remainder);
  }
  return answer;
}

BigInt DifferenceOfPositive(const BigInt& left, const BigInt& right) {
  BigInt answer;
  int64_t smallest_size = static_cast<int64_t>(
      std::min(left.big_int_.size(), right.big_int_.size()));
  int64_t biggest_size = static_cast<int64_t>(
      std::max(left.big_int_.size(), right.big_int_.size()));
  BigInt biggest = left < right ? right : left;
  BigInt smallest = biggest == left ? right : left;
  answer.sign_ = left == biggest ? 1 : -1;
  for (int64_t i = 0; i < smallest_size; ++i) {
    if (biggest.big_int_[i] < smallest.big_int_[i]) {
      biggest.big_int_[i] += biggest.kMod;
      --biggest.big_int_[i + 1];
    }
    answer.big_int_.push_back((biggest.big_int_[i] - smallest.big_int_[i]) %
                              answer.kMod);
  }
  for (int64_t i = smallest_size; i < biggest_size; ++i) {
    if (biggest.big_int_[i] < 0) {
      biggest.big_int_[i] += biggest.kMod;
      --biggest.big_int_[i + 1];
    }
    answer.big_int_.push_back((biggest.big_int_[i]) % answer.kMod);
  }
  answer.RemovingZeroes();
  answer.MinusZero();
  return answer;
}

BigInt& BigInt::operator+=(const BigInt& src) {
  if (sign_ == src.sign_) {
    *this = SumOfPositive(*this, src);
    if (src.sign_ == -1) {
      this->sign_ = -1;
    }
    return *this;
  }
  BigInt src_copy = src;
  src_copy.sign_ = 1;
  BigInt this_copy = *this;
  this_copy.sign_ = 1;
  int16_t this_sign = this->sign_;
  *this = DifferenceOfPositive(this_copy, src_copy);
  this->sign_ = src_copy > this_copy ? src.sign_ : this_sign;
  return *this;
}

BigInt& BigInt::operator-=(const BigInt& src) {
  BigInt copy = src;
  copy.sign_ = src.sign_ == -1 ? 1 : -1;
  *this = *this + copy;
  return *this;
}

BigInt operator+(const BigInt& left, const BigInt& right) {
  BigInt result = left;
  result += right;
  return result;
}

BigInt operator-(const BigInt& left, const BigInt& right) {
  BigInt result = left;
  result -= right;
  return result;
}

BigInt operator*(const BigInt& left, const BigInt& right) {
  BigInt result;
  int64_t is_null;
  result.big_int_.assign(left.big_int_.size() + right.big_int_.size() + 1, 0);
  for (size_t i = 0; i < left.big_int_.size(); ++i) {
    int64_t reminder = 0;
    for (size_t j = 0; j < right.big_int_.size() || reminder != 0; ++j) {
      is_null = 0;
      if (j < right.big_int_.size()) {
        is_null = right.big_int_[j];
      }
      result.big_int_[i + j] += left.big_int_[i] * is_null + reminder;
      reminder = result.big_int_[i + j] / left.kMod;
      result.big_int_[i + j] %= left.kMod;
    }
  }
  result.RemovingZeroes();
  if (left.sign_ == right.sign_) {
    result.sign_ = 1;
  } else {
    result.sign_ = -1;
  }
  return result;
}

BigInt& BigInt::operator*=(const BigInt& src) {
  *this = *this * src;
  return *this;
}

void BigInt::ShiftToRight() {
  if (big_int_.empty()) {
    big_int_.push_back(0);
    return;
  }
  big_int_.push_back(big_int_[big_int_.size() - 1]);
  for (size_t i = big_int_.size() - 2; i > 0; --i) {
    big_int_[i] = big_int_[i - 1];
  }
  big_int_[0] = 0;
}

int64_t BinSearch(BigInt& right_copy, BigInt& current) {
  int64_t quotient = 0;
  int64_t begin = 0;
  int64_t end = right_copy.kMod;
  while (begin <= end) {
    int64_t middle = (begin + end) / 2;
    BigInt tmp = right_copy * middle;
    if (tmp <= current) {
      quotient = middle;
      begin = middle + 1;
    } else {
      end = middle - 1;
    }
  }
  return quotient;
}

bool ModuloSmaller(const BigInt& left, const BigInt& right) {
  BigInt left_copy = left;
  BigInt right_copy = right;
  left_copy.sign_ = 1;
  right_copy.sign_ = 1;
  return left_copy < right_copy;
}

BigInt operator/(const BigInt& left, BigInt& right) {
  if (right == 0) {
    std::cerr << "division by zero?\n";
    exit(1);
  }
  if (ModuloSmaller(left, right)) {
    return 0;
  }
  BigInt right_copy = right;
  right_copy.sign_ = 1;
  BigInt answer;
  BigInt current;
  answer.big_int_.assign(left.big_int_.size(), 0);
  for (auto i = static_cast<int64_t>(left.big_int_.size() - 1); i >= 0; --i) {
    current.ShiftToRight();
    current.big_int_[0] = left.big_int_[i];
    current.RemovingZeroes();
    int64_t quotient = BinSearch(right_copy, current);
    answer.big_int_[i] = quotient;
    current = current - right_copy * quotient;
  }
  if (left.sign_ == right.sign_) {
    answer.sign_ = 1;
  } else {
    answer.sign_ = -1;
  }
  answer.RemovingZeroes();
  return answer;
}

BigInt operator%(const BigInt& left, BigInt& right) {
  BigInt result = left - (left / right) * right;
  result.RemovingZeroes();
  return result;
}

BigInt& BigInt::operator%=(BigInt& src) {
  *this = *this % src;
  return *this;
}

BigInt& BigInt::operator/=(BigInt& src) {
  *this = *this / src;
  return *this;
}

BigInt BigInt::operator++(int) {
  BigInt copy = *this;
  ++*this;
  return copy;
}

BigInt& BigInt::operator++() {
  BigInt one = 1;
  *this += one;
  this->RemovingZeroes();
  return *this;
}

BigInt BigInt::operator--(int) {
  BigInt copy = *this;
  --*this;
  return copy;
}

BigInt& BigInt::operator--() {
  BigInt one = 1;
  *this -= one;
  this->RemovingZeroes();
  return *this;
}

BigInt& operator-(BigInt& dst) {
  if (dst != 0) {
    dst.sign_ = dst.sign_ == 1 ? -1 : 1;
  }
  return dst;
}

std::istream& operator>>(std::istream& is, BigInt& number) {
  std::string string_to_scan;
  is >> string_to_scan;
  number = BigInt(string_to_scan);
  return is;
}

std::ostream& operator<<(std::ostream& os, const BigInt& number) {
  if (number.sign_ == -1) {
    os << '-';
  }
  if (number.big_int_.empty()) {
    return os << 0;
  }
  os << number.big_int_.back();
  for (auto i = static_cast<int64_t>(number.big_int_.size() - 2); i >= 0; i--) {
    const int64_t kNumber = number.big_int_[i];
    os << std::setw(number.kUnitSize) << std::setfill('0') << kNumber;
  }
  return os;
}

void BigInt::RemovingZeroes() {
  while (big_int_.size() > 1 && big_int_.back() == 0) {
    big_int_.erase(big_int_.end() - 1);
  }
  if (big_int_.size() == 1 && big_int_[0] == 0) {
    sign_ = 1;
  }
}
