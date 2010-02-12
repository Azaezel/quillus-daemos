//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
//
// Copyright (C) 2001 - 2008 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMEBUILDER_PROJECT_SERVICE_FACTORY_HPP_INCLUDED
#define GAMEBUILDER_PROJECT_SERVICE_FACTORY_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

//#include <Zen/Studio/Workbench/I_Workbench.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ProjectServiceFactory.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

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
private:
    void destroy(wpProjectService_type _pService);
    /// @}

    /// @name Static Methods
    /// @{
public:
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
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif  // GAMEBUILDER_PROJECT_SERVICE_FACTORY_HPP_INCLUDED
