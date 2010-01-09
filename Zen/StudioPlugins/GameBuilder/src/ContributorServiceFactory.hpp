//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMEBUILDER_CONTRIBUTOR_SERVICE_FACTORY_HPP_INCLUDED
#define GAMEBUILDER_CONTRIBUTOR_SERVICE_FACTORY_HPP_INCLUDED

#include <Zen/Studio/Workbench/I_ContributorServiceFactory.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class ContributorServiceFactory
:   public Zen::Studio::Workbench::I_ContributorServiceFactory
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Studio::Workbench::I_ContributorService>   wpContributorService_type;
    /// @}

    /// @name I_ContributorServiceFactory implementation
    /// @{
public:
    virtual pContributorService_type create(const std::string& _type, Zen::Studio::Workbench::I_Workbench& _workbench);
    /// @}

    /// @name ContributorServiceFactory implementation
    /// @{
public:
    static void destroy(wpContributorService_type _pService);
    /// @}

    /// @name Static methods
    /// @{
public:
    static ContributorServiceFactory& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             ContributorServiceFactory();
    virtual ~ContributorServiceFactory();
    /// @}

};  // class ContributorServiceFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMEBUILDER_CONTRIBUTOR_SERVICE_FACTORY_HPP_INCLUDED
