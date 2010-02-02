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
#include "GameObjectElement.hpp"
#include "TextCell.hpp"
#include "GameObjectTypeDocument.hpp"

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetView.hpp>

#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectElementDataMap.hpp>
#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectElementDomainObject.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectTypeView.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElement::GameObjectElementFunctions* GameObjectElement::sm_pNameFunctions = NULL;
GameObjectElement::GameObjectElementFunctions* GameObjectElement::sm_pTypeFunctions = NULL;
GameObjectElement::GameObjectElementFunctions* GameObjectElement::sm_pDefaultValueFunctions = NULL;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElement::GameObjectElement(GameObjectTypeDocument& _document, const std::string& _elementName, I_GameObjectElement* _pParentElement)
:   m_document(_document)
,   m_pParentElement(NULL)
,   m_position(0)
,   m_gameObjectElementId(0)
,   m_elementName(_elementName)
,   m_defaultValueModified(false)
,   m_properties(*this)
{
    if (_pParentElement)
    {
        m_pParentElement = dynamic_cast<GameObjectElement*>(_pParentElement);
    }

    // TODO Make this thread safe
    if (sm_pNameFunctions == NULL)
    {
        sm_pNameFunctions = new GameObjectElementFunctions;
        sm_pNameFunctions->getMutable   = &GameObjectElement::isElementNameMutable;
        sm_pNameFunctions->setTextValue = &GameObjectElement::setElementName;
        sm_pNameFunctions->getTextValue = &GameObjectElement::getElementName;

        sm_pTypeFunctions = new GameObjectElementFunctions;
        sm_pTypeFunctions->getMutable   = &GameObjectElement::isDataTypeMutable;
        sm_pTypeFunctions->setTextValue = &GameObjectElement::setDataType;
        sm_pTypeFunctions->getTextValue = &GameObjectElement::getDataType;

        sm_pDefaultValueFunctions = new GameObjectElementFunctions;
        sm_pDefaultValueFunctions->getMutable   = &GameObjectElement::isDefaultValueMutable;
        sm_pDefaultValueFunctions->setTextValue = &GameObjectElement::setDefaultValue;
        sm_pDefaultValueFunctions->getTextValue = &GameObjectElement::getDefaultValue;
    }

    // TODO use the _elementName parameter to initialize the first TextCell
    m_cells.push_back(new TextCell(this, sm_pNameFunctions));
    m_cells.push_back(new TextCell(this, sm_pTypeFunctions));
    m_cells.push_back(new TextCell(this, sm_pDefaultValueFunctions));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectElement::~GameObjectElement()
{
    // TODO Delete all of the m_cells
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_GameObjectTypeDocument&
GameObjectElement::getDocument() const
{
    return m_document;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TextCell*
GameObjectElement::getElementNameCell()
{
    return m_cells[0];
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TextCell*
GameObjectElement::getDataTypeCell()
{
    return m_cells[1];
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TextCell*
GameObjectElement::getDefaultValueCell()
{
    return m_cells[2];
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameObjectElement::isElementNameMutable() const
{
    // Not if it's a parent element
    if (m_pParentElement)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::setElementName(const std::string& _value)
{
    m_elementName = _value;
    m_document.onElementModified(*this, m_position);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObjectElement::getElementName() const
{
    return m_elementName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameObjectElement::isDataTypeMutable() const
{
    // Not if it's a parent element
    if (m_pParentElement)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::setDataType(const std::string& _type)
{
    m_dataType = _type;
    m_document.onElementModified(*this, m_position);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObjectElement::getDataType() const
{
    return m_dataType;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameObjectElement::isDefaultValueMutable() const
{
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::setDefaultValue(const std::string& _value)
{
    m_defaultValue = _value;
    m_defaultValueModified = true;
    m_document.onElementModified(*this, m_position);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObjectElement::getDefaultValue() const
{
    return m_defaultValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::setElementPosition(int _position)
{
    m_position = _position;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::setGameObjectElementId(boost::uint64_t _gameObjectElementId)
{
    m_gameObjectElementId = _gameObjectElementId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::notifyView(pSpreadSheetView_type _pView, const int _row)
{
    // Notify the view of the field values in this element.

    // TODO only notify the fields that have changed.

    _pView->onCellModified(*getElementNameCell(), 0, _row);
    _pView->onCellModified(*getDataTypeCell(), 1, _row);
    _pView->onCellModified(*getDefaultValueCell(), 2, _row);

    // TODO Handle custom fields
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::notifyView(pGameObjectTypeView_type _pView, const int _row)
{
    // Notify the view of the field values in this element.
    _pView->onElementModified(*this, _row);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::updateParentValues(I_GameObjectElement& _element)
{
    // This method is called when the parent GOT element is modified.

    // Update this element's values, but don't override any values that
    // have been set using the setter's.

    // These two fields aren't overridden by this; the parent value is always used.
    m_elementName = _element.getElementName();
    m_dataType = _element.getDataType();

    // This field may be overridden.
    if (!m_defaultValueModified)
    {
        m_defaultValue = _element.getDefaultValue();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::save(pDatabaseConnection_type _pDbConn)
{
    I_GameObjectElementDataMap::pGameObjectElementDataMap_type
        pElementDM = I_GameObjectElementDataMap::create(_pDbConn);

    I_GameObjectElementDataMap::pGameObjectElementDomainObject_type
        pElementDO = pElementDM->createNew();

    // TODO Only persist the modified elements, but for now I'm persisting
    // all of them.

    // Doing an update if the m_gameObjectElementId is 0 (i.e. the primary
    // key has not been set yet).
    if (m_gameObjectElementId != 0)
    {
        // Set the primary key in order to do an update.
        pElementDO->getGameObjectElementId() = m_gameObjectElementId;
    }
    else
    {
        // If this is a new element, go ahead and create a 
        // new entry in the properties table for it.
        m_properties.insert(_pDbConn);
    }

    pElementDO->getGameObjectTypeId() = m_document.getGameObjectTypeId();

    if (m_pParentElement != NULL)
    {
        pElementDO->getOverrideParent() = 1;

        // Save an empty string if it's the same value as the parent,
        // otherwise save the overriding value.
        if (m_pParentElement->getDefaultValue() == m_defaultValue)
        {
            pElementDO->getDefaultValue() = std::string("");
        }
        else
        {
            pElementDO->getDefaultValue() = m_defaultValue;
        }
    }
    else
    {
        pElementDO->getOverrideParent() = 0;
        pElementDO->getDefaultValue() = m_defaultValue;
    }

    pElementDO->getSequenceNumber() = m_position;
    pElementDO->getName() = m_elementName;
    pElementDO->getDataType() = m_dataType;
    pElementDO->getPropertiesId() = m_properties.getPropertiesId();

    m_gameObjectElementId = pElementDM->update(pElementDO)->getValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectElement::load(pDatabaseConnection_type _pDbConn, I_GameObjectElementDataCollection::pGameObjectElementDomainObject_type _pDomainObject, bool _parent, int _position)
{
    // m_pParentElement should already be set.
    m_position              = _position;
    m_gameObjectElementId   = _pDomainObject->getGameObjectElementId();
    m_elementName           = _pDomainObject->getName().getStringValue();
    m_dataType              = _pDomainObject->getDataType().getStringValue();
    m_defaultValue          = _pDomainObject->getDefaultValue().getStringValue();

    // What about m_defaultValueModified?

    // TODO load the properties?
    m_properties.setPropertiesId(_pDomainObject->getPropertiesId());
    m_properties.load(_pDbConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ElementProperties&
GameObjectElement::getProperties()
{
    return m_properties;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
