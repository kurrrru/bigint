#include <srcs/DynamicArray.hpp>

#include <iostream>

int main() {
    DynamicArray<int> arr;
    for (int i = 0; i < 10; ++i) {
        arr.push_back(i);
    }

    std::cout << "Array contents: ";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    arr.pop_back();
    std::cout << "After pop_back, size: " << arr.size() << std::endl;

    arr.clear();
    std::cout << "After clear, size: " << arr.size() << std::endl;

    return 0;
}