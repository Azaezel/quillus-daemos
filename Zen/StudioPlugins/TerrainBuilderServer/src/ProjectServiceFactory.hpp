//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Terrain Builder
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef TERRAINBUILDER_PROJECT_SERVICE_FACTORY_HPP_INCLUDED
#define TERRAINBUILDER_PROJECT_SERVICE_FACTORY_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ProjectServiceFactory.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ProjectServiceFactory
:   public Zen::Studio::Workbench::I_ProjectServiceFactory
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_ProjectService> wpProjectService_type;
    /// @}

    /// @name I_ProjectServiceFactory implementation
    /// @{
public:
    virtual pProjectService_type create();
    /// @}

    /// @name ProjectServiceFactory implementation
    /// @{
public:
    /// @}

    /// @name Static Methods
    /// @{
public:
    static void destroy(wpProjectService_type _wpService);

    static ProjectServiceFactory& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             ProjectServiceFactory();
    virtual ~ProjectServiceFactory();
    /// @}

};  // class ProjectServiceFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAINBUILDER_PROJECT_SERVICE_FACTORY_HPP_INCLUDED
