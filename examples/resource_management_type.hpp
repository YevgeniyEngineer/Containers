#pragma once

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