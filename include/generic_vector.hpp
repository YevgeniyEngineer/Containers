#ifndef CONTAINERS_GENERIC_VECTOR_HPP
#define CONTAINERS_GENERIC_VECTOR_HPP

#include "heap_allocation_policy.hpp"
#include "stack_allocation_policy.hpp"

#include <cstdint>
#include <stdexcept>
#include <type_traits>

namespace containers
{
template <typename T, std::size_t MaxSize, template <typename, std::size_t> class AllocationPolicy>
class GenericVector : public AllocationPolicy<T, MaxSize>
{
    using AllocationPolicy<T, MaxSize>::allocate;
    using AllocationPolicy<T, MaxSize>::deallocate;
    using AllocationPolicy<T, MaxSize>::getData;

  public:
    static constexpr auto MAX_SIZE = MaxSize;

    // Default constructor
    GenericVector() : AllocationPolicy<T, MaxSize>{}, size_{0U}
    {
    }

    // Destructor
    ~GenericVector()
    {
        clear();
    }

    // Copy constructor
    GenericVector(const GenericVector &other) : size_{other.size_}
    {
        try
        {
            for (std::size_t i = 0; i < other.size_; ++i)
            {
                allocate(i, other[i]);
                ++size_;
            }
        }
        catch (...)
        {
            clear();
            throw;
        }
    }

    // Copy assignment operator
    GenericVector &operator=(const GenericVector &other)
    {
        if (this != &other)
        {
            clear();
            try
            {
                for (std::size_t i = 0; i < other.size_; ++i)
                {
                    allocate(i, other[i]);
                }
                size_ = other.size_;
            }
            catch (...)
            {
                clear();
                throw;
            }
        }

        return *this;
    }

    // Move constructor
    GenericVector(GenericVector &&other) noexcept(noexcept(this->allocate(0U, std::move(other[0U]))))
        : size_{other.size_}
    {
        for (std::size_t i = 0; i < other.size_; ++i)
        {
            allocate(i, std::move(other[i]));
        }
        other.size_ = 0U;
    }

    // Move assignment operator
    GenericVector &operator=(GenericVector &&other) noexcept(noexcept(this->allocate(0U, std::move(other[0U]))))
    {
        if (this != &other)
        {
            // Clear current contents
            clear();

            // Transfer ownership of elements
            for (std::size_t i = 0; i < other.size_; ++i)
            {
                allocate(i, std::move(other[i]));
            }
            size_ = other.size_;
            other.size_ = 0U;
        }

        return *this;
    }

    void swap(GenericVector &other) noexcept
    {
        // Check if the allocation policies are the same
        if constexpr (std::is_same_v<decltype(*this), decltype(other)>)
        {
            using std::swap;

            // Swap sizes
            swap(this->size_, other.size_);

            // Swap contents
            for (std::size_t i = 0; i < std::max(this->size_, other.size_); ++i)
            {
                swap(this->getData(i), other.getData(i));
            }
        }
        else
        {
            throw std::logic_error("Cannot swap vectors with different allocation policies.");
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
        return iterator{&getData(0U)};
    }

    inline iterator end() noexcept
    {
        return iterator{&getData(size_)};
    }

    inline const_iterator cbegin() const noexcept
    {
        return const_iterator{&getData(0U)};
    }

    inline const_iterator cend() const noexcept
    {
        return const_iterator{&getData(size_)};
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

    template <typename U> inline void push_back(U &&value)
    {
        if (size_ >= MAX_SIZE)
        {
            throw std::runtime_error("Capacity exceeded.");
        }

        allocate(size_, std::forward<U>(value));
        ++size_;
    }

    template <typename... Args> inline void emplace_back(Args &&...args)
    {
        if (size_ >= MAX_SIZE)
        {
            throw std::runtime_error("Capacity exceeded.");
        }

        allocate(size_, std::forward<Args>(args)...);
        ++size_;
    }

    inline void pop_back()
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Attempting to remove an element from the empty container.");
        }

        --size_;
        deallocate(size_);
    }

    void resize(const std::size_t new_size, const T &value = T{})
    {
        if (new_size > MAX_SIZE)
        {
            throw std::runtime_error("Exceeds maximum size.");
        }

        if (new_size < size_)
        {
            for (std::size_t i = new_size; i < size_; ++i)
            {
                deallocate(i);
            }
            size_ = new_size;
        }
        else if (new_size > size_)
        {
            for (std::size_t i = size_; i < new_size; ++i)
            {
                allocate(i, value);
                ++size_;
            }
        }

        size_ = new_size;
    }

    inline void clear() noexcept
    {
        // Explicitly call the destructor for each constructed element
        for (std::size_t i = 0U; i < size_; ++i)
        {
            deallocate(i);
        }

        size_ = 0U;
    }

    inline T &at(const std::size_t index)
    {
        if (index >= size_)
        {
            throw std::runtime_error("Index out of range.");
        }

        return getData(index);
    }

    inline const T &at(const std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Index out of range.");
        }

        return getData(index);
    }

    constexpr inline T &operator[](const std::size_t index) noexcept
    {
        return getData(index);
    }

    constexpr inline const T &operator[](const std::size_t index) const noexcept
    {
        return getData(index);
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

        return getData(0U);
    }

    inline const T &front() const
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return getData(0U);
    }

    inline T &back()
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return getData(size_ - 1U);
    }

    inline const T &back() const
    {
        if (size_ == 0U)
        {
            throw std::runtime_error("Empty container.");
        }

        return getData(size_ - 1U);
    }

  private:
    std::size_t size_;
};

template <typename T, std::size_t MaxSize, template <typename, std::size_t> class AllocPolicy>
void swap(GenericVector<T, MaxSize, AllocPolicy> &lhs, GenericVector<T, MaxSize, AllocPolicy> &rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename T, std::size_t MaxSize> using StackVector = GenericVector<T, MaxSize, StackAllocationPolicy>;
template <typename T, std::size_t MaxSize> using HeapVector = GenericVector<T, MaxSize, HeapAllocationPolicy>;

} // namespace containers

#endif // CONTAINERS_GENERIC_VECTOR_HPP