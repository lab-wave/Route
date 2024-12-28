#pragma once

#include "lf_iagent.h"
#include "lf_igraph.h"

namespace lf {
    class CAgentSetter {
    public:
        std::function<lf::pAgent(const lf::CPropertyMap&)> m_creator;
        std::function<bool()> m_graphTester;

        virtual ~CAgentSetter() {}
        virtual bool set(const std::vector<lf::CPropertyMap>& props, const lf::pGraph graph) = 0;
    };

    class CAgentRandomSetter : public CAgentSetter {
    public:
        std::function<lf::pAgent(const lf::CPropertyMap&)> m_creator;
        std::function<bool(const std::vector<lf::CPropertyMap>&, const pGraph)> m_graphTester;

        CAgentRandomSetter(std::size_t p_seed) : m_seed(p_seed) {}

        virtual bool set(const std::vector<lf::CPropertyMap>& props, const pGraph graph);

        std::size_t m_seed;
    };
};

