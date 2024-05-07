#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class red_black_tree final:
    public binary_search_tree<tkey, tvalue>
{

public:
    
    enum class node_color
    {
        RED,
        BLACK
    };

private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {

    public:

        node_color _color;

    public:

        explicit node(
                tkey const &key,
                tvalue const &value);

        explicit node(
                tkey const &key,
                tvalue &&value);
        
    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {

        friend void red_black_tree<tkey, tvalue>::inject_additional_data(
                typename binary_search_tree<tkey, tvalue>::iterator_data *,
                typename binary_search_tree<tkey, tvalue>::node const *) const;
    
    public:
        
        node_color _color;
    
    public:

        iterator_data();
        
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            node_color color);

        explicit iterator_data(
                unsigned int depth,
                node *src_node);

    public:

        iterator_data(
                iterator_data const &other);

        iterator_data(
                iterator_data &&other) noexcept;

        iterator_data &operator=(
                iterator_data const &other);

        iterator_data &operator=(
                iterator_data &&other) noexcept;

        ~iterator_data() noexcept final = default;

    public:

        node_color get_color() const;
        
    };

private:
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
    public:
        
        explicit insertion_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);

    private:

        void balance(
                std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path) override;
        
    };
    
    class obtaining_template_method final:
        public binary_search_tree<tkey, tvalue>::obtaining_template_method
    {
    
    public:
        
        explicit obtaining_template_method(
            red_black_tree<tkey, tvalue> *tree);
        
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {

    private:

        node_color _deleted_color;
        bool _deleted_has_children;
    
    public:
        
        explicit disposal_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    private:

        void balance(
                std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path) override;

        virtual void identify_deleted_node(
                node* node_to_dispose);
        
    };

public:
    
    explicit red_black_tree(
        std::function<int(tkey const &, tkey const &)> comparer,
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~red_black_tree() noexcept final = default;
    
    red_black_tree(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree(
        red_black_tree<tkey, tvalue> &&other) noexcept;
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> &&other) noexcept;

private:

    inline void call_node_constructor(
            typename binary_search_tree<tkey,tvalue>::node *raw_space,
            tkey const &key,
            tvalue const &value) override;

    inline void call_node_constructor(
            typename binary_search_tree<tkey,tvalue>::node *raw_space,
            tkey const &key,
            tvalue &&value) override;

    void inject_additional_data(
            typename binary_search_tree<tkey,tvalue>::node *destination,
            typename binary_search_tree<tkey,tvalue>::node const *source) const override;

    void inject_additional_data(
            typename binary_search_tree<tkey,tvalue>::iterator_data *destination,
            typename binary_search_tree<tkey,tvalue>::node const *source) const override;

    typename binary_search_tree<tkey,tvalue>::iterator_data *create_iterator_data() const override;

    typename binary_search_tree<tkey,tvalue>::iterator_data *create_iterator_data(
            unsigned int depth,
            typename binary_search_tree<tkey,tvalue>::node *src_node) const override;

    virtual inline size_t get_node_size() const noexcept;

    static inline node_color get_color(
            typename binary_search_tree<tkey,tvalue>::node* node) noexcept;

    void set_color(
            typename binary_search_tree<tkey,tvalue>::node* node,
            node_color color) noexcept;

    inline std::string get_typename() const noexcept;
    
};

#pragma region node implementation

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::node::node(
        const tkey &key,
        const tvalue &value):
        binary_search_tree<tkey, tvalue>::node(key, value),
        _color(node_color::RED)
{ }

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::node::node(
        const tkey &key,
        tvalue &&value):
        binary_search_tree<tkey, tvalue>::node(key, std::move(value)),
        _color(node_color::RED)
{ }

#pragma endregion node implementation

#pragma region iterator_data implementation

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data():
        binary_search_tree<tkey, tvalue>::iterator_data(),
        _color(node_color::RED)
{ }

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    typename red_black_tree<tkey, tvalue>::node_color color):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value),
    _color(color)
{ }

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
        unsigned int depth,
        node *src_node):
        binary_search_tree<tkey, tvalue>::iterator_data(
                depth,
                reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node*>(src_node))
{
    if(src_node != nullptr)
    {
        _color = src_node->_color;
    }
}

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
        iterator_data const &other):
        binary_search_tree<tkey, tvalue>::iterator_data(other),
        _color(other._color)
{ }

template<
        typename tkey,
        typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
        iterator_data &&other) noexcept:
        binary_search_tree<tkey, tvalue>::iterator_data(std::move(other)),
        _color(std::move(other._color))
{ }

template<
        typename tkey,
        typename tvalue>
typename red_black_tree<tkey, tvalue>::iterator_data &red_black_tree<tkey, tvalue>::iterator_data::operator=(
        iterator_data const &other)
{
    if (&other != this)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(other);
        _color = other._color;
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename red_black_tree<tkey, tvalue>::iterator_data &red_black_tree<tkey, tvalue>::iterator_data::operator=(
        iterator_data &&other) noexcept
{
    if (&other != this)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(std::move(other));
        _color = std::move(other._color);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename red_black_tree<tkey, tvalue>::node_color red_black_tree<tkey, tvalue>::iterator_data::get_color() const
{
    if (!(this->_is_state_initialized))
    {
        throw std::logic_error("Tried to read from uninitialized iterator data");
    }

    return _color;
}

#pragma endregion iterator_data implementation

#pragma region template methods implementation

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
        binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{ }

template<
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::insertion_template_method::balance(
        std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
    this->trace_with_guard(get_typename() + "::insertion_template_method::balance(std::stack<node**>): called")
            ->debug_with_guard(get_typename() + "::insertion_template_method::balance(std::stack<node**>): called");

    typename binary_search_tree<tkey, tvalue>::node **cur;
    typename binary_search_tree<tkey, tvalue>::node **parent;
    typename binary_search_tree<tkey, tvalue>::node **grandparent;
    typename binary_search_tree<tkey, tvalue>::node **uncle;

    // TODO dynamic_cast ?

    while (path.size() >= 3)
    {
        cur = path.top();
        path.pop();
        parent = path.top();
        path.pop();

        // trivial
        if (get_color(*cur) == node_color::BLACK || get_color(*parent) == node_color::BLACK)
        {
            break;
        }

        grandparent = path.top();
        if ((*grandparent)->right_subtree == *parent)
        {
            uncle = &((*grandparent)->left_subtree);
        }
        else
        {
            uncle = &((*grandparent)->right_subtree);
        }

        // parent is red

        // uncle is red
        if (get_color(*uncle) == node_color::RED)
        {
            set_color(*parent, node_color::BLACK);
            set_color(*uncle, node_color::BLACK);
            set_color(*grandparent, node_color::RED);
        }
        else
        {
            // uncle is black
            set_color(*grandparent, node_color::RED);

            if ((*grandparent)->left_subtree == *parent)
            {
                // ll
                if ((*parent)->left_subtree == cur)
                {
                    this->small_right_rotation(*grandparent);
                    set_color(*parent, node_color::BLACK);
                }
                // lr
                else
                {
                    this->big_right_rotation(*grandparent);
                    set_color(*cur, node_color::BLACK);
                }
            }
            else
            {
                // rr
                if ((*parent)->right_subtree == cur)
                {
                    this->small_left_rotation(*grandparent);
                    set_color(*parent, node_color::BLACK);
                }
                // rl
                else
                {
                    this->big_left_rotation(*grandparent);
                    set_color(*cur, node_color::BLACK);
                }
            }
        }
    }

    // root
    if (path.size() == 1)
    {
        reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*(path.top()))->color = node_color::BLACK;
    }

    this->trace_with_guard(get_typename() + "::insertion_template_method::balance(std::stack<node**>): ended")
            ->debug_with_guard(get_typename() + "::insertion_template_method::balance(std::stack<node**>): ended");
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    red_black_tree<tkey, tvalue> *tree):
        binary_search_tree<tkey, tvalue>::obtaining_template_method(tree)
{ }

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy)
{ }

template<
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::disposal_template_method::balance(
        std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
    this->trace_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>): called")
            ->debug_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>): called");

    if (_deleted_color == node_color::RED)
    {
        // TODO automatic log call
        this->trace_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>): ended")
                ->debug_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>): ended");

        return;
    }

    typename binary_search_tree<tkey, tvalue>::node **cur;
    typename binary_search_tree<tkey, tvalue>::node **parent;
    typename binary_search_tree<tkey, tvalue>::node **grandparent;
    typename binary_search_tree<tkey, tvalue>::node **uncle;

    while (path.size() >= 3)
    {
        cur = path.top();
        path.pop();
        parent = path.top();
        path.pop();

    }

    this->trace_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>): ended")
            ->debug_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>): ended");
}

template<
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::disposal_template_method::identify_deleted_node(
        red_black_tree::node *node_to_dispose)
{
    _deleted_color = get_color(node_to_dispose);

    if (node_to_dispose->left_subtree != nullptr ||
        node_to_dispose->right_subtree != nullptr)
    {
        _deleted_has_children = true;
    }
    else
    {
        _deleted_has_children = false;
    }
}

#pragma endregion template methods implementation

#pragma region red_black_tree construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>(
                new(std::nothrow) red_black_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
                new(std::nothrow) red_black_tree<tkey, tvalue>::obtaining_template_method(this),
                new(std::nothrow) red_black_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
                comparer, allocator, logger)
{
    if (this->_insertion_template == nullptr || this->_obtaining_template == nullptr ||
        this->_disposal_template == nullptr)
    {
        delete this->_insertion_template;
        delete this->_obtaining_template;
        delete this->_disposal_template;
        throw std::bad_alloc();
    }
}


template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    red_black_tree<tkey, tvalue> const &other):
        binary_search_tree<tkey, tvalue>(
                new(std::nothrow) red_black_tree<tkey, tvalue>::insertion_template_method(this, other._insertion_template->_insertion_strategy),
                new(std::nothrow) red_black_tree<tkey, tvalue>::obtaining_template_method(this),
                new(std::nothrow) red_black_tree<tkey, tvalue>::disposal_template_method(this, other._disposal_template->_disposal_strategy),
                other._keys_comparer, other.get_allocator(), other.get_logger())
{
    try
    {
        if (this->_insertion_template == nullptr || this->_obtaining_template == nullptr ||
            this->_disposal_template == nullptr)
        {
            throw std::bad_alloc();
        }

        this->_root = this->copy(reinterpret_cast<node*>(other._root));
    }
    catch (const std::bad_alloc &)
    {
        this->clear(reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node**>(&this->_root));
        delete this->_insertion_template;
        delete this->_obtaining_template;
        delete this->_disposal_template;
        throw;
    }
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
    red_black_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(other);
    }

    return *this;
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    red_black_tree<tkey, tvalue> &&other) noexcept:
        binary_search_tree<tkey, tvalue>(std::move(other))
{ }

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
    red_black_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(std::move(other));
    }

    return *this;
}

#pragma endregion red_black_tree construction, assignment, destruction implementation

#pragma region red_black_tree extra functions implementation

template<
        typename tkey,
        typename tvalue>
inline void red_black_tree<tkey, tvalue>::call_node_constructor(
        typename binary_search_tree<tkey,tvalue>::node *raw_space,
        tkey const &key,
        tvalue const &value)
{
    allocator::construct(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(raw_space), key, value);
}

template<
        typename tkey,
        typename tvalue>
inline void red_black_tree<tkey, tvalue>::call_node_constructor(
        typename binary_search_tree<tkey,tvalue>::node *raw_space,
        tkey const &key,
        tvalue &&value)
{
    allocator::construct(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(raw_space), key, std::move(value));
}

template<
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::inject_additional_data(
        typename binary_search_tree<tkey,tvalue>::node *destination,
        typename binary_search_tree<tkey,tvalue>::node const *source) const
{
    auto *rbt_destination = dynamic_cast<red_black_tree<tkey, tvalue>::node*>(destination);
    auto *rbt_source = dynamic_cast<red_black_tree<tkey, tvalue>::node const*>(source);

    rbt_destination->_color = rbt_source->_color;
}

template<
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::inject_additional_data(
        typename binary_search_tree<tkey,tvalue>::iterator_data *destination,
        typename binary_search_tree<tkey,tvalue>::node const *source) const
{
    auto *rbt_destination = dynamic_cast<red_black_tree<tkey, tvalue>::iterator_data*>(destination);
    auto *rbt_source = dynamic_cast<red_black_tree<tkey, tvalue>::node const*>(source);

    rbt_destination->_color = rbt_source->_color;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *red_black_tree<tkey, tvalue>::create_iterator_data() const
{
    return new iterator_data;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *red_black_tree<tkey, tvalue>::create_iterator_data(
        unsigned int depth,
        typename binary_search_tree<tkey,tvalue>::node *src_node) const
{
    return new iterator_data(depth, reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(src_node));
}

template<
        typename tkey,
        typename tvalue>
inline size_t red_black_tree<tkey, tvalue>::get_node_size() const noexcept
{
    return sizeof(typename red_black_tree<tkey, tvalue>::node);
}

template<
        typename tkey,
        typename tvalue>
typename red_black_tree<tkey, tvalue>::node_color red_black_tree<tkey, tvalue>::get_color(
        typename binary_search_tree<tkey,tvalue>::node* node) noexcept
{
    auto *rbt_node = dynamic_cast<red_black_tree<tkey,tvalue>::node*>(node);

    if (rbt_node == nullptr)
    {
        return node_color::BLACK;
    }

    return rbt_node->_color;
}

template<
        typename tkey,
        typename tvalue>
void red_black_tree<tkey, tvalue>::set_color(
        typename binary_search_tree<tkey,tvalue>::node *node,
        red_black_tree::node_color color) noexcept
{
    auto *rbt_node = dynamic_cast<red_black_tree<tkey,tvalue>::node*>(node);

    if (rbt_node != nullptr)
    {
        rbt_node->_color = color;
    }
}

#pragma endregion red_black_tree extra functions implementation

template<
        typename tkey,
        typename tvalue>
inline std::string red_black_tree<tkey, tvalue>::get_typename() const noexcept
{
    return "red_black_tree<tkey, tvalue>";
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
