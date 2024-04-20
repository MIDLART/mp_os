#include <not_implemented.h>
#include <cmath>
#include <sstream>

#include "../include/allocator_buddies_system.h"

allocator_buddies_system::~allocator_buddies_system()
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

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system &&other) noexcept:
    _trusted_memory(other._trusted_memory)
{
    trace_with_guard(get_typename() + "::allocator_allocator_buddies_system(allocator_boundary_tags &&) called");

    other._trusted_memory = nullptr;

    trace_with_guard(get_typename() + "::allocator_allocator_buddies_system(allocator_boundary_tags &&) finished");
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system &&other) noexcept
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

allocator_buddies_system::allocator_buddies_system(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->trace(get_typename() + "::allocator_allocator_buddies_system(size_t, allocator *, logger *, fit_mode) called");
    }

    if (space_size > 127)
    {
        if (logger != nullptr)
        {
            logger->error(get_typename() + "::allocator_allocator_buddies_system(size_t, allocator *, logger *, fit_mode) size degree of " +
                          std::to_string(space_size) + "is too big to initialize allocator instance. Maximum size degree is 127");
        }

        throw std::logic_error("Cannot initialize allocator of this size");
    }

    block_size_t sp_size = 1 << space_size;

    if (sp_size < get_avail_block_meta_size())
    {
        if (logger != nullptr)
        {
            logger->error(get_typename() + "::allocator_allocator_buddies_system(size_t, allocator *, logger *, fit_mode) size of " +
                          std::to_string(sp_size) + "bytes is too small to initialize allocator instance. " +
                          "Minimum size is " + std::to_string(get_occup_block_meta_size()) + " bytes.");
        }

        throw std::logic_error("Cannot initialize allocator of this size");
    }

    auto ful_size = sp_size + get_meta_size();

    try
    {
        if (parent_allocator == nullptr)
        {
            _trusted_memory = ::operator new(ful_size);
        }
        else
        {
            _trusted_memory = parent_allocator->allocate(ful_size, 1);
        }

    }
    catch (std::bad_alloc const &)
    {
        if (logger != nullptr)
        {
            logger->error(get_typename() + "::allocator_allocator_buddies_system(size_t, allocator *, logger *, fit_mode) " +
                          "bad alloc occured while trying to allocate " + std::to_string(ful_size) + "bytes");
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

    *reinterpret_cast<unsigned char *>(ptr) = space_size;
    ptr += sizeof(unsigned char);

    *reinterpret_cast<block_size_t*>(ptr) = sp_size;
    ptr += sizeof(block_size_t);

    auto block_ptr= ptr + sizeof(block_pointer_t);

    *reinterpret_cast<block_pointer_t *>(ptr) = block_ptr;

    *reinterpret_cast<unsigned char *>(block_ptr) = space_size;
    block_ptr += sizeof(unsigned char);

    *reinterpret_cast<block_pointer_t*>(block_ptr) = nullptr; // prev
    block_ptr += sizeof(block_pointer_t);

    *reinterpret_cast<block_pointer_t*>(block_ptr) = nullptr; // next

    trace_with_guard(get_typename() + "::allocator_allocator_buddies_system(size_t, allocator *, logger *, fit_mode) finished");
}

[[nodiscard]] void *allocator_buddies_system::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard(get_typename() + "::allocate(size_t, size_t) called")->
    debug_with_guard(get_typename() + "::allocate(size_t, size_t) called (value_size = " +
        std::to_string(value_size) + ", value_count = " + std::to_string(values_count) + ")");

    std::lock_guard<std::mutex> lock (get_mutex());

    block_size_t req_size = value_size * values_count;
    block_size_t size = req_size + get_occup_block_meta_size();

    auto min_size = get_avail_block_meta_size();
    if (size < min_size)
    {
        size = min_size;
    }

    unsigned char degree = 1;
    block_size_t block_size = 2;

    while (block_size < size)
    {
        block_size *= 2;
        degree++;
    }

    block_pointer_t target_block = nullptr;
    block_pointer_t prev = nullptr;
    block_pointer_t next = nullptr;

    block_pointer_t &head_block = get_head_block();
    block_pointer_t cur_block = head_block;
    block_pointer_t prev_block = nullptr;
    unsigned char cur_degree;

    if (cur_block == nullptr)
    {
        error_with_guard(get_typename() + "::allocate(size_t, size_t) : no space");

        throw std::bad_alloc();
    }

    unsigned char target_degree = get_block_degree(cur_block);

    auto fit = get_fit_mode();

    while (cur_block != nullptr && !(fit == fit_mode::first_fit && target_block))
    {
        cur_degree = get_block_degree(cur_block);

        if ((cur_degree >= degree) && (target_block == nullptr ||
                                                       (fit == fit_mode::the_best_fit && (cur_degree < target_degree)) ||
                                                       (fit == fit_mode::the_worst_fit && (cur_degree > target_degree))))
        {
            target_block = cur_block;
            target_degree = cur_degree;
        }

        prev_block = cur_block;
        cur_block = get_next_block(cur_block);
    }

    if (target_block == nullptr)
    {
        error_with_guard(get_typename() + "::allocate(size_t, size_t) : no space to allocate requested " +
                         std::to_string(req_size) + " bytes");

        throw std::bad_alloc();
    }

    int d = target_degree - degree;
    for(int i = 0; i < d; ++i)
    {
        split(target_block);
    }

    get_free_space() -= block_size;

    prev = get_prev_block(target_block);
    next = get_next_block(target_block);

    if (prev != nullptr)
    {
        get_next_block(prev) = next;
    }

    if (next != nullptr)
    {
        get_prev_block(next) = prev;
    }

    if (target_block == get_head_block())
    {
        get_head_block() = next;
    }

    unsigned char mask = 0b10000000;

    get_block_status_and_degree(target_block) = degree | mask;
    get_block_allocator(target_block) = this;

    debug_with_guard(get_typename() + "::allocate(size_t, size_t) : allocated " + std::to_string(req_size) +
                     "(+ meta: " + std::to_string(get_occup_block_meta_size()) + ") bytes");
    debug_blocks_info(get_typename() + "::allocate(size_t, size_t)");
    information_with_guard(get_typename() + "::allocate(size_t, size_t) free space left: " +
                           std::to_string(get_free_space()) + " bytes")->
            trace_with_guard(get_typename() + "::allocate(size_t, size_t) finished")->
            debug_with_guard(get_typename() + "::allocate(size_t, size_t) finished");

    return reinterpret_cast<allocator_buddies_system*>(
            reinterpret_cast<unsigned char *>(target_block) + get_occup_block_meta_size());
}

void allocator_buddies_system::deallocate(
    void *at)
{
    trace_with_guard(get_typename() + "::deallocate(void *) called")
            ->debug_with_guard(get_typename() + "::deallocate(void *) called");

    std::lock_guard<std::mutex> lock (get_mutex());

    if (at == nullptr)
    {
        return;
    }

    auto at_begin = reinterpret_cast<unsigned char *>(at) - get_occup_block_meta_size();

    unsigned char* begin = reinterpret_cast<unsigned char*>(_trusted_memory) + get_meta_size();

    auto allocator_degree = get_allocator_degree();
    unsigned char* end = begin + (1 << allocator_degree);

    if (at < begin || at >= end || get_block_allocator(at_begin) != this)
    {
        error_with_guard(get_typename() + "::deallocate(void *) tried to deallocate non-related memory");
        throw std::logic_error("try of deallocation non-related memory");
    }

    block_size_t size = 1 << get_block_degree(at_begin);

    get_free_space() += size;

    auto block = reinterpret_cast<block_pointer_t>(at_begin);
    block_pointer_t buddy = get_buddy(block);
    unsigned char mask = 0b01111111;

    get_block_status_and_degree(block) = get_block_degree(block) & mask;

    if (get_block_degree(block) < allocator_degree && !get_block_status(buddy))
    {
        get_prev_block(block) = get_prev_block(buddy);
        get_next_block(block) = get_next_block(buddy);
    }
    else
    {
        block_pointer_t cur_avail = get_head_block();
        block_pointer_t next_avail = nullptr;
        if (cur_avail != nullptr)
        {
            next_avail = get_next_block(cur_avail);

            if (cur_avail > block)
            {
                next_avail = cur_avail;
                cur_avail = nullptr;
            }
        }

        while (next_avail != nullptr && next_avail < block)
        {
            cur_avail = next_avail;
            next_avail = get_next_block(next_avail);
        }

        get_next_block(block) = next_avail;
        get_prev_block(block) = cur_avail;
    }


    while (get_block_degree(block) < allocator_degree &&
            get_block_status_and_degree(buddy) == get_block_status_and_degree(block))
    {
        block = merge(block, buddy);

        buddy = get_buddy(block);
    }

    if (get_head_block() > block)
    {
        get_head_block() = block;
    }

    std::string dump = get_block_dump(at, size);

    debug_with_guard(get_typename() + "::deallocate(void *) deallocated " +
                     std::to_string(size) + " bytes" + dump);
    debug_blocks_info(get_typename() + "::deallocate(void *)");
    information_with_guard(get_typename() + "::deallocate(void *) free space left: " +
                           std::to_string(get_free_space()) + " bytes.")->
            trace_with_guard(get_typename() + "::deallocate(void *) finished")->
            debug_with_guard(get_typename() + "::deallocate(void *) finished");
}

inline void allocator_buddies_system::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    get_fit_mode() = mode;
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept
{
    trace_with_guard(get_typename() + "::get_blocks_info() called");

    std::lock_guard<std::mutex> lock (get_mutex());

    auto blocks = create_blocks_info();

    trace_with_guard(get_typename() + "::get_blocks_info() finished");

    return blocks;
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::create_blocks_info() const noexcept
{
    std::vector<allocator_test_utils::block_info> blocks(0);

    auto cur_block = reinterpret_cast<unsigned char *>(_trusted_memory) + get_meta_size();
    auto end = cur_block + (1 << get_allocator_degree());

    block_size_t size;

    while (cur_block < end)
    {
        size = 1 << get_block_degree(cur_block);

        blocks.push_back({.block_size = size, .is_block_occupied = get_block_status(cur_block)});
        cur_block += size;
    }

    return blocks;
}

void allocator_buddies_system::debug_blocks_info(std::string call_function_name) const
{
    std::ostringstream str_stream;
    auto blocks_info = create_blocks_info();

    for (auto data : blocks_info)
    {
        if (data.is_block_occupied)
        {
            str_stream << " occup " << data.block_size << " |";
        }
        else
        {
            str_stream << " avail " << data.block_size << " |";
        }
    }

    debug_with_guard(call_function_name + " : memory map: |" + str_stream.str());
}

inline allocator *allocator_buddies_system::get_allocator() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    return *reinterpret_cast<allocator**>(ptr);
}

inline logger *allocator_buddies_system::get_logger() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*);

    return *reinterpret_cast<logger**>(ptr);
}

inline allocator_with_fit_mode::fit_mode &allocator_buddies_system::get_fit_mode() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(logger*);

    return *reinterpret_cast<fit_mode*>(ptr);
}

inline std::mutex &allocator_buddies_system::get_mutex() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(fit_mode);

    return *reinterpret_cast<std::mutex*>(ptr);
}

inline allocator::block_pointer_t &allocator_buddies_system::get_head_block() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(class logger*) +
           sizeof(fit_mode) + sizeof(std::mutex) + sizeof(unsigned char) + sizeof(block_size_t);

    return *reinterpret_cast<allocator::block_pointer_t*>(ptr);
}

inline unsigned char allocator_buddies_system::get_allocator_degree() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(class logger*) + sizeof(fit_mode) + sizeof(std::mutex);

    return *ptr;
}

inline allocator::block_size_t &allocator_buddies_system::get_free_space() const
{
    auto *ptr = reinterpret_cast<unsigned char *>(_trusted_memory);

    ptr += sizeof(allocator*) + sizeof(class logger*) + sizeof(fit_mode) + sizeof(std::mutex) + sizeof(unsigned char);

    return *reinterpret_cast<block_size_t*>(ptr);
}

inline allocator::block_size_t allocator_buddies_system::get_meta_size() const
{
    return sizeof(allocator*) + sizeof(class logger*) + sizeof(fit_mode) +
           sizeof(std::mutex) + sizeof(unsigned char) + sizeof(block_size_t) + sizeof(block_pointer_t);
}

inline allocator::block_size_t allocator_buddies_system::get_avail_block_meta_size() const
{
    return  sizeof(unsigned char) + 2 * sizeof(block_pointer_t);
}

inline allocator::block_size_t allocator_buddies_system::get_occup_block_meta_size() const
{
    return sizeof(unsigned char) + sizeof(allocator*);
}

inline unsigned char &allocator_buddies_system::get_block_status_and_degree(
        block_pointer_t block) const
{
    return *reinterpret_cast<unsigned char *>(block);
}

inline bool allocator_buddies_system::get_block_status(
        block_pointer_t block) const
{
    unsigned char mask = 0b10000000;

    return *reinterpret_cast<unsigned char *>(block) & mask;
}

inline unsigned char allocator_buddies_system::get_block_degree(
        block_pointer_t block) const
{
    unsigned char mask = 0b01111111;

    return *reinterpret_cast<unsigned char *>(block) & mask;
}

inline allocator::block_pointer_t &allocator_buddies_system::get_next_block(
        block_pointer_t block) const
{
    return *reinterpret_cast<allocator::block_pointer_t*>(
            reinterpret_cast<unsigned char *>(block) + sizeof(unsigned char) + sizeof(block_pointer_t));
}

inline allocator::block_pointer_t &allocator_buddies_system::get_prev_block(
        block_pointer_t block) const
{
    return *reinterpret_cast<allocator::block_pointer_t*>(
            reinterpret_cast<unsigned char *>(block) + sizeof(unsigned char));
}

inline allocator *&allocator_buddies_system::get_block_allocator(
        block_pointer_t block) const
{
    return *reinterpret_cast<allocator**>(
            reinterpret_cast<unsigned char*>(block) + sizeof(unsigned char));
}

inline std::string allocator_buddies_system::get_typename() const noexcept
{
    return "allocator_buddies_system";
}

inline allocator::block_pointer_t allocator_buddies_system::get_buddy(
        block_pointer_t block) const
{
    size_t size = 1 << get_block_degree(block);
    auto address = reinterpret_cast<size_t>(block);

    return reinterpret_cast<block_pointer_t>(address ^ size);
}

void allocator_buddies_system::split(block_pointer_t block) const
{
    --get_block_status_and_degree(block);

    block_pointer_t new_block = reinterpret_cast<unsigned char *>(block) + (1 << get_block_degree(block));

    block_pointer_t next = get_next_block(block);
    if (next != nullptr)
    {
        get_prev_block(next) = new_block;
    }

    get_next_block(new_block) = next;
    get_prev_block(new_block) = block;
    get_next_block(block) = new_block;

    get_block_status_and_degree(new_block) = get_block_status_and_degree(block);
}

allocator::block_pointer_t &allocator_buddies_system::merge(
        block_pointer_t block, block_pointer_t buddy) const
{
    unsigned char degree = get_block_degree(block);

    block_pointer_t res_block = buddy;
    block_pointer_t second_block = block;

    if (block < buddy)
    {
        res_block = block;
        second_block = buddy;

        auto next = get_next_block(buddy);
        auto prev = get_prev_block(buddy);

        get_next_block(block) = next;
        get_prev_block(block) = prev;

        if (prev != nullptr)
        {
            get_next_block(prev) = block;
        }

        if (next != nullptr)
        {
            get_prev_block(next) = block;
        }
    }

    ++degree;

    get_block_status_and_degree(res_block) = degree;

    return res_block;
}