#include <srcs/BigInt.hpp>
#include <srcs/DynamicArray.hpp>

namespace {

BigInt mask(std::size_t bits) {
    return (BigInt(1) << bits) - BigInt(1);
}

}

void BigInt::division_and_remainder(const BigInt& divided,
                                const BigInt& divisor,
                                BigInt& quotient,
                                BigInt& remainder) const {
    const std::size_t BZ_OFFSET_THRESHOLD = 4;
    if (divisor.isZero()) {
        throw std::runtime_error("Division by zero");
    }
    if (divided.abs() < divisor.abs()) {
        quotient = BigInt();
        remainder = divided;
        return;
    }
    if (divisor.size() < BZ_THRESHOLD ||
        (divided.size() - divisor.size() < BZ_OFFSET_THRESHOLD)) {
        schoolbook_division(divided, divisor, quotient, remainder);
        return;
    }

    std::size_t shift = divisor.clz();
    BigInt normalized_divided = divided.abs() << shift;
    BigInt normalized_divisor = divisor.abs() << shift;

    recursive_division(normalized_divided, normalized_divisor, quotient, remainder);

    remainder >>= shift;
    if (divided.isNegative() != divisor.isNegative()) {
        quotient._isNegative = true;
    }
    if (divided.isNegative()) {
        remainder._isNegative = true;
    }
    quotient.normalize();
    remainder.normalize();
}

void BigInt::recursive_division(const BigInt& divided,
                                const BigInt& divisor,
                                BigInt& quotient,
                                BigInt& remainder) const {
    std::size_t m = divided.size();
    std::size_t n = divisor.size();
    if (m < n * 2) {
        BigInt divided_pad = divided.pad_leading_zeros(n * 2);
        divide_2n_by_n(divided_pad, divisor, quotient, remainder);
    } else {
        BigInt divided_high = divided >> (n * BITS_PER_DIGIT);
        BigInt divided_low = divided & mask(n * BITS_PER_DIGIT);
        BigInt q_high, r_high;
        recursive_division(divided_high, divisor, q_high, r_high);

        BigInt combined = (r_high << (n * BITS_PER_DIGIT)) + divided_low;
        BigInt q_low;
        divide_2n_by_n(combined, divisor, q_low, remainder);
        quotient = (q_high << (n * BITS_PER_DIGIT)) + q_low;
    }
}

void BigInt::divide_2n_by_n(const BigInt& divided,
                            const BigInt& divisor,
                            BigInt& quotient,
                            BigInt& remainder) const {
    std::size_t n = divisor.size();
    if (n < BZ_THRESHOLD) {
        schoolbook_division(divided, divisor, quotient, remainder);
        return;
    }
    std::size_t n_half = n / 2;
    BigInt A1A2 = divided >> (n * BITS_PER_DIGIT);
    BigInt A3 = (divided >> (n_half * BITS_PER_DIGIT)) & mask(n_half * BITS_PER_DIGIT);
    BigInt A4 = divided & mask(n_half * BITS_PER_DIGIT);

    BigInt Q1, R1;
    divide_3n_by_2n(A1A2, A3, divisor, Q1, R1);
    BigInt Q2;
    divide_3n_by_2n(R1, A4, divisor, Q2, remainder);
    quotient = (Q1 << (n_half * BITS_PER_DIGIT)) + Q2;
    quotient.normalize();
    remainder.normalize();
}

void BigInt::divide_3n_by_2n(const BigInt& divided_high,
                            const BigInt& divided_low,
                            const BigInt& divisor,
                            BigInt& quotient,
                            BigInt& remainder) const {
    std::size_t n = divisor.size();
    std::size_t n_half = n / 2;

    BigInt B1 = divisor >> (n_half * BITS_PER_DIGIT);
    BigInt B2 = divisor & mask(n_half * BITS_PER_DIGIT);

    BigInt A1 = divided_high >> (n_half * BITS_PER_DIGIT);
    BigInt A2 = divided_high & mask(n_half * BITS_PER_DIGIT);

    BigInt R_high;
    if (A1 < B1) {
        divide_2n_by_n(divided_high, B1, quotient, R_high);
    } else {
        quotient = mask(n - n_half);
        R_high = divided_high + divided_low - (B1 << (n * BITS_PER_DIGIT)) + (B1 << (n_half * BITS_PER_DIGIT));
    }

    remainder = R_high - (quotient * B2);
    while (remainder.isNegative()) {
        remainder += divisor;
        quotient -= BigInt(1);
    }
}

BigInt BigInt::pad_leading_zeros(std::size_t new_size) const {
    BigInt result(*this);
    if (new_size > result.size()) {
        result._digits.resize(new_size);
    }
    return result;
}