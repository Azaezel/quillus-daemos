//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZMICROPATHER_GRAPH_HPP_INCLUDED
#define ZMICROPATHER_GRAPH_HPP_INCLUDED

#include "micropather.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Engine/Navigation/I_Graph.hpp>

#include <Zen/Engine/Navigation/I_NodeComparator.hpp>

#include <boost/bimap/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZMicroPather {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Graph
:   public Zen::Engine::Navigation::I_Graph
,   public micropather::Graph
{
    /// @name Types
    /// @{
public:
    struct from {};
    struct to {};

    typedef boost::bimaps::bimap<
        boost::bimaps::multiset_of< boost::bimaps::tags::tagged<pNode_type, from> >,
        boost::bimaps::multiset_of< boost::bimaps::tags::tagged<pNode_type, to> >
    > Nodes_type;

    typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Navigation::I_Path>  wpPath_type;
    /// }

    /// @name I_Graph implementation
    /// @{
public:
    virtual void attachNodes(pNode_type _pStart, pNode_type _pEnd);
    virtual void detatchNodes(pNode_type _pStart, pNode_type _pEnd);
    virtual void detachNodes(pNode_type _pNode);
    virtual pNode_type getNextNode(pNode_type _pStart, pNode_type _pEnd, Zen::Math::Real* _pCost = NULL);
    virtual pPath_type getPath(pNode_type _pStart, pNode_type _pEnd);
    virtual void getPath(pNode_type _pStart, pNode_type _pEnd, Zen::Engine::Navigation::I_Graph::I_NodeVisitor& _visitor);
    virtual void clearCache();
    /// @}

    /// @name micropather::Graph implementation
    /// @{
public:
    virtual float LeastCostEstimate(void* _pStart, void* _pEnd);
	virtual void AdjacentCost(void* _pNode, std::vector<micropather::StateCost>* _pAdjacent);
	virtual void  PrintStateInfo(void* _pNode);
    /// @}

    /// @name Graph implementation
    /// @{
public:
    template<typename NodeSide_type>
    void detachNodes(pNode_type _pNode, NodeSide_type);

    static void destroy(wpPath_type _pPath);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class NavigationService;
             Graph(Zen::Engine::Navigation::I_NodeComparator& _nodeComparator);
    virtual ~Graph();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Engine::Navigation::I_NodeComparator&          m_nodeComparator;
    micropather::MicroPather*                           m_pPather;

    Nodes_type                                          m_nodes;
    /// @}

};  // class Graph

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename NodeSide_type>
inline
void
Graph::detachNodes(pNode_type _pNode, NodeSide_type)
{
    m_nodes.by<NodeSide_type>().erase(
        m_nodes.by<NodeSide_type>().lower_bound(_pNode),
        m_nodes.by<NodeSide_type>().upper_bound(_pNode)
    );
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMicroPather
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZMICROPATHER_GRAPH_HPP_INCLUDED
