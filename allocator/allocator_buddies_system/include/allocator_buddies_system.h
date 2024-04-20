#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>

#include <mutex>

class allocator_buddies_system final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

public:
    
    ~allocator_buddies_system() override;
    
    allocator_buddies_system(
        allocator_buddies_system const &other) = delete;
    
    allocator_buddies_system &operator=(
        allocator_buddies_system const &other) = delete;
    
    allocator_buddies_system(
        allocator_buddies_system &&other) noexcept;
    
    allocator_buddies_system &operator=(
        allocator_buddies_system &&other) noexcept;

public:
    
    explicit allocator_buddies_system(
        size_t space_size_power_of_two,
        allocator *parent_allocator = nullptr,
        logger *logger = nullptr,
        allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:
    
    [[nodiscard]] void *allocate(
        size_t value_size,
        size_t values_count) override;
    
    void deallocate(
        void *at) override;

public:
    
    inline void set_fit_mode(
        allocator_with_fit_mode::fit_mode mode) override;

private:
    
    inline allocator *get_allocator() const override;

    inline logger *get_logger() const override;

    inline allocator_with_fit_mode::fit_mode &get_fit_mode() const;

    inline std::mutex &get_mutex() const;

    // inline allocator::block_size_t get_allocator_size() const;

    inline unsigned char get_allocator_degree() const;

    inline allocator::block_size_t &get_free_space() const;

    inline allocator::block_pointer_t &get_head_block() const;

private:

    inline allocator::block_size_t get_meta_size() const;

    inline allocator::block_size_t get_avail_block_meta_size() const;

    inline allocator::block_size_t get_occup_block_meta_size() const;

private:

    inline unsigned char &get_block_status_and_degree(
            block_pointer_t block) const;

    inline bool get_block_status(
            block_pointer_t block) const;

    inline unsigned char get_block_degree(
            block_pointer_t block) const;

    inline allocator::block_pointer_t &get_next_block(
            block_pointer_t block) const; // for available block

    inline allocator::block_pointer_t &get_prev_block(
            block_pointer_t block) const; // for available block

//    inline allocator::block_size_t &get_block_size(
//            block_pointer_t block) const;

    inline allocator *&get_block_allocator(
            block_pointer_t block) const; // for occupied block

public:
    
    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

    std::vector<allocator_test_utils::block_info> create_blocks_info() const noexcept;

    void debug_blocks_info(std::string call_function_name) const;

private:
    
    inline std::string get_typename() const noexcept override;

private:

    inline allocator::block_pointer_t get_buddy(block_pointer_t block) const;

    void split(block_pointer_t block) const;

    allocator::block_pointer_t &merge(
            block_pointer_t block, block_pointer_t buddy) const;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BUDDIES_SYSTEM_H
