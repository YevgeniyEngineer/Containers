#include "reserved_pool_allocator.hpp"

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

    return 0;
}