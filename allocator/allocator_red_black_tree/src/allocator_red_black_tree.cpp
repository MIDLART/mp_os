#include <not_implemented.h>

#include "../include/allocator_red_black_tree.h"

allocator_red_black_tree::~allocator_red_black_tree()
{
    trace_with_guard(get_typename() + "::~allocator_buddies_system() called");

    logger *logger = get_logger();
    get_mutex().~mutex();

    deallocate_with_guard(_trusted_memory);

    if(logger)
    {
        logger->trace(get_typename() + "::~allocator_buddies_system() finished");
    }
}

allocator_red_black_tree::allocator_red_black_tree(
    allocator_red_black_tree &&other) noexcept:
        _trusted_memory(other._trusted_memory)
{
    trace_with_guard(get_typename() + "::allocator_allocator_buddies_system(allocator_boundary_tags &&) called");

    other._trusted_memory = nullptr;

    trace_with_guard(get_typename() + "::allocator_allocator_buddies_system(allocator_boundary_tags &&) finished");
}

allocator_red_black_tree &allocator_red_black_tree::operator=(
    allocator_red_black_tree &&other) noexcept
{
    trace_with_guard(get_typename() + "::allocator_allocator_buddies_system &operator=(allocator_boundary_tags &&) called");

    if (this != &other)
    {
        get_mutex().~mutex();
        deallocate_with_guard(_trusted_memory);

        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }

    trace_with_guard(get_typename() + "::allocator_allocator_buddies_system &operator=(allocator_boundary_tags &&) finished");

    return *this;
}

allocator_red_black_tree::allocator_red_black_tree(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->trace(get_typename() + "::allocator_allocator_buddies_system(size_t, allocator *, logger *, fit_mode) called");
    }



    trace_with_guard(get_typename() + "::allocator_allocator_buddies_system(size_t, allocator *, logger *, fit_mode) finished");
}

[[nodiscard]] void *allocator_red_black_tree::allocate(
    size_t value_size,
    size_t values_count)
{
    throw not_implemented("[[nodiscard]] void *allocator_red_black_tree::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_red_black_tree::deallocate(
    void *at)
{
    throw not_implemented("void allocator_red_black_tree::deallocate(void *)", "your code should be here...");
}

inline void allocator_red_black_tree::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    get_fit_mode() = mode;
}

inline allocator *allocator_red_black_tree::get_allocator() const
{
    throw not_implemented("inline allocator *allocator_red_black_tree::get_allocator() const", "your code should be here...");
}

std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_red_black_tree::get_logger() const
{
    throw not_implemented("inline logger *allocator_red_black_tree::get_logger() const", "your code should be here...");
}

inline std::string allocator_red_black_tree::get_typename() const noexcept
{
    return "allocator_red_black_tree";
}