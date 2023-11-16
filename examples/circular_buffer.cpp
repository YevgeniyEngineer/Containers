#include "circular_buffer.hpp"

#include <iostream>
#include <string>

int main()
{
    containers::CircularBuffer<int, 8> buffer;

    // Add elements to the buffer
    for (int i = 0; i < 4; ++i)
    {
        buffer.push(i);
    }

    // Read elements from the buffer
    while (!buffer.empty())
    {
        std::cout << buffer.pop() << std::endl;
    }

    // Try to add elements to the buffer
    for (int i = 0; i < 10; ++i)
    {
        if (!buffer.try_push(i))
        {
            std::cout << "Buffer is full. Could not push: " << i << std::endl;
        }
    }

    // Try to read elements from the buffer
    int value;
    while (buffer.try_pop(value))
    {
        std::cout << value << std::endl;
    }

    buffer.push(1);
    buffer.push(3);

    // Print front and back elements
    if (!buffer.empty())
    {
        std::cout << "Front element: " << buffer.front() << std::endl;
        std::cout << "Back element: " << buffer.back() << std::endl;
    }

    // Example of using emplace
    containers::CircularBuffer<std::pair<int, std::string>, 4> buffer_2;

    // Constructing a std::pair directly in the buffer
    buffer_2.emplace(1, "example");

    // Constructing another std::pair
    buffer_2.emplace(2, "buffer");

    // Constructing another std::pair
    buffer_2.emplace(std::make_pair<int, std::string>(2, "buffer"));

    // Example try_emplace
    if (buffer_2.try_emplace(1, "example"))
    {
        std::cout << "First element emplaced successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to emplace first element." << std::endl;
    }

    // Example try_emplace
    if (buffer_2.try_emplace(2, "buffer"))
    {
        std::cout << "Second element emplaced successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to emplace second element." << std::endl;
    }

    return 0;
}
