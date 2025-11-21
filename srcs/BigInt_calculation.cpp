#include <algorithm>
#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include <srcs/BigInt.hpp>

BigInt& BigInt::operator+=(const BigInt& rhs) {
    if (rhs.isZero()) {
        return *this;
    }
    if (isZero()) {
        *this = rhs;
        return *this;
    }
    if (isNegative() == rhs.isNegative()) {
        bool carry = false;
        for (std::size_t i = 0; i < rhs._digits.size(); ++i) {
            if (i < _digits.size()) {
                _digits[i] += rhs._digits[i] + (carry ? 1 : 0);
                if (_digits[i] >= BASE) {
                    _digits[i] -= BASE;
                    carry = true;
                } else {
                    carry = false;
                }
            } else {
                _digits.push_back(rhs._digits[i] + (carry ? 1 : 0));
                if (_digits[i] >= BASE) {
                    _digits[i] -= BASE;
                    carry = true;
                } else {
                    carry = false;
                }
            }
        }
        std::size_t i = rhs._digits.size();
        while (carry && i < _digits.size()) {
            _digits[i] += 1;
            if (_digits[i] >= BASE) {
                _digits[i] -= BASE;
                carry = true;
            } else {
                carry = false;
            }
            ++i;
        }
        if (carry) {
            _digits.push_back(1);
        }
    } else {
        if (!isNegative()) {
            *this -= (-rhs);
        } else {
            BigInt abs_this = -(*this);
            *this = rhs;
            *this -= abs_this;
        }
    }
    normalize();
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& rhs) {
    if (rhs.isZero()) {
        return *this;
    }
    if (isZero()) {
        *this = -rhs;
        return *this;
    }
    if (isNegative() != rhs.isNegative()) {
        *this += -rhs;
    } else {
        if ((!isNegative() && *this < rhs) || (isNegative() && *this > rhs)) {
            BigInt temp = rhs;
            temp -= *this;
            *this = -temp;
        } else {
            bool borrow = false;
            for (std::size_t i = 0; i < rhs._digits.size(); ++i) {
                DigitType rhsDigit = rhs._digits[i] + (borrow ? 1 : 0);
                if (_digits[i] < rhsDigit) {
                    borrow = true;
                    _digits[i] += BASE;
                } else {
                    borrow = false;
                }
                _digits[i] -= rhsDigit;
            }
            std::size_t i = rhs._digits.size();
            while (borrow && i < _digits.size()) {
                if (_digits[i] == 0) {
                    _digits[i] = BASE - 1;
                    borrow = true;
                } else {
                    _digits[i] -= 1;
                    borrow = false;
                }
                ++i;
            }
        }
    }
    normalize();
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
    *this = karatsuba_multiply(*this, rhs);
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& rhs) {
    BigInt quotient, remainder;
    division_and_remainder(*this, rhs, quotient, remainder);
    *this = quotient;
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& rhs) {
    BigInt quotient, remainder;
    division_and_remainder(*this, rhs, quotient, remainder);
    *this = remainder;
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

BigInt BigInt::shift_block_left(std::size_t n) const {
    BigInt res = *this;
    if (isZero() || n == 0) {
        return res;
    }
    res._digits.insert(res._digits.begin(), n, 0);
    return res;
}

BigInt BigInt::extract_range(std::size_t start, std::size_t end) const {
    if (start >= _digits.size()) return BigInt(0);
    BigInt res;
    std::size_t actual_end = std::min(end, _digits.size());
    if (start < actual_end) {
        res._digits.assign(_digits.begin() + start, _digits.begin() + actual_end);
    }
    res.normalize();
    return res;
}


BigInt BigInt::scalar_mul(int s) const {
    if (s == 1) return *this;
    if (s == 0) return BigInt(0);
    BigInt res;
    res._digits.resize(_digits.size());
    long long carry = 0;
    for (std::size_t i = 0; i < _digits.size(); ++i) {
        long long cur = (long long)_digits[i] * s + carry;
        res._digits[i] = cur % BASE;
        carry = cur / BASE;
    }
    if (carry) res._digits.push_back(carry);
    res.normalize();
    return res;
}

void BigInt::add_abs(BigInt& a, const BigInt& b) {
    if (a._digits.size() < b._digits.size()) {
        a._digits.resize(b._digits.size(), 0);
    }
    long long carry = 0;
    for (size_t i = 0; i < b._digits.size() || carry; ++i) {
        if (i == a._digits.size()) a._digits.push_back(0);
        long long sum = carry + a._digits[i] + (i < b._digits.size() ? b._digits[i] : 0);
        if (sum >= BASE) {
            sum -= BASE;
            carry = 1;
        } else {
            carry = 0;
        }
        a._digits[i] = sum;
    }
    a.normalize();
}

void BigInt::sub_abs(BigInt& a, const BigInt& b) {
    long long borrow = 0;
    for (size_t i = 0; i < b._digits.size() || borrow; ++i) {
        long long sub = a._digits[i] - borrow - (i < b._digits.size() ? b._digits[i] : 0);
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a._digits[i] = sub;
    }
    a.normalize();
}

// =========================================================
// 乗算 (Karatsuba)
// =========================================================

BigInt BigInt::karatsuba_multiply(const BigInt& a, const BigInt& b) const {
    if (a.isZero() || b.isZero()) return BigInt(0);
    
    if (a._digits.size() < MULTIPLY_THRESHOLD || b._digits.size() < MULTIPLY_THRESHOLD) {
        return schoolbook_multiply(a, b);
    }

    std::size_t n = std::max(a._digits.size(), b._digits.size());
    std::size_t m = (n + 1) / 2;

    BigInt a_high = a.extract_range(m, a._digits.size());
    BigInt a_low = a.extract_range(0, m);
    BigInt b_high = b.extract_range(m, b._digits.size());
    BigInt b_low = b.extract_range(0, m);

    BigInt z2 = karatsuba_multiply(a_high, b_high);
    BigInt z0 = karatsuba_multiply(a_low, b_low);
    
    BigInt sum_a = a_high; sum_a += a_low; sum_a._isNegative = false;
    BigInt sum_b = b_high; sum_b += b_low; sum_b._isNegative = false;
    BigInt z1 = karatsuba_multiply(sum_a, sum_b);

    BigInt t = z1; t -= z2; t -= z0;

    BigInt res = z2.shift_block_left(2 * m);
    res += t.shift_block_left(m);
    res += z0;

    res._isNegative = (a._isNegative != b._isNegative);
    res.normalize();
    return res;
}

BigInt BigInt::schoolbook_multiply(const BigInt& a, const BigInt& b) const {
    if (a.isZero() || b.isZero()) return BigInt(0);
    
    BigInt res;
    res._digits.resize(a._digits.size() + b._digits.size(), 0);

    for (std::size_t i = 0; i < a._digits.size(); ++i) {
        long long carry = 0;
        for (std::size_t j = 0; j < b._digits.size(); ++j) {
            long long cur = res._digits[i + j] + 
                            (long long)a._digits[i] * b._digits[j] + carry;
            res._digits[i + j] = cur % BASE;
            carry = cur / BASE;
        }
        res._digits[i + b._digits.size()] += carry;
    }
    res._isNegative = (a._isNegative != b._isNegative);
    res.normalize();
    return res;
}

// =========================================================
// 除算 (Burnikel-Ziegler) - Burnikel-Ziegler Division
// =========================================================

void BigInt::division_and_remainder(const BigInt& divided,
                                const BigInt& divisor,
                                BigInt& quotient,
                                BigInt& remainder) const {
    if (divisor.isZero()) throw std::runtime_error("Division by zero");
    
    BigInt u = divided.abs();
    BigInt v = divisor.abs();

    if (u < v) {
        quotient = BigInt(0);
        remainder = divided;
        return;
    }
    
    int v_top = v._digits.back();
    int d = BASE / (v_top + 1);
    
    if (d > 1) {
        u = u.scalar_mul(d);
        v = v.scalar_mul(d);
    }

    recursive_division(u, v, quotient, remainder);

    if (d > 1) {
        long long rem = 0;
        for (int i = remainder._digits.size() - 1; i >= 0; --i) {
            long long cur = remainder._digits[i] + rem * BASE;
            remainder._digits[i] = cur / d;
            rem = cur % d;
        }
        remainder.normalize();
    }

    if (divided._isNegative != divisor._isNegative) {
        quotient._isNegative = true;
    }
    if (divided._isNegative) {
        remainder._isNegative = true;
    }
    if (quotient.isZero()) quotient._isNegative = false;
    if (remainder.isZero()) remainder._isNegative = false;
}

void BigInt::schoolbook_division(const BigInt& u_in, const BigInt& v_in, BigInt& q, BigInt& r) const {
    if (u_in < v_in) {
        q = BigInt(0);
        r = u_in;
        return;
    }
    
    std::size_t n = v_in._digits.size();
    std::size_t m = u_in._digits.size() - n;
    
    q._digits.assign(m + 1, 0);
    std::vector<DigitType> u = u_in._digits;
    u.push_back(0);
    
    long long v_top = v_in._digits.back();
    long long v_sec = (n > 1) ? v_in._digits[n - 2] : 0;

    for (int j = m; j >= 0; --j) {
        long long r_high = u[j + n];
        long long r_low = u[j + n - 1];
        unsigned __int128 dividend = ((unsigned __int128)r_high * BASE) + r_low;
        
        long long q_hat = dividend / v_top;
        long long r_hat = dividend % v_top;

        while (q_hat >= BASE || 
               (unsigned __int128)q_hat * v_sec > (unsigned __int128)r_hat * BASE + ((n > 1) ? u[j + n - 2] : 0)) {
            q_hat--;
            r_hat += v_top;
            if (r_hat >= BASE) break;
        }

        long long borrow = 0;
        long long carry = 0;
        for (size_t i = 0; i < n; ++i) {
            unsigned __int128 prod = (unsigned __int128)q_hat * v_in._digits[i] + carry;
            int prod_digit = prod % BASE;
            carry = prod / BASE;

            long long sub = (long long)u[j + i] - prod_digit - borrow;
            if (sub < 0) {
                sub += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }
            u[j + i] = sub;
        }
        
        long long sub = u[j + n] - carry - borrow;
        u[j + n] = sub;

        if (sub < 0) {
            q_hat--;
            long long add_carry = 0;
            for (size_t i = 0; i < n; ++i) {
                long long sum = u[j + i] + v_in._digits[i] + add_carry;
                u[j + i] = sum % BASE;
                add_carry = sum / BASE;
            }
            u[j + n] += add_carry;
        }
        q._digits[j] = q_hat;
    }
    q.normalize();
    r._digits = u;
    r.normalize();
}

void BigInt::recursive_division(const BigInt& a, const BigInt& b, BigInt& q, BigInt& r) const {
    std::size_t m = a._digits.size();
    std::size_t n = b._digits.size();

    if (n < DIVISION_THRESHOLD || m < n) {
        schoolbook_division(a, b, q, r);
        return;
    }
    
    if (n % 2 != 0) {
        schoolbook_division(a, b, q, r);
        return;
    }
    q._digits.clear();
    r = BigInt(0);
    
    std::size_t pos = m;    
    std::size_t first_chunk_len = m % n;
    if (first_chunk_len > 0) {
        BigInt chunk = a.extract_range(m - first_chunk_len, m);
        r = chunk; 
        pos -= first_chunk_len;
    }
    
    std::vector<BigInt> q_parts;
    
    while (pos >= n) {
        BigInt chunk = a.extract_range(pos - n, pos);
        pos -= n;

        BigInt current = r.shift_block_left(n);
        add_abs(current, chunk);
        
        BigInt q_part;
        divide_2n_by_n(current, b, q_part, r);
        q_parts.push_back(q_part);
    }

    q = BigInt(0);
    for (size_t i = 0; i < q_parts.size(); ++i) {
        q = q.shift_block_left(n);
        add_abs(q, q_parts[i]);
    }
}

void BigInt::divide_2n_by_n(const BigInt& a, const BigInt& b, BigInt& q, BigInt& r) const {
    std::size_t n = b._digits.size();
    std::size_t k = n / 2;

    BigInt a_high = a.extract_range(2 * k, a._digits.size());
    BigInt a_low = a.extract_range(0, 2 * k);
    
    BigInt a3 = a_low.extract_range(k, 2 * k);
    BigInt a4 = a_low.extract_range(0, k);
    
    BigInt d1 = a_high.shift_block_left(k);
    add_abs(d1, a3);

    BigInt q1, r1;
    divide_3n_by_2n(d1, b, q1, r1);

    BigInt d2 = r1.shift_block_left(k);
    add_abs(d2, a4);
    
    BigInt q2;
    divide_3n_by_2n(d2, b, q2, r);

    q = q1.shift_block_left(k);
    add_abs(q, q2);
}

void BigInt::divide_3n_by_2n(const BigInt& divided, const BigInt& divisor, BigInt& quotient, BigInt& remainder) const {
    std::size_t n = divisor._digits.size(); // 2k
    std::size_t k = n / 2;
    
    BigInt a1 = divided.extract_range(k, divided._digits.size());
    BigInt a2 = divided.extract_range(0, k);
    
    BigInt b1 = divisor.extract_range(k, n);
    BigInt b2 = divisor.extract_range(0, k);

    BigInt q_hat, r1;
    
    if (a1 < b1) {
        divide_2n_by_n(a1, b1, q_hat, r1);
    } else {
        schoolbook_division(a1, b1, q_hat, r1);
    }

    BigInt d = karatsuba_multiply(q_hat, b2);
    
    BigInt r_temp = r1.shift_block_left(k);
    add_abs(r_temp, a2);
    sub_abs(r_temp, d);

    while (r_temp._isNegative) {
        sub_abs(q_hat, BigInt(1));
        add_abs(r_temp, divisor);
    }
    BigInt r_curr = r1.shift_block_left(k);
    add_abs(r_curr, a2);
    
    while (r_curr < d) {
        sub_abs(q_hat, BigInt(1));
        add_abs(r_curr, divisor);
    }
    sub_abs(r_curr, d);

    quotient = q_hat;
    remainder = r_curr;
}

BigInt BigInt::pow(const BigInt& base, std::size_t exp) {
    if (exp == 0) {
        return BigInt(1);
    }
    if (exp == 1) {
        return base;
    }
    BigInt result(1);
    BigInt current_base = base;
    std::size_t current_exp = exp;
    while (current_exp > 0) {
        if (current_exp & 1) {
            result *= current_base;
        }
        current_base *= current_base;
        current_exp >>= 1;
    }
    return result;
}