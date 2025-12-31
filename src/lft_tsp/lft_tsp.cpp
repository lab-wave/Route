#include "lft_tsp.h"

namespace lft {
    std::vector<bool> CBranchAndBoundTSP::visited(pNode node) const
    {
        std::vector<bool> res(n, false);
        pNode n = node;
        while (n)
        {
            res[n->m_id] = true;
            n = n->m_parent;
        }

        return res;
    }

    std::vector<std::size_t> CBranchAndBoundTSP::path(pNode node) const
    {
        std::vector<std::size_t> res(node->m_level + 1);
        pNode n = node;

        while (n/* && n->m_level != 0*/)
        {
            res[n->m_level] = n->m_id;
            n = n->m_parent;
        }

        return res;
    }

    bool CBranchAndBoundTSP::contains(const pNode currNode, std::size_t node) const
    {
        pNode n = currNode;
        while (n)
        {
            if (node == n->m_id)
                return true;
            n = n->m_parent;
        }
        return false;
    }

    lf::IBranchAndBound::eFinishedNodeType CBranchAndBoundTSP::finished(const pNode node) const
    {
        return (node->m_level == n) ? E_FINISHED_BY_SOLVE : E_NOT_FINISHED;
    }

    CBranchAndBoundTSP::pNode CBranchAndBoundTSP::generateRoot()
    {
        pNode root = std::make_shared<lf::SNode>(0);
        root->m_level = 0;
        root->m_cost = 0;
        auto bound = calculateBound(root);
        root->m_minBound = bound.first;
        root->m_maxBound = bound.second;

        return root;
    }

    std::vector<lf::pNode> CBranchAndBoundTSP::generateBranch(const pNode currNode)
    {
        if (currNode->m_level == n - 1) {
            return std::vector<pNode>(1, std::make_shared<lf::SNode>(currNode, 0, m_costMatrix(currNode->m_id, 0))); // Завершаем путь возвратом к началу
        }

        std::vector<pNode> result;

        for (std::size_t i = 0; i < n; ++i) {
            if (!contains(currNode, i)) {
                result.push_back(std::make_shared<lf::SNode>(currNode, i, m_costMatrix(currNode->m_id, i)));
            }
        }
        return result;
    }

    std::pair<lf::TValue, lf::TValue> CBranchAndBoundTSP::calculateBound(const pNode node) const
    {
        std::pair<lf::TValue, lf::TValue> bound = std::make_pair(node->m_cost, node->m_cost);
        std::vector<bool> v = visited(node);

        auto mm = minMaxCostEdge(node->m_id, v);
        bound = std::make_pair(bound.first + mm.first, bound.second + mm.second);
        for (std::size_t i = 0; i < n; ++i) {
            if (!v[i]) {
                mm = minMaxCostEdge(i, v);
                bound = std::make_pair(bound.first + mm.first, bound.second + mm.second);
            }
        }
        return bound;
    }


    std::pair<lf::TValue, lf::TValue> CBranchAndBoundTSP::minMaxCostEdge(std::size_t node, const std::vector<bool>& visited) const
    {
        bool hasUnvisited = false;
        lf::TValue minCost = std::numeric_limits<lf::TValue>::max();
        lf::TValue maxCost = std::numeric_limits<lf::TValue>::min();
        for (std::size_t i = 0; i < n; ++i) {
            if (!visited[i]) {
                hasUnvisited = true;
                minCost = std::min(minCost, m_costMatrix(node, i));
                maxCost = std::max(maxCost, m_costMatrix(node, i));
            }
        }

        if (!hasUnvisited)
            return std::make_pair(m_costMatrix(node, 0), m_costMatrix(node, 0));

        return std::make_pair(minCost, maxCost);
    }

}
