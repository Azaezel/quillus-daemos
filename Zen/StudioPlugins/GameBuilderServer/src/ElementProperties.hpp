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
#ifndef GAME_BUILDER_ELEMENT_PROPERTIES_HPP_INCLUDED
#define GAME_BUILDER_ELEMENT_PROPERTIES_HPP_INCLUDED

#include "Properties.hpp"

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCellProperties.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameObjectElement;

/// Game Object Type Element Properties.
class ElementProperties
:   public Properties
,   public Zen::Studio::Workbench::I_SpreadSheetCellProperties
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
    friend class GameObjectElement;
             ElementProperties(GameObjectElement& _element);
    virtual ~ElementProperties();
    /// @}

    /// @name Member Variables
    /// @{
private:
    GameObjectElement&              m_element;

    PersistenceProperty             m_persistence;
    ReplicationPropertyGroup        m_replication;
    ExtensionPointProperty          m_factory;
    /// @}

};  // class ElementProperties

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // GAME_BUILDER_ELEMENT_PROPERTIES_HPP_INCLUDED
