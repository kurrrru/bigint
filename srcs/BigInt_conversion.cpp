#include <algorithm>
#include <iostream>
#include <iomanip> // std::setw, std::setfill のため
#include <sstream> // std::stringstream のため
#include <stdexcept>
#include <string>
#include <vector>

#include <srcs/BigInt.hpp>

namespace {

int parse_substring_with_stream(const std::string& str, std::size_t start, std::size_t len) {
    std::string sub = str.substr(start, len);
    std::stringstream ss(sub);
    int val;
    if (!(ss >> val)) {
        throw std::invalid_argument("BigInt conversion error: failed to parse substring via stringstream");
    }
    return val;
}

}  // namespace

BigInt::BigInt(int value)
    : _digits(), _isNegative(false) {
    if (value < 0) {
        _isNegative = true;
        unsigned int absVal = static_cast<unsigned int>(-(static_cast<long long>(value)));
        
        if (absVal == 0) {
            _isNegative = false;
            _digits.push_back(0);
        } else {
            while (absVal > 0) {
                _digits.push_back(absVal % 1000000000);
                absVal /= 1000000000;
            }
        }
    } else {
        unsigned int v = static_cast<unsigned int>(value);
        if (v == 0) {
            _digits.push_back(0);
        } else {
            while (v > 0) {
                _digits.push_back(v % 1000000000);
                v /= 1000000000;
            }
        }
    }
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
    while (startIndex < str.size() - 1 && str[startIndex] == '0') {
        startIndex++;
    }
    for (int i = (int)str.size(); i > (int)startIndex; i -= 9) {
        int start = i - 9;
        int len = 9;
        if (start < (int)startIndex) {
            len = i - startIndex;
            start = startIndex;
        }
        _digits.push_back(parse_substring_with_stream(str, start, len));
    }
    if (_digits.empty()) {
        _digits.push_back(0);
        _isNegative = false;
    }
    normalize();
}

std::string BigInt::toString() const {
    if (isZero()) {
        return "0";
    }
    std::ostringstream oss;
    if (_isNegative) {
        oss << '-';
    }
    oss << _digits.back();
    for (int i = (int)_digits.size() - 2; i >= 0; --i) {
        oss << std::setw(9) << std::setfill('0') << _digits[i];
    }
    return oss.str();
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
    os << num.toString();
    return os;
}