#ifndef _ALGORITHM_BINARY_TREE_HPP_
#define _ALGORITHM_BINARY_TREE_HPP_

#include <iostream>
#include <deque>

namespace Algorithm {

    template <typename T>
    struct Node {
        T data;
        Node<T>* m_left;
        Node<T>* m_right;
        Node<T>* m_parent;

    public:
        Node(const T& data, Node<T>* left = nullptr, Node<T>* right = nullptr)
            : data(data)
            , m_left(left)
            , m_right(right)
            , m_parent(nullptr) {
            if (left != nullptr)
                left->m_parent = this;
            if (right != nullptr)
                right->m_parent = this;
        }

        T& retrieve() { return data; }
        Node<T>* left() { return m_left; }
        Node<T>* right() { return m_right; }
        Node<T>* parent() { return m_parent; }
        const T& retrieve() const { return data; }
        const Node<T>* left() const { return m_left; }
        const Node<T>* right() const { return m_right; }
        const Node<T>* parent() const { return m_parent; }

        bool is_leaf() const { return left() == nullptr && right() == nullptr; }
        std::size_t height() const { return is_leaf() ? 0 : max(left()->height(), right()->height()) + 1; }
        std::size_t size() const { return 1 + (left() == nullptr ? 0 : left()->size()) + (right() == nullptr ? 0 : right()->size()); }

    public:
        void Preorder() const {
            std::cout << retrieve() << ' ';
            if (left() != nullptr)
                left()->Inorder();
            if (right() != nullptr)
                right()->Inorder();
        }

        void Inorder() const {
            if (left() != nullptr)
                left()->Inorder();
            std::cout << retrieve() << ' ';
            if (right() != nullptr)
                right()->Inorder();
        }

        void Postorder() const {
            if (left() != nullptr)
                left()->Inorder();
            if (right() != nullptr)
                right()->Inorder();
            std::cout << retrieve() << ' ';
        }

        Node<T>* RecursiveSearch(const T& value) { return const_cast<Node<T>*>(const_cast<const Node<T>*>(this)->RecursiveSearch(value)); }
        const Node<T>* RecursiveSearch(const T& value) const {
            if (value == retrieve())
                return this;
            else if (value < retrieve()) {
                if (left() == nullptr)
                    return nullptr;
                else
                    return left()->RecursiveSearch(value);
            }
            else {
                if (right() == nullptr)
                    return nullptr;
                else
                    return right()->RecursiveSearch(value);
            }
        }

        Node<T>* RecursiveMinimum() { return const_cast<Node<T>*>(const_cast<const Node<T>*>(this)->RecursiveMinimum()); }
        const Node<T>* RecursiveMinimum() const {
            if (left() == nullptr)
                return this;
            else
                return left()->RecursiveMinimum();
        }

        Node<T>* RecursiveMaximum() { return const_cast<Node<T>*>(const_cast<const Node<T>*>(this)->RecursiveMaximum()); }
        const Node<T>* RecursiveMaximum() const {
            if (right() == nullptr)
                return this;
            else
                return right()->RecursiveMaximum();
        }
    };

    template <typename T>
    class BinaryTree {
    protected:
        Node<T>* m_root{nullptr};

    public:
        explicit BinaryTree(Node<T>* root)
            : m_root(root) {}
        ~BinaryTree() = default;

    public:
        Node<T>* Root() { return m_root; }
        const Node<T>* Root() const { return m_root; }

        void BreadthFirst() const {
            if (m_root == nullptr)
                return;
            std::deque<Node<T>*> nodes;
            nodes.push_back(m_root);

            while (nodes.size() > 0) {
                Node<T>* temp = nodes.front();
                nodes.pop_front();
                std::cout << temp->retrieve() << ' ';
                if (temp->left())
                    nodes.push_back(temp->left());
                if (temp->right())
                    nodes.push_back(temp->right());
            }
            std::cout << std::endl;
        }
        
        void DepthFirst() const {
            if (m_root == nullptr)
                return;
            std::deque<Node<T>*> nodes;
            nodes.push_back(m_root);

            while (nodes.size() > 0) {
                Node<T>* temp = nodes.back();
                nodes.pop_back();
                std::cout << temp->retrieve() << ' ';
                if (temp->right())
                    nodes.push_back(temp->right());
                if (temp->left())
                    nodes.push_back(temp->left());
            }
            std::cout << std::endl;
        }

        void Preorder() const {
            if (m_root != nullptr) {
                m_root->Preorder();
            }
            std::cout << std::endl;
        }

        void Inorder() const {
            if (m_root != nullptr) {
                m_root->Inorder();
            }
            std::cout << std::endl;
        }

        void Postorder() const {
            if (m_root != nullptr) {
                m_root->Postorder();
            }
            std::cout << std::endl;
        }
    };

} // namespace Algorithm

#endif // _ALGORITHM_BINARY_TREE_HPP_