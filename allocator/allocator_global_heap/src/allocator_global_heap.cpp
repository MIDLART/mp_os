#include <not_implemented.h>
#include <iomanip>

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
    allocator_global_heap &&other) noexcept:
    _logger(other._logger)
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) called");
    other._logger = nullptr;
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) ended");
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) called");
    if(this != &other)
    {
        _logger = other._logger;
        other._logger = nullptr;
    }
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) ended");

    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard("allocator_global_heap::allocate(size_t value_size, size_t values_count) called (value_size = " +
                     std::to_string(value_size) + ", values_count = " + std::to_string(values_count) + ")");
    debug_with_guard("allocator_global_heap::allocate(size_t value_size, size_t values_count) called (value_size = " +
                     std::to_string(value_size) + ", values_count = " + std::to_string(values_count) + ")");

    unsigned char *allocated_memory = nullptr;
    size_t size = value_size * values_count;
    size_t meta_size = sizeof(allocator_global_heap*) + sizeof(size_t);

    try
    {
        allocated_memory = reinterpret_cast<unsigned char*>(
                ::operator new(size + meta_size));
    }
    catch (std::bad_alloc &)
    {
        error_with_guard("void *allocator_global_heap::allocate bad_alloc (size = "
            + std::to_string(size) + ")");
        throw;
    }

    *reinterpret_cast<allocator_global_heap**>(allocated_memory) = this;
    *reinterpret_cast<size_t*>(allocated_memory + sizeof(allocator_global_heap*)) = size;

    trace_with_guard("void *allocator_global_heap::allocate successfully ended");
    debug_with_guard("void *allocator_global_heap::allocate successfully ended");

    return reinterpret_cast<void*>(allocated_memory + meta_size);
}

void allocator_global_heap::deallocate(
    void *at)
{
    trace_with_guard("allocator_global_heap::deallocate(void *at) called");
    debug_with_guard("allocator_global_heap::deallocate(void *at) called");

    auto allocated_memory = reinterpret_cast<unsigned char*>(at) - sizeof(allocator_global_heap*) - sizeof(size_t);

    if (*reinterpret_cast<allocator_global_heap**>(allocated_memory) != this)
    {
        error_with_guard("Trying to delete memory not from this allocator");
        throw std::logic_error("Trying to delete memory not from this allocator");
    }

    size_t size = *reinterpret_cast<size_t*>(allocated_memory + sizeof(allocator_global_heap*));

    std::ostringstream str_stream;


    str_stream << "dump: ";

    if(size == 0)
    {
        str_stream << "(size = 0)";
    }

    str_stream << std::hex << std::setfill('0');

    for (size_t i = 0; i < size; ++i)
    {
        str_stream << "0x" << std::setw(2) << static_cast<unsigned int>
        (*(reinterpret_cast<unsigned char*>(at) + i)) << ' ';
    }

    debug_with_guard("allocator_global_heap::deallocate(void *): deallocated " +
                     std::to_string(size) + " bytes" + str_stream.str());

    ::operator delete (allocated_memory);

    trace_with_guard("allocator_global_heap::deallocate(void *at) ended");
    debug_with_guard("allocator_global_heap::deallocate(void *at) ended");
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return "allocator_global_heap";
}