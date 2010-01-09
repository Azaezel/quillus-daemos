//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2009 Jason Smith
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
//  Tony Richards trichards@indiezen.org
//  Jason Smith jsmith@airsteampunk.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZMICROPATHER_NAVIGATION_SERVICE_FACTORY_HPP_INCLUDED
#define ZMICROPATHER_NAVIGATION_SERVICE_FACTORY_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Engine/Navigation/I_NavigationServiceFactory.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZMicroPather{
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class NavigationServiceFactory
:   public Zen::Engine::Navigation::I_NavigationServiceFactory
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Navigation::I_NavigationService>  wpNavigationService_type;
    /// @}

    /// @name I_NavigationServiceFactory implementation
    /// @{
public:
    virtual pNavigationService_type create(const std::string& _type);
    /// @}

    /// @name NavigationServiceFactory implementation
    /// @{
public:
    static void destroy(wpNavigationService_type _pNavigationService);
    /// @}

    /// @name Static Methods
    /// @{
public:
    static NavigationServiceFactory& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             NavigationServiceFactory();
    virtual ~NavigationServiceFactory();
    /// @}

};	// interface I_NavigationServiceFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace MicroPather
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZMICROPATHER_NAVIGATION_SERVICE_FACTORY_HPP_INCLUDED
