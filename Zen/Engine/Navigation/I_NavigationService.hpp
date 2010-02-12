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
#ifndef ZEN_ENGINE_NAVIGATION_I_NAVIGATION_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_NAVIGATION_I_NAVIGATION_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Navigation {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_NavigationServiceFactory;
class I_Graph;
class I_NodeComparator;

class NAVIGATION_DLL_LINK I_NavigationService
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::string                                         index_type;
    typedef I_NavigationServiceFactory                          factory_type;

    typedef Memory::managed_ptr<I_Graph>                        pGraph_type;
    typedef Memory::managed_weak_ptr<I_Graph>                   wpGraph_type;
    typedef Memory::managed_ptr<I_NodeComparator>               pNodeComparator_type;

    typedef Zen::Memory::managed_ptr<I_NavigationService>       pService_type;
    typedef Zen::Memory::managed_weak_ptr<I_NavigationService>  wpService_type;
    typedef Zen::Event::Event<wpService_type>                   ServiceEvent_type;
    /// @}

    /// @name I_NavigationService interface
    /// @{
public:
    /// @param _nodeComparator The object that compares two nodes.  This is 
    ///     generally a flyweight object that the returned graph will use to 
    ///     compare two nodes.  The graph retains a reference to the comparator,
    ///     so be sure the lifetime of the comparator is >= the lifetime of the
    ///     navigation service.  Normally you would do this by making the 
    ///     comparator retain a managed_ptr reference to the I_Graph.
    ///
    virtual pGraph_type createNavigationGraph(I_NodeComparator& _nodeComparator) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// Fired immediately before this object is destroyed.
    /// The payload is about to be destroyed, so do not keep a reference of it around.
    ServiceEvent_type onDestroyEvent;
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_NavigationService();
    virtual ~I_NavigationService();
    /// @}

};  // interface I_NavigationService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Navigation
}   // namespace Engine
namespace Memory 
{
    /// I_NavigationService is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Navigation::I_NavigationService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_NAVIGATION_I_NAVIGATION_SERVICE_HPP_INCLUDED
