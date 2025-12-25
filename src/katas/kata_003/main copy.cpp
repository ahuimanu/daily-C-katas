#include <atomic>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

/*
Exactly one producer thread and one consumer thread

Use std::atomic<std::size_t> for indices

No locks, no mutexes

Capacity is fixed at construction

One slot must remain unused to distinguish full vs empty

No dynamic resizing
*/

class SpScRingBuffer
{
public:
    explicit SpScRingBuffer(std::size_t capacity);
    ~SpScRingBuffer() = default;

    SpScRingBuffer(const SpScRingBuffer &) = delete;
    SpScRingBuffer &operator=(const SpScRingBuffer &) = delete;

    bool push(int value); // false if full
    bool pop(int &value); // false if empty

    std::size_t size() const;
    std::size_t capacity() const;

private:
    // Backing storage size is (logical_capacity + 1) so one slot is always unused.
    std::size_t storage_capacity_ = 0;
    std::vector<int> buffer_;

    // Indices into [0, storage_capacity_). Single producer writes head_, single consumer writes tail_.
    alignas(64) std::atomic<std::size_t> head_{0};
    alignas(64) std::atomic<std::size_t> tail_{0};
};

SpScRingBuffer::SpScRingBuffer(std::size_t capacity)
{
    // One slot remains unused to distinguish full vs empty.
    // If capacity == 0, this becomes a valid zero-capacity ring (push always fails).
    storage_capacity_ = capacity;
    buffer_.resize(storage_capacity_);

    head_.store(0, std::memory_order_relaxed);
    tail_.store(0, std::memory_order_relaxed);
}

bool SpScRingBuffer::push(int value)
{
    const std::size_t head = head_.load(std::memory_order_relaxed);
    const std::size_t next = (head + 1) % storage_capacity_;

    // Full when next head would collide with current tail.
    const std::size_t tail = tail_.load(std::memory_order_acquire);
    if (next == tail)
    {
        return false;
    }

    buffer_[head] = value;
    head_.store(next, std::memory_order_release);
    return true;
}

bool SpScRingBuffer::pop(int &value)
{
    const std::size_t tail = tail_.load(std::memory_order_relaxed);
    const std::size_t head = head_.load(std::memory_order_acquire);

    if (tail == head)
    {
        return false;
    }

    value = buffer_[tail];
    const std::size_t next = (tail + 1) % storage_capacity_;
    tail_.store(next, std::memory_order_release);
    return true;
}

std::size_t SpScRingBuffer::size() const
{
    const std::size_t head = head_.load(std::memory_order_acquire);
    const std::size_t tail = tail_.load(std::memory_order_acquire);
    if (head >= tail)
    {
        return head - tail;
    }
    return (storage_capacity_ - tail) + head;
}

std::size_t SpScRingBuffer::capacity() const
{
    // One slot is unused.
    return storage_capacity_ - 1;
}

int main()
{

    SpScRingBuffer ring_buffer(8);
    for (int i = 1; i <= 7; ++i)
    {
        assert(ring_buffer.push(i) && "Push should succeed");
    }

    int dummy = 0;
    assert(!ring_buffer.push(999) && "Push should fail when full");

    for (size_t i = 1; i <= 7; i++)
    {
        int out = 0;
        assert(ring_buffer.pop(out) && "Pop should succeed");
        assert(out == i);
    }

    assert(!ring_buffer.pop(dummy) && "Pop should fail when empty");

    return 0;
}