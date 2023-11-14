#ifndef CONTAINERS_STACK_ALLOCATION_POLICY
#define CONTAINERS_STACK_ALLOCATION_POLICY

#include <cstddef>
#include <cstdint>
#include <utility>

namespace containers
{
template <typename T, std::size_t MaxSize> class StackAllocationPolicy
{
  protected:
    alignas(alignof(T)) std::byte data_[sizeof(T) * MaxSize];

    template <typename... Args> inline void allocate(const std::size_t index, Args &&...args)
    {
        new (&data_[index * sizeof(T)]) T{std::forward<Args>(args)...};
    }

    inline void deallocate(const std::size_t index)
    {
        static_cast<T *>(static_cast<void *>(&data_[index * sizeof(T)]))->~T();
    }

    inline T &getData(const std::size_t index) noexcept
    {
        return *static_cast<T *>(static_cast<void *>(&data_[index * sizeof(T)]));
    }
};
} // namespace containers

#endif // CONTAINERS_STACK_ALLOCATION_POLICY