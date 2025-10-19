#include <iostream>
#include <string>
#include <stdexcept>

#include <srcs/BigInt.hpp>
#include <srcs/DynamicArray.hpp>

BigInt& BigInt::operator+=(const BigInt& rhs) {
    // Implementation of addition assignment
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& rhs) {
    // Implementation of subtraction assignment
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
    // Implementation of multiplication assignment
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& rhs) {
    // Implementation of division assignment
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& rhs) {
    // Implementation of modulus assignment
    return *this;
}

BigInt BigInt::operator-() const {
    BigInt result(*this);
    if (!isZero()) {
        result._isNegative = !result._isNegative;
    }
    return result;
}

BigInt& BigInt::operator++() {
    *this += BigInt(1);
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt temp(*this);
    ++(*this);
    return temp;
}

BigInt& BigInt::operator--() {
    *this -= BigInt(1);
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt temp(*this);
    --(*this);
    return temp;
}

const BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
    BigInt result(lhs);
    result += rhs;
    return result;
}

const BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
    BigInt result(lhs);
    result -= rhs;
    return result;
}

const BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
    BigInt result(lhs);
    result *= rhs;
    return result;
}

const BigInt operator/(const BigInt& lhs, const BigInt& rhs) {
    BigInt result(lhs);
    result /= rhs;
    return result;
}

const BigInt operator%(const BigInt& lhs, const BigInt& rhs) {
    BigInt result(lhs);
    result %= rhs;
    return result;
}
