#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <utility>

class BigInt {
 public:
    typedef unsigned int DigitType;
    // BigInt_basic.cpp
    BigInt();
    BigInt(const BigInt& other);
    BigInt& operator=(const BigInt& rhs);
    ~BigInt();
    void swap(BigInt& other);
    void normalize();
    bool isZero() const;
    bool isNegative() const;
    std::size_t size() const;
    BigInt abs() const;

    // BigInt_calculation.cpp
    BigInt& operator+=(const BigInt& rhs);
    BigInt& operator-=(const BigInt& rhs);
    BigInt& operator*=(const BigInt& rhs);
    BigInt& operator/=(const BigInt& rhs);
    BigInt& operator%=(const BigInt& rhs);
    BigInt operator-() const;
    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);
    static BigInt pow(const BigInt& base, std::size_t exp);

    // BigInt_comparison.cpp
    bool operator==(const BigInt& rhs) const;
    bool operator!=(const BigInt& rhs) const;
    bool operator<(const BigInt& rhs) const;
    bool operator<=(const BigInt& rhs) const;
    bool operator>(const BigInt& rhs) const;
    bool operator>=(const BigInt& rhs) const;

    // BigInt_conversion.cpp
    explicit BigInt(int value);
    explicit BigInt(const std::string& str);
    std::string toString() const;

 private:
    std::vector<DigitType> _digits;
    bool _isNegative;
    static const unsigned int BASE = 1000000000;
    static const std::size_t MULTIPLY_THRESHOLD = 10;
    static const std::size_t DIVISION_THRESHOLD = 70;

    BigInt karatsuba_multiply(const BigInt& a, const BigInt& b) const;
    BigInt schoolbook_multiply(const BigInt& a, const BigInt& b) const;
    void schoolbook_division(const BigInt& divided,
                                const BigInt& divisor,
                                BigInt& quotient,
                                BigInt& remainder) const;
    void division_and_remainder(const BigInt& divided,
                                const BigInt& divisor,
                                BigInt& quotient,
                                BigInt& remainder) const;
    void recursive_division(const BigInt& divided,
                                const BigInt& divisor,
                                BigInt& quotient,
                                BigInt& remainder) const;
    void divide_2n_by_n(const BigInt& divided,
                        const BigInt& divisor,
                        BigInt& quotient,
                        BigInt& remainder) const;
    void divide_3n_by_2n(const BigInt& divided_high,
                        const BigInt& divisor,
                        BigInt& quotient,
                        BigInt& remainder) const;

    // help functions
    BigInt shift_block_left(std::size_t n) const;
    BigInt extract_range(std::size_t start, std::size_t end) const;
    BigInt scalar_mul(int s) const;
    static void add_abs(BigInt& a, const BigInt& b);
    static void sub_abs(BigInt& a, const BigInt& b);
};

// BigInt_basic.cpp
void swap(BigInt& a, BigInt& b);

// BigInt_calculation.cpp
const BigInt operator+(const BigInt& lhs, const BigInt& rhs);
const BigInt operator-(const BigInt& lhs, const BigInt& rhs);
const BigInt operator*(const BigInt& lhs, const BigInt& rhs);
const BigInt operator/(const BigInt& lhs, const BigInt& rhs);
const BigInt operator%(const BigInt& lhs, const BigInt& rhs);

// BigInt_conversion.cpp
std::ostream& operator<<(std::ostream& os, const BigInt& num);
std::istream& operator>>(std::istream& is, BigInt& num);

