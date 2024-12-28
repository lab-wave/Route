#pragma once

#include <limits>
#include <memory>
#include <map>
#include <unordered_map>
#include <ranges>
#include <functional>
#include <deque>
#include <set>

#include <qglobal.h>
#include <QPointF>
#include <QString>

typedef double TTime;
typedef double TVel;
typedef double TAngularVel;
typedef double TDist;
typedef double TAngle;//degree
typedef double TEnergy;// kWh
typedef double TPower;// kW
typedef double TCost;
typedef QPointF TCoordinate;
static const TTime InfTime = std::numeric_limits<TTime>::max();
static const TTime VoidTime = InfTime;
static const size_t VoidIndex = std::numeric_limits<size_t>::max();
static const TEnergy VoidEnergy = std::numeric_limits<TEnergy>::signaling_NaN();
static const double gAccel = 10;//[m/s^2]

#ifdef _DEBUG
#define VALUE_OR_EXCEPTION(p_rvalue, p_message) \
    { Q_ASSERT_X(false, __FUNCTION__, p_message); p_rvalue; }
#else
#define VALUE_OR_EXCEPTION(p_rvalue, p_message) \
    { qDebug() << p_message; p_rvalue; }
#endif // _DEBUG


#define ASSERT_VALUE_OR_EXCEPTION(condition, p_rvalue, p_message) \
    { if(!(condition)) { VALUE_OR_EXCEPTION(p_rvalue, p_message); } }

#define CAST_VALUE_OR_EXCEPTION(p_type, var, from, p_rvalue) \
    var = std::dynamic_pointer_cast<p_type>(from); if (!var) VALUE_OR_EXCEPTION(p_rvalue, "ClassCastException(" #p_type "," __FUNCTION__ ")")

#define DECLARE_CASTED_OR_EXCEPTION(p_type, var, from, p_rvalue) \
    std::shared_ptr<p_type> CAST_VALUE_OR_EXCEPTION(p_type, var, from, p_rvalue)

namespace lf {
    template<typename T>
    void hash_combine(std::size_t& seed, T const& key) {
        std::hash<T> hasher;
        seed ^= hasher(key) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<class K, class T, class Hash>
    T value(const std::unordered_map<K, T, Hash>& map, const K& k, const T& d) { auto i = map.find(k); return (i == map.end()) ? d : i->second; }

    template<class K, class T>
    T value(const std::map<K, T>& map, const K& k, const T& d) { auto i = map.find(k); return (i == map.end()) ? d : i->second; }

    // append vectors (before c++23)
    template<class T>
    std::vector<T>& append(std::vector<T>& dst, const std::vector<T>& src) { dst.insert(dst.end(), src.cbegin(), src.cend()); return dst; }

    template<typename T> struct remove_pointer { using type = T; };
    template<typename T> struct remove_pointer<T*> { using type = T; };
    template<typename T> struct remove_pointer<std::shared_ptr<T>> { using type = T; };
    template<typename T> struct remove_pointer<std::weak_ptr<T>> { using type = T; };
    template<typename T> struct remove_pointer<std::unique_ptr<T>> { using type = T; };

    template <class _Ty>
    using remove_pointer_t = typename remove_pointer<_Ty>::type;

    template<class _Ty, class _Advance>
    class fwd_iterator {
    public:
        using value_type = _Ty;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::input_iterator_tag;
        using iterator_concept = std::input_iterator_tag;

        fwd_iterator(value_type b = value_type{}) : m_item(b) {}
        fwd_iterator& operator++() { m_item = _Advance{}(m_item); return *this; }
        fwd_iterator operator++(int) { fwd_iterator tmp = *this; ++(*this); return tmp; }
        reference operator*() { return m_item; }
        //pointer operator->() { return m_item; }
        constexpr reference operator*() const { return const_cast<reference>(m_item); }
        //constexpr pointer operator->() const { return const_cast<pointer>(m_item); }
        bool operator==(const fwd_iterator& other) const { return m_item == other.m_item; }
        bool operator!=(const fwd_iterator& other) const { return !(*this == other); }

    private:
        value_type m_item;
    };

    template<class _Ty, class _Advance>
    class fwd_view : public std::ranges::view_interface< fwd_view <_Ty, _Advance> > {
    public:
        typedef typename fwd_iterator<_Ty, _Advance> iterator;

        fwd_view(iterator b, iterator e = iterator{}) : m_begin(b), m_end(e) {}

        constexpr iterator begin() const { return m_begin; }
        constexpr iterator end() const { return m_end; }
    protected:
        iterator m_begin;
        iterator m_end;
    };

    template<typename View, typename Pred>
    requires std::ranges::range<View> && std::predicate<Pred, std::ranges::range_reference_t<View>>
    auto operator|(View&& view, Pred&& pred) {
        return std::ranges::filter_view{std::forward<View>(view), std::forward<Pred>(pred)};
    }

    struct to_vector {
        template <std::ranges::range R>
        auto operator()(R&& r) const {
            auto r_common = r | std::views::common;
            return std::vector(r_common.begin(), r_common.end());
        }
    };

    template <std::ranges::range R>
    auto operator|(R&& r, const to_vector& v) {
        return v(std::forward<R>(r));
    }

    template<typename T>
    class comparator {
    public:
        comparator(const std::function<bool(const T&, const T&)>& in = [](const T& l, const T& r) { return std::less<T>{}(l, r); })
            : m_f(in)
        {}
        constexpr bool operator()(const T& lhs, const T& rhs) const { return m_f(lhs, rhs); }
    protected:
        std::function<bool(const T&, const T&)> m_f;
    };

    template<typename T, class Compare = comparator<T>>
    class sorted_deque : public std::multiset<T, Compare> {
    public:
        using std::multiset<T, Compare>::begin;
        using std::multiset<T, Compare>::rbegin;
        using std::multiset<T, Compare>::end;
        using std::multiset<T, Compare>::rend;
        using std::multiset<T, Compare>::erase;

        // Конструктор с компаратором
        sorted_deque(const Compare& c = Compare())
            : std::multiset<T, Compare>(c)
        {}

        // Вставка элемента в отсортированное место
        void insert(const T& value) {
            // Найти позицию, где элемент должен быть вставлен
            //auto it = std::lower_bound(this->begin(), this->end(), value, m_comparator);
            std::multiset<T, Compare>::insert(/*it, */value);
        }

        void pop_front() {
            erase(begin());
        }

        void pop_back() {
            erase(std::prev(end()));
        }

        const T& front() const {
            return *begin();
        }

        const T& back() const {
            return *rbegin();
        }

        T take_front() {
            T res = front();
            pop_front();
            return res;
        }
    };

    class CError {
    public:
        enum eErrorType
        {
            E_INFO,
            E_WARNING,
            E_ERROR,
            E_CRITICAL
        };

        eErrorType m_type;
        QString m_msg;
    };

    class CErrors : public std::vector<CError> {
    public:
        typedef typename std::shared_ptr<CErrors> Ptr;

        using eErrorType = CError::eErrorType;
        void push_back(const CError& e) { if (e.m_type > m_type) m_type = e.m_type; std::vector<CError>::push_back(e); }

        QString msg() const { QString res; for (const auto& m : *this) { res += m.m_msg + "\n"; } return res; }

        eErrorType m_type;
    };

    typedef typename CErrors::Ptr pErrors;

};
