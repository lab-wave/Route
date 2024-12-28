#pragma once

#include <limits>

#include "lf_types.h"

namespace lf {
    class IState {
    public:
        virtual ~IState() {}

        virtual std::partial_ordering operator<=>(const IState& r) const = 0;
    };

    template<class T>
    class CState : public IState {
    public:
        CState(T t = 0) : m_time(t) {}

        std::partial_ordering operator<=>(const IState& r) const override {
            auto rs = dynamic_cast<const CState*>(&r);
            if (!rs)
                return std::partial_ordering::unordered;
            return (m_time <=> rs->m_time);
        }
        std::partial_ordering operator<=>(const CState& r) const { return (m_time <=> r.m_time); }
        std::partial_ordering operator<=>(const T& t) const { return (m_time <=> t); }

        constexpr const T& time() const { return m_time; }
        constexpr void time(const T& t) { m_time = t; }
    protected:
        T m_time;
    };
}
