//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_NAVIGATION_I_GRAPH_HPP_INCLUDED
#define ZEN_ENGINE_NAVIGATION_I_GRAPH_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Math/Math.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Navigation {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Node;
class I_Path;

/// Basic navigation graph interface.
class NAVIGATION_DLL_LINK I_Graph
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    struct I_NodeVisitor;

    typedef I_Node*                                             pNode_type;
    typedef Memory::managed_ptr<I_Path>                         pPath_type;
    /// @}

    /// @name I_Graph interface
    /// @{
public:
    /// Attach two nodes that indicates a legal path between _pStart to _pEnd.
    /// I_NodeComparator is used to compute the cost for this traversal.
    /// @see I_NodeComparator is used to compute the cost for this traversal.
    virtual void attachNodes(pNode_type _pStart, pNode_type _pEnd) = 0;

    /// Detach two previously attached nodes.
    virtual void detatchNodes(pNode_type _pStart, pNode_type _pEnd) = 0;

    /// Detach all nodes that join to and from this node.
    virtual void detachNodes(pNode_type _pNode) = 0;

    /// Get the next node along the path of _pStart to _pEnd.
    /// @param _pCost - Optional output of the cost to traverse from _pStart to _pEnd
    virtual pNode_type getNextNode(pNode_type _pStart, pNode_type _pEnd, Math::Real* _pCost = NULL) = 0;

    /// Get the path starting from _pStart ending at _pEnd.
    virtual pPath_type getPath(pNode_type _pStart, pNode_type _pEnd) = 0;

    /// Get the path starting from _pStart ending at _pEnd.
    /// This version of the method calls the visitor for each node in the path starting with
    /// the first 
    virtual void getPath(pNode_type _pStart, pNode_type _pEnd, I_NodeVisitor& _visitor) = 0;

    /// Clear the result cache.  Any time the relationship between two nodes change,
    /// be sure to call this method, otherwise incorrect paths may be generated.
    virtual void clearCache() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Graph();
    virtual ~I_Graph();
    /// @}

    /// @name Inner types
    /// @{
public:
    struct I_NodeVisitor
    {
        virtual void begin() = 0;
        /// @param _cost Cost of traversing to this node.
        /// @return true to continue visiting, false to stop visiting.
        virtual bool visit(pNode_type _pNode) = 0;
        virtual void end() = 0;
    };
    /// @}

};  // interface I_Graph

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Navigation
}   // namespace Engine
namespace Memory 
{
    /// I_Graph is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Navigation::I_Graph> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_NAVIGATION_I_GRAPH_HPP_INCLUDED
