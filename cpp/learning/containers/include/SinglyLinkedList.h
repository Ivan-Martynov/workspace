#ifndef H_INCLUDE_SINGLYLINKEDLIST_H_H
#define H_INCLUDE_SINGLYLINKEDLIST_H_H

/**
 * @brief SinglyLinkedList class for educational purposes.
 *
 * @remarks If someone wants to use a singly linked list, then std::forward_list
 * is a default option. This file exists only for educational purposes.
 * One difference is that an empty tag class is used in constructors to avoid
 * ambiguity when creating objects with std::initializer_list or a number of
 * elements with a value to fill the container.
 *
 */

#include "ContainerHelper.h"
#include <cassert>

namespace Marvin
{

template <typename T>
class SinglyLinkedList
{
  public:
    using value_type = T;
    using size_type = int;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

  private:
    struct Node {
        value_type value;
        Node* next;

        template <typename... Args>
        explicit Node(Node* next_ptr, Args&&... args)
            : value {std::forward<Args>(args)...}, next {next_ptr}
        {
        }

        template <typename... Args>
        explicit Node(Args&&... args)
            : value {std::forward<Args>(args)...}, next {nullptr}
        {
        }

        Node(const Node&) = default;
        Node& operator=(const Node&) = default;

        void swap(Node& rhs) noexcept
        {
            using std::swap;
            swap(value, rhs.value);
            swap(next, rhs.next);
        }
    };

  public:
    /***************************************************************************
     * Iterator section                                                        *
     **************************************************************************/

    struct ConstIterator; // Forward declaration.

    struct Iterator {
        friend class ConstIterator;

        using iterator_category = std::forward_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        using self_t = Iterator;
        using node_t = Node;

        Iterator() : m_ptr {} {}
        explicit Iterator(Node* ptr) : m_ptr {ptr} {}

        reference operator*() const { return m_ptr->value; }
        pointer operator->() const { return &m_ptr->value; }

        self_t& operator++()
        {
            m_ptr = m_ptr->next;
            return *this;
        }

        self_t operator++(int)
        {
            auto temp {*this};
            m_ptr = m_ptr->next;
            return temp;
        }

        bool operator==(const self_t& rhs) const { return m_ptr == rhs.m_ptr; }
        bool operator!=(const self_t& rhs) const { return m_ptr != rhs.m_ptr; }

      private:
        Node* m_ptr;
    }; // struct IteratorForward
    static_assert(std::forward_iterator<Iterator>);

    struct ConstIterator {
        friend class SinglyLinkedList;

        using iterator_category = std::forward_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const value_type*;
        using reference = const value_type&;

        using self_t = ConstIterator;
        using node_t = const Node;

        using iterator_t = Iterator;

        ConstIterator() : m_ptr {} {}
        explicit ConstIterator(const Node* ptr) : m_ptr {ptr} {}
        ConstIterator(const iterator_t& it) : m_ptr {it.m_ptr} {}

        reference operator*() const { return m_ptr->value; }
        pointer operator->() const { return &m_ptr->value; }

        self_t& operator++()
        {
            m_ptr = m_ptr->next;
            return *this;
        }

        self_t operator++(int)
        {
            auto temp {*this};
            m_ptr = m_ptr->next;
            return temp;
        }

        bool operator==(const self_t& rhs) const { return m_ptr == rhs.m_ptr; }
        bool operator!=(const self_t& rhs) const { return m_ptr != rhs.m_ptr; }

      private:
        const Node* m_ptr;
    }; // struct ConstIteratorForward
    static_assert(std::forward_iterator<ConstIterator>);

    Iterator begin() { return Iterator {m_head}; }
    Iterator end() { return Iterator {nullptr}; }

    ConstIterator begin() const { return ConstIterator {m_head}; }
    ConstIterator end() const { return ConstIterator {nullptr}; }

    ConstIterator cbegin() const { return begin(); }
    ConstIterator cend() const { return end(); }

    /***************************************************************************
     * End of Iterator section                                                 *
     **************************************************************************/

    /***************************************************************************
     * Constructors & destructor section                                       *
     **************************************************************************/

    /**
     * @brief Default constructor with null pointer and zero elements.
     *
     */
    SinglyLinkedList() : m_head {}, m_length {0} {}

    /**
     * @brief Construct a SinglyLinkedList object with a number of elements,
     * each element assigned to a provided (or default) value.
     *
     * @remarks Using tag class to distinguish from the constructor having
     * initializer_list as a parameter.
     * Using private function to allocate elements to check for bad_alloc and
     * throw before initializing the members.
     *
     * @param[in] count Number of elements to create.
     * @param[in] value Value to assign to each element.
     */
    SinglyLinkedList(init_container_with_size_t, size_type count,
        const value_type& value = value_type {})
        : SinglyLinkedList {}
    {
        m_from_count_value(count, value);
    }

    /**
     * @brief Construct a SinglyLinkedList object with a pair of input iterators
     * to fill the linked list.
     *
     * @remark Using private function to allocate elements to check for
     * bad_alloc and throw before initializing the members.
     *
     * @param[in] first First iterator.
     * @param[in] last Second iterator.
     */
    SinglyLinkedList(std::input_iterator auto first, std::input_iterator auto last) : SinglyLinkedList {}
    {
        m_from_range(first, last);
    }

    /**
     * @brief Construct a SinglyLinkedList object from an initializer_list.
     *
     * @remark Essentially, using constructor taking two pointers, the beginning
     * and end of the initializer_list.
     *
     * @param[in] list Initializer_list to create a linked list from.
     */
    SinglyLinkedList(std::initializer_list<value_type> rhs) : SinglyLinkedList(rhs.begin(), rhs.end()) {}

    /**
     * @brief Copy constructor.
     *
     * @remark Essentially, using constructor taking two pointers, the beginning
     * and end of the linked list.
     *
     * @param[in] rhs Another linked list to build from.
     */
    SinglyLinkedList(const SinglyLinkedList& rhs) : SinglyLinkedList {rhs.cbegin(), rhs.cend()} {}

    void assign(size_type count, const value_type& value)
    {
        clear();
        m_from_count_value(count, value);
    }

    void assign(std::input_iterator auto first, std::input_iterator auto last)
    {
        clear();
        m_from_range(first, last);
    }

    void assign(std::initializer_list<value_type> rhs)
    {
        assign(rhs.begin(), rhs.end());
    }

    /**
     * @brief Assignment operator.
     *
     * @param[in] rhs Another linked list to build from.
     * @return SinglyLinkedList& Reference to the current instance.
     */
    SinglyLinkedList& operator=(const SinglyLinkedList& rhs)
    {
        if (this != &rhs) {
            assign(rhs.cbegin(), rhs.cend());
        }
        return *this;
    }

    SinglyLinkedList& operator=(std::initializer_list<value_type> rhs)
    {
        assign(rhs.begin(), rhs.end());
        return *this;
    }

    /**
     * @brief Move constructor.
     *
     * @param[in] rhs Another linked list to move.
     */
    SinglyLinkedList(SinglyLinkedList&& rhs) noexcept { swap(rhs); }

    /**
     * @brief Move operator.
     *
     * @param[in] rhs Another linked list to move.
     * @return SinglyLinkedList& Reference to the current instance.
     */
    SinglyLinkedList& operator=(SinglyLinkedList&& rhs) noexcept
    {
        if (this != &rhs) {
            clear();
            swap(rhs);
        }
        return *this;
    }

    ~SinglyLinkedList() noexcept { clear(); }

    /***************************************************************************
     * End Constructors & destructor section                                   *
     **************************************************************************/

    /***************************************************************************
     * Accessors section                                                       *
     **************************************************************************/

    value_type& front() noexcept { return m_head->value; }
    const value_type& front() const noexcept { return m_head->value; }

    /**
     * @brief Length (size) of the linked list.
     *
     * @return size_type Number of elements in the linked list.
     */
    size_type length() const noexcept { return m_length; }

    /**
     * @brief Size of linked list in bytes.
     *
     * @return std::size_t Number of bytes.
     */
    std::size_t size_in_bytes() const { return m_length * sizeof(value_type); }

    bool empty() const noexcept { return !m_head; }

    /***************************************************************************
     * End of Accessors section                                                *
     **************************************************************************/

    /***************************************************************************
     * Element insertion section                                               *
     **************************************************************************/

    /**
     * @brief Insert a value after the element pointed by the iterator.
     *
     * @param[in] it Iterator pointing to the item to insert after.
     * @param[in] value Value to insert.
     * @return Iterator Iterator pointing to the inserted element.
     */
    Iterator insert_after(ConstIterator it, const value_type& value)
    {
        auto* target_ptr {const_cast<Node*>(it.m_ptr)};
        target_ptr->next = m_create_node(target_ptr->next, value);
        ++m_length;

        return Iterator {target_ptr->next};
    }

    /**
     * @brief Insert an r-value after the element pointed by the iterator.
     *
     * @param[in] it Iterator pointing to the item to insert after.
     * @param[in] value Value to insert.
     * @return Iterator Iterator pointing to the inserted element.
     */
    Iterator insert_after(ConstIterator it, value_type&& value)
    {
        auto* target_ptr {const_cast<Node*>(it.m_ptr)};
        target_ptr->next = m_create_node(target_ptr->next, std::move(value));
        ++m_length;

        return Iterator {target_ptr->next};
    }

    /**
     * @brief Insert several copies of a value after the element pointed by the
     * iterator.
     *
     * @param[in] it Iterator pointing to the item to insert after.
     * @param[in] count Number of copies to create.
     * @param[in] value Value to insert.
     * @return Iterator Iterator pointing to the last inserted element.
     */
    Iterator insert_after(
        ConstIterator it, size_type count, const value_type& value)
    {
        return Iterator {
            m_insert_after(const_cast<Node*>(it.m_ptr), count, value)};
    }

    Iterator insert_after(ConstIterator it, size_type count, value_type&& value)
    {
        return Iterator {m_insert_after(
            const_cast<Node*>(it.m_ptr), count, std::move(value))};
    }

    /**
     * @brief Insert elements from the input iterators range after the element
     * pointed by the iterator.
     *
     * @param[in] it Iterator pointing to the item to insert after.
     * @param[in] first Start of the range, inclusive.
     * @param[in] last End of the range, exclusive.
     * @return Iterator Iterator pointing to the last inserted element.
     */
    Iterator insert_after(ConstIterator it, std::input_iterator auto first,
        std::input_iterator auto last)
    {
        return Iterator {
            m_insert_after(const_cast<Node*>(it.m_ptr), first, last)};
    }

    /**
     * @brief Insert elements from an initializer list after the element pointed
     * by the iterator.
     *
     * @param[in] it
     * @param[in] list
     * @return Iterator
     */
    Iterator insert_after(
        ConstIterator it, std::initializer_list<value_type> list)
    {
        return insert_after(it, list.begin(), list.end());
    }

    template <typename... Args>
    Iterator emplace_after(ConstIterator it, Args&&... args)
    {
        return insert_after(it, std::forward<Args>(args)...);
    }

    /**
     * @brief Add element to the front of the linked list.
     *
     * @param[in] value Value to add.
     */
    void push_front(const value_type& value)
    {
        m_head = m_create_node(m_head, value);
        ++m_length;
    }

    /**
     * @brief Move element to the front of the linked list.
     *
     * @param[in] value Value to move.
     */
    void push_front(value_type&& value)
    {
        m_head = m_create_node(m_head, std::move(value));
        ++m_length;
    }

    /**
     * @brief Emplace element to the front of the linked list using inner type
     * constructor's parameters.
     *
     * @tparam Args Types of arguments to construct an element.
     * @param[in] args Parameters to use for object's construction.
     */
    template <typename... Args>
    void emplace_front(Args&&... args)
    {
        m_head = m_create_node(m_head, std::forward<Args>(args)...);
        ++m_length;
    }

    void splice_after(ConstIterator it, SinglyLinkedList& rhs) noexcept
    {
        m_splice_after(const_cast<Node*>(it.m_ptr), rhs);
    }

    void splice_after(ConstIterator it, SinglyLinkedList&& rhs) noexcept
    {
        m_splice_after(const_cast<Node*>(it.m_ptr), rhs);
    }

    /***************************************************************************
     * End of Element insertion section                                        *
     **************************************************************************/

    /***************************************************************************
     * Element deletion section                                                *
     **************************************************************************/

    void pop_front() noexcept
    {
        if (m_head) {
            auto* next {m_head->next};
            delete m_head;
            m_head = next;
            --m_length;
        }
    }

    Iterator erase_after(ConstIterator it) noexcept
    {
        return Iterator {m_erase_after(const_cast<Node*>(it.m_ptr))};
    }

    Iterator erase_after(ConstIterator first, ConstIterator last) noexcept
    {
        return Iterator {m_erase_after(
            const_cast<Node*>(first.m_ptr), const_cast<Node*>(last.m_ptr))};
    }

    void clear()
    {
        erase_after(cbegin(), cend());
        pop_front();
#if 1
        assert(m_length == 0);
        assert(!m_head);
#endif
    }

    /***************************************************************************
     * End of Element deletion section                                         *
     **************************************************************************/

    void swap(SinglyLinkedList& rhs) noexcept
    {
        using std::swap;
        swap(m_head, rhs.m_head);
        swap(m_length, rhs.m_length);
    }

    /***************************************************************************
     * Sort section                                                            *
     **************************************************************************/

    // Merge sort, bottom-up.
    template <typename Comp>
    void sort(Comp compare_func)
    {
        auto* list {m_head};
        if (!m_head->next) {
            return;
        }

        size_type n {1};
        while (true) {
            auto* p {list};
            list = nullptr;
            Node* tail {nullptr};

            size_type merge_count {0};
            while (p) {
                ++merge_count;
                auto* q {p};
                size_type p_size {0};
                for (size_type i {0}; i < n; ++i) {
                    ++p_size;
                    q = q->next;
                    if (!q) {
                        break;
                    }
                }

                size_type q_size {n};
                while ((p_size > 0) || ((q_size > 0) && q)) {
                    Node* e {};
                    if (p_size == 0) {
                        e = q;
                        q = q->next;
                        --q_size;
                    } else if ((q_size == 0) || !q) {
                        e = p;
                        p = p->next;
                        --p_size;
                    } else if (!compare_func(q->value, p->value)) {
                        e = p;
                        p = p->next;
                        --p_size;
                    } else {
                        e = q;
                        q = q->next;
                        --q_size;
                    }

                    if (tail) {
                        tail->next = e;
                    } else {
                        list = e;
                    }
                    tail = e;
                }
                p = q;
            }
            tail->next = nullptr;
            if (merge_count < 2) {
                m_head->next = list;
                return;
            } else {
                n <<= 1;
            }
        }
    }

    void sort() { return sort(std::less<value_type>()); }

    /***************************************************************************
     * End of Sort section                                                     *
     **************************************************************************/

  private:
    Node* m_head;
    size_type m_length;

    template <typename... Args>
    static Node* m_create_node(Node* next, Args&&... args)
    {
        Node* node_ptr {};
        try {
            node_ptr = new Node {next, std::forward<Args>(args)...};
        } catch (...) {
            delete node_ptr;
            throw;
        }
        return node_ptr;
    }

    template <typename... Args>
    static Node* m_create_node(Args&&... args)
    {
        Node* node_ptr {};
        try {
            node_ptr = new Node {std::forward<Args>(args)...};
        } catch (...) {
            delete node_ptr;
            throw;
        }
        return node_ptr;
    }

    Node* m_erase_after(Node* node)
    {
        if (node) {
            auto* current {node->next};
            node->next = current->next;
            delete current;
            return node->next;
        } else {
            return nullptr;
        }
    }

    Node* m_erase_after(Node* first, Node* last)
    {
        if (first != last) {
            auto* current {first->next};
            while (current != last) {
                auto* temp {current};
                current = current->next;
                delete temp;
                --m_length;
            }
            first->next = last;
        }
        return last;
    }

    void m_splice_after(Node* node, SinglyLinkedList& rhs) noexcept
    {
        if (rhs.empty()) {
            return;
        }
        auto* temp {node->next};
        node->next = rhs.m_head;
        auto* slide {rhs.m_head};
        while (slide->next) {
            slide = slide->next;
        }
        slide->next = temp;
        m_length += rhs.m_length;

        rhs.m_head = nullptr;
        rhs.m_length = 0;
    }

    template <typename... Args>
    Iterator m_insert_after(Node* node, size_type count, Args&&... args)
    {
        auto* current {node};
        auto&& arg {std::forward<Args>(args)...};
        while (count-- > 0) {
            current->next = m_create_node(current->next, arg);
            current = current->next;
            ++m_length;
        }
        return Iterator {current};
    }

    Iterator m_insert_after(Node* node, std::input_iterator auto first,
        std::input_iterator auto last)
    {
        auto* current {node};
        for (; first != last; ++first) {
            current->next = m_create_node(current->next, *first);
            current = current->next;
            ++m_length;
        }
        return Iterator {current};
    }

    template <typename... Args>
    void m_from_count_value(size_type count, Args&&... args)
    {
        if (count < 1) {
            return;
        }

        auto&& arg {std::forward<Args>(args)...};
        m_head = m_create_node(arg);
        ++m_length;
        m_insert_after(m_head, --count, arg);
    }

    void m_from_range(
        std::input_iterator auto first, std::input_iterator auto last)
    {
        if (first == last) {
            return;
        }

        m_head = m_create_node(*first);
        ++m_length;
        m_insert_after(m_head, ++first, last);
    }
};

#if 0
template <typename T>
class SinglyLinkedList
{
  private:
    struct NodeBase {
        NodeBase* next;
        explicit NodeBase(NodeBase* ptr = nullptr) : next {ptr} {}
    };

    /**
     * @brief Private Node struct to use only inside the SingleLinkedClass.
     * Assumes to be very simple.
     *
     */
    struct Node : public NodeBase {
        T value;

#if 0
        explicit Node(const T& v, NodeBase* node_ptr = nullptr)
            : NodeBase {node_ptr}, value {v}
        {
        }
#endif

#if 1
        template <typename... Args>
        Node(NodeBase* ptr, Args&&... args)
            : NodeBase {ptr}, value {std::forward<Args>(args)...}
        {
        }

        template <typename... Args>
        Node(Args&&... args) : NodeBase {}, value {std::forward<Args>(args)...}
        {
        }
#endif
    };

  public:
    using value_type = T;
    using size_type = int;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    /***************************************************************************
     * Iterator section                                                        *
     **************************************************************************/

    struct ConstIterator; // Forward declaration.

    struct Iterator {
        friend class ConstIterator;

        using iterator_category = std::forward_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        using self_t = Iterator;
        using node_t = Node;

        Iterator() : m_ptr {} {}
        explicit Iterator(NodeBase* ptr) : m_ptr {ptr} {}

        reference operator*() const
        {
            return static_cast<node_t*>(m_ptr)->value;
        }
        pointer operator->() const
        {
            return &static_cast<node_t*>(m_ptr)->value;
        }

        self_t& operator++()
        {
            m_ptr = m_ptr->next;
            return *this;
        }

        self_t operator++(int)
        {
            auto temp {*this};
            m_ptr = m_ptr->next;
            return temp;
        }

        bool operator==(const self_t& rhs) const { return m_ptr == rhs.m_ptr; }
        bool operator!=(const self_t& rhs) const { return m_ptr != rhs.m_ptr; }

        self_t next() const { return Iterator {m_ptr ? m_ptr->next : nullptr}; }

      private:
        NodeBase* m_ptr;
    }; // struct IteratorForward
    static_assert(std::forward_iterator<Iterator>);

    struct ConstIterator {
        friend class SinglyLinkedList;

        using iterator_category = std::forward_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const value_type*;
        using reference = const value_type&;

        using self_t = ConstIterator;
        using node_t = const Node;

        using iterator_t = Iterator;

        ConstIterator() : m_ptr {} {}
        explicit ConstIterator(const NodeBase* ptr) : m_ptr {ptr} {}
        ConstIterator(const iterator_t& it) : m_ptr {it.m_ptr} {}

        reference operator*() const
        {
            return static_cast<node_t*>(m_ptr)->value;
        }
        pointer operator->() const
        {
            return &static_cast<node_t*>(m_ptr)->value;
        }

        self_t& operator++()
        {
            m_ptr = m_ptr->next;
            return *this;
        }

        self_t operator++(int)
        {
            auto temp {*this};
            m_ptr = m_ptr->next;
            return temp;
        }

        bool operator==(const self_t& rhs) const { return m_ptr == rhs.m_ptr; }
        bool operator!=(const self_t& rhs) const { return m_ptr != rhs.m_ptr; }

        self_t next() const
        {
            return ConstIterator {m_ptr ? m_ptr->next : nullptr};
        }

      private:
        const NodeBase* m_ptr;
    }; // struct ConstIteratorForward
    static_assert(std::forward_iterator<ConstIterator>);

    Iterator before_begin() { return Iterator {&m_head}; }
    Iterator begin() { return Iterator {m_head.next}; }
    Iterator end() { return Iterator {nullptr}; }

    ConstIterator before_begin() const { return ConstIterator {&m_head}; }
    ConstIterator begin() const { return ConstIterator {m_head.next}; }
    ConstIterator end() const { return ConstIterator {nullptr}; }

    ConstIterator cbefore_begin() const { return before_begin(); }
    ConstIterator cbegin() const { return begin(); }
    ConstIterator cend() const { return end(); }

    /***************************************************************************
     * End of Iterator section                                                 *
     **************************************************************************/

    /***************************************************************************
     * Constructors & destructor                                               *
     **************************************************************************/

    /**
     * @brief Default constructor with null pointer and zero elements.
     *
     */
    explicit SinglyLinkedList() : m_head {}, m_length {0} {}

    /**
     * @brief Construct a SinglyLinkedList object with a number of elements,
     * each element assigned to a provided (or default) value.
     *
     * @remarks Using tag class to distinguish from the constructor having
     * initializer_list as a parameter.
     * Using private function to allocate elements to check for bad_alloc and
     * throw before initializing the members.
     *
     * @param[in] count Number of elements to create.
     * @param[in] value Value to assign to each element.
     */
    explicit SinglyLinkedList(init_container_with_size_t, size_type count,
        const value_type& value = value_type {})
    {
        m_from_count_value(count, value);
    }

    /**
     * @brief Construct a SinglyLinkedList object with a pair of input iterators
     * to fill the linked list.
     *
     * @remark Using private function to allocate elements to check for
     * bad_alloc and throw before initializing the members.
     *
     * @param[in] first First iterator.
     * @param[in] last Second iterator.
     */
    explicit SinglyLinkedList(
        std::input_iterator auto first, std::input_iterator auto last)
    {
        m_from_range(first, last);
    }

    /**
     * @brief Construct a SinglyLinkedList object from an initializer_list.
     *
     * @remark Essentially, using constructor taking two pointers, the beginning
     * and end of the initializer_list.
     *
     * @param[in] list Initializer_list to create a linked list from.
     */
    explicit SinglyLinkedList(std::initializer_list<value_type> rhs)
        : SinglyLinkedList(rhs.begin(), rhs.end())
    {
    }

    /**
     * @brief Copy constructor.
     *
     * @remark Essentially, using constructor taking two pointers, the beginning
     * and end of the linked list.
     *
     * @param[in] rhs Another linked list to build from.
     */
    explicit SinglyLinkedList(const SinglyLinkedList& rhs)
        : SinglyLinkedList {rhs.cbegin(), rhs.cend()}
    {
    }

    void assign(size_type count, const value_type& value)
    {
        clear();
        try {
            m_from_count_value(count, value);
        } catch (...) {
            throw;
        }
    }

    void assign(std::input_iterator auto first, std::input_iterator auto last)
    {
        clear();
        try {
            m_from_range(first, last);
        } catch (...) {
            throw;
        }
    }

    void assign(std::initializer_list<value_type> rhs)
    {
        assign(rhs.begin(), rhs.end());
    }

    /**
     * @brief Assignment operator.
     *
     * @param[in] rhs Another linked list to build from.
     * @return SinglyLinkedList& Reference to the current instance.
     */
    SinglyLinkedList& operator=(const SinglyLinkedList& rhs)
    {
        if (this != &rhs) {
            assign(rhs.cbegin(), rhs.cend());
        }
        return *this;
    }

    SinglyLinkedList& operator=(std::initializer_list<value_type> rhs)
    {
        assign(rhs.begin(), rhs.end());
        return *this;
    }

    /**
     * @brief Move constructor.
     *
     * @param[in] rhs Another linked list to move.
     */
    explicit SinglyLinkedList(SinglyLinkedList&& rhs) noexcept
    {
        std::swap(m_head, rhs.m_head);
        std::swap(m_length, rhs.m_length);
    }

    /**
     * @brief Move operator.
     *
     * @param[in] rhs Another linked list to move.
     * @return SinglyLinkedList& Reference to the current instance.
     */
    SinglyLinkedList& operator=(SinglyLinkedList&& rhs) noexcept
    {
        if (this != &rhs) {
            clear();
            std::swap(m_head, rhs.m_head);
            std::swap(m_length, rhs.m_length);
        }
        return *this;
    }

    /**
     * @brief Destroy the Singly Linked List object.
     *
     */
    ~SinglyLinkedList() noexcept { clear(); }

    /***************************************************************************
     * End of Constructors & destructor section                                *
     **************************************************************************/

    /***************************************************************************
     * Accessors section                                                       *
     **************************************************************************/

    /**
     * @brief Length (size) of the linked list.
     *
     * @return size_type Number of elements in the linked list.
     */
    size_type length() const { return m_length; }

    /**
     * @brief Size of linked list in bytes.
     *
     * @return std::size_t Number of bytes.
     */
    std::size_t size_in_bytes() const { return m_length * sizeof(value_type); }

    /***************************************************************************
     * End of Accessors section                                                *
     **************************************************************************/

    /***************************************************************************
     * Element insertion section                                               *
     **************************************************************************/
    Iterator insert_after(ConstIterator it, const value_type& value)
    {
        return Iterator {m_insert_after(it, value)};
    }

    template <typename... Args>
    Iterator emplace_after(ConstIterator it, Args&&... args)
    {
        return Iterator {m_insert_after(it, std::forward<Args>(args)...)};
    }

    void push_front(const value_type& value)
    {
        m_insert_after(cbefore_begin(), value);
    }

    template <typename... Args>
    void emplace_front(Args&&... args)
    {
        m_insert_after(cbefore_begin(), std::forward<Args>(args)...);
    }

    /***************************************************************************
     * End of Element insertion section                                        *
     **************************************************************************/

    /***************************************************************************
     * Element deletion section                                                *
     **************************************************************************/

    void erase_after(ConstIterator it) noexcept
    {
        auto* target_ptr {const_cast<NodeBase*>(it.m_ptr)};
        auto* node_ptr {static_cast<Node*>(target_ptr->next)};
        target_ptr->next = node_ptr->next;
        --m_length;
        delete node_ptr;

        // auto* ptr {it.m_ptr};
        // auto* node_ptr {static_cast<Node*>(ptr->next)};
        // ptr->next = node_ptr->next;
        //--m_length;
        // delete node_ptr;
    }

    void erase_after(NodeBase* pos, NodeBase* last) noexcept
    {
        auto* node_ptr {static_cast<Node*>(pos->next)};
        while (node_ptr != last) {
            auto* temp {node_ptr};
            node_ptr = static_cast<Node*>(node_ptr->next);
            delete temp;
            --m_length;
        }
        pos->next = last;
    }

    void clear() noexcept { erase_after(&m_head, nullptr); }

    /***************************************************************************
     * End of Element deletion section                                         *
     **************************************************************************/

    // Merge sort, bottom-up.
    template <typename Comp>
    void sort(Comp compare_func)
    {
        auto* list {static_cast<Node*>(m_head.next)};
        if (!m_head.next) {
            return;
        }

        size_type n {1};
        while (true) {
            auto* p {list};
            list = nullptr;
            Node* tail {nullptr};

            size_type merge_count {0};
            while (p) {
                ++merge_count;
                auto* q {p};
                size_type p_size {0};
                for (size_type i {0}; i < n; ++i) {
                    ++p_size;
                    q = static_cast<Node*>(q->next);
                    if (!q) {
                        break;
                    }
                }

                size_type q_size {n};
                while ((p_size > 0) || ((q_size > 0) && q)) {
                    Node* e {};
                    if (p_size == 0) {
                        e = q;
                        q = static_cast<Node*>(q->next);
                        --q_size;
                    } else if ((q_size == 0) || !q) {
                        e = p;
                        p = static_cast<Node*>(p->next);
                        --p_size;
                    } else if (!compare_func(q->value, p->value)) {
                        e = p;
                        p = static_cast<Node*>(p->next);
                        --p_size;
                    } else {
                        e = q;
                        q = static_cast<Node*>(q->next);
                        --q_size;
                    }

                    if (tail) {
                        tail->next = e;
                    } else {
                        list = e;
                    }
                    tail = e;
                }
                p = q;
            }
            tail->next = nullptr;
            if (merge_count < 2) {
                m_head.next = list;
                return;
            } else {
                n <<= 1;
            }
        }
    }

    void sort() { return sort(std::less<value_type>()); }

  private:
    NodeBase m_head {};
    size_type m_length {};

    template <typename... Args>
    static Node* m_create_node(Args&&... args)
    {
        Node* node_ptr {};
        try {
            node_ptr = new Node {std::forward<Args>(args)...};
        } catch (...) {
            delete node_ptr;
            throw;
        }
        return node_ptr;
    }

    template <typename... Args>
    static Node* m_create_node(NodeBase* ptr, Args&&... args)
    {
        Node* node_ptr {};
        try {
            node_ptr = new Node {ptr, std::forward<Args>(args)...};
        } catch (...) {
            delete node_ptr;
            throw;
        }
        return node_ptr;
    }

    void m_from_count_value(size_type count, const value_type& value)
    {
        if (count < 1) {
            return;
        }

        try {
            NodeBase* current_ptr {&m_head};
            for (; count; --count) {
                current_ptr->next = m_create_node(value);
                current_ptr = current_ptr->next;
                ++m_length;
            }
        } catch (...) {
            // Try to clean up the successfully allocated nodes.
            clear();
            throw;
        }
    }

    void m_from_range(
        std::input_iterator auto first, std::input_iterator auto last)
    {
        if (first == last) {
            return;
        }

        try {
            NodeBase* current_ptr {&m_head};
            for (; first != last; ++first) {
                current_ptr->next = m_create_node(*first);
                current_ptr = current_ptr->next;
                ++m_length;
            }
        } catch (...) {
            // Try to clean up the successfully allocated nodes.
            clear();
            throw;
        }
    }

    template <typename... Args>
    NodeBase* m_insert_after(ConstIterator pos, Args&&... args)
    {
        auto* target_ptr {const_cast<NodeBase*>(pos.m_ptr)};

        Node* node_ptr {
            m_create_node(target_ptr->next, std::forward<Args>(args)...)};
        target_ptr->next = node_ptr;

        ++m_length;

        return target_ptr->next;
    }

    void m_erase_after(ConstIterator it)
    {
        auto* target_ptr {it.m_ptr};
        auto* node_ptr {static_cast<Node*>(*target_ptr->next)};
        target_ptr->next = node_ptr->next;
        delete node_ptr;
        --m_length;
    }
}; // class SinglyLinkedList
#endif

template <typename T>
void swap(SinglyLinkedList<T>& lhs, SinglyLinkedList<T>& rhs) noexcept
{
    using std::swap;
    lhs.swap(rhs);
}

} // namespace Marvin

#endif // H_INCLUDE_SINGLYLINKEDLIST_H_H
