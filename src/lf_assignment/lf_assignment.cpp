#include <vector>
#include <thread>

#include <QFile>
#include <QTextStream>

#include "lf_assignment.h"

namespace lf {
    bool hungarian(const MatrixXd& a) {
//        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> tttt;

        int n = 10;
        int m = 10;

        int max_number = std::numeric_limits<int>::max();


        std::vector<int> u(n + 1), v(m + 1), p(m + 1), way(m + 1);
        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            int j0 = 0;
            std::vector<int> minv(m + 1, max_number);
            std::vector<char> used(m + 1, false);
            do {
                used[j0] = true;
                int i0 = p[j0], delta = max_number, j1;
                for (int j = 1; j <= m; ++j)
                    if (!used[j]) {
                        //int cur = a[i0][j] - u[i0] - v[j];
                        int cur = a(i0, j) - u[i0] - v[j];
                        if (cur < minv[j])
                            minv[j] = cur, way[j] = j0;
                        if (minv[j] < delta)
                            delta = minv[j], j1 = j;
                    }
                for (int j = 0; j <= m; ++j)
                    if (used[j])
                        u[p[j]] += delta, v[j] -= delta;
                    else
                        minv[j] -= delta;
                j0 = j1;
            } while (p[j0] != 0);
            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }

        return true;
    }

    bool operator<(const SNode::pNode& l, const SNode::pNode& r) {
        return *l < *r;
    }

    SNode::SNode(std::size_t nn)
        : m_parent()
        , m_level(0)
        , m_id(nn)
        , m_cost(0)
        , m_minBound(0)
        , m_maxBound(0)
    {
    }

    SNode::SNode(const pNode parent, std::size_t nn, TValue cc)
        : m_parent(parent)
        , m_level(parent ? (parent->m_level + 1) : 0)
        , m_id(nn)
        , m_cost(parent ? (parent->m_cost + cc) : 0)
        , m_minBound(0)
        , m_maxBound(0)
    {
    }


    TValue SNode::value() const
    {
        return (m_cost + m_minBound/*+ (m_maxBound + m_minBound) / 2.*/) /*/ (m_level + 1)*/;
    }

    bool SNode::operator<(const SNode& b) const
    {
        if (m_level != b.m_level)
            return m_level > b.m_level;
        return value() < b.value();
        //return m_bound / static_cast<double>(1.1 * m_level + 1) < b.m_bound / static_cast<double>(1.1 * b.m_level + 1);
    }

    CBranchAndBound::CBranchAndBound(const CSolverParameters& params)
        : m_params(params)
        , m_minCost(std::numeric_limits<TValue>::max())
    {
        onSkip = [this](pNode node) {
        };

        onStep = [this](pNode node) {
        };

        eSearchStrategy ss = E_GREED;

        auto selectNextGreed = [this](sorted_deque<pNode>& queue) -> pNode {
            pNode next = queue.front();
            queue.pop_front();
            return next;
        };

        auto selectNextRandom = [this](sorted_deque<pNode>& queue) -> pNode {
            int64_t random_value = std::rand();
            int element = random_value * (queue.size() - 1) / RAND_MAX;

            auto i = queue.begin();
            while (element > 0) {
                element--;
                i++;
            }

            pNode next = *i;
            queue.erase(i);
            return next;
        };

        auto selectNextGreedRandom = [this, selectNextGreed, selectNextRandom](sorted_deque<pNode>& queue) -> pNode {
            int64_t random_value = std::rand();

            if (random_value * 2 > RAND_MAX) {
                return selectNextGreed(queue);
            }
            else {
                return selectNextRandom(queue);
            }
        };

        if (ss == E_GREED) {
            selectNext = selectNextGreed;
        }
        else if (ss == E_RANDOM) {
            selectNext = selectNextRandom;
        }
        else if (ss == E_GREED_RANDOM) {
            selectNext = selectNextGreedRandom;
        }
    }

    void CBranchAndBound::switchStrategy(sorted_deque<pNode>& queue, std::function<pNode(sorted_deque<pNode>&)>& select)
    {
        selectNext = select;
    }

    void CBranchAndBound::step(sorted_deque<pNode>& queue)
    {
        pNode currNode = selectNext(queue);

        onStep(currNode);

        if (currNode->m_minBound < m_minCost) {
#if 1
            std::vector<pNode> variants;
            while (finished(currNode) == E_NOT_FINISHED)
            {
                variants = generateBranch(currNode);
                if (variants.size() != 1) {
                    break;
                }
                currNode = variants.front();
                auto bound = calculateBound(currNode);
                currNode->m_minBound = bound.first;
                currNode->m_maxBound = bound.second;
            }
#else
            std::vector<pNode> variants = generateBranch(currNode);
#endif

            for (auto childNode : variants) {

                auto bound = calculateBound(childNode);
                childNode->m_minBound = bound.first;
                childNode->m_maxBound = bound.second;

                eFinishedNodeType fnt = finished(childNode);

                if (fnt != E_NOT_FINISHED) {
                    if (fnt == E_FINISHED_BY_SOLVE && childNode->m_cost < m_minCost) {
                        m_variantLock.lock();
                        if (childNode->m_cost < m_minCost) {
                            m_minCost = childNode->m_cost;
                            if (m_minPathNode.load()) {
                                switchStrategy(queue, selectNext);
                            }
                            m_minPathNode = childNode;

                            onVariant(childNode);
                        }
                        m_variantLock.unlock();
                    }
                }
                else {
                    if (childNode->m_minBound < m_minCost) {
                        queue.insert(childNode);
                    }
                    else {
                        onSkip(childNode);
                    }
                }
            }
        }
        else {
            onSkip(currNode);
        }
    }

    eSolverResult CBranchAndBound::solve()
    {
        if (m_params.m_threadLimit >= 2)
            return internalParallelSolve(m_params.m_threadLimit);

        return internalSolve();
    }

    eSolverResult CBranchAndBound::internalSolve()
    {
        sorted_deque<pNode> queue;
        queue.insert(generateRoot());

        QElapsedTimer timer;
        timer.start();

        eSolverResult result = E_UNKNOWN;

        while (!queue.empty()) {
            if (m_params.m_timeLimit > 0 && timer.elapsed() > m_params.m_timeLimit * 1000) {
                result = E_FEASIBLE;
                break;
            }

            step(queue);
        }

        if (!m_minPathNode.load()) {
            result = E_INFEASIBLE;
        } else if (queue.empty()) {
            result = E_OPTIMAL;
        }

        return result;
    }


    eSolverResult CBranchAndBound::internalParallelSolve(std::size_t thr_num)
    {
        using namespace std::chrono_literals;

        struct CThread {
            std::jthread m_thread;
            pAtomicNode m_node;
            std::atomic_flag m_waiter;

            CThread(CThread&& t)
                : m_thread(std::forward<decltype(t.m_thread)>(t.m_thread))
                , m_node(t.m_node.load())
            {}
            CThread(std::jthread&& t) : m_thread(std::forward<decltype(t)>(t)) {}
        };

        std::vector<CThread> threads;
        threads.reserve(thr_num);

        std::atomic_bool terminate = false;
        std::atomic_int free_threads = thr_num;

        for (auto i : std::views::iota(std::size_t(0), thr_num)) {
            threads.emplace_back(CThread(std::jthread(
                [&threads, this, i, &terminate, &free_threads](){
                    // Передавать сам вектор потоков можно только если все уже выделено

                    while (!terminate)
                    {
                        // Ожидание появления не пустого указателя и его захват
                        pNode node = threads[i].m_node.exchange(nullptr);
                        if (node) {
                            // Обработка узла
                            sorted_deque<pNode> queue;
                            queue.insert(node);

                            while (!queue.empty()) {
                                if(terminate)
                                    break;

                                step(queue);
                            }

                            free_threads++;
                        }
                        else {
                            if (!threads[i].m_waiter.test()) {
                                threads[i].m_waiter.wait(true);
                                threads[i].m_waiter.clear();
                            }
                        }
                    }
                }
            )));
        }

        QElapsedTimer timer;
        timer.start();

        sorted_deque<pNode> queue;
        queue.insert(generateRoot());

        eSolverResult result = E_UNKNOWN;

        while (!queue.empty()) {
            if (m_params.m_timeLimit > 0 && timer.elapsed() > m_params.m_timeLimit * 1000) {
                result = E_FEASIBLE;
                terminate = true;
                break;
            }

            step(queue);

            if (!queue.empty() && queue.size() > 10 && free_threads > 0) {
                for (auto& tn : threads) {
                    if (!queue.empty() && !tn.m_node.load()) {
                        pNode lastNode = queue.back();
                        queue.pop_back();

                        tn.m_node = lastNode;
                        tn.m_waiter.test_and_set();
                        tn.m_waiter.notify_one();
                        free_threads--;
                        break;
                    }
                }
            }
        }

        terminate = true;
        for (auto i : std::views::iota(std::size_t(0), thr_num)) {
            threads[i].m_waiter.test_and_set();
            threads[i].m_waiter.notify_one();
            threads[i].m_thread.join();
        }

        if (!m_minPathNode.load()) {
            result = E_INFEASIBLE;
        }
        else if (queue.empty()) {
            result = E_OPTIMAL;
        }

        return result;
    }
}
