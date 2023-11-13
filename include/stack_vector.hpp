#ifndef CONTAINERS_STACK_VECTOR_HPP
#define CONTAINERS_STACK_VECTOR_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace containers
{
template <typename T, std::size_t MaxSize> class StackVector final
{
  public:
    static constexpr auto MAX_SIZE = MaxSize;

    // Default constructor
    constexpr StackVector() : size_{0U}
    {
    }

    // Copy constructor
    StackVector(const StackVector &other) : size_{other.size_}
    {
        std::copy(other.data_.begin(), other.data_.begin() + size_, data_.begin());
    }

    // Copy assignment operator
    StackVector &operator=(const StackVector &other)
    {
        if (this != &other)
        {
            size_ = other.size_;
            std::copy(other.data_.begin(), other.data_.begin() + size_, data_.begin());
        }

        return *this;
    }

    // Move constructor
    StackVector(StackVector &&other) noexcept : size_{other.size_}
    {
        std::move(other.data_.begin(), other.data_.begin() + size_, data_.begin());
        other.size_ = 0U;
    }

    // Move assignment operator
    StackVector &operator=(StackVector &&other) noexcept
    {
        if (this != &other)
        {
            size_ = other.size_;
            std::move(other.data_.begin(), other.data_.begin() + size_, data_.begin());
            other.size_ = 0U;
        }

        return *this;
    }

    inline void push_back(const T &value)
    {
        if (size_ >= MAX_SIZE)
        {
            throw std::runtime_error("Capacity exceeded.");
        }

        data_[size_++] = value;
    }

    template <typename... Args> inline void emplace_back(Args &&...args)
    {
        if (size_ >= MAX_SIZE)
        {
            throw std::runtime_error("Capacity exceeded.");
        }

        new (&data_[size_++]) T{std::forward<Args>(args)...};
    }

    inline void pop_back()
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Attempting to remove an element from the empty container.");
        }

        --size_;
    }

    void resize(const std::size_t new_size, const T &value = T{})
    {
        if (new_size > MAX_SIZE)
        {
            throw std::runtime_error("Exceeds maximum size.");
        }

        if (new_size > size_)
        {
            std::fill(data_.begin() + size_, data_.begin() + new_size, value);
        }

        size_ = new_size;
    }

    inline void clear() noexcept
    {
        size_ = 0U;
    }

    inline T &at(const std::size_t index)
    {
        if (index >= size_)
        {
            throw std::runtime_error("Index out of range.");
        }

        return data_[index];
    }

    inline const T &at(const std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Index out of range.");
        }

        return data_[index];
    }

    constexpr inline T &operator[](const std::size_t index) noexcept
    {
        return data_[index];
    }

    constexpr inline const T &operator[](const std::size_t index) const noexcept
    {
        return data_[index];
    }

    inline std::size_t size() const noexcept
    {
        return size_;
    }

    constexpr static inline std::size_t maxSize() noexcept
    {
        return MAX_SIZE;
    }

    inline bool empty() const noexcept
    {
        return (size_ == 0U);
    }

    constexpr inline auto begin() noexcept
    {
        return data_.begin();
    }

    constexpr inline auto end() noexcept
    {
        return data_.begin() + size_;
    }

    constexpr inline auto begin() const noexcept
    {
        return data_.begin();
    }

    constexpr inline auto end() const noexcept
    {
        return data_.begin() + size_;
    }

    constexpr inline auto cbegin() noexcept
    {
        return data_.cbegin();
    }

    constexpr inline auto cend() noexcept
    {
        return data_.cend();
    }

    constexpr inline auto rbegin() noexcept
    {
        return data_.rbegin();
    }

    constexpr inline auto rend() noexcept
    {
        return data_.rend();
    }

    constexpr inline auto crbegin() noexcept
    {
        return data_.crbegin();
    }

    constexpr inline auto crend() noexcept
    {
        return data_.crend();
    }

    inline T &front()
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return data_.front();
    }

    inline const T &front() const
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return data_.front();
    }

    inline T &back()
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return data_[size_ - 1U];
    }

    inline const T &back() const
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return data_[size_ - 1U];
    }

    constexpr inline T *data() noexcept
    {
        return data_.data();
    }

    constexpr inline const T *data() const noexcept
    {
        return data_.data();
    }

  private:
    std::size_t size_;
    std::array<T, MAX_SIZE> data_;
};
} // namespace containers

#endif // CONTAINERS_STACK_VECTOR