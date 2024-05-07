#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class AVL_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:

    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {

    public:

        size_t _subtree_height;

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

        friend void AVL_tree<tkey, tvalue>::inject_additional_data(
                typename binary_search_tree<tkey, tvalue>::iterator_data *,
                typename binary_search_tree<tkey, tvalue>::node const *) const;

    public:

        size_t _subtree_height;

    public:

        iterator_data();

        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            size_t subtree_height);

        explicit iterator_data(
                unsigned int depth,
                node *src_node);

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

        unsigned int get_subtree_height() const;

    };

private:

    void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path);

private:

    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {

    public:

        explicit insertion_template_method(
            AVL_tree<tkey, tvalue> *tree,
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
            AVL_tree<tkey, tvalue> *tree);

    };

    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {

    public:

        explicit disposal_template_method(
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    private:

        void balance(
                std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path) override;

    };

public:

    explicit AVL_tree(
        std::function<int(tkey const &, tkey const &)> comparer,
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

    ~AVL_tree() noexcept final = default;

    AVL_tree(
        AVL_tree<tkey, tvalue> const &other);

    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> const &other);

    AVL_tree(
        AVL_tree<tkey, tvalue> &&other) noexcept;

    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> &&other) noexcept;

private:

    inline void update_node_data(
            typename binary_search_tree<tkey,tvalue>::node *node) const noexcept override;

    virtual inline size_t get_node_size() const noexcept;

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

    static inline unsigned int get_subtree_height(
            typename binary_search_tree<tkey,tvalue>::node* node) noexcept;

    inline std::string get_typename() const noexcept;

};

#pragma region node implementation

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::node::node(
        const tkey &key,
        const tvalue &value):
        binary_search_tree<tkey, tvalue>::node(key, value),
        _subtree_height(1)
{ }

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::node::node(
        const tkey &key,
        tvalue &&value):
        binary_search_tree<tkey, tvalue>::node(key, std::move(value)),
        _subtree_height(1)
{ }

#pragma endregion node implementation

#pragma region iterator_data implementation

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data():
    binary_search_tree<tkey, tvalue>::iterator_data(),
    _subtree_height(0)
{ }

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    size_t subtree_height):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value),
    _subtree_height(subtree_height)
{ }

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
        unsigned int depth,
        node *src_node):
        binary_search_tree<tkey, tvalue>::iterator_data(
                depth,
                reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node*>(src_node))
{
    if(src_node != nullptr)
    {
        _subtree_height = src_node->_subtree_height;
    }
}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
        iterator_data const &other):
        binary_search_tree<tkey, tvalue>::iterator_data(other),
        _subtree_height(other._subtree_height)
{ }

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
        iterator_data &&other) noexcept:
        binary_search_tree<tkey, tvalue>::iterator_data(std::move(other)),
        _subtree_height(std::move(other._subtree_height))
{ }

template<
        typename tkey,
        typename tvalue>
typename AVL_tree<tkey, tvalue>::iterator_data &AVL_tree<tkey, tvalue>::iterator_data::operator=(
        iterator_data const &other)
{
    if (&other != this)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(other);
        _subtree_height = other._subtree_height;
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename AVL_tree<tkey, tvalue>::iterator_data &AVL_tree<tkey, tvalue>::iterator_data::operator=(
        iterator_data &&other) noexcept
{
    if (&other != this)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(std::move(other));
        _subtree_height = std::move(other._subtree_height);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
unsigned int AVL_tree<tkey, tvalue>::iterator_data::get_subtree_height() const
{
    if (!(this->_is_state_initialized))
    {
        throw std::logic_error("Tried to read from uninitialized iterator data");
    }

    return _subtree_height;
}

#pragma endregion iterator_data implementation

#pragma region AVL balance implementation

template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::balance(
        std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
    this->trace_with_guard(get_typename() + "::balance(std::stack<node**>): called")
            ->debug_with_guard(get_typename() + "::balance(std::stack<node**>): called");

    typename binary_search_tree<tkey, tvalue>::node **cur = path.top();

    if (*cur == nullptr)
    {
        path.pop();
    }

    int height_difference;

    while (!path.empty())
    {
        cur = path.top();
        path.pop();

        update_node_data(*cur);

        height_difference = get_subtree_height((*cur)->right_subtree) - get_subtree_height((*cur)->left_subtree);

        this->debug_with_guard(get_typename() + "::balance(std::stack<node**>): checking node with key: " +
                               extra_utility::make_string((*cur)->key) +
                               ". Height difference (right - left) is " + std::to_string(height_difference));

        if (height_difference == 2)
        {
            height_difference =
                    get_subtree_height((*cur)->right_subtree->right_subtree) -
                    get_subtree_height((*cur)->right_subtree->left_subtree);

            if (height_difference >= 0)
            {
                this->small_left_rotation(*cur);
            }
            else
            {
                this->big_left_rotation(*cur);
            }
        }
        else if (height_difference == -2)
        {
            height_difference =
                    get_subtree_height((*cur)->left_subtree->right_subtree) -
                    get_subtree_height((*cur)->left_subtree->left_subtree);

            if (height_difference <= 0)
            {
                this->small_right_rotation(*cur);
            }
            else
            {
                this->big_right_rotation(*cur);
            }
        }
    }

    this->trace_with_guard(get_typename() + "::balance(std::stack<node**>): ended")
            ->debug_with_guard(get_typename() + "::balance(std::stack<node**>): ended");
}

#pragma endregion AVL balance implementation

#pragma region template methods implementation

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
    binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{ }

template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::insertion_template_method::balance(
        std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->balance(path);
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    AVL_tree<tkey, tvalue> *tree):
        binary_search_tree<tkey, tvalue>::obtaining_template_method(tree)
{ }

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy)
{ }

template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::disposal_template_method::balance(
        std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->balance(path);
}

#pragma endregion template methods implementation

#pragma region AVL construction, assignment, destruction implementation

//template<
//    typename tkey,
//    typename tvalue>
//AVL_tree<tkey, tvalue>::AVL_tree(
//    std::function<int(tkey const &, tkey const &)> comparer,
//    allocator *allocator,
//    logger *logger,
//    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
//    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
//        search_tree<tkey, tvalue>(comparer, allocator, logger)
//{
//    try
//    {
//        this->_insertion_template = new AVL_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy);
//        this->_obtaining_template = new AVL_tree<tkey, tvalue>::obtaining_template_method(this);
//        this->_disposal_template = new AVL_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy);
//    }
//    catch (const std::bad_alloc &)
//    {
//        delete this->_insertion_template;
//        delete this->_obtaining_template;
//        delete this->_disposal_template;
//
//        throw;
//    }
//}

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
        std::function<int(tkey const &, tkey const &)> comparer,
        allocator *allocator,
        logger *logger,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>(
                new(std::nothrow) AVL_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
                new(std::nothrow) AVL_tree<tkey, tvalue>::obtaining_template_method(this),
                new(std::nothrow) AVL_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
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

//template<
//        typename tkey,
//        typename tvalue>
//AVL_tree<tkey, tvalue>::AVL_tree(
//        std::function<int(tkey const &, tkey const &)> comparer,
//        allocator *allocator,
//        logger *logger,
//        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
//        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
//        binary_search_tree<tkey, tvalue>(
//                comparer, allocator, logger, insertion_strategy, disposal_strategy)
//{ }

//template<
//    typename tkey,
//    typename tvalue>
//AVL_tree<tkey, tvalue>::AVL_tree(
//    AVL_tree<tkey, tvalue> const &other):
//        binary_search_tree<tkey, tvalue>(other)
//{ }

template<
        typename tkey,
        typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
        AVL_tree<tkey, tvalue> const &other):
        binary_search_tree<tkey, tvalue>(
                new(std::nothrow) AVL_tree<tkey, tvalue>::insertion_template_method(this, other._insertion_template->_insertion_strategy),
                new(std::nothrow) AVL_tree<tkey, tvalue>::obtaining_template_method(this),
                new(std::nothrow) AVL_tree<tkey, tvalue>::disposal_template_method(this, other._disposal_template->_disposal_strategy),
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
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> const &other)
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
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> &&other) noexcept:
        binary_search_tree<tkey, tvalue>(std::move(other))
{ }

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::operator=(std::move(other));
    }

    return *this;
}

#pragma endregion AVL construction, assignment, destruction implementation

#pragma region avl extra functions implementation

template<
        typename tkey,
        typename tvalue>
inline void AVL_tree<tkey, tvalue>::update_node_data(
        typename binary_search_tree<tkey,tvalue>::node *node) const noexcept
{
    auto *avl_node = dynamic_cast<AVL_tree<tkey, tvalue>::node*>(node);

    size_t left_height = get_subtree_height(node->left_subtree);
    size_t right_height = get_subtree_height(node->right_subtree);

    // std::cout << "---" << std::max(left_height, right_height) + 1 << std::endl;

    avl_node->_subtree_height = std::max(left_height, right_height) + 1;

//    auto *avl_node = dynamic_cast<AVL_tree<tkey, tvalue>::node*>(node);
//
//    size_t left_height = get_subtree_height(avl_node->left_subtree);
//    size_t right_height = get_subtree_height(avl_node->right_subtree);
//
//    avl_node->_subtree_height = std::max(left_height, right_height) + 1;
}

template<
        typename tkey,
        typename tvalue>
inline size_t AVL_tree<tkey, tvalue>::get_node_size() const noexcept
{
    return sizeof(typename AVL_tree<tkey, tvalue>::node);
}

template<
        typename tkey,
        typename tvalue>
inline void AVL_tree<tkey, tvalue>::call_node_constructor(
        typename binary_search_tree<tkey,tvalue>::node *raw_space,
        tkey const &key,
        tvalue const &value)
{
    allocator::construct(reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(raw_space), key, value);
}

template<
        typename tkey,
        typename tvalue>
inline void AVL_tree<tkey, tvalue>::call_node_constructor(
        typename binary_search_tree<tkey,tvalue>::node *raw_space,
        tkey const &key,
        tvalue &&value)
{
    allocator::construct(reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(raw_space), key, std::move(value));
}

template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::inject_additional_data(
        typename binary_search_tree<tkey,tvalue>::node *destination,
        typename binary_search_tree<tkey,tvalue>::node const *source) const
{
    auto *avl_destination = dynamic_cast<AVL_tree<tkey, tvalue>::node*>(destination);
    auto *avl_source = dynamic_cast<AVL_tree<tkey, tvalue>::node const*>(source);

    avl_destination->_subtree_height = avl_source->_subtree_height;
}

template<
        typename tkey,
        typename tvalue>
void AVL_tree<tkey, tvalue>::inject_additional_data(
        typename binary_search_tree<tkey,tvalue>::iterator_data *destination,
        typename binary_search_tree<tkey,tvalue>::node const *source) const
{
    auto *avl_destination = dynamic_cast<AVL_tree<tkey, tvalue>::iterator_data*>(destination);
    auto *avl_source = dynamic_cast<AVL_tree<tkey, tvalue>::node const*>(source);

    avl_destination->_subtree_height = avl_source->_subtree_height;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *AVL_tree<tkey, tvalue>::create_iterator_data() const
{
    return new iterator_data;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *AVL_tree<tkey, tvalue>::create_iterator_data(
        unsigned int depth,
        typename binary_search_tree<tkey,tvalue>::node *src_node) const
{
    return new iterator_data(depth, reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(src_node));
}

template<
        typename tkey,
        typename tvalue>
inline unsigned int AVL_tree<tkey, tvalue>::get_subtree_height(
        typename binary_search_tree<tkey,tvalue>::node* node) noexcept
{
    auto *avl_node = dynamic_cast<AVL_tree<tkey,tvalue>::node*>(node);

    if (avl_node == nullptr)
    {
        return 0;
    }

    return avl_node->_subtree_height;
}

template<
        typename tkey,
        typename tvalue>
inline std::string AVL_tree<tkey, tvalue>::get_typename() const noexcept
{
    return "AVL_tree<tkey, tvalue>";
}

#pragma endregion avl extra functions implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H