#pragma once

#include "Eigen/Core"

#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include <mutex>

#include "lf_types.h"

#include <QElapsedTimer>

#define EXTENDED_STAT

namespace lf {
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> MatrixXd;

    bool hungarian(const MatrixXd& a);

    enum eSolverResult {
        E_OPTIMAL,
        E_FEASIBLE,
        E_INFEASIBLE,
        E_UNKNOWN
    };

    class ISolver {
    public:
        virtual eSolverResult solve() = 0;
    };

    typedef double TValue;

    struct SNode {
        typedef std::shared_ptr<SNode> pNode;
        typedef std::atomic<pNode> pAtomicNode;
        typedef std::atomic<TValue> TAtomicValue;

        pNode m_parent;
        int m_level;
        std::size_t m_id;
        TValue m_cost;
        TValue m_minBound;
        TValue m_maxBound;

        SNode(std::size_t nn = 0);
        SNode(const pNode parent, std::size_t nn, TValue cc);
        virtual ~SNode() {}

        TValue value() const;
        bool operator<(const SNode& b) const;
    };

    typedef SNode::pNode pNode;

    bool operator<(const pNode& l, const pNode& r);

    class CSolverParameters {
    public:
        uint64_t m_timeLimit = 0;               //!< Solver time limit [sec]
        uint32_t m_threadLimit = 1;             //!< Solver thread limit
        uint32_t m_variantsPerLevelLimit = 0;
    };


    class IBranchAndBound : public ISolver {
    public:
        enum eFinishedNodeType
        {
            E_NOT_FINISHED,
            E_FINISHED_BY_RESTRICTION,
            E_FINISHED_BY_SOLVE,
        };

        virtual ~IBranchAndBound() {}

        typedef SNode::pNode pNode;
        typedef SNode::pAtomicNode pAtomicNode;
        typedef SNode::TAtomicValue TAtomicValue;

        virtual eFinishedNodeType finished(const pNode node) const = 0;
        virtual pNode generateRoot() = 0;
        virtual std::vector<pNode> generateBranch(const pNode currNode) = 0;
        virtual std::pair<TValue, TValue> calculateBound(const pNode node) const = 0;
//        virtual std::pair<TValue, TValue> minMaxCostEdge(std::size_t node, const std::vector<bool>& visited) const = 0;
    };

    class CBranchAndBound : public IBranchAndBound {
    public:
        using IBranchAndBound::eFinishedNodeType;

        enum eSearchStrategy {
            E_GREED,
            E_RANDOM,
            E_GREED_RANDOM,
            E_RANDOM_LEVEL
        };

        CBranchAndBound(const CSolverParameters& params = CSolverParameters());

        TValue minCost() const { return m_minCost; }

        eSolverResult solve();

        std::function<void(pNode)> onVariant = [](pNode) {};
        std::function<void(pNode)> onSkip = [](pNode) {};
        std::function<void(pNode)> onStep = [](pNode) {};
        std::function<pNode(sorted_deque<pNode>&)> selectNext = [](sorted_deque<pNode>& q) { return q.take_front(); };

#ifdef EXTENDED_STAT
        std::array<std::atomic_size_t, 100> m_skippedList;
        std::array<std::atomic_size_t, 100> m_stepsList;
#endif
    protected:
        void switchStrategy(sorted_deque<pNode>& queue, std::function<pNode(sorted_deque<pNode>&)>& select);
        void step(sorted_deque<pNode>& queue);
        eSolverResult internalSolve();
        eSolverResult internalParallelSolve(std::size_t thr_num = 10);

        CSolverParameters m_params;

        std::mutex m_variantLock;
        TAtomicValue m_minCost;
        pAtomicNode m_minPathNode;
    };
};
