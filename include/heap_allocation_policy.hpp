#ifndef CONTAINERS_HEAP_ALLOCATION_POLICY
#define CONTAINERS_HEAP_ALLOCATION_POLICY

#include <cstddef>
#include <cstdint>
#include <utility>

namespace containers
{
template <typename T, std::size_t MaxSize> class HeapAllocationPolicy
{
  protected:
    T *data_{nullptr};

    HeapAllocationPolicy() : data_{static_cast<T *>(operator new[](MaxSize * sizeof(T)))}
    {
    }

    ~HeapAllocationPolicy()
    {
        operator delete[](data_);
        data_ = nullptr;
    }

    template <typename... Args> inline void allocate(const std::size_t index, Args &&...args)
    {
        new (&data_[index]) T{std::forward<Args>(args)...};
    }

    inline void deallocate(const std::size_t index)
    {
        data_[index].~T();
    }

    inline T &getData(const std::size_t index) noexcept
    {
        return data_[index];
    }

    inline const T &getData(const std::size_t index) const noexcept
    {
        return data_[index];
    }
};
} // namespace containers

#endif // CONTAINERS_HEAP_ALLOCATION_POLICY