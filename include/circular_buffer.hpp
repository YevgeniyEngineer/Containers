#ifndef CONTAINERS_CIRCULAR_BUFFER_HPP
#define CONTAINERS_CIRCULAR_BUFFER_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace containers
{
template <typename T, typename Arg, typename = void> struct is_invocable : std::false_type
{
};

template <typename T, typename Arg>
struct is_invocable<T, Arg, std::void_t<decltype(std::declval<T>()(std::declval<Arg>()))>> : std::true_type
{
};

// Convenience variable template
template <typename T, typename Arg> constexpr bool is_invocable_v = is_invocable<T, Arg>::value;

template <typename T, std::size_t Size> class CircularBuffer
{
    static_assert(((Size % 2U) == 0U), "CircularBuffer's Size must be a power of 2.");
    static_assert((Size > 0U), "CircularBuffer must have non-zero size.");

    static constexpr std::size_t SIZE = Size;
    static constexpr std::size_t LAST_INDEX = SIZE - 1U;

  public:
    // Default constructor
    CircularBuffer() : buffer_{std::make_unique<T[]>(SIZE)}, head_{0U}, tail_{0U}, count_{0U}
    {
    }

    template <typename U> void push(U &&value)
    {
        if (full())
        {
            throw std::runtime_error("CircularBuffer is full.");
        }

        buffer_[tail_] = std::forward<U>(value);
        tail_ = (tail_ + 1U) & LAST_INDEX;
        ++count_;
    }

    template <typename U> bool try_push(U &&value) noexcept
    {
        if (full())
        {
            // Buffer is full, cannot push
            return false;
        }

        buffer_[tail_] = std::forward<U>(value);
        tail_ = (tail_ + 1U) & LAST_INDEX;
        ++count_;

        // Successfully pushed
        return true;
    }

    template <typename... Args> void emplace(Args &&...args)
    {
        if (full())
        {
            throw std::runtime_error("CircularBuffer is full.");
        }

        new (&buffer_[tail_]) T{std::forward<Args>(args)...};
        tail_ = (tail_ + 1U) & LAST_INDEX;
        ++count_;
    }

    template <typename... Args> bool try_emplace(Args &&...args) noexcept
    {
        if (full())
        {
            // Buffer is full, cannot emplace
            return false;
        }

        new (&buffer_[tail_]) T{std::forward<Args>(args)...};
        tail_ = (tail_ + 1U) & LAST_INDEX;
        ++count_;

        // Successfully emplaced
        return true;
    }

    T pop()
    {
        if (empty())
        {
            throw std::runtime_error("Buffer is empty.");
        }

        T value = std::move(buffer_[head_]);
        head_ = (head_ + 1U) & LAST_INDEX;
        --count_;

        return value;
    }

    bool try_pop(T &out_value) noexcept
    {
        if (empty())
        {
            // Buffer is empty, cannot pop
            return false;
        }

        out_value = std::move(buffer_[head_]);
        head_ = (head_ + 1U) & LAST_INDEX;
        --count_;

        // Successfully popped
        return true;
    }

    inline const T &front() const
    {
        if (empty())
        {
            throw std::runtime_error("Buffer is empty.");
        }

        return buffer_[head_];
    }

    inline const T &back() const
    {
        if (empty())
        {
            throw std::runtime_error("Buffer is empty.");
        }

        const std::size_t last_index = (tail_ == 0U) ? LAST_INDEX : (tail_ - 1U);
        return buffer_[last_index];
    }

    inline bool empty() const noexcept
    {
        return (count_ == 0U);
    }

    inline bool full() const noexcept
    {
        return (count_ == SIZE);
    }

    inline std::size_t size() const noexcept
    {
        return count_;
    }

  private:
    std::unique_ptr<T[]> buffer_;
    std::size_t head_;
    std::size_t tail_;
    std::size_t count_;
};
} // namespace containers

#endif // CONTAINERS_CIRCULAR_BUFFER_HPP
