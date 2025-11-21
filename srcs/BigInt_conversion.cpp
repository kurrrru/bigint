#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

#include <srcs/BigInt.hpp>
#include <srcs/DynamicArray.hpp>
#include <toolbox/string.hpp>

namespace {

BigInt stringToBigIntRecursive(const std::string& str, std::size_t start, std::size_t end) {
    std::size_t len = end - start;
    if (len <= 9) {
        int val = 0;
        for (std::size_t i = start; i < end; ++i) {
            val = val * 10 + (str[i] - '0');
        }
        return BigInt(val);
    }
    std::size_t mid = start + len / 2;
    std::size_t lenLow = end - mid;
    BigInt high = stringToBigIntRecursive(str, start, mid);
    BigInt low = stringToBigIntRecursive(str, mid, end);
    return high * BigInt::pow(BigInt(10), lenLow) + low;
}

} // namespace

BigInt::BigInt(int value)
    : _digits(), _isNegative(false) {
    DigitType absValue;
    if (value >= 0) {
        absValue = static_cast<DigitType>(value);
    } else {
        _isNegative = true;
        absValue = -static_cast<DigitType>(value);
    }
    _digits.push_back(absValue);
}

BigInt::BigInt(const std::string& str)
    : _digits(), _isNegative(false) {
    if (str.empty()) {
        throw std::invalid_argument("Invalid string for BigInt: empty string");
    }
    std::size_t startIndex = 0;
    if (str[0] == '-') {
        _isNegative = true;
        startIndex = 1;
    } else if (str[0] == '+') {
        startIndex = 1;
    }
    if (startIndex == str.size()) {
        throw std::invalid_argument("Invalid string for BigInt: no digits");
    }
    for (std::size_t i = startIndex; i < str.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(str[i]))) {
            throw std::invalid_argument("Invalid string for BigInt: non-digit character");
        }
    }
    BigInt result = stringToBigIntRecursive(str, startIndex, str.size());
    _digits.swap(result._digits);
    if (isZero()) {
        _isNegative = false;
    }
}

std::string BigInt::toStringRecursive(const BigInt& n) const {
    if (n.size() <= 1) {
        if (n.isZero()) {
            return "0";
        }
        std::ostringstream oss;
        oss << n._digits[0];
        return oss.str();
    }
    std::size_t half_digits = n.size() * 4;
    BigInt powerOf10 = BigInt::pow(BigInt(10), half_digits);

    BigInt quotient, remainder;
    division_and_remainder(n, powerOf10, quotient, remainder);

    std::string high = toStringRecursive(quotient);
    std::string low = toStringRecursive(remainder);
    if (high == "0") {
        return low;
    }
    std::string padding;
    if (low.length() < half_digits) {
        padding.assign(half_digits - low.length(), '0');
    }
    return high + padding + low;
}

std::string BigInt::toString() const {
    if (isZero()) {
        return "0";
    }
    bool negative = isNegative();
    BigInt absVal = *this;
    if (negative) {
        absVal = -absVal; 
    }
    std::string str = toStringRecursive(absVal);
    if (negative) {
        return "-" + str;
    }
    return str;
}

std::istream& operator>>(std::istream& is, BigInt& bigint) {
    std::string str;
    is >> str;
    if (is) {
        try {
            bigint = BigInt(str);
        } catch (...) {
            is.setstate(std::ios::failbit);
        }
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    std::string str = num.toString();
    os << str;
    return os;
}
