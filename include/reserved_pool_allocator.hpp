#ifndef CONTAINERS_POOL_ALLOCATOR
#define CONTAINERS_POOL_ALLOCATOR

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>

namespace containers
{
struct StackPolicy
{
};

struct HeapPolicy
{
};

template <typename T, std::size_t MaxSize> class StackStorage
{
    static_assert(MaxSize > 0U, "StackStorage must contain allocated space for at least 1 element!");

  public:
    using Policy = StackPolicy;

    constexpr StackStorage() = default;
    ~StackStorage() = default;

    // Delete copy and move constructors and assignment operators to prevent accidental moving of the buffer.
    StackStorage(const StackStorage &) = delete;
    StackStorage &operator=(const StackStorage &) = delete;
    StackStorage(StackStorage &&) noexcept = delete;
    StackStorage &operator=(StackStorage &&) noexcept = delete;

    constexpr inline T *buffer() noexcept
    {
        return static_cast<T *>(static_cast<void *>(buffer_));
    }

    constexpr inline const T *buffer() const noexcept
    {
        return static_cast<T *>(static_cast<void *>(buffer_));
    }

  private:
    alignas(alignof(T)) std::byte buffer_[MaxSize * sizeof(T)];
};

template <typename T, std::size_t MaxSize> class HeapStorage
{
    static_assert(MaxSize > 0U, "HeapStorage must contain allocated space for at least 1 element!");

  public:
    using Policy = HeapPolicy;

    // Copy and move operations are deleted to prevent accidental copying
    HeapStorage(const HeapStorage &) = delete;
    HeapStorage &operator=(const HeapStorage &) = delete;
    HeapStorage(HeapStorage &&) noexcept = delete;
    HeapStorage &operator=(HeapStorage &&) noexcept = delete;

    // Allocate aligned memory for MaxSize elements of type T
    HeapStorage() : buffer_{static_cast<T *>(std::aligned_alloc(alignof(T), MaxSize * sizeof(T)))}
    {
        if (nullptr == buffer_)
        {
            throw std::bad_alloc();
        }
    }

    ~HeapStorage()
    {
        // Free the allocated memory
        std::free(buffer_);
    }

    inline T *buffer() noexcept
    {
        return buffer_;
    }

    inline const T *buffer() const noexcept
    {
        return buffer_;
    }

  private:
    T *buffer_;
};

template <typename T, std::size_t MaxSize, template <typename, std::size_t> class StoragePolicy>
class ReservedPoolAllocator
{
    static_assert(std::is_same_v<typename StoragePolicy<T, MaxSize>::Policy, StackPolicy> ||
                      std::is_same_v<typename StoragePolicy<T, MaxSize>::Policy, HeapPolicy>,
                  "StoragePolicy must be either StackStorage or HeapStorage");

  public:
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using value_type = T;

    constexpr ReservedPoolAllocator() : storage_{}, used_{0U}
    {
    }

    template <typename U> struct rebind
    {
        using other = ReservedPoolAllocator<U, MaxSize, StoragePolicy>;
    };

    T *allocate(const std::size_t n)
    {
        if ((used_ + n) > MaxSize)
        {
            throw std::bad_alloc();
        }

        T *result = storage_.buffer() + static_cast<std::ptrdiff_t>(used_);
        used_ += n;
        return result;
    }

    constexpr void deallocate(T * /* p */, std::size_t /* n */)
    {
        // Deallocate method is a no-op for both stack and heap in this design
    }

    template <typename U> inline void construct(pointer p, U &&value)
    {
        new (p) T{std::forward<U>(value)};
    }

    inline void destroy(pointer p)
    {
        p->~T();
    }

  private:
    StoragePolicy<T, MaxSize> storage_;
    std::size_t used_;
};

} // namespace containers

#endif // CONTAINERS_POOL_ALLOCATOR