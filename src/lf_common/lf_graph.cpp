#include "lf_graph.h"

bool lf::Vertex::addEdge(pEdge edge)
{
    m_adjacencyList.push_back(edge);

    auto e = std::dynamic_pointer_cast<Edge>(edge);
    if (e) {
        if (e->dir() == IEdge::MUTUAL || (e->dir() == IEdge::DIRECTED && e->from()->id() == id()))
            m_outputList.push_back(e);
        if (e->dir() == IEdge::MUTUAL || (e->dir() == IEdge::DIRECTED && e->to()->id() == id()))
            m_inputList.push_back(e);
    }
    else {
        m_outputList.push_back(e);
        m_inputList.push_back(e);
    }
    return true;
}

void lf::Vertex::removeEdge(pEdge edge)
{
    //            m_adjacencyList.erase(std::remove(m_adjacencyList.begin(), m_adjacencyList.end()), m_adjacencyList.end());
    //            m_inputList.erase(std::remove(m_inputList.begin(), m_inputList.end()), m_inputList.end());
    //            m_outputList.erase(std::remove(m_outputList.begin(), m_outputList.end()), m_outputList.end());
    std::erase(m_adjacencyList, edge);
    std::erase(m_inputList, edge);
    std::erase(m_outputList, edge);
}

void lf::Graph::removeVertex(pVertex v)
{
    m_edges.erase(std::remove_if(m_edges.begin(), m_edges.end(), [v](pEdge e) {
        if (e->from()->id() == v->id()) {
            e->to()->removeEdge(e);
            return true;
        }

        if (e->to()->id() == v->id()) {
            e->from()->removeEdge(e);
            return true;
        }

        return false;
        }), m_edges.end());
    m_vertices[v->id()].reset();
}
