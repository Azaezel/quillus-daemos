//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAME_BUILDER_DOCUMENT_PROPERTIES_HPP_INCLUDED
#define GAME_BUILDER_DOCUMENT_PROPERTIES_HPP_INCLUDED

#include "Properties.hpp"

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetProperties.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameObjectType;

/// Game Object Type Properties.
class DocumentProperties
:   public Properties
,   public Zen::Studio::Workbench::I_SpreadSheetProperties
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_PropertiesPublisher implementation
    /// @{
public:
    virtual const std::string& getPropertiesName() const;
    virtual pSubscription_type subscribe(pPropertiesListener_type _pListener) { return Properties::subscribe(_pListener); }
    virtual Zen::Studio::Workbench::I_PropertiesController& getController();
    virtual const Zen::Studio::Workbench::I_Property* getPropertyByName(const std::string& _name) const;
    /// @}

    /// @name I_SpreadSheetProperties implementation
    /// @{
public:
    virtual bool cellPropertiesChangeEachRow() const;
    virtual bool cellPropertiesChangeEachColumn() const;
    virtual const Zen::Studio::Workbench::I_Property* getPropertyByFullName(const std::string& _fullName);
    /// @}

    /// @name Properties overrides
    /// @{
protected:
    virtual void notifyListener(pPropertiesListener_type _pListener);
    virtual void loadProperty(pPropertyDomainObject_type _pDomainObject);
    virtual void saveChildren(pDatabaseConnection_type _pDBConn);
    virtual Zen::Studio::Workbench::I_Project& getProject();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class GameObjectType;
             DocumentProperties(GameObjectType& _document);
    virtual ~DocumentProperties();
    /// @}

    /// @name Member Variables
    /// @{
private:
    GameObjectType&                 m_document;

    PersistenceProperty             m_persistence;
    ReplicationPropertyGroup        m_replication;
    ExtensionPointProperty          m_factory;
    CreatorPropertyGroup            m_creator;
    /// @}

};  // class DocumentProperties

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // GAME_BUILDER_DOCUMENT_PROPERTIES_HPP_INCLUDED
