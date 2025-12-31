#include "lf_assignment.h"

namespace lft {
    //! Branch and Bound algorithm for Travelling salesman problem
    class CBranchAndBoundTSP : public lf::CBranchAndBound {
    public:
        using lf::IBranchAndBound::eFinishedNodeType;
        typedef Eigen::Matrix<lf::TValue, Eigen::Dynamic, Eigen::Dynamic> MatrixX;

        CBranchAndBoundTSP(const MatrixX& costMatrix, const lf::CSolverParameters& params = lf::CSolverParameters())
            : lf::CBranchAndBound(params), m_costMatrix(costMatrix), n(costMatrix.rows()) {
        }

        std::vector<std::size_t> minPath() const { return path(m_minPathNode.load()); }
    private:
        std::vector<bool> visited(pNode n) const;
        std::vector<std::size_t> path(pNode n) const;
        bool contains(const pNode currNode, std::size_t node) const;

        virtual eFinishedNodeType finished(const pNode node) const;
        pNode generateRoot() override;
        std::vector<pNode> generateBranch(const pNode currNode) override;
        std::pair<lf::TValue, lf::TValue> calculateBound(const pNode node) const override;
        std::pair<lf::TValue, lf::TValue> minMaxCostEdge(std::size_t node, const std::vector<bool>& visited) const;

        MatrixX m_costMatrix;
        std::size_t n;
    };
}
