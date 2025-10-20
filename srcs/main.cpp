#include <srcs/BigInt.hpp>

#include <iostream>

int main() {
    {
        BigInt a("123456789012345678901234567890");
        std::cout << "a: " << a.toString() << std::endl;
    }
    {
        BigInt b(1);
        for (int i = 0; i < 10; ++i) {
            b *= BigInt(10000000);
            std::cout << "b: " << b.toString() << std::endl;
        }
        for (int i = 0; i < 10; ++i) {
            b /= BigInt(10000000);
            BigInt c = b % BigInt(10000000);
            std::cout << "b: " << b.toString() << ", c: " << c.toString() << std::endl;
        }
    }
    {
        BigInt c(-5);
        BigInt d(3);
        std::cout << c / d << ", " << c % d << std::endl;
        std::cout << -5 / 3 << ", " << -5 % 3 << std::endl;
    }
}