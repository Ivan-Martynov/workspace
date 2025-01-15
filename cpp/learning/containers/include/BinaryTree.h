#ifndef H_INCLUDE_BINARYTREE_H_H
#define H_INCLUDE_BINARYTREE_H_H

namespace Marvin
{

template <typename T>
class BinaryTree
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
        Node* left;
        Node* right;

        template <typename... Args>
        Node(Args&&... args)
            : value {std::forward<Args>(args)...}, left {nullptr},
              right {nullptr}
        {
        }

        template <typename... Args>
        Node(Node* left_ptr, Node* right_ptr, Args&&... args)
            : value {std::forward<Args>(args)...}, left {left_ptr},
              right {right_ptr}
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
    BinaryTree() : m_root {nullptr} {}

    BinaryTree(const value_type& value) : m_root {m_create_node(value)} {}

    ~BinaryTree() = default;

    BinaryTree(const BinaryTree& rhs) = default;
    BinaryTree& operator=(const BinaryTree& rhs) = default;

    BinaryTree(BinaryTree&& rhs) = default;
    BinaryTree& operator=(BinaryTree&& rhs) = default;

    void swap(BinaryTree& rhs) noexcept {
        using std::swap;
        swap(m_root, rhs.m_root);
    }

    void push_front_to_left(const value_type& value)
    {
        m_root = m_create_node {m_root, nullptr, value};
    }

    void push_front_to_right(const value_type& value)
    {
        m_root = m_create_node {nullptr, m_root, value};
    }

    template <typename... Args>
    void emplace_front_to_left(Args... args)
    {
        m_root = m_create_node {m_root, nullptr, std::forward<Args>(args)...};
    }

    template <typename... Args>
    void emplace_front_to_right(Args... args)
    {
        m_root = m_create_node {nullptr, m_root, std::forward<Args>(args)...};
    }

  private:
    Node* m_root;

    template <typename... Args>
    static Node* m_create_node(Node* left, Node* right, Args&&... args)
    {
        Node* node_ptr {};
        try {
            node_ptr = new Node {left, right, std::forward<Args>(args)...};
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

    template <typename... Args>
    void m_insert_after_left(Node* node, Args&&... args)
    {
        node->left = m_create_node(
            node->left, node->right, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void m_insert_after_right(Node* node, Args&&... args)
    {
        node->right = m_create_node(
            node->left, node->right, std::forward<Args>(args)...);
    }
};


} // namespace Marvin

#endif // H_INCLUDE_BINARYTREE_H_H
