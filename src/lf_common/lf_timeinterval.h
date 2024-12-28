#pragma once

#include "lf_types.h"

#include <QString>
#include <QVariant>
#include <QColor>
#include <QMap>
#include <QIcon>

namespace lf
{
    template<class T>
    class CTimeInterval {
    public:
        typedef T time_type;

        T m_start;
        T m_end;

        bool intersects(T t0, T t1) const {
            if (t0 == t1)
                return m_start < t0 && t0 < m_end;

            //    return std::min(r.second, t1) - std::max(r.first, t0) > 0;
            return (t0 <= m_start && t1 >= m_end) ||
                (m_start <= t0 && t0 < m_end) ||
                (m_start < t1 && t1 <= m_end);
        }

        bool intersects(const CTimeInterval& other) const {
            return intersects(other.m_start, other.m_end);
        }

        T intersectSize(T t0, T t1) const {
            T s = std::max(m_start, t0);
            T e = std::min(m_end, t1);
            if (e > s)
                return e - s;
            return 0;
        }

        bool operator<(const CTimeInterval& other) const {
            return m_start < other.m_start;
        }
    };

    template<class Ti>
    class CRestrictions : public sorted_deque<Ti> {
    public:
        typedef Ti::time_type time_type;
        using sorted_deque<Ti>::begin;
        using sorted_deque<Ti>::end;

        sorted_deque<Ti>::const_iterator intersected(time_type t0, time_type t1) const
        {
            auto w = std::upper_bound(begin(), end(), t0, [](time_type t, const Ti& r) { return t < r.m_start; });
            if (w != begin())
                w = std::prev(w);

            for (auto i = w; i != end(); ++i) {
                if (i->intersects(t0, t1)) {
                    return i;
                }
            }
            return end();
        }

        bool intersects(time_type t0, time_type t1) const {
            return intersected(t0, t1) != end();
        }
    };
}
