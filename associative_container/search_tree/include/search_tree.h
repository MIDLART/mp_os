#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H

#include <iostream>
#include <functional>
#include <stack>
#include <vector>

#include <allocator.h>
#include <allocator_guardant.h>
#include <associative_container.h>
#include <logger.h>
#include <logger_guardant.h>
#include <not_implemented.h>

template<
        typename tkey,
        typename tvalue>
class search_tree:
        public associative_container<tkey, tvalue>,
        protected allocator_guardant,
        protected logger_guardant
{

public:

    struct __attribute__((unused)) common_node
    {

    public:

        typename associative_container<tkey, tvalue>::key_value_pair *keys_and_values;

        common_node **subtrees;

        size_t virtual_size;

    public:

        common_node();

        virtual ~common_node() noexcept;

    };

protected:

    std::function<int(tkey const &, tkey const &)> _keys_comparer;

protected:

    void *_root;

    logger *_logger;

    allocator *_allocator;

protected:

    explicit search_tree(
            std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
            allocator *allocator = nullptr,
            logger *logger = nullptr,
            void *_root = nullptr);

public:

    virtual std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
            tkey const &lower_bound,
            tkey const &upper_bound,
            bool lower_bound_inclusive,
            bool upper_bound_inclusive) = 0;

protected:

    [[nodiscard]] inline allocator *get_allocator() const final;

    [[nodiscard]] inline logger *get_logger() const final;

};

#pragma region search_tree<tkey, tvalue>::node implementation

template<
        typename tkey,
        typename tvalue>
search_tree<tkey, tvalue>::common_node::common_node()
{
    throw not_implemented("template<typename tkey, typename tvalue> search_tree<tkey, tvalue>::common_node::common_node()", "your code should be here...");
}

template<
        typename tkey,
        typename tvalue>
search_tree<tkey, tvalue>::common_node::~common_node() noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> search_tree<tkey, tvalue>::common_node::~common_node() noexcept", "your code should be here...");
}

#pragma endregion search_tree<tkey, tvalue>::node implementation

template<
        typename tkey,
        typename tvalue>
search_tree<tkey, tvalue>::search_tree(
        std::function<int(tkey const &, tkey const &)> keys_comparer,
        allocator *allocator,
        logger *logger,
        void *root):
        _root(root),
        _keys_comparer(keys_comparer),
        _logger(logger),
        _allocator(allocator)
{
    std::cout << "__search_tree__\n";
}

template<
        typename tkey,
        typename tvalue>
[[nodiscard]] inline allocator *search_tree<tkey, tvalue>::get_allocator() const
{
    return _allocator;
}

template<
        typename tkey,
        typename tvalue>
[[nodiscard]] inline logger *search_tree<tkey, tvalue>::get_logger() const
{
    return _logger;
}

namespace extra_utility
{

    template<
            typename T>
    std::string make_string(T const &value);

    template<> std::string make_string(int const &value);
    template<> std::string make_string(long const &value);
    template<> std::string make_string(long long const &value);
    template<> std::string make_string(unsigned const &value);
    template<> std::string make_string(unsigned long const &value);
    template<> std::string make_string(unsigned long long const &value);
    template<> std::string make_string(float const &value);
    template<> std::string make_string(double const &value);
    template<> std::string make_string(long double const &value);
    template<> std::string make_string(std::string const &value);

}

template<
        typename T>
std::string extra_utility::make_string(T const &value)
{
    return "unimplemented parse";
}

template<> std::string extra_utility::make_string(int const &value)                 { return std::to_string(value); }
template<> std::string extra_utility::make_string(long const &value)                { return std::to_string(value); }
template<> std::string extra_utility::make_string(long long const &value)           { return std::to_string(value); }
template<> std::string extra_utility::make_string(unsigned const &value)            { return std::to_string(value); }
template<> std::string extra_utility::make_string(unsigned long const &value)       { return std::to_string(value); }
template<> std::string extra_utility::make_string(unsigned long long const &value)  { return std::to_string(value); }
template<> std::string extra_utility::make_string(float const &value)               { return std::to_string(value); }
template<> std::string extra_utility::make_string(double const &value)              { return std::to_string(value); }
template<> std::string extra_utility::make_string(long double const &value)         { return std::to_string(value); }
template<> std::string extra_utility::make_string(std::string const &value)         { return value; }

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H