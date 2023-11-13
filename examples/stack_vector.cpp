#include "stack_vector.hpp"

#include <cstring>
#include <iostream>

class ResourceManagingType
{
  public:
    ResourceManagingType(const char *text)
    {
        data_ = new char[std::strlen(text) + 1];
        std::strcpy(data_, text);
    }

    ~ResourceManagingType()
    {
        std::cout << "Destroying: " << data_ << std::endl;
        delete[] data_;
        data_ = nullptr;
    }

    // Copy and move constructors and assignment operators for demonstration
    ResourceManagingType(const ResourceManagingType &other)
    {
        data_ = new char[std::strlen(other.data_) + 1];
        std::strcpy(data_, other.data_);
    }

    ResourceManagingType(ResourceManagingType &&other) noexcept : data_(other.data_)
    {
        other.data_ = nullptr;
    }

    ResourceManagingType &operator=(const ResourceManagingType &other)
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = new char[std::strlen(other.data_) + 1];
            std::strcpy(data_, other.data_);
        }
        return *this;
    }

    ResourceManagingType &operator=(ResourceManagingType &&other) noexcept
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    }

    // For demonstration
    const char *getData() const
    {
        return data_;
    }

  private:
    char *data_;
};

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