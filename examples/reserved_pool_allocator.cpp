#include "reserved_pool_allocator.hpp"

#include <chrono>
#include <cmath>
#include <exception>
#include <iostream>
#include <vector>

int main()
{
    try
    {
        int stack_variable; // A variable on the stack

        std::vector<int, containers::ReservedPoolAllocator<int, 40, containers::StackStorage>> stack_vector;

        // Reserve space for the maximum number of elements that the allocator can handle
        stack_vector.reserve(40);

        // Check if the vector is likely on the stack
        std::cout << "Address of stack variable: " << &stack_variable << std::endl;
        std::cout << "Address of vector's first element: " << stack_vector.data() << std::endl;

        if (std::abs(reinterpret_cast<char *>(&stack_variable) - reinterpret_cast<char *>(stack_vector.data())) <
            1024 * 1024)
        {
            std::cout << "The vector is likely on the stack." << std::endl;
        }
        else
        {
            std::cout << "The vector is likely not on the stack." << std::endl;
        }

        for (int i = 0; i < 10; ++i)
        {
            stack_vector.push_back(i);
        }

        std::cout << "Vector capacity after adding elements: " << stack_vector.capacity() << std::endl;

        for (auto &num : stack_vector)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        stack_vector.clear();
        std::cout << "Number of element after clearing: " << stack_vector.size() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!" << std::endl;
        return 2;
    }

    std::cout << std::endl;

    try
    {
        int stack_variable; // A variable on the stack

        std::vector<int, containers::ReservedPoolAllocator<int, 40, containers::HeapStorage>> heap_vector;

        // Reserve space for the maximum number of elements that the allocator can handle
        heap_vector.reserve(40);

        // Check if the vector is likely on the stack
        std::cout << "Address of stack variable: " << &stack_variable << std::endl;
        std::cout << "Address of vector's first element: " << heap_vector.data() << std::endl;

        if (std::abs(reinterpret_cast<char *>(&stack_variable) - reinterpret_cast<char *>(heap_vector.data())) <
            1024 * 1024)
        {
            std::cout << "The vector is likely on the stack." << std::endl;
        }
        else
        {
            std::cout << "The vector is likely not on the stack." << std::endl;
        }

        for (int i = 0; i < 10; ++i)
        {
            heap_vector.push_back(i);
        }

        std::cout << "Vector capacity after adding elements: " << heap_vector.capacity() << std::endl;

        for (auto &num : heap_vector)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        heap_vector.clear();
        std::cout << "Number of element after clearing: " << heap_vector.size() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!" << std::endl;
        return 2;
    }

    try
    {
        static constexpr std::size_t MAX_SIZE = 1000'000;

        const auto t1 = std::chrono::steady_clock::now();

        std::vector<double, containers::ReservedPoolAllocator<double, MAX_SIZE, containers::HeapStorage>> heap_vector;
        heap_vector.reserve(MAX_SIZE);
        for (int i = 0; i < MAX_SIZE; ++i)
        {
            heap_vector.push_back(static_cast<double>(i));
        }

        const auto t2 = std::chrono::steady_clock::now();

        std::vector<double> vector;
        vector.reserve(MAX_SIZE);
        for (int i = 0; i < MAX_SIZE; ++i)
        {
            vector.push_back(static_cast<double>(i));
        }

        const auto t3 = std::chrono::steady_clock::now();

        std::vector<double, containers::ReservedPoolAllocator<double, MAX_SIZE, containers::StackStorage>> stack_vector;
        stack_vector.reserve(MAX_SIZE);
        for (int i = 0; i < MAX_SIZE; ++i)
        {
            stack_vector.push_back(static_cast<double>(i));
        }

        const auto t4 = std::chrono::steady_clock::now();

        std::cout << std::endl;

        std::cout << "Allocation of heap_vector() [microsec]: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;

        std::cout << "Allocation of vector() [microsec]: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << std::endl;

        std::cout << "Allocation of stack_vector() [microsec]: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!" << std::endl;
        return 2;
    }

    return 0;
}