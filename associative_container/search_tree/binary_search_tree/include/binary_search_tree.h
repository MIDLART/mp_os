#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <optional>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>

template<
        typename tkey,
        typename tvalue>
class binary_search_tree:
        public search_tree<tkey, tvalue>
{

protected:

    struct node
    {

    public:

        tkey key;

        tvalue value;

        node *left_subtree;

        node *right_subtree;

    public:

        explicit node(
                tkey const &key,
                tvalue const &value);

        explicit node(
                tkey const &key,
                tvalue &&value);

    public:

        virtual ~node() noexcept = default;

    };

#pragma region iterators definition

private:

    class iterator;
    class const_iterator;

public:

    struct iterator_data
    {

        friend class const_iterator;
        friend void binary_search_tree<tkey, tvalue>::inject_additional_data(
                iterator_data *,
                node *) const;

    protected:

        tkey *_key;

        tvalue *_value;

        unsigned int _depth;

        bool _is_state_initialized;

    public:

        inline tkey const &get_key() const;

        inline tvalue const &get_value() const;

        inline bool is_state_initialized() const noexcept;

        inline unsigned int get_depth() const;

    public:

        iterator_data();

        explicit iterator_data(
                unsigned int depth,
                tkey const &key,
                tvalue const &value);

        explicit iterator_data(
                unsigned int depth,
                node **src_node);

    public:

        iterator_data(
                iterator_data const &other);

        iterator_data &operator=(
                iterator_data const &other);

        iterator_data(
                iterator_data &&other) noexcept;

        iterator_data &operator=(
                iterator_data &&other) noexcept;

        virtual ~iterator_data() noexcept;

    };

private:

    class const_iterator
    {

        friend class binary_search_tree<tkey, tvalue>;

    protected:

        iterator_data *_data;
        binary_search_tree<tkey, tvalue> const *_holder;
        std::stack<node*> _state;

    public:

        explicit const_iterator(
                binary_search_tree<tkey, tvalue> const *holder);

        explicit const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

        const_iterator(
                const_iterator const &other);

        const_iterator(
                const_iterator &&other) noexcept;

        const_iterator &operator=(
                const_iterator const &other);

        const_iterator &operator=(
                const_iterator &&other) noexcept;

        virtual ~const_iterator() noexcept;

    public:

        bool operator==(
                const_iterator const &other) const noexcept;

        bool operator!=(
                const_iterator const &other) const noexcept;

        iterator_data *operator*() const;

        const_iterator &operator++();

        const_iterator const operator++(
                int not_used);

    protected:

        virtual void pass_to_next_node() = 0;

        void assign_data();

    };

private:

    class iterator:
            public const_iterator
    {

    public:

        explicit iterator(
                binary_search_tree<tkey, tvalue> const *holder);

        explicit iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

        iterator(
                iterator const &other);

        iterator(
                iterator &&other) noexcept;

        iterator &operator=(
                iterator const &other);

        iterator &operator=(
                iterator &&other) noexcept;

        virtual ~iterator() noexcept = default;

    public:

        void set_value(
                tvalue const &value);

        void set_value(
                tvalue &&value) noexcept;

    };

    class prefix_iterator final:
            public iterator
    {

    public:

        explicit prefix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit prefix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        prefix_iterator(
                prefix_iterator const &other);

        prefix_iterator &operator=(
                prefix_iterator const &other);

        prefix_iterator(
                prefix_iterator &&other) noexcept;

        prefix_iterator &operator=(
                prefix_iterator &&other) noexcept;

        ~prefix_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class prefix_const_iterator final:
            public const_iterator
    {

    public:

        explicit prefix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit prefix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        prefix_const_iterator(
                prefix_const_iterator const &other);

        prefix_const_iterator &operator=(
                prefix_const_iterator const &other);

        prefix_const_iterator(
                prefix_const_iterator &&other) noexcept;

        prefix_const_iterator &operator=(
                prefix_const_iterator &&other) noexcept;

        ~prefix_const_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class prefix_reverse_iterator final:
        public iterator
    {

        public:

        explicit prefix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit prefix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

        public:

        prefix_reverse_iterator(
                prefix_reverse_iterator const &other);

        prefix_reverse_iterator &operator=(
                prefix_reverse_iterator const &other);

        prefix_reverse_iterator(
                prefix_reverse_iterator &&other) noexcept;

        prefix_reverse_iterator &operator=(
                prefix_reverse_iterator &&other) noexcept;

        ~prefix_reverse_iterator() noexcept = default;

        private:

        void pass_to_next_node() override;

    };

    class prefix_const_reverse_iterator final:
            public const_iterator
    {

    public:

        explicit prefix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit prefix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        prefix_const_reverse_iterator(
                prefix_const_reverse_iterator const &other);

        prefix_const_reverse_iterator &operator=(
                prefix_const_reverse_iterator const &other);

        prefix_const_reverse_iterator(
                prefix_const_reverse_iterator &&other) noexcept;

        prefix_const_reverse_iterator &operator=(
                prefix_const_reverse_iterator &&other) noexcept;

        ~prefix_const_reverse_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class infix_iterator final:
            public iterator
    {

    public:

        explicit infix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit infix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        infix_iterator(
                infix_iterator const &other);

        infix_iterator &operator=(
                infix_iterator const &other);

        infix_iterator(
                infix_iterator &&other) noexcept;

        infix_iterator &operator=(
                infix_iterator &&other) noexcept;

        ~infix_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class infix_const_iterator final:
            public const_iterator
    {

    public:

        explicit infix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit infix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        infix_const_iterator(
                infix_const_iterator const &other);

        infix_const_iterator &operator=(
                infix_const_iterator const &other);

        infix_const_iterator(
                infix_const_iterator &&other) noexcept;

        infix_const_iterator &operator=(
                infix_const_iterator &&other) noexcept;

        ~infix_const_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class infix_reverse_iterator final:
            public iterator
    {

    public:

        explicit infix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit infix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        infix_reverse_iterator(
                infix_reverse_iterator const &other);

        infix_reverse_iterator &operator=(
                infix_reverse_iterator const &other);

        infix_reverse_iterator(
                infix_reverse_iterator &&other) noexcept;

        infix_reverse_iterator &operator=(
                infix_reverse_iterator &&other) noexcept;

        ~infix_reverse_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class infix_const_reverse_iterator final:
            public const_iterator
    {

    public:

        explicit infix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit infix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        infix_const_reverse_iterator(
                infix_const_reverse_iterator const &other);

        infix_const_reverse_iterator &operator=(
                infix_const_reverse_iterator const &other);

        infix_const_reverse_iterator(
                infix_const_reverse_iterator &&other) noexcept;

        infix_const_reverse_iterator &operator=(
                infix_const_reverse_iterator &&other) noexcept;

        ~infix_const_reverse_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class postfix_iterator final:
            public iterator
    {

    public:

        explicit postfix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit postfix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        postfix_iterator(
                postfix_iterator const &other);

        postfix_iterator &operator=(
                postfix_iterator const &other);

        postfix_iterator(
                postfix_iterator &&other) noexcept;

        postfix_iterator &operator=(
                postfix_iterator &&other) noexcept;

        ~postfix_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class postfix_const_iterator final:
            public const_iterator
    {

    public:

        explicit postfix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit postfix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        postfix_const_iterator(
                postfix_const_iterator const &other);

        postfix_const_iterator &operator=(
                postfix_const_iterator const &other);

        postfix_const_iterator(
                postfix_const_iterator &&other) noexcept;

        postfix_const_iterator &operator=(
                postfix_const_iterator &&other) noexcept;

        ~postfix_const_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class postfix_reverse_iterator final:
            public iterator
    {

    public:

        explicit postfix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit postfix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        postfix_reverse_iterator(
                postfix_reverse_iterator const &other);

        postfix_reverse_iterator &operator=(
                postfix_reverse_iterator const &other);

        postfix_reverse_iterator(
                postfix_reverse_iterator &&other) noexcept;

        postfix_reverse_iterator &operator=(
                postfix_reverse_iterator &&other) noexcept;

        ~postfix_reverse_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

    class postfix_const_reverse_iterator final:
            public const_iterator
    {

    public:

        explicit postfix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root);

        explicit postfix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node*> const &state);

    public:

        postfix_const_reverse_iterator(
                postfix_const_reverse_iterator const &other);

        postfix_const_reverse_iterator &operator=(
                postfix_const_reverse_iterator const &other);

        postfix_const_reverse_iterator(
                postfix_const_reverse_iterator &&other) noexcept;

        postfix_const_reverse_iterator &operator=(
                postfix_const_reverse_iterator &&other) noexcept;

        ~postfix_const_reverse_iterator() noexcept = default;

    private:

        void pass_to_next_node() override;

    };

#pragma endregion iterators definition

public:

#pragma region target operations strategies definition

    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };

    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };

#pragma endregion target operations strategies definition

#pragma region target operations associated exception types

    class insertion_of_existent_key_attempt_exception final:
            public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit insertion_of_existent_key_attempt_exception(
                tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    class obtaining_of_nonexistent_key_attempt_exception final:
            public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit obtaining_of_nonexistent_key_attempt_exception(
                tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    class disposal_of_nonexistent_key_attempt_exception final:
            public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit disposal_of_nonexistent_key_attempt_exception(
                tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

#pragma endregion target operations associated exception types

#pragma region template methods definition

protected:

    class template_method_basics:
            public logger_guardant
    {

    protected:

        binary_search_tree<tkey, tvalue> *_tree;

    public:

        explicit template_method_basics(
                binary_search_tree<tkey, tvalue> *tree);

    protected:

        std::stack<node**> find_path(
                tkey const &key) const;

        virtual void balance(
                std::stack<node **> &path);

    protected:

        [[nodiscard]] logger *get_logger() const noexcept final;

    };

    class insertion_template_method:
            public template_method_basics,
            public allocator_guardant
    {

    public:

        binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy _insertion_strategy;

    public:

        explicit insertion_template_method(
                binary_search_tree<tkey, tvalue> *tree,
                typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);

    public:

        void insert(
                tkey const &key,
                tvalue const &value);

        void insert(
                tkey const &key,
                tvalue &&value);

//        void set_insertion_strategy(
//                typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

    private:

        [[nodiscard]] allocator *get_allocator() const noexcept final;

    };

    class obtaining_template_method:
            public template_method_basics
    {

    public:

        explicit obtaining_template_method(
                binary_search_tree<tkey, tvalue> *tree);

    public:

        tvalue const &obtain(
                tkey const &key);

        std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
                tkey const &lower_bound,
                tkey const &upper_bound,
                bool lower_bound_inclusive,
                bool upper_bound_inclusive);

    };

    class disposal_template_method:
            public template_method_basics,
            public allocator_guardant
    {

    private:

        binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy _disposal_strategy;

    public:

        explicit disposal_template_method(
                binary_search_tree<tkey, tvalue> *tree,
                typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    public:

        tvalue dispose(
                tkey const &key);

        void set_disposal_strategy(
                typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

//    private: // TODO: !=std::swap?
//
//        template<
//                typename T>
//        inline void swap(
//                T &&one,
//                T &&another)
//        {
//            T temp = std::move(one);
//            one = std::move(another);
//            another = std::move(temp);
//        }

    private:

        [[nodiscard]] allocator *get_allocator() const noexcept final;

    };

#pragma endregion template methods definition

protected:

    insertion_template_method *_insertion_template;

    obtaining_template_method *_obtaining_template;

    disposal_template_method *_disposal_template;

protected:

    explicit binary_search_tree(
            typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
            typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
            typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
            std::function<int(tkey const &, tkey const &)>,
            allocator *allocator,
            logger *logger);

public:

    explicit binary_search_tree(
            std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),
            allocator *allocator = nullptr,
            logger *logger = nullptr,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

    binary_search_tree(
            binary_search_tree<tkey, tvalue> const &other);

    binary_search_tree(
            binary_search_tree<tkey, tvalue> &&other) noexcept;

    binary_search_tree<tkey, tvalue> &operator=(
            binary_search_tree<tkey, tvalue> const &other);

    binary_search_tree<tkey, tvalue> &operator=(
            binary_search_tree<tkey, tvalue> &&other) noexcept;

    ~binary_search_tree() override;

protected:

    void clear(
            node *&subtree_root);

    node *copy(
            node const *subtree_root);

    virtual inline size_t get_node_size() const noexcept;

    virtual inline void update_node_data(node *node) const noexcept;

    virtual inline void call_node_constructor(
            node *raw_space,
            tkey const &key,
            tvalue const &value);

    virtual inline void call_node_constructor(
            node *raw_space,
            tkey const &key,
            tvalue &&value);

    virtual void inject_additional_data(
            iterator_data *destination,
            node *source) const;

    virtual iterator_data *create_iterator_data() const;

    virtual iterator_data *create_iterator_data(
            unsigned int depth,
            node *&src_node) const;

public:

    void insert(
            tkey const &key,
            tvalue const &value) final;

    void insert(
            tkey const &key,
            tvalue &&value) final;

    tvalue const &obtain(
            tkey const &key) final;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
            tkey const &lower_bound,
            tkey const &upper_bound,
            bool lower_bound_inclusive,
            bool upper_bound_inclusive) final;

    tvalue dispose(
            tkey const &key) final;

public:

    void set_insertion_strategy(
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

    void set_removal_strategy(
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

public:

#pragma region iterators requests definition

    prefix_iterator begin_prefix() const noexcept;

    prefix_iterator end_prefix() const noexcept;

    prefix_const_iterator cbegin_prefix() const noexcept;

    prefix_const_iterator cend_prefix() const noexcept;

    prefix_reverse_iterator rbegin_prefix() const noexcept;

    prefix_reverse_iterator rend_prefix() const noexcept;

    prefix_const_reverse_iterator crbegin_prefix() const noexcept;

    prefix_const_reverse_iterator crend_prefix() const noexcept;

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

    infix_reverse_iterator rbegin_infix() const noexcept;

    infix_reverse_iterator rend_infix() const noexcept;

    infix_const_reverse_iterator crbegin_infix() const noexcept;

    infix_const_reverse_iterator crend_infix() const noexcept;

    postfix_iterator begin_postfix() const noexcept;

    postfix_iterator end_postfix() const noexcept;

    postfix_const_iterator cbegin_postfix() const noexcept;

    postfix_const_iterator cend_postfix() const noexcept;

    postfix_reverse_iterator rbegin_postfix() const noexcept;

    postfix_reverse_iterator rend_postfix() const noexcept;

    postfix_const_reverse_iterator crbegin_postfix() const noexcept;

    postfix_const_reverse_iterator crend_postfix() const noexcept;

#pragma endregion iterators requests definition

protected:

#pragma region subtree rotations definition

    void small_left_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void small_right_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void big_left_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void big_right_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void double_left_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool at_grandparent_first,
            bool validate = true) const;

    void double_right_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool at_grandparent_first,
            bool validate = true) const;

#pragma endregion subtree rotations definition

private:

    inline std::string get_typename() const noexcept;

};

#pragma region binary_search_tree<tkey, tvalue>::node methods implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
        tkey const &key,
        tvalue const &value):
        key(key),
        value(value),
        left_subtree(nullptr),
        right_subtree(nullptr)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
        tkey const &key,
        tvalue &&value):
        key(key),
        value(std::move(value)),
        left_subtree(nullptr),
        right_subtree(nullptr)
{ }

#pragma endregion binary_search_tree<tkey, tvalue>::node methods implementation

#pragma region iterators implementation

#pragma region iterator data implementation

template<
        typename tkey,
        typename tvalue>
inline tkey const &binary_search_tree<tkey, tvalue>::iterator_data::get_key() const
{
    if (!_is_state_initialized)
    {
        throw std::logic_error("Cannot read from uninitialized iterator data");
    }

    return *_key;
}

template<
        typename tkey,
        typename tvalue>
inline tvalue const &binary_search_tree<tkey, tvalue>::iterator_data::get_value() const
{
    if (!_is_state_initialized)
    {
        throw std::logic_error("Tried to read from uninitialized iterator data");
    }

    return *_value;
}

template<
        typename tkey,
        typename tvalue>
inline unsigned int binary_search_tree<tkey, tvalue>::iterator_data::get_depth() const
{
    if (!_is_state_initialized)
    {
        throw std::logic_error("Tried to read from uninitialized iterator data");
    }

    return _depth;
}

template<
        typename tkey,
        typename tvalue>
inline bool binary_search_tree<tkey, tvalue>::iterator_data::is_state_initialized() const noexcept
{
    return _is_state_initialized;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data():
        _is_state_initialized(false)
{
    try
    {
        _key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey)));
        _value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)));
    }
    catch (const std::bad_alloc &)
    {
        ::operator delete(_key);
        ::operator delete(_value);
        throw;
    }
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
        unsigned int depth,
        tkey const &key,
        tvalue const &value):
        iterator_data()
{
    allocator::construct(_key, key);
    allocator::construct(_value, value);

    _depth = depth;
    _is_state_initialized = true;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
        unsigned int depth,
        node **src_node):
        _depth(depth),
        _key(nullptr),
        _value(nullptr),
        _is_state_initialized(false)
{
    if (*src_node != nullptr)
    {
        _key = &((*src_node)->key);
        _value = &((*src_node)->value);
        _is_state_initialized = true;
    }
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
        iterator_data const &other):
        _depth(other._depth),
        _is_state_initialized(other._is_state_initialized)
{
    try
    {
        _key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey)));
        _value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)));
    }
    catch (const std::bad_alloc &)
    {
        ::operator delete(_key);
        ::operator delete(_value);
        throw;
    }

    allocator::construct(_key, *other._key);
    allocator::construct(_value, *other._value);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data &binary_search_tree<tkey, tvalue>::iterator_data::operator=(
        iterator_data const &other)
{
    if (this != &other)
    {
        try {
            _key = reinterpret_cast<tkey *>(::operator new(sizeof(tkey)));
            _value = reinterpret_cast<tvalue *>(::operator new(sizeof(tvalue)));
        }
        catch (const std::bad_alloc &) {
            ::operator delete(_key);
            ::operator delete(_value);
            throw;
        }

        allocator::construct(_key, *other._key);
        allocator::construct(_value, *other._value);

        _depth = other._depth;
        _is_state_initialized = other.is_state_initialized;
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
        iterator_data &&other) noexcept:
        _key(std::move(other._key)),
        _value(std::move(other._value)),
        _depth(std::move(other._depth)),
        _is_state_initialized(std::move(other._is_state_initialized))
{
    other._key = nullptr;
    other._value = nullptr;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data &binary_search_tree<tkey, tvalue>::iterator_data::operator=(
        iterator_data &&other) noexcept
{
    if (this != &other)
    {
        _depth = std::move(other._depth);
        _key = std::move(other._key);
        _value = std::move(other._value);
        _is_state_initialized = std::move(other._is_state_initialized);

        other._key = nullptr;
        other._value = nullptr;
    }

    return  *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::~iterator_data() noexcept
{
    if (_is_state_initialized)
    {
        allocator::destruct(_key);
        allocator::destruct(_value);
    }

    ::operator delete(_key);
    _key = nullptr;
    ::operator delete(_value);
    _value = nullptr;

    _is_state_initialized = false;
}

#pragma endregion iterator data implementation

#pragma region iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator::iterator(
        binary_search_tree<tkey, tvalue> const *holder):
        const_iterator(holder)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator::iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        const_iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator::iterator(
        iterator const &other):
        const_iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::iterator::iterator(
        iterator &&other) noexcept:
        const_iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator &binary_search_tree<tkey, tvalue>::iterator::operator=(
        iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator &binary_search_tree<tkey, tvalue>::iterator::operator=(
        iterator &&other) noexcept
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator::set_value(
        tvalue const &value)
{
    this->_state.top().value = value;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator::set_value(
        tvalue &&value) noexcept
{
    this->_state.top().value = std::move(value);
}

#pragma endregion iterator implementation

#pragma region const_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::const_iterator(
        binary_search_tree<tkey, tvalue> const *holder):
        _holder(holder)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::const_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        _holder(holder),
        _state(state)
{
    if (_state.empty())
    {
        _data = _holder->create_iterator_data();
    }
    else
    {
        _data = _holder->create_iterator_data(_state.size() - 1, _state.top());
    }
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::const_iterator(
        const_iterator const &other):
        _holder(other._holder),
        _state(other._state)
{
    if (_state.empty())
    {
        _data = _holder->create_iterator_data();
    }
    else
    {
        _data = _holder->create_iterator_data(_state.size() - 1, _state.top());
    }
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::const_iterator(
        const_iterator &&other) noexcept:
        _holder(std::move(other._holder)),
        _state(std::move(other._state)),
        _data(std::move(other._data))
{
    other._holder = nullptr;
    other._data = nullptr;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::const_iterator &binary_search_tree<tkey, tvalue>::const_iterator::operator=(
        const_iterator const &other)
{
    if (this != &other)
    {
        _holder = other._holder;
        _state = other._state;

        if (_state.empty())
        {
            _data = _holder->create_iterator_data();
        }
        else
        {
            _data = _holder->create_iterator_data(_state.size() - 1, _state.top());
        }
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::const_iterator &binary_search_tree<tkey, tvalue>::const_iterator::operator=(
        const_iterator &&other) noexcept
{
    if (this != &other)
    {
        _data = std::move(_data);
        _holder = std::move(other._holder);
        _state = std::move(other._state);

        other._holder = nullptr;
        other._data = nullptr;
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::const_iterator::~const_iterator() noexcept
{
    delete _data;
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::const_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::const_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }

    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::const_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::const_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("Cannot get data from empty iterator");
    }

    return _data;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::const_iterator &binary_search_tree<tkey, tvalue>::const_iterator::operator++()
{
    pass_to_next_node();

    if (!_state.empty())
    {
        this->assign_data();
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::const_iterator const binary_search_tree<tkey, tvalue>::const_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;

    return it;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::const_iterator::assign_data()
{
    _data->_key = &(_state.top()->key);
    _data->_value = &(_state.top()->value);
    _data->_is_state_initialized = true;

//    if (_data->is_state_initialized())
//    {
//        *(_data->_key) = _state.top()->key;
//        *(_data->_value) = _state.top()->value;
//    }
//    else
//    {
//        allocator::construct(_data->_key, _state.top()->key);
//        allocator::construct(_data->_value, _state.top()->value);
//        _data->_is_state_initialized = true;
//    }

    _data->_depth = _state.size() - 1;
    _holder->inject_additional_data(_data, _state.top());
}

#pragma endregion const_iterator implementation

#pragma region prefix_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        iterator(holder)
{
    if (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, subtree_root);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(
        prefix_iterator const &other):
        iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(
        prefix_iterator &&other) noexcept:
        iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator=(
        prefix_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator=(
        prefix_iterator &&other) noexcept
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
    }
    else if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
    }
    else
    {
        node *current;

        while (true)
        {
            current = _state.top();
            _state.pop();

            if (_state.empty())
            {
                break;
            }

            if (_state.top()->left_subtree == current && _state.top()->right_subtree != nullptr)
            {
                _state.push(_state.top()->right_subtree);

                break;
            }
        }
    }
}

#pragma endregion prefix_iterator implementation

#pragma region prefix_const_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        const_iterator(holder)
{
    if (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, subtree_root);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        const_iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
        prefix_const_iterator const &other):
        const_iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
        prefix_const_iterator &&other) noexcept:
        const_iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator=(
        prefix_const_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator=(
        prefix_const_iterator &&other) noexcept
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_const_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
    }
    else if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
    }
    else
    {
        node *current;

        while (true)
        {
            current = _state.top();
            _state.pop();

            if (_state.empty())
            {
                break;
            }

            if (_state.top()->left_subtree == current && _state.top()->right_subtree != nullptr)
            {
                _state.push(_state.top()->right_subtree);

                break;
            }
        }
    }
}

#pragma endregion prefix_const_iterator implementation

#pragma region prefix_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        iterator(holder)
{
    if (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, subtree_root);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
        prefix_reverse_iterator const &other):
        iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
        prefix_reverse_iterator &&other) noexcept:
        iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator=(
        prefix_reverse_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator=(
        prefix_reverse_iterator &&other) noexcept
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
    }
    else if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
    }
    else
    {
        while (true)
        {
            node *current = _state.top();
            _state.pop();

            if (_state.empty())
            {
                break;
            }

            if (_state.top()->right_subtree == current && _state.top()->left_subtree != nullptr)
            {
                _state.push(_state.top()->left_subtree);

                break;
            }
        }
    }
}

#pragma endregion prefix_reverse_iterator implementation

#pragma region prefix_const_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        const_iterator(holder)
{
    if (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, subtree_root);
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        const_iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
        prefix_const_reverse_iterator const &other):
        const_iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
        prefix_const_reverse_iterator &&other) noexcept:
        const_iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator=(
        prefix_const_reverse_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator=(
        prefix_const_reverse_iterator &&other) noexcept
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
    }
    else if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
    }
    else
    {
        while (true)
        {
            node *current = _state.top();
            _state.pop();

            if (_state.empty())
            {
                break;
            }

            if (_state.top()->right_subtree == current && _state.top()->left_subtree != nullptr)
            {
                _state.push(_state.top()->left_subtree);

                break;
            }
        }
    }
}

#pragma endregion prefix_const_reverse_iterator implementation

#pragma region infix_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        iterator(holder)
{
    while (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree;
    }

    if (this->_state.empty())
    {
        return;
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, this->_state.top());
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(
        infix_iterator const &other):
        iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(
        infix_iterator &&other) noexcept:
        iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator=(
        infix_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator=(
        infix_iterator &&other) noexcept
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        // undefined behavior
        return;
    }

    if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
        while (_state.top()->left_subtree != nullptr)
        {
            _state.push(_state.top()->left_subtree);
        }
    }
    else
    {
        node *current = nullptr;
        do
        {
            current = _state.top();
            _state.pop();

            if (_state.empty() || _state.top()->left_subtree == current)
            {
                break;
            }
        }
        while (true);
    }
}

#pragma endregion infix_iterator implementation

#pragma region infix_const_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        const_iterator(holder)
{
    while (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree;
    }

    if (this->_state.empty())
    {
        return;
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, this->_state.top());
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        const_iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
        infix_const_iterator const &other):
        const_iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
        infix_const_iterator &&other) noexcept:
        const_iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator=(
        infix_const_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator=(
        infix_const_iterator &&other) noexcept
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_const_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        // undefined behavior
        return;
    }

    if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
        while (_state.top()->left_subtree != nullptr)
        {
            _state.push(_state.top()->left_subtree);
        }
    }
    else
    {
        node *current = nullptr;
        do
        {
            current = _state.top();
            _state.pop();

            if (_state.empty() || _state.top()->left_subtree == current)
            {
                break;
            }
        }
        while (true);
    }
}

#pragma endregion infix_const_iterator implementation

#pragma region infix_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        iterator(holder)
{
    while (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree;
    }

    if (this->_state.empty())
    {
        return;
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, this->_state.top());
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
        infix_reverse_iterator const &other):
        iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
        infix_reverse_iterator &&other) noexcept:
        iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator=(
        infix_reverse_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator=(
        infix_reverse_iterator &&other) noexcept
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_reverse_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        // undefined behavior
        return;
    }

    if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
        while (_state.top()->right_subtree != nullptr)
        {
            _state.push(_state.top()->right_subtree);
        }
    }
    else
    {
        node *current = nullptr;
        do
        {
            current = _state.top();
            _state.pop();

            if (_state.empty() || _state.top()->right_subtree == current)
            {
                break;
            }
        }
        while (true);
    }
}

#pragma endregion infix_reverse_iterator implementation

#pragma region infix_const_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        const_iterator(holder)
{
    while (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree;
    }

    if (this->_state.empty())
    {
        return;
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, this->_state.top());
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        const_iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        infix_const_reverse_iterator const &other):
        const_iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        infix_const_reverse_iterator &&other) noexcept:
        const_iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator=(
        infix_const_reverse_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator=(
        infix_const_reverse_iterator &&other) noexcept
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        // undefined behavior
        return;
    }

    if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
        while (_state.top()->right_subtree != nullptr)
        {
            _state.push(_state.top()->right_subtree);
        }
    }
    else
    {
        node *current = nullptr;
        do
        {
            current = _state.top();
            _state.pop();

            if (_state.empty() || _state.top()->right_subtree == current)
            {
                break;
            }
        }
        while (true);
    }
}

#pragma endregion infix_const_reverse_iterator implementation

#pragma region postfix_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        iterator(holder)
{
    while (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree != nullptr
                       ? subtree_root->left_subtree
                       : subtree_root->right_subtree;
    }

    if (this->_state.empty())
    {
        return;
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, this->_state.top());
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(
        postfix_iterator const &other):
        iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(
        postfix_iterator &&other) noexcept:
        iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator=(
        postfix_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator=(
        postfix_iterator &&other) noexcept
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    node *current = _state.top();
    _state.pop();

    if (_state.empty() || _state.top()->right_subtree == current || _state.top()->right_subtree == nullptr)
    {
        return;
    }

    current = _state.top()->right_subtree;

    while (current != nullptr)
    {
        this->_state.push(current);
        current = current->left_subtree != nullptr
                  ? current->left_subtree
                  : current->right_subtree;
    }
}

#pragma endregion postfix_iterator implementation

#pragma region postfix_const_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        const_iterator(holder)
{
    while (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree != nullptr
                       ? subtree_root->left_subtree
                       : subtree_root->right_subtree;
    }

    if (this->_state.empty())
    {
        return;
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, this->_state.top());
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        const_iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(
        postfix_const_iterator const &other):
        const_iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(
        postfix_const_iterator &&other) noexcept:
        const_iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator=(
        postfix_const_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator=(
        postfix_const_iterator &&other) noexcept
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_const_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    node *current = _state.top();
    _state.pop();

    if (_state.empty() || _state.top()->right_subtree == current || _state.top()->right_subtree == nullptr)
    {
        return;
    }

    current = _state.top()->right_subtree;

    while (current != nullptr)
    {
        this->_state.push(current);
        current = current->left_subtree != nullptr
                  ? current->left_subtree
                  : current->right_subtree;
    }
}

#pragma endregion postfix_const_iterator implementation

#pragma region postfix_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        iterator(holder)
{
    while (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree != nullptr
                       ? subtree_root->left_subtree
                       : subtree_root->right_subtree;
    }

    if (this->_state.empty())
    {
        return;
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, this->_state.top());
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(
        postfix_reverse_iterator const &other):
        iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(
        postfix_reverse_iterator &&other) noexcept:
        iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator=(
        postfix_reverse_iterator const &other)
{
    if (*this != other)
    {
        iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator=(
        postfix_reverse_iterator &&other) noexcept
{
    if (*this != other)
    {
        iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    node *current = _state.top();
    _state.pop();

    if (_state.empty() || _state.top()->left_subtree == current || _state.top()->left_subtree == nullptr)
    {
        return;
    }

    current = _state.top()->left_subtree;

    while (current != nullptr)
    {
        this->_state.push(current);
        current = current->right_subtree != nullptr
                  ? current->right_subtree
                  : current->left_subtree;
    }
}

#pragma endregion postfix_reverse_iterator implementation

#pragma region postfix_const_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        typename binary_search_tree<tkey, tvalue>::node *subtree_root):
        const_iterator(holder)
{
    while (subtree_root != nullptr)
    {
        this->_state.push(subtree_root);
        subtree_root = subtree_root->left_subtree != nullptr
                       ? subtree_root->left_subtree
                       : subtree_root->right_subtree;
    }

    if (this->_state.empty())
    {
        return;
    }

    this->_data = holder->create_iterator_data(this->_state.size() - 1, this->_state.top());
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
        binary_search_tree<tkey, tvalue> const *holder,
        std::stack<node*> const &state):
        const_iterator(holder, state)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
        postfix_const_reverse_iterator const &other):
        const_iterator(other)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
        postfix_const_reverse_iterator &&other) noexcept:
        const_iterator(std::move(other))
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator=(
        postfix_const_reverse_iterator const &other)
{
    if (*this != other)
    {
        const_iterator::operator=(other);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator=(
        postfix_const_reverse_iterator &&other) noexcept
{
    if (*this != other)
    {
        const_iterator::operator=(std::move(other));
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::pass_to_next_node()
{
    auto &_state = this->_state;

    if (_state.empty())
    {
        return;
    }

    node *current = _state.top();
    _state.pop();

    if (_state.empty() || _state.top()->left_subtree == current || _state.top()->left_subtree == nullptr)
    {
        return;
    }

    current = _state.top()->left_subtree;

    while (current != nullptr)
    {
        this->_state.push(current);
        current = current->right_subtree != nullptr
                  ? current->right_subtree
                  : current->left_subtree;
    }
}

#pragma endregion postfix_const_reverse_iterator implementation

#pragma endregion iterators implementation

#pragma region target operations associated exception types implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
        tkey const &key):
        std::logic_error("Attempt to insert already existing key inside the tree.")
{ }

template<
        typename tkey,
        typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
        tkey const &key):
        std::logic_error("Attempt to obtain a value by non-existing key from the tree.")
{ }

template<
        typename tkey,
        typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
        tkey const &key):
        std::logic_error("Attempt to dispose a value by non-existing key from the tree.")
{ }

template<
        typename tkey,
        typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

#pragma endregion target operations associated exception types implementation

#pragma region template methods implementation

#pragma region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
        binary_search_tree<tkey, tvalue> *tree):
        _tree(tree)
{ }

template<
        typename tkey,
        typename tvalue>
std::stack<typename binary_search_tree<tkey, tvalue>::node **> binary_search_tree<tkey, tvalue>::template_method_basics::find_path(
    tkey const &key) const
{
    std::stack<node **> result_path;

    node **path_finder = reinterpret_cast<node**>(&(_tree->_root));
    auto const &comparer = _tree->_keys_comparer;

    int comparison_result;

    while (true)
    {
        result_path.push(path_finder);

        if (*path_finder == nullptr)
        {
            break;
        }

        comparison_result = comparer(key, (*path_finder)->key);
        if (comparison_result == 0)
        {
            break;
        }

        path_finder = comparison_result < 0
                      ? &((*path_finder)->left_subtree)
                      : &((*path_finder)->right_subtree);
    }

    return result_path;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::template_method_basics::balance(
        std::stack<node**> &path)
{ }

template<
        typename tkey,
        typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
    return this->_tree->get_logger();
}

#pragma endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

#pragma region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
        binary_search_tree<tkey, tvalue> *tree,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
        binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree),
        _insertion_strategy(insertion_strategy)
{ }

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
        tkey const &key,
        tvalue const &value)
{
    auto path = this->find_path(key);

    if (*(path.top()) != nullptr)
    {
        switch (_insertion_strategy)
        {
            case insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw insertion_of_existent_key_attempt_exception(key);
            case insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = value;
                break;
        }
    }
    else
    {
        this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node *>(
                allocate_with_guard(this->_tree->get_node_size(), 1)), key, value);

        this->balance(path);
    }
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
        tkey const &key,
        tvalue &&value)
{
    auto path = this->find_path(key);

    if (*(path.top()) != nullptr)
    {
        switch (_insertion_strategy)
        {
            case insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw insertion_of_existent_key_attempt_exception(key);
            case insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = std::move(value);
                break;
        }
    }
    else
    {
        this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node *>(
                allocate_with_guard(this->_tree->get_node_size(), 1)), key,
                std::move(value));

        this->balance(path);
    }
}

template<
        typename tkey,
        typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
}

//template<
//        typename tkey,
//        typename tvalue>
//void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(
//        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
//{
//    _insertion_strategy = insertion_strategy;
//}

#pragma endregion search_tree<tkey, tvalue>::insertion_template_method implementation

#pragma region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
        binary_search_tree<tkey, tvalue> *tree):
        binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{ }

template<
        typename tkey,
        typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
        tkey const &key)
{
    auto path = this->find_path(key);

    if (*(path.top()) == nullptr)
    {
        throw obtaining_of_nonexistent_key_attempt_exception(key);
    }

    tvalue const &got_value = (*(path.top()))->value;

    this->balance(path);

    return got_value;
}

template<
        typename tkey,
        typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair>
    binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive)
{
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> range;
    auto const &comparer = this->_tree->_keys_comparer;
    std::stack<node *> path;

    node *path_finder = reinterpret_cast<node*>(this->_tree->_root);
    int comparison_result;

    while (true)
    {
        path.push(path_finder);

        if (path_finder == nullptr)
        {
            path.pop();

            path_finder = path.top(); // TODO may be wrong

            while (!path.empty() && path_finder == path.top()->right_subtree)
            {
                path_finder = path.top();
                path.pop();
            }

            break;
        }

        comparison_result = comparer(lower_bound, path_finder->key);
        if (comparison_result == 0)
        {
            break;
        }

        path_finder = comparison_result < 0
                      ? path_finder->left_subtree
                      : path_finder->right_subtree;
    }

    if (path.empty())
    {
        return range;
    }

    if (lower_bound_inclusive)
    {
        range.push_back({ path.top()->key, path.top()->value });
    }

    auto it = infix_const_iterator(this->_tree, path);
    ++it;

    while ((it != this->_tree->end_infix()) &&
        (comparer(upper_bound, (*it)->get_key()) > (upper_bound_inclusive ? -1 : 0)))
    {
        range.push_back({ (*it)->get_key(), (*it)->get_value() });
        ++it;
    }

    return range;
}

#pragma endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

#pragma region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
        binary_search_tree<tkey, tvalue> *tree,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>::template_method_basics(tree),
        _disposal_strategy(disposal_strategy)
{ }

template<
        typename tkey,
        typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
        tkey const &key)
{
    auto path = this->find_path(key);

    if (*(path.top()) == nullptr)
    {
        switch (_disposal_strategy)
        {
            case disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
                throw disposal_of_nonexistent_key_attempt_exception(key);
            case disposal_of_nonexistent_key_attempt_strategy::do_nothing:
                return tvalue();
        }
    }

    if ((*(path.top()))->left_subtree != nullptr && (*(path.top()))->right_subtree != nullptr)
    {
        auto *target_to_swap = *(path.top());
        auto **current = &((*(path.top()))->left_subtree);

        while (*current != nullptr)
        {
            path.push(current);
            current = &((*current)->right_subtree);
        }

        std::swap(target_to_swap->key, (*(path.top()))->key);
        std::swap(target_to_swap->value, (*(path.top()))->value);
    }

    tvalue value = std::move((*(path.top()))->value);

    node *subtree = (*(path.top()))->left_subtree == nullptr
                             ? (*(path.top()))->right_subtree
                             : (*(path.top()))->left_subtree;

    allocator::destruct(*(path.top()));
    deallocate_with_guard(*(path.top()));

    *(path.top()) = subtree;
    this->balance(path);

    return value;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    _disposal_strategy = disposal_strategy;
}

template<
        typename tkey,
        typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
}

#pragma endregion search_tree<tkey, tvalue>::disposal_template_method implementation

#pragma endregion template methods

#pragma region construction, assignment, destruction implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
        typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
        typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
        typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
        std::function<int(tkey const &, tkey const &)> comparer,
        allocator *allocator,
        logger *logger):
        search_tree<tkey, tvalue>(comparer, logger, allocator),
        _insertion_template(insertion_template),
        _obtaining_template(obtaining_template),
        _disposal_template(disposal_template)
{ }

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
        std::function<int(tkey const &, tkey const &)> comparer,
        allocator *allocator,
        logger *logger,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        search_tree<tkey, tvalue>(comparer, allocator, logger)
{
    try
    {
//        _root = nullptr;
        _insertion_template = new binary_search_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy);
        _obtaining_template = new binary_search_tree<tkey, tvalue>::obtaining_template_method(this);
        _disposal_template = new binary_search_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy);
    }
    catch (const std::bad_alloc &)
    {
        delete _insertion_template;
        delete _obtaining_template;
        delete _disposal_template;

        throw;
    }
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other):
        search_tree<tkey, tvalue>(other._keys_comparer, other.get_allocator(), other.get_logger())
{
    try
    {
        reinterpret_cast<node*&>(this->_root) = copy(other._root);

        _insertion_template = new binary_search_tree<tkey, tvalue>::insertion_template_method(this, other._insertion_template->_insertion_strategy);
        _obtaining_template = new binary_search_tree<tkey, tvalue>::obtaining_template_method(this);
        _disposal_template = new binary_search_tree<tkey, tvalue>::disposal_template_method(this, other._disposal_template->_disposal_strategy);
    }
    catch (const std::bad_alloc &)
    {
        clear(reinterpret_cast<node**>(&this->_root));

        delete _insertion_template;
        delete _obtaining_template;
        delete _disposal_template;

        throw;
    }
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept:
        search_tree<tkey, tvalue>(other._keys_comparer, other.get_allocator(), other.get_logger(), other._root)
{
    _insertion_template = std::move(other._insertion_template);
    _obtaining_template = std::move(other._obtaining_template);
    _disposal_template = std::move(other._disposal_template);

    other._insertion_template = nullptr;
    other._obtaining_template = nullptr;
    other._disposal_template = nullptr;
    other._root = nullptr;
    other._logger = nullptr;
    other._allocator = nullptr;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
        binary_search_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
        clear(reinterpret_cast<node**>(&this->_root));

        this->_allocator = other._allocator;
        this->_logger = other._logger;
        this->_keys_comparer = other._keys_comparer;

        *_insertion_template = *(other._insertion_template);
        *_obtaining_template = *(other._obtaining_template);
        *_disposal_template = *(other._disposal_template);

        reinterpret_cast<node*&>(this->_root) = copy(other._root);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        clear(reinterpret_cast<node**>(&this->_root));
        this->_keys_comparer = std::move(other._keys_comparer);

        this->_logger = std::move(other._logger);
        other._logger = nullptr;

        this->_allocator = std::move(other._allocator);
        other._allocator = nullptr;

        _insertion_template = std::move(other._insertion_template);
        other._insertion_template = nullptr;

        _obtaining_template = std::move(other._obtaining_template);
        other._obtaining_template = nullptr;

        _disposal_template = std::move(other._disposal_template);
        other._disposal_template = nullptr;

        reinterpret_cast<node*&>(this->_root) = std::move(other._root);
        other._root = nullptr;
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    delete _insertion_template;
    delete _obtaining_template;
    delete _disposal_template;
    clear(reinterpret_cast<node*&>(this->_root));
}

#pragma endregion construction, assignment, destruction implementation

#pragma region bst extra functions implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::clear(
        node *&subtree_root)
{
    if (subtree_root == nullptr)
    {
        return;
    }

    clear(subtree_root->left_subtree);
    clear(subtree_root->right_subtree);
    subtree_root->~node();
    this->deallocate_with_guard(subtree_root);

    subtree_root = nullptr;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::node *binary_search_tree<tkey, tvalue>::copy(
        node const *subtree_root)
{
    if (subtree_root == nullptr)
    {
        return nullptr;
    }

    node *subtree_root_copied = nullptr;

    try
    {
        subtree_root_copied = reinterpret_cast<node *>(this->allocate_with_guard(get_node_size(), 1));
        call_node_constructor(subtree_root_copied, subtree_root->key, subtree_root->value);
        subtree_root_copied->left_subtree = copy(subtree_root->left_subtree);
        subtree_root_copied->right_subtree = copy(subtree_root->right_subtree);
    }
    catch (const std::bad_alloc &)
    {
        clear(&subtree_root_copied);

        throw;
    }

    return subtree_root_copied;
}

template<
        typename tkey,
        typename tvalue>
inline size_t binary_search_tree<tkey, tvalue>::get_node_size() const noexcept
{
    return sizeof(typename binary_search_tree<tkey, tvalue>::node);
}

template<
        typename tkey,
        typename tvalue>
inline void binary_search_tree<tkey, tvalue>::update_node_data(node *node) const noexcept
{ }

template<
        typename tkey,
        typename tvalue>
inline void binary_search_tree<tkey, tvalue>::call_node_constructor(
        node *raw_space,
        tkey const &key,
        tvalue const &value)
{
    allocator::construct(raw_space, key, value);
}

template<
        typename tkey,
        typename tvalue>
inline void binary_search_tree<tkey, tvalue>::call_node_constructor(
        node *raw_space,
        tkey const &key,
        tvalue &&value)
{
    allocator::construct(raw_space, key, std::move(value));
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::inject_additional_data(
        iterator_data *destination,
        node *source) const
{ }

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::create_iterator_data() const
{
    return new iterator_data;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::create_iterator_data(
        unsigned int depth,
        node *&src_node) const
{
    return new iterator_data(depth, &src_node);
}

#pragma endregion bst extra functions implementation

#pragma region associative_container<tkey, tvalue> contract implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
        tkey const &key,
        tvalue const &value)
{
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue const &): called")
            ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &): called")
            ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &): inserting node with key: " +
                extra_utility::make_string(key));

    _insertion_template->insert(key, value);

    this->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : now root has with key: " +
                extra_utility::make_string(reinterpret_cast<node*>(this->_root)->key) + "\"")
            ->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue const &): ended")
            ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &): ended");
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
        tkey const &key,
        tvalue &&value)
{
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue &&): called")
            ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue &&): called")
            ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &): inserting node with key: " +
                extra_utility::make_string(key));

    _insertion_template->insert(key, std::move(value));

    this->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &): now root has key: " +
                extra_utility::make_string(reinterpret_cast<node*>(this->_root)->key))
            ->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue &&): ended")
            ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue &&): ended");
}

template<
        typename tkey,
        typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
        tkey const &key)
{
    this->trace_with_guard(get_typename() + "::obtain(tkey const &): called")
            ->debug_with_guard(get_typename() + "::obtain(tkey const &): called");

    return _obtaining_template->obtain(key);

    this->trace_with_guard(get_typename() + "::obtain(tkey const &): ended")
            ->debug_with_guard(get_typename() + "::obtain(tkey const &): ended");
}

template<
        typename tkey,
        typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive)
{
    this->trace_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool): called")
            ->debug_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool): called");

    return _obtaining_template->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);

    this->trace_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool): ended")
            ->debug_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool): ended");
}

template<
        typename tkey,
        typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
        tkey const &key)
{
    this->trace_with_guard(get_typename() + "::dispose(tkey const &): called")
            ->debug_with_guard(get_typename() + "::dispose(tkey const &): called")
            ->debug_with_guard(get_typename() + "::dispose(tkey const &): disposing node with key: " +
                extra_utility::make_string(key));

    return _disposal_template->dispose(key);

    this->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &): now root has key: " +
                extra_utility::make_string(reinterpret_cast<node*>(this->_root)->key))
            ->trace_with_guard(get_typename() + "::dispose(tkey const &): ended")
            ->debug_with_guard(get_typename() + "::dispose(tkey const &): ended");
}

#pragma endregion associative_containers contract implementations

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    this->trace_with_guard(get_typename() + "::set_insertion_strategy(disposal_of_nonexistent_key_attempt_strategy): called")
            ->debug_with_guard(get_typename() + "::set_insertion_strategy(disposal_of_nonexistent_key_attempt_strategy): called");

    _insertion_template->_insertion_strategy = insertion_strategy;

    this->trace_with_guard(get_typename() + "::set_insertion_strategy(disposal_of_nonexistent_key_attempt_strategy): ended")
            ->debug_with_guard(get_typename() + "::set_insertion_strategy(disposal_of_nonexistent_key_attempt_strategy): ended");
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    this->trace_with_guard(get_typename() + "::set_disposal_strategy(disposal_of_nonexistent_key_attempt_strategy): called")
            ->debug_with_guard(get_typename() + "::set_disposal_strategy(disposal_of_nonexistent_key_attempt_strategy): called");

    _disposal_template->_disposal_strategy = disposal_strategy;

    this->trace_with_guard(get_typename() + "::set_disposal_strategy(disposal_of_nonexistent_key_attempt_strategy): ended")
            ->debug_with_guard(get_typename() + "::set_disposal_strategy(disposal_of_nonexistent_key_attempt_strategy): ended");
}

#pragma region iterators requesting implementation

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(this, nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(this, static_cast<node*>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(this, nullptr);
}

#pragma endregion iterators request implementation

#pragma region subtree rotations implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate) const
{
    this->trace_with_guard(get_typename() + "::small_left_rotation(): called")
            ->debug_with_guard(get_typename() + "::small_left_rotation(): called")
            ->debug_with_guard(get_typename() + "::small_left_rotation(): rotating node with key: " +
                extra_utility::make_string(subtree_root->key));

    if (validate && (subtree_root == nullptr || subtree_root->right_subtree == nullptr))
    {
        throw std::logic_error("It is impossible to make a small left rotation");
    }

    binary_search_tree<tkey, tvalue>::node *new_subtree_root = subtree_root->right_subtree;

    subtree_root->right_subtree = new_subtree_root->left_subtree;
    new_subtree_root->left_subtree = subtree_root;
    subtree_root = new_subtree_root;

    update_node_data(new_subtree_root->left_subtree);
    update_node_data(new_subtree_root);

    this->trace_with_guard(get_typename() + "::small_left_rotation(): ended")
            ->debug_with_guard(get_typename() + "::small_left_rotation(): ended");
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate) const
{
    this->trace_with_guard(get_typename() + "::small_right_rotation(): called")
            ->debug_with_guard(get_typename() + "::small_right_rotation(): called")
            ->debug_with_guard(get_typename() + "::small_right_rotation(): rotating node with key: " +
                extra_utility::make_string(subtree_root->key));

    if (validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr))
    {
        throw std::logic_error("It is impossible to make a small right rotation");
    }

    binary_search_tree<tkey, tvalue>::node *new_subtree_root = subtree_root->left_subtree;

    subtree_root->left_subtree = new_subtree_root->right_subtree;
    new_subtree_root->right_subtree = subtree_root;
    subtree_root = new_subtree_root;

    update_node_data(new_subtree_root->right_subtree);
    update_node_data(new_subtree_root);

    this->trace_with_guard(get_typename() + "::small_right_rotation(): ended")
            ->debug_with_guard(get_typename() + "::small_right_rotation(): ended");
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate) const
{
    this->trace_with_guard(get_typename() + "::big_left_rotation(): called")
            ->debug_with_guard(get_typename() + "::big_left_rotation(): called");

    if (validate && (subtree_root == nullptr || subtree_root->right_subtree == nullptr ||
                     subtree_root->right_subtree->left_subtree == nullptr))
    {
        throw std::logic_error("It is impossible to make a big left rotation");
    }

    small_right_rotation(subtree_root->right_subtree, false);
    small_left_rotation(subtree_root, false);

    this->trace_with_guard(get_typename() + "::big_left_rotation(): ended")
            ->debug_with_guard(get_typename() + "::big_left_rotation(): ended");
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate) const
{
    this->trace_with_guard(get_typename() + "::big_right_rotation(): called")
            ->debug_with_guard(get_typename() + "::big_right_rotation(): called");

    if (validate && (subtree_root == nullptr || subtree_root->left_subtreel == nullptr ||
                     subtree_root->left_subtree->right_subtree == nullptr))
    {
        throw std::logic_error("It is impossible to make a big right rotation");
    }

    small_left_rotation(subtree_root->left_subtree, false);
    small_right_rotation(subtree_root, false);

    this->trace_with_guard(get_typename() + "::big_right_rotation(): ended")
            ->debug_with_guard(get_typename() + "::big_right_rotation(): ended");
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate) const
{
    this->trace_with_guard(get_typename() + "::double_left_rotation(): called")
            ->debug_with_guard(get_typename() + "::double_left_rotation(): called");

    if (validate && (subtree_root == nullptr || subtree_root->right_subtree == nullptr ||
                     subtree_root->right_subtree->right_subtree == nullptr))
    {
        throw std::logic_error("It is impossible to make a double left rotation");
    }

    if(at_grandparent_first)
    {
        small_left_rotation(subtree_root, false);
        small_left_rotation(subtree_root, false);
    }
    else
    {
        small_left_rotation(subtree_root->right_subtree, false);
        small_left_rotation(subtree_root, false);
    }

    this->trace_with_guard(get_typename() + "::double_left_rotation(): ended")
            ->debug_with_guard(get_typename() + "::double_left_rotation(): ended");
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate) const
{
    this->trace_with_guard(get_typename() + "::double_right_rotation(): called")
            ->debug_with_guard(get_typename() + "::double_right_rotation(): called");

    if (validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr ||
                     subtree_root->left_subtree->left_subtree == nullptr))
    {
        throw std::logic_error("It is impossible to make a double right rotation");
    }

    if(at_grandparent_first)
    {
        small_right_rotation(subtree_root, false);
        small_right_rotation(subtree_root, false);
    }
    else
    {
        small_right_rotation(subtree_root->left_subtree, false);
        small_right_rotation(subtree_root, false);
    }

    this->trace_with_guard(get_typename() + "::double_right_rotation(): ended")
            ->debug_with_guard(get_typename() + "::double_right_rotation(): ended");
}

#pragma endregion subtree rotations implementation

template<
        typename tkey,
        typename tvalue>
inline std::string binary_search_tree<tkey, tvalue>::get_typename() const noexcept
{
    return "binary_search_tree<tkey, tvalue>";
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H