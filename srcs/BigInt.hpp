#pragma once

#include <string>
#include <iostream>

#include <srcs/DynamicArray.hpp>


class BigInt {
public:
    typedef unsigned int DigitType;
    // BigInt_basic.cpp
    BigInt();
    BigInt(const BigInt& other);
    BigInt& operator=(const BigInt& rhs);
    ~BigInt();
    void swap(BigInt& other);
    bool isZero() const;
    bool isNegative() const;

    // BigInt_calculation.cpp
    BigInt& operator+=(const BigInt& rhs);  // not implemented yet
    BigInt& operator-=(const BigInt& rhs);  // not implemented yet
    BigInt& operator*=(const BigInt& rhs);  // not implemented yet
    BigInt& operator/=(const BigInt& rhs);  // not implemented yet
    BigInt& operator%=(const BigInt& rhs);  // not implemented yet
    BigInt operator-() const;
    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);

    // BigInt_comparison.cpp
    bool operator==(const BigInt& rhs) const;
    bool operator!=(const BigInt& rhs) const;
    bool operator<(const BigInt& rhs) const;
    bool operator<=(const BigInt& rhs) const;
    bool operator>(const BigInt& rhs) const;
    bool operator>=(const BigInt& rhs) const;

    // BigInt_conversion.cpp
    explicit BigInt(int value);
    explicit BigInt(const std::string& str);  // not implemented yet
    std::string toString() const;  // not implemented yet

private:
    DynamicArray<DigitType> _digits;
    bool _isNegative;
    static const std::size_t bits = sizeof(DigitType) * 8;
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

