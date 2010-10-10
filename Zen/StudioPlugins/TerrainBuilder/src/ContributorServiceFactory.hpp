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
#ifndef TERRAINBUILDER_CONTRIBUTOR_SERVICE_FACTORY_HPP_INCLUDED
#define TERRAINBUILDER_CONTRIBUTOR_SERVICE_FACTORY_HPP_INCLUDED

#include <Zen/Studio/Workbench/I_ContributorServiceFactory.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ContributorServiceFactory
:   public Zen::Studio::Workbench::I_ContributorServiceFactory
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_ContributorService> wpContributorService_type;
    /// @}

    /// @name I_ContributorServiceFactory implementation
    /// @{
public:
    virtual pContributorService_type create(const std::string& _type, Zen::Studio::Workbench::I_Workbench& _workbench);
    /// @}

    /// @name ContributorServiceFactory implementation
    /// @{
public:
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroy(wpContributorService_type _wpService);
    static ContributorServiceFactory& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             ContributorServiceFactory();
    virtual ~ContributorServiceFactory();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class ContributorServiceFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAINBUILDER_CONTRIBUTOR_SERVICE_FACTORY_HPP_INCLUDED
