#include <iostream>
#include <stdexcept>
#include <string>

#include <srcs/BigInt.hpp>
#include <srcs/DynamicArray.hpp>

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

}

std::string BigInt::toString() const {
    if (isZero()) {
        return "0";
    }

    std::string str;
    if (_isNegative) {
        str += '-';
    }

    for (std::size_t i = 0; i < _digits.size(); ++i) {
        str += std::to_string(_digits[i]);
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
