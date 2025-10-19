#pragma once

#include <string>
#include <iosfwd>

#include <srcs/DynamicArray.hpp>

typedef unsigned int DigitType;

class BigInt {
public:
    BigInt();
    BigInt(const BigInt& other);
    BigInt& operator=(const BigInt& rhs);
    ~BigInt();

    // non-explicitでもいいけど
    explicit BigInt(long value);

    explicit BigInt(const std::string& str);

    void swap(BigInt& other);

    bool operator==(const BigInt& rhs) const;
    bool operator!=(const BigInt& rhs) const;
    bool operator<(const BigInt& rhs) const;
    bool operator<=(const BigInt& rhs) const;
    bool operator>(const BigInt& rhs) const;
    bool operator>=(const BigInt& rhs) const;

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

    std::string toString() const;

    bool isZero() const;

    bool isNegative() const;

private:
    DynamicArray<DigitType> _digits;
    bool _isNegative;
};

const BigInt operator+(const BigInt& lhs, const BigInt& rhs);
const BigInt operator-(const BigInt& lhs, const BigInt& rhs);
const BigInt operator*(const BigInt& lhs, const BigInt& rhs);
const BigInt operator/(const BigInt& lhs, const BigInt& rhs);
const BigInt operator%(const BigInt& lhs, const BigInt& rhs);

std::ostream& operator<<(std::ostream& os, const BigInt& num);
std::istream& operator>>(std::istream& is, BigInt& num);

void swap(BigInt& a, BigInt& b);
