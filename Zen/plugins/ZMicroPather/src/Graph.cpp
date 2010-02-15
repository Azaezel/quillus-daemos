//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2009 Jason Smith
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//  Jason Smith jsmith@airsteampunk.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "Graph.hpp"
#include "Path.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Engine/Navigation/I_Node.hpp>
#include <Zen/Engine/Navigation/I_NodeIterator.hpp>

#include <algorithm>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZMicroPather {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Graph::Graph(Zen::Engine::Navigation::I_NodeComparator& _nodeComparator)
:   m_nodeComparator(_nodeComparator)
,   m_pPather(NULL)
{
    m_pPather = new micropather::MicroPather(this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Graph::~Graph()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Graph::attachNodes(pNode_type _pStart, pNode_type _pEnd)
{
    m_nodes.insert(Nodes_type::value_type(_pStart, _pEnd));

    clearCache();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Graph::detatchNodes(pNode_type _pStart, pNode_type _pEnd)
{
#if 0   // Graceful implementation but it doesn't compile o.O
    // This is an algorithm structure
    struct ShouldRemove
    {
        ShouldRemove(pNode_type _pStart, pNode_type _pEnd)
        :   m_pStart(_pStart)
        ,   m_pEnd(_pEnd)
        {
        }

        // Logic operator() that returns true when the item should
        // be removed from m_nodes.
#if 0
        bool operator()(Nodes_type::relation::left_pair _pair)
#else
        typedef boost::bimaps::relation::structured_pair<pNode_type, pNode_type, 
            boost::bimaps::relation::normal_layout
        > pair_type;
        bool operator()(pair_type _pair)
#endif
        {
            // Double-check to make sure I'm not crazy :P
            assert(_pair.get<from>() == m_pStart);

            // Return true if the second is m_pEnd.
            return (_pair.get<to>() == m_pEnd);
        }

        pNode_type m_pStart;
        pNode_type m_pEnd;
    };

    // Remove the nodes where "from" is _pStart and "to" is _pEnd.
    // Remove from a range of the lower_bound and upper_bound
    // of the multimap that _pStart is in the "from" collection.
    m_nodes.by<from>().erase(
        std::remove_if(
            m_nodes.by<from>().lower_bound(_pStart),
            m_nodes.by<from>().upper_bound(_pStart),
            ShouldRemove(_pStart, _pEnd)
        ),
        m_nodes.by<from>().end()
    );
#else
    // Hackish implementation... I really liked the other one better.
    const Nodes_type::map_by<from>::iterator start = 
        m_nodes.by<from>().lower_bound(_pStart);
    const Nodes_type::map_by<from>::iterator end = 
        m_nodes.by<from>().lower_bound(_pStart);

    bool found = true;
    while(found)
    {
        found = false;
        for(Nodes_type::map_by<from>::iterator iter = start; iter != end; iter++)
        {
            if (iter->get<to>() == _pEnd)
            {
                m_nodes.by<from>().erase(iter);
                found = true;
                break;
            }
        }
    }
#endif

    clearCache();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Graph::detachNodes(pNode_type _pNode)
{
    detachNodes(_pNode, from());
    detachNodes(_pNode, to());

    clearCache();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Graph::pNode_type
Graph::getNextNode(pNode_type _pStart, pNode_type _pEnd, Zen::Math::Real* _pCost)
{
    std::vector<void*> path;
    float totalCost;

    int result = m_pPather->Solve(_pStart, _pEnd, &path, &totalCost);

    if (_pCost != NULL)
    {
        *_pCost = totalCost;
    }

    if (path.size() < 2)
    {
        return NULL;
    }
    else
    {
        return (pNode_type)path[1];
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Graph::pPath_type
Graph::getPath(pNode_type _pStart, pNode_type _pEnd)
{
    Path* pRawPath = new Path;
    pPath_type pPath(pRawPath, &Graph::destroy);

    m_pPather->Solve(_pStart, _pEnd, pRawPath->getVectorReference(), pRawPath->getTotalCostReference());

    return pPath;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Graph::getPath(pNode_type _pStart, pNode_type _pEnd, Zen::Engine::Navigation::I_Graph::I_NodeVisitor& _visitor)
{
    Path path;
    m_pPather->Solve(_pStart, _pEnd, path.getVectorReference(), path.getTotalCostReference());

    Zen::Engine::Navigation::I_Path::pNodeIterator_type iter = path.front();

    _visitor.begin();
    while(iter->hasNext())
    {
        _visitor.visit(iter->next());
    }
    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Graph::clearCache()
{
    m_pPather->Reset();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
float
Graph::LeastCostEstimate(void* _pStart, void* _pEnd)
{
    // Find the cost between _pStart and _pEnd using the comparator.
    return m_nodeComparator.compare(
        *(Zen::Engine::Navigation::I_Node*)_pStart, 
        *(Zen::Engine::Navigation::I_Node*)_pEnd
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Graph::AdjacentCost(void* _pNode, std::vector<micropather::StateCost>* _pAdjacent )
{
    const Nodes_type::map_by<from>::iterator start = 
        m_nodes.by<from>().lower_bound((Zen::Engine::Navigation::I_Node*)_pNode);

    const Nodes_type::map_by<from>::iterator end = 
        m_nodes.by<from>().upper_bound((Zen::Engine::Navigation::I_Node*)_pNode);


    // Search through the multi-map for this node from the lower-bound (start) 
    // to the upper-bound (end) and push a StateCost for each pair.
    for(Nodes_type::map_by<from>::iterator iter = start; iter != end; iter++)
    {
        micropather::StateCost nodeCost = {
            iter->get<to>(), 
            m_nodeComparator.compare(
                *(Zen::Engine::Navigation::I_Node*)_pNode, 
                *(Zen::Engine::Navigation::I_Node*)iter->get<to>()
            )
        };
        _pAdjacent->push_back(nodeCost);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Graph::PrintStateInfo(void* _pNode)
{
    // Ignore.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Graph::destroy(wpPath_type _pPath)
{
    Path* pPath = dynamic_cast<Path*>(_pPath.get());

    if( pPath != NULL )
    {
        delete pPath;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Graph::destroy() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMicroPather
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
