#ifndef H_INCLUDE_TREE_H_H
#define H_INCLUDE_TREE_H_H

namespace Marvin {

template <typename T>
class Tree {
  public:
    using value_type = T;
    using size_type = int;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    Tree() : m_head {} {}

    explicit Tree(const value_type& v)
    try : m_head(new Node {v}) {
    }
    catch (...) {
        throw;
    }

    Tree(const Tree& other) = default;
    Tree& operator=(const Tree& other) = default;

    Tree(Tree&& other) = default;
    Tree& operator=(Tree&& other) = default;

    ~Tree() = default;

  private:
    struct Node {
        T value;
        Node* left;
        Node* right;

        Node(const T& v, Node* l, Node* r) : value {v}, left {l}, right {r} {}

        explicit Node(const T& v) : Node {v, nullptr, nullptr} {}
    };

    Node* m_head;
}; // namespace Marvin

} // namespace Marvin

#endif // H_INCLUDE_TREE_H_H
