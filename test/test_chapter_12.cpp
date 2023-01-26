#include <random>

#include <gtest/gtest.h>

#include <Part 3/Chapter 12/BinarySearchTree.hpp>
#include <stl_vector.hpp>
#include <Utility/Utility.hpp>

class TestBinarySearchTree : public testing::Test {
protected:
    Algorithm::BinarySearchTree<int> m_tree1;
    Algorithm::BinarySearchTree<int> m_tree2;
    TinySTL::vector<Algorithm::Node<int>*> m_nodes;

protected:
    virtual void SetUp() override {
        {
            for (int i = 0; i < 100; i += 13) {
                m_nodes.push_back(new Algorithm::Node<int>(i));
                m_tree1.Insert(m_nodes.back());
            }
        }
        {
            int data[] = {15, 18, 6, 3, 2, 4, 7, 13, 9, 17, 20};
            for (auto&& i : data) {
                m_nodes.push_back(new Algorithm::Node<int>(i));
                m_tree2.Insert(m_nodes.back());
            }
        }
    }

    virtual void TearDown() override {
        for (auto&& node : m_nodes) {
            delete node;
        }
    }
};

TEST_F(TestBinarySearchTree, TestBinarySearchTree) {
    m_tree1.Inorder();

    Algorithm::Node<int>* node = nullptr;

    node = m_tree1.RecursiveSearch(39);
    EXPECT_EQ(node->retrieve(), 39);

    node = m_tree1.RecursiveSearch(55);
    EXPECT_EQ(node, nullptr);

    node = m_tree1.IterativeSearch(39);
    EXPECT_EQ(node->retrieve(), 39);

    node = m_tree1.IterativeSearch(55);
    EXPECT_EQ(node, nullptr);

    EXPECT_EQ(m_tree1.RecursiveMinimum()->retrieve(), 0);
    EXPECT_EQ(m_tree1.RecursiveMaximum()->retrieve(), 91);

    m_tree1.Erase(m_tree1.IterativeSearch(39));
    m_tree1.Inorder();

    m_tree1.Erase(m_tree1.IterativeSearch(100));
    m_tree1.Inorder();
}

TEST_F(TestBinarySearchTree, a) {
    std::cout << "Preorder:     "; m_tree2.Preorder();
    std::cout << "Inorder:      "; m_tree2.Inorder();
    std::cout << "Postorder:    "; m_tree2.Postorder();
    std::cout << "BreadthFirst: "; m_tree2.BreadthFirst();
    std::cout << "DepthFirst:   "; m_tree2.DepthFirst();
    std::cout << "Deleting one by one:" << std::endl;
    while (m_tree2.Root()) {
        m_tree2.Inorder();
        m_tree2.Erase(m_tree2.Root());
    }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
