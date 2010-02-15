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
#ifndef GAMEBUILDER_GAME_OBJECT_ELEMENT_HPP_INCLUDED
#define GAMEBUILDER_GAME_OBJECT_ELEMENT_HPP_INCLUDED

#include "ElementProperties.hpp"

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectElement.hpp>

#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectElementDataCollection.hpp>

#include <boost/cstdint.hpp>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_SpreadSheetView;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class TextCell;
class I_GameObjectTypeView;
class GameObjectTypeDocument;

class GameObjectElement
:   public I_GameObjectElement
{
    /// @name Types
    /// @{
public:
    typedef std::vector<TextCell*>                      Cells_type;
    typedef Zen::Studio::Workbench::I_SpreadSheetView*  pSpreadSheetView_type;
    typedef I_GameObjectTypeView*                       pGameObjectTypeView_type;
    typedef Zen::Database::I_DatabaseService::pDatabaseConnection_type  pDatabaseConnection_type;
    /// @}

    /// @name I_GameObjectElement implementation
    /// @{
public:
    virtual I_GameObjectTypeDocument& getDocument() const;

    virtual bool isElementNameMutable() const;
    virtual void setElementName(const std::string& _value);
    virtual const std::string& getElementName() const;

    virtual bool isDataTypeMutable() const;
    virtual void setDataType(const std::string& _type);
    virtual const std::string& getDataType() const;

    virtual bool isDefaultValueMutable() const;
    virtual void setDefaultValue(const std::string& _value);
    virtual const std::string& getDefaultValue() const;
    /// @}

    /// @name GameObjectElement implementation
    /// @{
public:
    TextCell* getElementNameCell();
    TextCell* getDataTypeCell();
    TextCell* getDefaultValueCell();

    void setElementPosition(int _position);
    void setGameObjectElementId(boost::uint64_t _gameObjectElementId);

    /// Notify the view of the values in this element.
    /// Called by GameObjectTypeDocument::notifyView() during initial
    /// view subscription.
    /// @see GameObjectTypeDocument::notifyView()
    void notifyView(pSpreadSheetView_type _pView, const int _row);

    /// Notify the view of the values in this element.
    /// Called by GameObjectTypeDocument::notifyView() during initial
    /// view subscription.
    /// @see GameObjectTypeDocument::notifyView()
    void notifyView(pGameObjectTypeView_type _pView, const int _row);

    /// Called when the parent element is updated.
    void updateParentValues(I_GameObjectElement& _element);

    /// Save this element if it has been modified.
    void save(pDatabaseConnection_type _pDbConn);

    // TODO implement.
    /// Called from GameObjectTypeDocument::addElement().
    /// @see GameObjectTypeDocument::addElement()
    void load(pDatabaseConnection_type _pDbConn, I_GameObjectElementDataCollection::pGameObjectElementDomainObject_type _pDomainObject, bool _parent, int _position);

    /// Get the properties associated with this element.
    ElementProperties& getProperties();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class GameObjectTypeDocument;
    explicit GameObjectElement(GameObjectTypeDocument& _document, const std::string& _elementName, I_GameObjectElement* _pParentElement = NULL);
    virtual ~GameObjectElement();
    /// @}

    /// @name Inner Types
    /// @{
public:
    struct GameObjectElementFunctions
    {
        typedef bool(GameObjectElement::*GetMutable_type)() const;
        typedef void(GameObjectElement::*SetString_type)(const std::string&);
        typedef const std::string&(GameObjectElement::*GetString_type)() const;

        GetMutable_type getMutable;
        SetString_type  setTextValue;
        GetString_type  getTextValue;
    };

    /// Public list of functions used by TextCell and TextCellData
    static GameObjectElementFunctions*              sm_pNameFunctions;
    static GameObjectElementFunctions*              sm_pTypeFunctions;
    static GameObjectElementFunctions*              sm_pDefaultValueFunctions;
    /// @}

    /// @name Member Variables
    /// @{
private:
    GameObjectTypeDocument&                         m_document;

    /// Parent element, or NULL if this element doesn't have a
    /// parent element.
    GameObjectElement*                              m_pParentElement;
    int                                             m_position;

    /// Cell representation of the fields in the element.
    Cells_type                                      m_cells;

    /// Data model primary key, or 0 if this element has not been
    /// persisted yet.
    boost::uint64_t                                 m_gameObjectElementId;
    std::string                                     m_elementName;
    std::string                                     m_dataType;
    std::string                                     m_defaultValue;

    /// What does this do?  Is it needed anymore?
    bool                                            m_defaultValueModified;
    ElementProperties                               m_properties;
    /// @}

};  // class GameObjectElement

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // GAMEBUILDER_GAME_OBJECT_ELEMENT_HPP_INCLUDED
