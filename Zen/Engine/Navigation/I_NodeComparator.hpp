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
#ifndef ZEN_ENGINE_NAVIGATION_I_NODE_COMPARATOR_HPP_INCLUDED
#define ZEN_ENGINE_NAVIGATION_I_NODE_COMPARATOR_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Math/Math.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Navigation {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Node;

/// I_Node comparator.
///
/// You must implement one of these to compare your custom I_Node implementation.
/// A simple implementation would compute the distance between _start and _stop
/// in the compare method and return the results.  You may also want to add
/// other factors into the equation based on terrain difficulty, danger, etc.
/// depending on the needs of your game.
///
/// An I_Comparator must always return the same value between _start and _stop
/// because this value might be cached by I_Graph.  If something changes that
/// would result in a different value to be returned by compare, you must call
/// I_Graph::clearCache() to ensure subsequent path queries will return valid
/// results.
///
/// @see I_Node
/// @see I_Graph
class NAVIGATION_DLL_LINK I_NodeComparator
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_NodeComparator interface
    /// @{
public:
    /// Compare two nodes and return the weight that it costs to traverse from _start
    /// to _end.
    virtual Math::Real compare(const I_Node& _start, const I_Node& _end) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_NodeComparator();
    virtual ~I_NodeComparator();
    /// @}

};  // interface I_NodeComparator

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Navigation
}   // namespace Engine
namespace Memory 
{
    /// I_NodeComparator is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Navigation::I_NodeComparator> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_NAVIGATION_I_NODE_COMPARATOR_HPP_INCLUDED
