#pragma once

#include <atomic>
#include "lf_iagent.h"
#include "lf_statistic.h"
#include "lf_igraph.h"
#include "lf_properties.h"

namespace lf {

    class ISimulator {
    public:
        virtual ~ISimulator() {}

        virtual const pAgent agent(IAgent::TId id) const = 0;

        virtual TTime time() const = 0;
        virtual void terminate() = 0;
        virtual pStat stat() const = 0;
        virtual pSimStat simStat() const = 0;
        virtual pGraph graph() const = 0;

        virtual bool graph(const pGraph& g, pErrors err = pErrors()) = 0;
        virtual pAgent createAgent(const CPropertyMap& m_props) = 0;
        virtual std::size_t agentsNum() const = 0;
        virtual bool simulateTimeMove(TTime& endTime) = 0;
    };

    typedef std::shared_ptr<ISimulator> pSimulator;

    class CSimulator : public ISimulator {
    public:
        TTime time() const override { return m_time.load(); }
        void terminate() override { m_terminate.store(true); }
        virtual pStat stat() const override { return m_stat; }
        virtual pSimStat simStat() const override { return m_simStat; }
    protected:
        std::atomic<TTime> m_time;
        std::atomic_bool m_terminate;
        CPropertyMap m_props;

        pStat m_stat;
        pSimStat m_simStat;
    };
}
