#include <QCoreApplication>
#include <QDateTime>
#include <QElapsedTimer>
#include <iomanip>

#include "lft_tsp.h"

#define APP_VERSION "1.0.0"

lf::MatrixXd convert(const std::vector<std::vector<lf::TValue>>& costMatrix) {
    std::size_t nx = 0;
    std::size_t ny = costMatrix.size();
    for (auto& x : costMatrix) {
        if (nx == 0)
            nx = x.size();
        else if (nx != x.size()) {
            std::cout << "Wrong matrix size" << std::endl;
            return lf::MatrixXd();
        }
    }

    lf::MatrixXd res;
    res.resize(ny, nx);
    for (auto iy : std::views::iota(std::size_t(0), ny))
        for (auto ix : std::views::iota(std::size_t(0), nx)) {
            res(iy, ix) = costMatrix[iy][ix];
        }

    return res;
}

void solveBaB(const std::vector<std::vector<lf::TValue>>& costMatrix, uint32_t thr) {
    using namespace lf;

    QElapsedTimer timer;
    timer.start();

    lft::CBranchAndBoundTSP solver(convert(costMatrix), lf::CSolverParameters({0, thr, 0}));

    std::mutex mx;
    auto print = [&mx](const QString& s) {
        mx.lock();
        printf(s.toStdString().c_str());
        mx.unlock();
    };

    std::atomic_size_t variants;
    solver.onVariant = [&solver, &variants](pNode path) {
        variants++;
    };

    std::atomic_size_t skipped;
    solver.onSkip = [&solver, &skipped](pNode node) {
        skipped++;
#ifdef EXTENDED_STAT
        solver.m_skippedList[node->m_level]++;
#endif
    };

    std::atomic_int32_t counter;
    std::atomic_size_t steps;

    solver.onStep = [&solver, &print, &counter, &variants, &steps, &skipped](pNode node) {
        steps++;
#ifdef EXTENDED_STAT
        if (solver.m_stepsList.size() > node->m_level)
            solver.m_stepsList[node->m_level]++;
#endif

        counter++;
        if (counter > 3000) {
            counter.store(0);
            print(QString("%1 | %2 | %3\r").arg(variants.load(), 2).arg(skipped.load(), 8).arg(steps.load(), 8));
        }
    };

    auto result = solver.solve();
    print(QString("%1\r").arg(" ", 50));
    constexpr int nameSize = 12;

    std::cout << std::left << std::setw(nameSize) << "Task" << ": " << costMatrix.size() << "x" << costMatrix.size() << std::endl;
    std::cout << std::setw(nameSize) << "Variants" << ": " << variants << std::endl;
    std::cout << std::setw(nameSize) << "Step" << ": " << steps << std::endl;
    std::cout << std::setw(nameSize) << "Minimum Cost" << ": " << solver.minCost() << std::endl;
    std::cout << std::setw(nameSize) << "Path" << ": ";
    for (size_t node : solver.minPath()) {
        std::cout << node << " ";
    }
    std::cout << std::endl;
    std::cout << std::setw(nameSize) << "CTime" << ": " << timer.elapsed() / 1000. << " [sec]" << std::endl;
    //std::cout << std::setw(nameSize) << "Max queue" << ": " << solver.maxQueue() << std::endl << std::endl;

#ifdef EXTENDED_STAT
    int last = -std::distance(solver.m_stepsList.rend(), std::ranges::find_if(solver.m_stepsList | std::views::reverse, [](const std::atomic_size_t& i) { return i > 0; }));
    std::cout << std::setw(nameSize) << "Max level" << ": " << last << std::endl;

    std::cout << "Level "
        << std::right << std::setw(8) << "Skipped" << " | "
        << std::setw(8) << "Steps"
        << std::endl;
    for (auto i : std::views::iota(0, last)) {
        std::cout
            << "[" << std::right << std::setw(3) << i << "] "
            << std::setw(8) << solver.m_skippedList[i] << " | "
            << std::setw(8) << solver.m_stepsList[i]
            << std::endl;
    }
#endif // EXTENDED_STAT

}

void bab(std::size_t N = 8, uint32_t thr = 1) {
    std::vector<std::vector<lf::TValue>> costMatrix16 = {
        {0, 23, 67, 54, 92, 43, 88, 27, 59, 31, 74, 66, 85, 45, 32, 77},
        {58, 0, 19, 75, 41, 29, 83, 54, 72, 94, 34, 88, 60, 37, 91, 26},
        {34, 61, 0, 42, 66, 89, 24, 95, 12, 49, 78, 53, 81, 39, 56, 91},
        {82, 13, 58, 0, 35, 94, 72, 38, 27, 87, 16, 43, 76, 92, 64, 51},
        {19, 76, 24, 83, 0, 49, 61, 72, 38, 58, 84, 21, 37, 45, 93, 29},
        {45, 33, 89, 15, 53, 0, 47, 59, 96, 12, 88, 34, 42, 80, 67, 52},
        {73, 48, 91, 66, 39, 27, 0, 14, 82, 61, 57, 94, 19, 28, 77, 35},
        {25, 64, 14, 71, 53, 88, 47, 0, 92, 42, 31, 68, 51, 95, 76, 40},
        {61, 39, 78, 16, 24, 87, 34, 53, 0, 84, 19, 25, 62, 14, 91, 58},
        {93, 26, 49, 77, 65, 12, 31, 46, 59, 0, 88, 54, 37, 63, 82, 18},
        {54, 87, 12, 94, 73, 45, 21, 66, 84, 36, 0, 53, 29, 47, 65, 79},
        {32, 14, 83, 55, 61, 72, 93, 24, 43, 78, 19, 0, 57, 62, 36, 88},
        {47, 76, 62, 25, 43, 58, 17, 93, 51, 69, 34, 82, 0, 72, 49, 16},
        {15, 55, 29, 78, 57, 46, 33, 64, 27, 89, 48, 35, 19, 0, 85, 62},
        {64, 72, 15, 31, 88, 69, 42, 53, 23, 45, 92, 39, 57, 21, 0, 34},
        {81, 19, 56, 62, 38, 83, 74, 31, 66, 41, 23, 91, 52, 87, 61, 0}
    };

    std::vector<std::vector<lf::TValue>> costMatrixN;
    costMatrixN.reserve(N);

    for (std::size_t i = 0; i < N; ++i) {
        costMatrixN.emplace_back(
            costMatrix16[i].begin(),
            costMatrix16[i].begin() + N
        );
    }

    solveBaB(costMatrixN, thr);
}

int main(int argc, char* argv[])
{
#ifdef _WIN32
    system("chcp 65001");
#endif

    QCoreApplication a(argc, argv);

    bool is_singleThread = false;
    uint32_t threads = 4;
    uint32_t N = 8;

    for (int i = 1; i < a.arguments().size(); ++i) {
        auto s = a.arguments().at(i);
        auto s_next = (i + 1 < a.arguments().size()) ? a.arguments().at(i + 1) : QString();
        if (s == "--help" || s == "-h") {
            std::cout <<
R"(Usage of traveling salesman problem app:
  lf_test_tsp [options]

Options:
  -n <N>           Matrix size N (2 <= N <= 16)
  -single          Run in single-threaded mode
  -multi <threads> Run in multi-threaded mode with given thread count (2 <= N <= 128)
  -h, --help       Show this help message
)";
            return 0;
        }
        else if (s.compare("-n") == 0) { auto n = s_next.toUInt(); if (n >= 2 && n <= 16) N = n; }
        else if (s.compare("-single") == 0) { is_singleThread = true; }
        else if (s.compare("-multi") == 0) { auto th = s_next.toUInt(); if(th > 1 && th <= 128) threads = th; }
    }

    bab(N, threads);
    return 0;
}
