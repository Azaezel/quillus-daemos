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
#ifndef ZEN_ENGINE_CINEMATICS_I_CINEMATIC_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_CINEMATICS_I_CINEMATIC_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Cinematics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Choreographer;
class I_CinematicServiceFactory;

class CINEMATICS_DLL_LINK I_CinematicService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                         index_type;
    typedef I_CinematicServiceFactory                           factory_type;

    typedef Zen::Memory::managed_ptr<I_Choreographer>         pChoreographer_type;

    typedef Zen::Memory::managed_ptr<I_CinematicService>      pCinematicService_type;
    typedef Zen::Memory::managed_weak_ptr<I_CinematicService> wpCinematicService_type;

    typedef Zen::Event::Event<wpCinematicService_type>       ServiceEvent_type;
    /// @}

    /// @name I_CinematicService interface
    /// @{
public:
    virtual pChoreographer_type createCoreographer() = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    /// Fired immediately before this object is destroyed.
    /// The payload is about to be destroyed, so do not keep a reference of it.
    ServiceEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_CinematicService();
    virtual ~I_CinematicService();
    /// @}

};	// interface I_CinematicService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Cinematics
}   // namespace Engine
namespace Memory 
{
    /// I_CinematicService is managed by a factory
    template<>
	struct is_managed_by_factory<Zen::Engine::Cinematics::I_CinematicService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CINEMATICS_I_CINEMATIC_SERVICE_HPP_INCLUDED
