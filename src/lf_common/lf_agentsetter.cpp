#pragma once

#include "lf_agentsetter.h"

#include <random>

bool lf::CAgentRandomSetter::set(const std::vector<lf::CPropertyMap>& props, const pGraph graph, lf::pErrors err)
{
    std::seed_seq seed{ m_seed };

    //if 3/4 of map points is less than cart number, assert
    if (!m_graphTester(props, graph)) {
        //                VALUE_OR_EXCEPTION(, "map is to small for this cart number");
        return false;
    }

    std::default_random_engine eng{ seed };
    std::mt19937 _gen(eng());
    int mapSize = static_cast<int>(graph->verticesNum());
    //TODO check it!
    std::uniform_int_distribution<int> startPointDstrb(0, mapSize - 1); // guaranteed unbiase

    for (auto& p : props) {
        int steps = 0;
        lf::CPropertyMap fixedProps = p;

        //TODO make random select from lesser array every time
        do {
            int startPointShift = startPointDstrb(_gen);
            fixedProps[QByteArray("vertex")] = startPointShift;
            steps++;
        } while (!m_creator(fixedProps, err) && steps < 100);

        if (steps >= 100) {
            return false;
        }
    }
    return true;
}
