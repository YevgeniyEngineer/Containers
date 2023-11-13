#ifndef CONTAINERS_STACK_VECTOR_HPP
#define CONTAINERS_STACK_VECTOR_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <new>
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

    // Destructor
    ~StackVector()
    {
        clear();
    }

    // Copy constructor
    StackVector(const StackVector &other) : size_{other.size_}
    {
        for (std::size_t i = 0; i < size_; ++i)
        {
            new (&data_[i * sizeof(T)]) T{other[i]};
        }
    }

    // Copy assignment operator
    StackVector &operator=(const StackVector &other)
    {
        if (this != &other)
        {
            // Destruct existing elements
            clear();

            // Copy construct new elements
            for (std::size_t i = 0; i < other.size_; ++i)
            {
                new (&data_[i * sizeof(T)]) T{other[i]};
            }
            size_ = other.size_;
        }

        return *this;
    }

    // Move constructor
    StackVector(StackVector &&other) noexcept : size_{other.size_}
    {
        for (std::size_t i = 0; i < size_; ++i)
        {
            new (&data_[i * sizeof(T)]) T{std::move(other[i])};
        }
        other.size_ = 0U;
    }

    // Move assignment operator
    StackVector &operator=(StackVector &&other) noexcept
    {
        if (this != &other)
        {
            // Destruct existing elements
            clear();

            // Move construct new elements
            for (std::size_t i = 0; i < other.size_; ++i)
            {
                new (&data_[i * sizeof(T)]) T{std::move(other[i])};
            }
            size_ = other.size_;
            other.size_ = 0U;
        }

        return *this;
    }

    void swap(StackVector &other) noexcept
    {
        // Swap sizes
        std::swap(size_, other.size_);

        // Swap contents
        for (std::size_t i = 0U; i < std::max(size_, other.size_); ++i)
        {
            std::swap(data_[i], other.data_[i]);
        }
    }

    class iterator final
    {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        iterator(pointer ptr) noexcept : ptr_{ptr}
        {
        }

        inline reference operator*() const noexcept
        {
            return *ptr_;
        }
        inline pointer operator->() noexcept
        {
            return ptr_;
        }

        // Arithmetic operations
        inline iterator &operator++() noexcept
        {
            ptr_++;
            return *this;
        }
        inline iterator operator++(int) noexcept
        {
            iterator temp = *this;
            ++ptr_;
            return temp;
        }
        inline iterator &operator--() noexcept
        {
            ptr_--;
            return *this;
        }
        inline iterator operator--(int) noexcept
        {
            iterator temp = *this;
            --ptr_;
            return temp;
        }
        inline iterator &operator+=(const difference_type n) noexcept
        {
            ptr_ += n;
            return *this;
        }
        inline iterator &operator-=(const difference_type n) noexcept
        {
            ptr_ -= n;
            return *this;
        }
        inline iterator operator+(const difference_type n) const noexcept
        {
            return iterator{ptr_ + n};
        }
        inline iterator operator-(const difference_type n) const noexcept
        {
            return iterator{ptr_ - n};
        }
        inline difference_type operator-(const iterator &other) const noexcept
        {
            return (ptr_ - other.ptr_);
        }

        // Comparison operators
        inline friend bool operator==(const iterator &a, const iterator &b) noexcept
        {
            return (a.ptr_ == b.ptr_);
        }
        inline friend bool operator!=(const iterator &a, const iterator &b) noexcept
        {
            return (a.ptr_ != b.ptr_);
        }
        inline friend bool operator<(const iterator &a, const iterator &b) noexcept
        {
            return (a.ptr_ < b.ptr_);
        }
        inline friend bool operator>(const iterator &a, const iterator &b) noexcept
        {
            return (a.ptr_ > b.ptr_);
        }
        inline friend bool operator<=(const iterator &a, const iterator &b) noexcept
        {
            return (a.ptr_ <= b.ptr_);
        }
        inline friend bool operator>=(const iterator &a, const iterator &b) noexcept
        {
            return (a.ptr_ >= b.ptr_);
        }

        // Subscript operator
        inline reference operator[](const difference_type n) const noexcept
        {
            return *(ptr_ + n);
        }

      private:
        pointer ptr_;
    };

    class const_iterator final
    {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;

        const_iterator(pointer ptr) noexcept : ptr_{ptr}
        {
        }

        inline reference operator*() const noexcept
        {
            return *ptr_;
        }
        inline pointer operator->() const noexcept
        {
            return ptr_;
        }

        // Arithmetic operations
        inline const_iterator &operator++() noexcept
        {
            ptr_++;
            return *this;
        }
        inline const_iterator operator++(int) noexcept
        {
            const_iterator temp = *this;
            ++ptr_;
            return temp;
        }
        inline const_iterator &operator--() noexcept
        {
            ptr_--;
            return *this;
        }
        inline const_iterator operator--(int) noexcept
        {
            const_iterator temp = *this;
            --ptr_;
            return temp;
        }
        inline const_iterator &operator+=(const difference_type n) noexcept
        {
            ptr_ += n;
            return *this;
        }
        inline const_iterator &operator-=(const difference_type n) noexcept
        {
            ptr_ -= n;
            return *this;
        }
        inline const_iterator operator+(const difference_type n) const noexcept
        {
            return const_iterator{ptr_ + n};
        }
        inline const_iterator operator-(const difference_type n) const noexcept
        {
            return const_iterator{ptr_ - n};
        }
        inline difference_type operator-(const const_iterator &other) const noexcept
        {
            return (ptr_ - other.ptr_);
        }

        // Comparison operators
        inline friend bool operator==(const const_iterator &a, const const_iterator &b) noexcept
        {
            return (a.ptr_ == b.ptr_);
        }
        inline friend bool operator!=(const const_iterator &a, const const_iterator &b) noexcept
        {
            return (a.ptr_ != b.ptr_);
        }
        inline friend bool operator<(const const_iterator &a, const const_iterator &b) noexcept
        {
            return (a.ptr_ < b.ptr_);
        }
        inline friend bool operator>(const const_iterator &a, const const_iterator &b) noexcept
        {
            return (a.ptr_ > b.ptr_);
        }
        inline friend bool operator<=(const const_iterator &a, const const_iterator &b) noexcept
        {
            return (a.ptr_ <= b.ptr_);
        }
        inline friend bool operator>=(const const_iterator &a, const const_iterator &b) noexcept
        {
            return (a.ptr_ >= b.ptr_);
        }

        // Subscript operator
        inline reference operator[](const difference_type n) const noexcept
        {
            return *(ptr_ + n);
        }

      private:
        pointer ptr_;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    inline iterator begin() noexcept
    {
        return iterator{static_cast<T *>(static_cast<void *>(data_))};
    }

    inline iterator end() noexcept
    {
        return iterator{static_cast<T *>(static_cast<void *>(data_)) + size_};
    }

    inline const_iterator cbegin() const noexcept
    {
        return const_iterator{static_cast<const T *>(static_cast<const void *>(data_))};
    }

    inline const_iterator cend() const noexcept
    {
        return const_iterator{static_cast<const T *>(static_cast<const void *>(data_)) + size_};
    }

    inline reverse_iterator rbegin() noexcept
    {
        return reverse_iterator{end()};
    }

    inline reverse_iterator rend() noexcept
    {
        return reverse_iterator{begin()};
    }

    inline const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator{end()};
    }

    inline const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator{begin()};
    }

    inline const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator{cend()};
    }

    inline const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator{cbegin()};
    }

    inline void push_back(const T &value)
    {
        if (size_ >= MAX_SIZE)
        {
            throw std::runtime_error("Capacity exceeded.");
        }

        new (&data_[size_ * sizeof(T)]) T{value};
        ++size_;
    }

    template <typename... Args> inline void emplace_back(Args &&...args)
    {
        if (size_ >= MAX_SIZE)
        {
            throw std::runtime_error("Capacity exceeded.");
        }

        new (&data_[size_ * sizeof(T)]) T{std::forward<Args>(args)...};
        ++size_;
    }

    inline void pop_back()
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Attempting to remove an element from the empty container.");
        }

        --size_;

        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            static_cast<T *>(static_cast<void *>(&data_[size_ * sizeof(T)]))->~T();
        }
    }

    void resize(const std::size_t new_size, const T &value = T{})
    {
        if (new_size > MAX_SIZE)
        {
            throw std::runtime_error("Exceeds maximum size.");
        }

        // Construct new elements if the new size is greater than the current size
        if (new_size > size_)
        {
            for (std::size_t i = size_; i < new_size; ++i)
            {
                new (&data_[i * sizeof(T)]) T{value};
            }
        }

        else
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                // Destruct excess elements if the new size is smaller than the current size
                if (new_size < size_)
                {
                    for (std::size_t i = new_size; i < size_; ++i)
                    {
                        static_cast<T *>(static_cast<void *>(&data_[i * sizeof(T)]))->~T();
                    }
                }
            }
        }

        size_ = new_size;
    }

    inline void clear() noexcept
    {
        // Explicitly call the destructor for each constructed element
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            for (std::size_t i = 0U; i < size_; ++i)
            {
                static_cast<T *>(static_cast<void *>(&data_[i * sizeof(T)]))->~T();
            }
        }
        size_ = 0U;
    }

    inline T &at(const std::size_t index)
    {
        if (index >= size_)
        {
            throw std::runtime_error("Index out of range.");
        }

        return *static_cast<T *>(static_cast<void *>(&data_[index * sizeof(T)]));
    }

    inline const T &at(const std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Index out of range.");
        }

        return *static_cast<const T *>(static_cast<const void *>(&data_[index * sizeof(T)]));
    }

    constexpr inline T &operator[](const std::size_t index) noexcept
    {
        return *static_cast<T *>(static_cast<void *>(&data_[index * sizeof(T)]));
    }

    constexpr inline const T &operator[](const std::size_t index) const noexcept
    {
        return *static_cast<const T *>(static_cast<const void *>(&data_[index * sizeof(T)]));
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

    inline T &front()
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return *static_cast<T *>(static_cast<void *>(&data_[0]));
    }

    inline const T &front() const
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return *static_cast<const T *>(static_cast<const void *>(&data_[0]));
    }

    inline T &back()
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return *static_cast<T *>(static_cast<void *>(&data_[(size_ - 1) * sizeof(T)]));
    }

    inline const T &back() const
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return *static_cast<const T *>(static_cast<const void *>(&data_[(size_ - 1) * sizeof(T)]));
    }

  private:
    std::size_t size_;
    alignas(alignof(T)) std::byte data_[sizeof(T) * MAX_SIZE];
};

template <typename T, std::size_t MaxSize>
inline void swap(StackVector<T, MaxSize> &lhs, StackVector<T, MaxSize> &rhs) noexcept
{
    lhs.swap(rhs);
}
} // namespace containers

#endif // CONTAINERS_STACK_VECTOR