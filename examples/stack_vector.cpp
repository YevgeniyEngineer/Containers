#include "stack_vector.hpp"
#include "resource_management_type.hpp"

#include <iostream>

int main()
{
    containers::StackVector<ResourceManagingType, 5> stack_vector{};

    // Adding elements
    stack_vector.emplace_back("Hello");
    stack_vector.emplace_back("World");
    stack_vector.emplace_back("StackVector");

    // Forward iteration
    std::cout << "Forward iteration: ";
    for (const auto &element : stack_vector)
    {
        std::cout << element.getData() << " ";
    }
    std::cout << std::endl;

    // Reverse iteration
    std::cout << "Reverse iteration: ";
    for (auto rit = stack_vector.rbegin(); rit != stack_vector.rend(); ++rit)
    {
        std::cout << (*rit).getData() << " ";
    }
    std::cout << std::endl;

    // Random index access
    std::cout << "Element at index 1: " << stack_vector[1].getData() << std::endl;

    // Using 'at' for safe index access
    try
    {
        std::cout << "Element at index 2: " << stack_vector.at(2).getData() << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    // Removing elements
    stack_vector.pop_back();
    std::cout << "After pop_back: ";
    for (const auto &element : stack_vector)
    {
        std::cout << element.getData() << " ";
    }
    std::cout << std::endl;

    // Accessing front and back elements
    if (!stack_vector.empty())
    {
        std::cout << "First element: " << stack_vector.front().getData() << std::endl;
        std::cout << "Last element: " << stack_vector.back().getData() << std::endl;
    }

    // Resize the vector
    stack_vector.resize(2, ResourceManagingType("Resize"));
    std::cout << "After resizing to 2: ";
    for (const auto &element : stack_vector)
    {
        std::cout << element.getData() << " ";
    }
    std::cout << std::endl;

    return 0;
}