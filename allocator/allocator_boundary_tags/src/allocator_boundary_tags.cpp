#include <not_implemented.h>
#include <sstream>

#include "../include/allocator_boundary_tags.h"

allocator_boundary_tags::~allocator_boundary_tags()
{
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() called");

    logger* logger = get_logger();

    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);

    trace_with_guard(get_typename() + "::~allocator_boundary_tags() finished");

    if (logger)
    {
        logger->trace(get_typename() + "::~allocator_boundary_tags() finished");
    }
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept:
    _trusted_memory(other._trusted_memory)
{
    trace_with_guard(get_typename() + "::allocator_boundary_tags(allocator_boundary_tags &&) called");

    other._trusted_memory = nullptr;

    trace_with_guard(get_typename() + "::allocator_boundary_tags(allocator_boundary_tags &&) finished");
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
{
    trace_with_guard(get_typename() + "::allocator_boundary_tags(allocator_boundary_tags &&) called");

    if (this != &other)
    {
        get_mutex().~mutex();
        deallocate_with_guard(_trusted_memory);

        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }

    trace_with_guard(get_typename() + "::allocator_boundary_tags(allocator_boundary_tags &&) finished");

    return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->trace(get_typename() + "::allocator_boundary_tags(size_t, allocator *, logger *, fit_mode) called");
    }

    if (space_size < get_block_meta_size())
    {
        if (logger != nullptr)
        {
            logger->error(get_typename() + "::allocator_boundary_tags(size_t, allocator *, logger *, fit_mode) size of " +
                          std::to_string(space_size) + "bytes is too small to initialize allocator instance. " +
                          "Minimum size is " + std::to_string(get_block_meta_size()) + " bytes.");
        }

        throw std::logic_error("Cannot initialize allocator of this size");
    }

    auto size = space_size + get_meta_size();

    try
    {
        if (parent_allocator == nullptr)
        {
            _trusted_memory = ::operator new(space_size + get_meta_size());
        }
        else
        {
            _trusted_memory = parent_allocator->allocate(size, 1);
        }

    }
    catch (std::bad_alloc const &)
    {
        if (logger != nullptr)
        {
            logger->error(get_typename() + "::allocator_boundary_tags(size_t, allocator *, logger *, fit_mode) " +
                "bad alloc occured while trying to allocate " + std::to_string(size) + "bytes");
        }

        throw;
    }

    auto *ptr = reinterpret_cast<unsigned char*>(_trusted_memory);

    *reinterpret_cast<allocator**>(ptr) = parent_allocator;
    ptr += sizeof(allocator*);

    *reinterpret_cast<class logger**>(ptr) = logger;
    ptr += sizeof(class logger*);

    *reinterpret_cast<fit_mode*>(ptr) = allocate_fit_mode;
    ptr += sizeof(fit_mode);

    new(reinterpret_cast<std::mutex*>(ptr)) std::mutex;
    ptr += sizeof(std::mutex);

    *reinterpret_cast<block_size_t*>(ptr) = space_size;
    ptr += sizeof(block_size_t);

    *reinterpret_cast<block_size_t*>(ptr) = space_size;
    ptr += sizeof(block_size_t);

    *reinterpret_cast<block_pointer_t*>(ptr) = nullptr;

    trace_with_guard(get_typename() + "::allocator_boundary_tags(size_t, allocator *, logger *, allocator_with_fit_mode::fit_mode) finished");
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
        size_t value_size,
        size_t values_count)
{
    trace_with_guard(get_typename() + "::allocate(size_t, size_t) called")->
            debug_with_guard(get_typename() + "::allocate(size_t, size_t) called (value_size = " +
            std::to_string(value_size) + ", value_count = " + std::to_string(values_count) + ")");

    std::lock_guard<std::mutex> lock (get_mutex());

    block_size_t req_size = value_size * values_count;
    block_size_t block_size = req_size + get_block_meta_size();

    block_pointer_t target_block = nullptr;
    block_pointer_t prev = nullptr;
    block_pointer_t next = nullptr;
    block_size_t target_size = 0;

    block_pointer_t &head_block = get_head_block();
    block_pointer_t cur_block = head_block;
    block_pointer_t prev_block;
    block_size_t cur_size = 0;

    if (cur_block == nullptr)
    {
        cur_size = get_allocator_size();
    }
    else
    {
        cur_size = reinterpret_cast<unsigned char*>(cur_block) - get_block_meta_size() -
                reinterpret_cast<unsigned char*>(_trusted_memory) - get_meta_size();
    }

    if (cur_size >= block_size)
    {
        target_block = reinterpret_cast<unsigned char*>(_trusted_memory) + get_meta_size() + get_block_meta_size();
        target_size = cur_size;
    }

    auto fit = get_fit_mode();
    unsigned char *block_end;
    unsigned char *block_begin;

    while (cur_block != nullptr)
    {
        prev_block = cur_block;

        cur_block = get_next_block(cur_block);

        block_begin = reinterpret_cast<unsigned char *>(prev_block) - get_block_size(prev_block);

        if (cur_block != nullptr)
        {
            block_end = reinterpret_cast<unsigned char *>(cur_block) - get_block_meta_size();
        }
        else
        {
            block_end = reinterpret_cast<unsigned char*>(_trusted_memory) + get_meta_size() + get_allocator_size();
        }

        cur_size = block_end - block_begin;

        if ((cur_size >= block_size) && (target_block == nullptr ||
            (fit == fit_mode::the_best_fit && (cur_size < target_size)) ||
            (fit == fit_mode::the_worst_fit && (cur_size > target_size))))
        {
            prev = prev_block;
            target_block = reinterpret_cast<block_pointer_t>(reinterpret_cast<unsigned char*>(block_begin) + get_block_meta_size());
            target_size = cur_size;
        }

        if (fit == fit_mode::first_fit && target_block)
        {
            break;
        }
    }

    if (target_block == nullptr)
    {
        error_with_guard(get_typename() + "::allocate(size_t, size_t) : no space to allocate requested " +
            std::to_string(req_size) + " bytes");

        throw std::bad_alloc();
    }

    if (target_size < block_size + get_block_meta_size())
    {
        warning_with_guard(get_typename() + "::allocate(size_t, size_t) size has been increased from " +
            std::to_string(req_size) + " to " + std::to_string(target_size - get_block_meta_size()) + " bytes");

        req_size = target_size - get_block_meta_size();
    }

    if (prev != nullptr)
    {
        next = get_next_block(prev);
        get_next_block(prev) = target_block;
    }
    else
    {
        next = head_block;
    }

    if (next != nullptr)
    {
        get_prev_block(next) = target_block;
    }

    get_block_allocator(target_block) = this;
    get_block_size(target_block) = req_size;
    get_prev_block(target_block) = prev;
    get_next_block(target_block) = next;

    if (head_block == nullptr || head_block == get_next_block(target_block))
    {
        head_block = target_block;
    }

    get_free_space() -= req_size + get_block_meta_size();

    debug_with_guard(get_typename() + "::allocate(size_t, size_t) : allocated " + std::to_string(req_size) +
                     "(+ meta: " + std::to_string(get_block_meta_size()) + ") bytes");
    debug_blocks_info(get_typename() + "::allocate(size_t, size_t)");
    information_with_guard(get_typename() + "::allocate(size_t, size_t) free space left: " +
                           std::to_string(get_free_space()) + " bytes")->
        trace_with_guard(get_typename() + "::allocate(size_t, size_t) finished")->
        debug_with_guard(get_typename() + "::allocate(size_t, size_t) finished");

    return reinterpret_cast<allocator_boundary_tags*>(target_block);
}

void allocator_boundary_tags::deallocate(
    void *at)
{
    trace_with_guard(get_typename() + "::deallocate(void *) called")
            ->debug_with_guard(get_typename() + "::deallocate(void *) called");

    std::lock_guard<std::mutex> lock (get_mutex());

    if (at == nullptr)
    {
        return;
    }

    unsigned char* begin = reinterpret_cast<unsigned char*>(_trusted_memory) + get_meta_size();
    unsigned char* end = begin + get_allocator_size();

    if (at < begin || at >= end || get_block_allocator(at) != this)
    {
        error_with_guard(get_typename() + "::deallocate(void *) tried to deallocate non-related memory");
        throw std::logic_error("try of deallocation non-related memory");
    }

    block_size_t size = get_block_size(at);
    block_pointer_t prev = get_prev_block(at);
    block_pointer_t next = get_next_block(at);
    std::string dump = get_block_dump(at, size);

    if (prev != nullptr)
    {
        get_next_block(prev) = next;
    }
    else
    {
        get_head_block() = next;
    }

    if (next != nullptr)
    {
        get_prev_block(next) = prev;
    }

    get_free_space() += get_block_meta_size() + size;

    debug_with_guard(get_typename() + "::deallocate(void *) deallocated " + std::to_string(size)
                     + "(+ meta: " + std::to_string(get_block_meta_size()) + ") bytes" + dump);
    debug_blocks_info(get_typename() + "::deallocate(void *)");
    information_with_guard(get_typename() + "::deallocate(void *) free space left: " +
                           std::to_string(get_free_space()) + " bytes.")->
        trace_with_guard(get_typename() + "::deallocate(void *) finished")->
        debug_with_guard(get_typename() + "::deallocate(void *) finished");
}

inline void allocator_boundary_tags::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    get_fit_mode() = mode;
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
    trace_with_guard(get_typename() + "::get_blocks_info() called");

    std::lock_guard<std::mutex> lock (get_mutex());

    auto blocks = create_blocks_info();

    trace_with_guard(get_typename() + "::get_blocks_info() finished");

    return blocks;
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::create_blocks_info() const noexcept
{
    std::vector<allocator_test_utils::block_info> blocks(0);

    block_pointer_t cur_block = get_head_block();
    block_pointer_t prev_block;
    block_size_t free_size, occupied_size;

    if (cur_block == nullptr)
    {
        free_size = get_allocator_size();
    }
    else
    {
        free_size = reinterpret_cast<unsigned char*>(cur_block) - get_block_meta_size() -
                    reinterpret_cast<unsigned char*>(_trusted_memory) - get_meta_size();
    }

    if (free_size)
    {
        blocks.push_back({.block_size = free_size, .is_block_occupied = false});
    }

    unsigned char *block_begin;

    while (cur_block != nullptr)
    {
        prev_block = cur_block;

        cur_block = get_next_block(cur_block);

        if (cur_block != nullptr)
        {
            block_begin = reinterpret_cast<unsigned char *>(cur_block) - get_block_meta_size();
        }
        else
        {
            block_begin = reinterpret_cast<unsigned char*>(_trusted_memory) + get_meta_size() + get_allocator_size();
        }

        free_size = block_begin - reinterpret_cast<unsigned char *>(prev_block) - get_block_size(prev_block);
        occupied_size = get_block_meta_size() + get_block_size(prev_block);

        blocks.push_back({.block_size = occupied_size, .is_block_occupied = true});

        if (free_size)
        {
            blocks.push_back({.block_size = free_size, .is_block_occupied = false});
        }
    }

    return blocks;
}

void allocator_boundary_tags::debug_blocks_info(std::string call_function_name) const
{
    std::ostringstream str_stream;
    auto blocks_info = create_blocks_info();

    for (auto data : blocks_info)
    {
        if (data.is_block_occupied)
        {
            str_stream << " occup " << data.block_size - get_block_meta_size() << " |";
        }
        else
        {
            str_stream << " avail " << data.block_size << " |";
        }
    }

    debug_with_guard(call_function_name + " : memory map: |" + str_stream.str());
}

inline allocator *allocator_boundary_tags::get_allocator() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    return *reinterpret_cast<allocator**>(ptr);
}

inline logger *allocator_boundary_tags::get_logger() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*);

    return *reinterpret_cast<logger**>(ptr);
}

inline allocator_with_fit_mode::fit_mode &allocator_boundary_tags::get_fit_mode() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(logger*);

    return *reinterpret_cast<fit_mode*>(ptr);
}

inline std::mutex &allocator_boundary_tags::get_mutex() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(fit_mode);

    return *reinterpret_cast<std::mutex*>(ptr);
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_head_block() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(class logger*) +
           sizeof(fit_mode) + sizeof(std::mutex) + 2 * sizeof(block_size_t);

    return *reinterpret_cast<allocator::block_pointer_t*>(ptr);
}

inline allocator::block_size_t allocator_boundary_tags::get_allocator_size() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(class logger*) + sizeof(fit_mode) + sizeof(std::mutex);

    return *reinterpret_cast<block_size_t*>(ptr);
}

inline allocator::block_size_t &allocator_boundary_tags::get_free_space() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(class logger*) + sizeof(fit_mode) + sizeof(std::mutex) + sizeof(size_t);

    return *reinterpret_cast<block_size_t*>(ptr);
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return "allocator_boundary_tags";
}

inline allocator::block_size_t allocator_boundary_tags::get_meta_size() const
{
    return sizeof(allocator*) + sizeof(class logger*) + sizeof(fit_mode) +
            sizeof(std::mutex) + 2 * sizeof(block_size_t) + sizeof(block_pointer_t);
}

inline allocator::block_size_t allocator_boundary_tags::get_block_meta_size() const
{
    return sizeof(block_size_t) + 2 * sizeof(block_pointer_t) + sizeof(allocator*);
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_next_block(
        block_pointer_t block) const
{
    return *reinterpret_cast<allocator::block_pointer_t*>(
            reinterpret_cast<unsigned char *>(block) - sizeof(block_pointer_t));
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_prev_block(
        block_pointer_t block) const
{
    return *reinterpret_cast<allocator::block_pointer_t*>(
            reinterpret_cast<unsigned char *>(block) - 2 * sizeof(block_pointer_t));
}

inline allocator::block_size_t &allocator_boundary_tags::get_block_size(
        block_pointer_t block) const
{
    return *reinterpret_cast<allocator::block_size_t *>(
            reinterpret_cast<unsigned char *>(block) - 2 * sizeof(block_pointer_t) - sizeof(block_size_t));
}

inline allocator *&allocator_boundary_tags::get_block_allocator(
        block_pointer_t block) const
{
    return *reinterpret_cast<allocator**>(
            reinterpret_cast<unsigned char*>(block) - 2 * sizeof(block_pointer_t) - sizeof(block_size_t) - sizeof(allocator*));
}