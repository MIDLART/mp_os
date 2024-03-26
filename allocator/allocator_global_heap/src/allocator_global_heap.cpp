#include <not_implemented.h>

#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger):
    _logger(logger)
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(logger *logger): _logger(logger) called");
    trace_with_guard("allocator_global_heap::allocator_global_heap(logger *logger): _logger(logger) ended");
}

allocator_global_heap::~allocator_global_heap()
{
    trace_with_guard("allocator_global_heap::~allocator_global_heap() called");
    _logger = nullptr;
    trace_with_guard("allocator_global_heap::~allocator_global_heap() ended");
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) called");
    _logger = other._logger;
    other._logger = nullptr;
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) ended");
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) called");
    _logger = other._logger;
    other._logger = nullptr;
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) ended");

    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    debug_with_guard("void *allocator_global_heap::allocate called");

    void *allocated_memory;

    try
    {
        allocated_memory = reinterpret_cast<void*> (new unsigned char (value_size * values_count) + sizeof(size_t));
    }
    catch (std::bad_alloc &err)
    {
        error_with_guard("void *allocator_global_heap::allocate bad_alloc");
        throw err;
    }

    debug_with_guard("void *allocator_global_heap::allocate ended");

    return allocated_memory;
}

void allocator_global_heap::deallocate(
    void *at)
{
    debug_with_guard("allocator_global_heap::deallocate(void *at) call");

    if (at >= this && (char*) at < ((char*) this + sizeof (allocator_global_heap)))
    {
        get_logger()->error("Trying to delete part of allocator");
        throw std::logic_error("Trying to delete part of allocator");
    }

    delete reinterpret_cast<unsigned char*>(at);

    debug_with_guard("allocator_global_heap::deallocate(void *at) end");
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return "allocator_global_heap";
}