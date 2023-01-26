#ifndef _ALGORITHM_BINARY_SEARCH_TREE_HPP_
#define _ALGORITHM_BINARY_SEARCH_TREE_HPP_

#include <iostream>

#include "BinaryTree.hpp"

namespace Algorithm {

    template <typename T>
    class BinarySearchTree : public BinaryTree<T> {
    public:
        BinarySearchTree(Node<T>* root = nullptr)
            : BinaryTree<T>(root) {}
        ~BinarySearchTree() = default;

        
        Node<T>* RecursiveSearch(const T& value) { return const_cast<Node<T>*>(const_cast<const BinarySearchTree<T>*>(this)->RecursiveSearch(value)); }
        Node<T>* IterativeSearch(const T& value) { return const_cast<Node<T>*>(const_cast<const BinarySearchTree<T>*>(this)->IterativeSearch(value)); }
        Node<T>* RecursiveMinimum(Node<T>* node = nullptr) { return const_cast<Node<T>*>(const_cast<const BinarySearchTree<T>*>(this)->RecursiveMinimum(node)); }
        Node<T>* RecursiveMaximum(Node<T>* node = nullptr) { return const_cast<Node<T>*>(const_cast<const BinarySearchTree<T>*>(this)->RecursiveMaximum(node)); }
        Node<T>* IterativeMinimum(Node<T>* node = nullptr) { return const_cast<Node<T>*>(const_cast<const BinarySearchTree<T>*>(this)->IterativeMinimum(node)); }
        Node<T>* IterativeMaximum(Node<T>* node = nullptr) { return const_cast<Node<T>*>(const_cast<const BinarySearchTree<T>*>(this)->IterativeMaximum(node)); }

        const Node<T>* RecursiveSearch(const T& value) const;
        const Node<T>* IterativeSearch(const T& value) const;
        const Node<T>* RecursiveMinimum(Node<T>* node = nullptr) const;
        const Node<T>* RecursiveMaximum(Node<T>* node = nullptr) const;
        const Node<T>* IterativeMinimum(Node<T>* node = nullptr) const;
        const Node<T>* IterativeMaximum(Node<T>* node = nullptr) const;

        void Insert(Node<T>* node);
        void Erase(Node<T>* node);

    private:
        void Transplant(Node<T>*& src, Node<T>*& dst);
    };

    template <typename T>
    const Node<T>* BinarySearchTree<T>::RecursiveSearch(const T& value) const {
        if (m_root == nullptr) {
            return m_root;
        }
        return m_root->RecursiveSearch(value);
    }

    template <typename T>
    const Node<T>* BinarySearchTree<T>::IterativeSearch(const T& value) const {
        Node<T>* temp = m_root;
        while (temp != nullptr && temp->retrieve() != value) {
            if (value < temp->retrieve())
                temp = temp->left();
            else
                temp = temp->right();
        }
        return temp;
    }

    template <typename T>
    const Node<T>* BinarySearchTree<T>::RecursiveMinimum(Node<T>* node) const {
        if (node == nullptr)
            node = m_root;
        return node->RecursiveMinimum();
    }

    template <typename T>
    const Node<T>* BinarySearchTree<T>::RecursiveMaximum(Node<T>* node) const {
        if (node == nullptr)
            node = m_root;
        return node->RecursiveMaximum();
    }

    template <typename T>
    const Node<T>* BinarySearchTree<T>::IterativeMinimum(Node<T>* node) const {
        if (node == nullptr)
            node = m_root;
        Node<T>* temp = node;
        while (temp->left() != nullptr) {
            temp = temp->left();
        }
        return temp;
    }

    template <typename T>
    const Node<T>* BinarySearchTree<T>::IterativeMaximum(Node<T>* node) const {
        if (node == nullptr)
            node = m_root;
        Node<T>* temp = node;
        while (temp->right() != nullptr) {
            temp = temp->right();
        }
        return temp;
    }

    template <typename T>
    void BinarySearchTree<T>::Insert(Node<T>* node) {
        Node<T>* curr = m_root; // Node being compared with given node.
        Node<T>* last = curr;   // The parent of the given node.

        // Descend until reaching a leaf.
        while (curr != nullptr) {
            last = curr;
            if (node->retrieve() < curr->retrieve())
                curr = curr->left();
            else
                curr = curr->right();
        }

        node->m_parent = last;

        // Insert the node.
        if (last == nullptr)
            m_root = node;
        else if (node->retrieve() < last->retrieve())
            last->m_left = node;
        else
            last->m_right = node;
    }

    template <typename T>
    void BinarySearchTree<T>::Erase(Node<T>* node) {
        if (node == nullptr)
            return;

        // `left` is null, just transplant `right` subtree to `node`.
        if (node->left() == nullptr)
            Transplant(node->m_right, node);
        // `left` is not null, `right` is null, just transplant `left` subtree to `node`.
        else if (node->right() == nullptr)
            Transplant(node->m_left, node);
        // Neither is null.
        else {
            // Find the succesor of node to transparent.
            Node<T>* succ = IterativeMinimum(node->right());
            // Swap `succ` and `node`'s right subtree if `succ` and `node` are not directly linked.
            if (succ->parent() != node) {
                Transplant(succ->m_right, succ);
                succ->m_right = node->m_right;
                succ->m_right->m_parent = succ;
            }
            // Move `succ` to `node`'s position.
            Transplant(succ, node);
            succ->m_left = node->m_left;
            succ->m_left->m_parent = succ;
        }
    }

    template <typename T>
    void BinarySearchTree<T>::Transplant(Node<T>*& src, Node<T>*& dst) {
        // `dst` is root.
        if (dst == m_root)
            m_root = src;
        // `dst` is on the left.
        else if (dst == dst->parent()->left())
            dst->m_parent->m_left = src;
        // `dst` is on the right.
        else
            dst->m_parent->m_right = src;
        // Set parent of `dst`.
        if (src != nullptr)
            src->m_parent = dst->parent();    
    }

} // namespace Algorithm

#endif //!_ALGORITHM_BINARY_SEARCH_TREE_HPP_
