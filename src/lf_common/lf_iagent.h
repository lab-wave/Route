#pragma once

#include <memory>
#include <random>
#include "lf_istate.h"

namespace lf {
    class IAgent {
    public:
        enum TId : uint32_t { VoidId = std::numeric_limits<uint32_t>::max() };

        typedef std::shared_ptr<IAgent> Ptr;

        virtual ~IAgent() {}

        virtual TId id() const = 0;
        virtual void setId(TId id) = 0;
    };

    class Agent : public IAgent {
    public:
        Agent(TId _id = TId::VoidId) : m_id(_id) {}

        constexpr TId id() const { return m_id; }
        constexpr void setId(TId id) { m_id = id; }
    protected:
        TId m_id;
    };

    typedef IAgent::Ptr pAgent;
}
