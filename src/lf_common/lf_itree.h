#pragma once

#include <memory>
#include <vector>
#include <stack>

#include "lf_types.h"

namespace lf
{
    template<class _Ty>
    class childrens_fwd_iterator {
    public:
        typedef _Ty value_type;
        typedef _Ty* pointer;
        typedef _Ty& reference;

        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using iterator_concept = std::forward_iterator_tag;

        childrens_fwd_iterator(value_type b = value_type{}) { if (b) m_stack.push_back(b); }
        childrens_fwd_iterator& operator++() {
            value_type node = m_stack.back();
            m_stack.pop_back();
            for (auto it = node->children().rbegin(); it != node->children().rend(); ++it) {
                m_stack.push_back(*it);
            }
            return *this;
        }
        childrens_fwd_iterator operator++(int) { childrens_fwd_iterator tmp = *this; ++(*this); return tmp; }
        reference operator*() { return m_stack.back(); }
        //pointer operator->() { return m_stack.back(); }
        constexpr reference operator*() const { return const_cast<reference>(m_stack.back()); }
        //constexpr pointer operator->() const { return const_cast<pointer>(m_stack.back()); }
        bool operator==(const childrens_fwd_iterator& other) const { return m_stack == other.m_stack; }
        bool operator!=(const childrens_fwd_iterator& other) const { return !(*this == other); }

    private:
        std::vector<value_type> m_stack;
    };

    template<typename _Ty>
    class children_view : public std::ranges::view_interface<children_view<_Ty>> {
    public:
        typedef typename childrens_fwd_iterator<_Ty> iterator;

        explicit children_view(_Ty root) : m_root(root) {}

        iterator begin() const { return iterator(m_root); }
        iterator end() const { return iterator(); }
    private:
        _Ty m_root;
    };


    template<class T>
    class TreeItem : public std::enable_shared_from_this<T> {
    public:
        typedef std::shared_ptr<T> Ptr;
        typedef std::weak_ptr<T> wPtr;

        struct CParentsAdvance { Ptr operator()(Ptr p) const { return p->parent(); } };
        //! View for all parents
        typedef lf::fwd_view<Ptr, CParentsAdvance> parents_view;

        //! View for all children
        typedef lf::children_view<Ptr> children_view;

        virtual ~TreeItem() {}

        template<class T>
        static void treeTraversal(Ptr node, T pred) {
            if (!node) {
                return;
            }

            std::stack<Ptr> stack;
            stack.push(node);

            while (!stack.empty()) {
                Ptr curr = stack.top();
                stack.pop();

                // Push the children onto the stack in reverse order
                for (int i = static_cast<int>(curr->m_children.size()) - 1; i >= 0; i--) {
                    stack.push(curr->m_children[i]);
                }

                if (pred(curr))
                    break;
            }
        }

        static auto leafs(const Ptr node) { return children_view(node) | std::views::filter([](const Ptr p) -> bool { return p->isLeaf(); }); }
        static auto branch(const Ptr node) { return parents_view(node); }

        virtual void addChild(Ptr child) {
            m_children.push_back(child);
            child->m_parent = this->shared_from_this();
        }

        virtual void removeChild(Ptr child) {
            m_children.erase(std::find(m_children.begin(), m_children.end(), child));
            child->m_parent.reset();
        }

        virtual bool isLeaf() const { return m_children.empty(); }

        const std::vector<Ptr>& children() const { return m_children; }
        Ptr parent() { return m_parent.lock(); }
        const Ptr parent() const { return m_parent.lock(); }

    protected:
        wPtr m_parent;
        std::vector<Ptr> m_children;
    };
}
