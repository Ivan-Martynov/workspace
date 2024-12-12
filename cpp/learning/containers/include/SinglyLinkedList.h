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

namespace Marvin {

template <typename T>
class SinglyLinkedList {
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
        }
        catch (...) {
            throw;
        }
    }

    void assign(std::input_iterator auto first, std::input_iterator auto last)
    {
        clear();
        try {
            m_from_range(first, last);
        }
        catch (...) {
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
                    }
                    else if ((q_size == 0) || !q) {
                        e = p;
                        p = static_cast<Node*>(p->next);
                        --p_size;
                    }
                    else if (!compare_func(q->value, p->value)) {
                        e = p;
                        p = static_cast<Node*>(p->next);
                        --p_size;
                    }
                    else {
                        e = q;
                        q = static_cast<Node*>(q->next);
                        --q_size;
                    }

                    if (tail) {
                        tail->next = e;
                    }
                    else {
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
            }
            else {
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
        }
        catch (...) {
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
        }
        catch (...) {
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
        }
        catch (...) {
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
        }
        catch (...) {
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

struct ListNodeBase {
    ListNodeBase* m_next;

    static void swap(ListNodeBase& lhs, ListNodeBase& rhs)
    {
        std::swap(lhs.m_next, rhs.m_next);
    }

    ListNodeBase* transfer_after(ListNodeBase* begin, ListNodeBase* end)
    {
        auto* root {begin->m_next};
        if (end) {
            begin->m_next = end->m_next;
            end->m_next = m_next;
        }
        else {
            begin->m_next = nullptr;
        }

        m_next = root;
        return end;
    }

    ListNodeBase* transfer_after(ListNodeBase* begin)
    {
        auto* end {begin};
        while (end && end->m_next) {
            end = end->m_next;
        }
        return transfer_after(begin, end);
    }

    void reverse_after()
    {
        if (!m_next) {
            return;
        }
        auto* tail {m_next};
        while (auto* temp {tail->m_next}) {
            auto* keep {m_next};
            m_next = temp;
            tail->m_next = temp->m_next;
            m_next->m_next = keep;
        }
    }
}; // struct ListNodeBase

#if 0
template <typename T>
void sort_list(SinglyLinkedList<T>& linked_list)
{
}
#endif

} // namespace Marvin

#endif // H_INCLUDE_SINGLYLINKEDLIST_H_H
