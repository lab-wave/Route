#pragma once

#include "lf_igraph.h"
#include "lf_properties.h"
#include <vector>

namespace lf {
    template<class T, class C>
    auto cast(C& conteiner) {
        auto fcast = [](const auto e) { return std::dynamic_pointer_cast<T>(e); };
        auto filter = [](const std::shared_ptr<T> e) { return !!e; };
        return conteiner | std::views::transform(fcast) | std::views::filter(filter);
    }

    class Vertex;
    class Edge : public lf::IEdge {
    public:
        typedef std::weak_ptr<lf::IVertex> pwVertex;

        Edge(pwVertex f, pwVertex t, TValue val = 0, eDirectionType _dir = IEdge::MUTUAL) : m_from(f), m_to(t), m_val(val), m_dir(_dir) {}

        virtual lf::pVertex from() const { return m_from.lock(); }
        virtual lf::pVertex to() const { return m_to.lock(); }
        virtual TValue value(lf::pAgent sa) const override { return m_val; }
        virtual eDirectionType dir() const { return m_dir; }
        virtual const CPropertyMap& props() const { return m_props; }
        virtual CPropertyMap& props() { return m_props; }
        virtual void setProp(const QByteArray& key, const QVariant& val) { if (val.isValid()) m_props[key] = val; else m_props.remove(key); }
    protected:
        pwVertex m_from;
        pwVertex m_to;
        TValue m_val;
        eDirectionType m_dir;
        CPropertyMap m_props;
    };

    class Vertex : public IVertex {
    public:
        using IVertex::TId;

        Vertex(TId _id) : m_id(_id) {}

        virtual const std::vector<pEdge>& adjacencyList() const { return m_adjacencyList; }
        virtual const std::vector<pEdge>& inputList() const { return m_inputList; }
        virtual const std::vector<pEdge>& outputList() const { return m_outputList; }

        template<class T>
        auto adjacencyList_c() const {
            auto cast = [](const lf::pEdge e) { return std::dynamic_pointer_cast<T>(e); };
            auto filter = [](const std::shared_ptr<T> e) { return !!e; };
            return adjacencyList() | std::views::transform(cast) | std::views::filter(filter);
        }

        template<class T>
        auto inputList_c() const {
            auto cast = [](const lf::pEdge e) { return std::dynamic_pointer_cast<T>(e); };
            auto filter = [](const std::shared_ptr<T> e) { return !!e; };
            return inputList() | std::views::transform(cast) | std::views::filter(filter);
        }

        template<class T>
        auto outputList_c() const {
            auto cast = [](const lf::pEdge e) { return std::dynamic_pointer_cast<T>(e); };
            auto filter = [](const std::shared_ptr<T> e) { return !!e; };
            return outputList() | std::views::transform(cast) | std::views::filter(filter);
        }

        virtual bool addEdge(pEdge edge);
        virtual void removeEdge(pEdge edge);
        virtual TId id() const { return m_id; }
        virtual const CPropertyMap& props() const { return m_props; }
        virtual CPropertyMap& props() { return m_props; }
        virtual void setProp(const QByteArray& key, const QVariant& val) { if (val.isValid()) m_props[key] = val; else m_props.remove(key); }
    protected:
        TId m_id;
        std::vector<pEdge> m_adjacencyList;
        std::vector<pEdge> m_inputList;
        std::vector<pEdge> m_outputList;
        CPropertyMap m_props;
    };

    class Graph : public lf::IGraph {
    public:
        Graph() {}

        virtual size_t verticesNum() const { return m_vertices.size(); }
        virtual size_t edgesNum() const { return m_edges.size(); }

        virtual lf::pVertex vertex(const lf::IVertex::TId id) const { return m_vertices[id]; }
        const std::vector<pVertex>& vertices() const { return m_vertices; }
        const std::vector<pEdge>& edges() const { return m_edges; }
        virtual pStat stat() const override { return m_stat; }
        virtual const CPropertyMap& props() const { return m_props; }
        virtual CPropertyMap& props() { return m_props; }
        virtual void setProp(const QByteArray& key, const QVariant& val) { if (val.isValid()) m_props[key] = val; else m_props.remove(key); }

        template<class T, class... _Types>
        std::shared_ptr<T> createVertex(_Types&&... _Args) {
            size_t s = m_vertices.size();
            auto res = std::make_shared<T>(static_cast<Vertex::TId>(s), std::forward<_Types>(_Args)...);
            m_vertices.push_back(res);
            return res;
        }
        template<class T, class... _Types>
        std::shared_ptr<T> createEdge(pVertex _f, pVertex _t, _Types&&... _Args) {
            auto f = std::dynamic_pointer_cast<Vertex>(_f);
            auto t = std::dynamic_pointer_cast<Vertex>(_t);

            bool added = false;
            auto res = std::make_shared<T>(f, t, std::forward<_Types>(_Args)...);
            if (f) { added = f->addEdge(res); }
            if (t) { added = t->addEdge(res) || added; }
            if (added) m_edges.push_back(res);
            return res;
        }

        virtual lf::pVertex createVertex(const lf::pVertex& v, pErrors err = pErrors()) { return createVertex<Vertex>(); }
        virtual lf::pEdge createEdge(lf::Vertex::TId fromId, lf::Vertex::TId toId, const lf::pEdge& v, pErrors err = pErrors()) {
            if (fromId != Vertex::VoidId && toId != Vertex::VoidId && fromId < m_vertices.size() && toId < m_vertices.size())
                return createEdge<Edge>(lf::Graph::vertex(fromId), lf::Graph::vertex(toId));
            if (fromId == Vertex::VoidId || fromId >= m_vertices.size())
                err->push_back(CError{ CError::E_ERROR, QString("Edge from vertex with id \"%1\" not found").arg(fromId) });
            if (toId == Vertex::VoidId || toId >= m_vertices.size())
                err->push_back(CError{ CError::E_ERROR, QString("Edge to vertex with id \"%1\" not found").arg(toId) });

            return lf::pEdge();
        }

        template<class T>
        static std::shared_ptr<T> createGraph(const lf::pGraph src, pErrors err = pErrors()) {
            auto result = std::make_shared<T>();

            std::vector<Vertex::TId> oldToNew(src->verticesNum(), Vertex::VoidId);

            for (auto& srcv : src->vertices()) {
                if (!srcv)
                    continue;

                auto vert = result->createVertex(srcv, err);
                if (err->m_type >= CError::E_ERROR)
                    return std::shared_ptr<T>();

                if (vert) {
                    oldToNew[srcv->id()] = vert->id();
                }
            }

            for (auto& srce : src->edges()) {
                if (!srce)
                    continue;

                auto fromId = oldToNew[srce->from()->id()];
                auto toId = oldToNew[srce->to()->id()];

                auto props = srce->props();
                auto edge = result->createEdge(fromId, toId, srce, err);

                if (err->m_type >= CError::E_ERROR)
                    return std::shared_ptr<T>();
            }

            result->m_props = src->props();
            return result;
        }

        void removeVertex(pVertex v);

        std::optional<std::string> checkMap() const override { return std::optional<std::string>(); }
    protected:
        std::vector<pEdge> m_edges;
        std::vector<pVertex> m_vertices;
        CPropertyMap m_props;
        pStat m_stat;
    };
};
