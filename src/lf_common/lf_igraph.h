#pragma once

#include <memory>
#include <vector>
#include <set>

#include "lf_iagent.h"
#include "lf_statistic.h"
#include "lf_properties.h"

namespace lf
{
    class IEdge;
    class IVertex;
    typedef typename std::shared_ptr<IEdge> pEdge;
    typedef typename std::shared_ptr<IVertex> pVertex;

    class IVertex {
    public:
        typedef CPropertyMap::TKey TPropKey;
        typedef CPropertyMap::TValue TPropValue;

        enum TId : uint32_t { VoidId = std::numeric_limits<uint32_t>::max() };
        virtual ~IVertex() {}

        virtual const std::vector<pEdge>& adjacencyList() const = 0;
        virtual const std::vector<pEdge>& inputList() const = 0;
        virtual const std::vector<pEdge>& outputList() const = 0;
        virtual TId id() const = 0;
        virtual const CPropertyMap& props() const = 0;
        virtual CPropertyMap& props() = 0;

        virtual bool addEdge(pEdge edge) = 0;
        virtual void removeEdge(pEdge edge) = 0;

        template<class T>
        T prop_c(const TPropKey& key, const T& defVal = T()) const { return props().value<T>(key, defVal); }
    };

    class IEdge {
    public:
        typedef CPropertyMap::TKey TPropKey;
        typedef CPropertyMap::TValue TPropValue;

        enum eDirectionType
        {
            DIRECTED,
            UNDIRECTED,
            MUTUAL
        };

        typedef double TValue;

        virtual ~IEdge() {}

        virtual pVertex from() const = 0;
        virtual pVertex to() const = 0;
        virtual TValue value(lf::pAgent sa) const = 0;
        virtual const CPropertyMap& props() const = 0;
        virtual CPropertyMap& props() = 0;
        virtual eDirectionType dir() const = 0;
        template<class T>
        T prop_c(const TPropKey& key, const T& defVal = T()) const { return props().value<T>(key, defVal); }
    };

    class IGraph {
    public:
        typedef CPropertyMap::TKey TPropKey;
        typedef CPropertyMap::TValue TPropValue;
        typedef typename std::shared_ptr<IGraph> Ptr;

        virtual ~IGraph() {}

        virtual size_t verticesNum() const = 0;
        virtual size_t edgesNum() const = 0;
        virtual const std::vector<pVertex>& vertices() const = 0;
        virtual const std::vector<pEdge>& edges() const = 0;

        virtual pVertex vertex(const IVertex::TId id) const = 0;
        virtual pStat stat() const = 0;
        virtual std::optional<std::string> checkMap() const = 0;
        virtual const CPropertyMap& props() const = 0;
        virtual CPropertyMap& props() = 0;

        template<class T>
        auto edges_c() const {
            auto cast = [](const lf::pEdge e) { return std::dynamic_pointer_cast<T>(e); };
            auto filter = [](const std::shared_ptr<T> e) { return !!e; };
            return edges() | std::views::transform(cast) | std::views::filter(filter);
        }

        template<class T>
        auto vertices_c() const {
            auto cast = [](const lf::pVertex e) { return std::dynamic_pointer_cast<T>(e); };
            auto filter = [](const std::shared_ptr<T> e) { return !!e; };
            return vertices() | std::views::transform(cast) | std::views::filter(filter);
        }

        template<class T>
        T prop_c(const TPropKey& key, const T& defVal = T()) const { return props().value<T>(key, defVal); }
    };

    typedef IGraph::Ptr pGraph;

    template<class T>
    class IAction {
    public:
        typedef std::shared_ptr<IAction> Ptr;

        IAction(T t0, T t1) : m_t0(t0), m_t1(1) {}
        virtual ~IAction() {}

        constexpr const T& timeBegin() const { return m_t0; }
        constexpr const T& timeEnd() const { return m_t1; }

        virtual pVertex begin() const = 0;
        virtual pVertex end() const = 0;
    protected:
        T m_t0;
        T m_t1;
    };

    template<class T>
    class CStayAction : public IAction<T>
    {
    public:
        typedef std::shared_ptr<CStayAction> Ptr;

        CStayAction(T t0, T t1, pVertex e) : IAction<T>(t0, t1), m_vertex(e) {}

        virtual pVertex begin() const { return m_vertex; }
        virtual pVertex end() const { return m_vertex; }
    protected:
        pVertex m_vertex;
    };

    template<class T>
    class CMoveAction : public IAction<T>
    {
    public:
        typedef std::shared_ptr<CMoveAction> Ptr;

        CMoveAction(T t0, T t1, pEdge e) : IAction<T>(t0, t1), m_edge(e) {}

        virtual pVertex begin() const { return m_edge->from(); }
        virtual pVertex end() const { return m_edge->to(); }
    protected:
        pEdge m_edge;
    };

    template<class T>
    struct IPathFinder {
        typedef typename std::shared_ptr<IPathFinder> Ptr;
        using pAction = IAction<T>::Ptr;

        virtual ~IPathFinder() {}

        virtual std::vector<pAction> operator()(T t0, pVertex start, pVertex end, lf::pAgent sa) = 0;

        //CTime move(pEdge e, CTime from, CTime to);
        //CTime move(pEdge e, CTime from);
        //
        //std::vector<pEdge> operator()() {}
    };
}
