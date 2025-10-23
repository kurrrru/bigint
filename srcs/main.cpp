#include <iostream>

#include <srcs/BigInt.hpp>

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
            std::cout << "b: " << b.toString()
                << ", c: " << c.toString() << std::endl;
        }
    }
    {
        BigInt c(-5);
        BigInt d(3);
        std::cout << c / d << ", " << c % d << std::endl;
        std::cout << -5 / 3 << ", " << -5 % 3 << std::endl;
    }
    {
        BigInt e(1);
        for (int i = 0; i < 50; ++i) {
            e *= BigInt(3);
        }
        // this should be 717897987691852588770249 (3^50 by wolframalpha)
        std::cout << "e: " << e.toString() << std::endl;
        e *= e;
        // this should be 515377520732011331036461129765621272702107522001
        // (3^100 by wolframalpha)
        std::cout << "e^2: " << e.toString() << std::endl;
    }
    {
        BigInt f("9999999999999999999999999999999999999999999999999999999");
        BigInt g("33554432");
        BigInt h = f / g;
        BigInt i = f % g;
        std::cout << "f / g: " << h.toString() << std::endl;
        std::cout << "f % g: " << i.toString() << std::endl;
    }
    {
        BigInt j("1234567890123456789012345678901234567890696986976986989879"
            "878979876978666666666666666666666666987970777988888888888888888"
            "8807099999999");
        BigInt k("9876543210987654321098765432100708709070709709790709809809"
            "908097070709809780978996976768986798786876968769");
        BigInt l = j * k;
        std::cout << "j * k: " << l.toString() << std::endl;
        l /= k;
        std::cout << "l / k: " << l.toString() << std::endl;
        l /= j;
        std::cout << "l / j: " << l.toString() << std::endl;
    }
}
