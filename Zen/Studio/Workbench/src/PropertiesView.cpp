//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
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
#include "PropertiesView.hpp"
#include "PropertiesHandle.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/wxUtilities.hpp>

#include <Zen/Studio/WorkbenchCommon/I_PropertiesPublisher.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_EnumProperty.hpp>

/// Requires wxWidgets 2.9.x
#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/props.h>
#include <wx/propgrid/property.h>

#include <boost/bind.hpp>

#include <sstream>

BEGIN_EVENT_TABLE(Zen::Studio::Workbench::PropertiesView, wxPanel)
    EVT_PG_CHANGING(ID_WXPROPERTYGRID1, PropertiesView::onPropertyGridChanging)
    EVT_PG_CHANGED(ID_WXPROPERTYGRID1, PropertiesView::onPropertyGridChanged)
END_EVENT_TABLE()

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PropertiesView::PropertiesView(wxWindow* _pParent, Workbench* _pWorkbench)
:   wxPanel(_pParent, 1)
,   m_pWorkbench(_pWorkbench)
,   m_pBoxSizer(NULL)
,   m_pPropertyGrid(NULL)
{
	createGUIControls();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PropertiesView::~PropertiesView()
{
    // Disable all of the remaining handles so they won't try
    // to notify this dead view.
    for(PropertiesHandles_type::iterator iter = m_propertiesHandles.begin(); iter != m_propertiesHandles.end(); iter++)
    {
        iter->first->disable();
        iter->second = I_PropertiesPublisher::pSubscription_type();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::onAddProperty(I_PropertiesPublisher& _publisher, I_Property& _property)
{
    // Get the handle associated with this publisher.
    PropertiesHandle* const pHandle = m_propertiesMap[&_publisher];

    // The name is a concatenation of the property set name and the property name.
    std::stringstream name;
    name << pHandle->getPublisher().getPropertiesName();

    // Get the parent category
    // Use pParent now.
    //wxPropertyCategory* const pCategory = pHandle->getCategory();

    if (_property.getParent() != NULL)
    {
        // Append the parent name.  This gives us the fully qualified
        // parent name.
        name << "\\" << _property.getParent()->getFullName();
    }

    // Get the parent by name.
    wxPGProperty* pParent = m_pPropertyGrid->GetPropertyByName(std2wx(name.str()));

    // Make sure the parent is not NULL.  It should at least return the
    // top category.
    assert(pParent != NULL);

    // Now create the child name.
    name << "\\" << _property.getName();

    wxPGProperty* pChild = NULL;

    switch(_property.getType())
    {
    case I_Property::STRING_TYPE:
        // The first parameter is the label which is shown on the screen.
        // The second parameter is the label, which is the unqiue identifier of the property.
        pChild = new wxStringProperty(std2wx(_property.getName()), std2wx(name.str()), std2wx(_property.getValue()));
        break;
    case I_Property::ENUM_TYPE:
        pChild = createEnum(_property, name.str());
        break;
    case I_Property::CATEGORY_TYPE:
        pChild = createCategory(_property, name.str());
        break;
    default:
        throw Zen::Utility::runtime_exception("Error, invalid property type.");
    }

    pChild->SetClientData(&_property);
    pParent->AppendChild(pChild);

    // Assume a string value type for now.

    // TEST See how to get the property (name or label)
    wxPGProperty* pByName = m_pPropertyGrid->GetPropertyByName(std2wx(name.str()));
#if 0
    m_pPropertyGrid->Insert(pCategory,
        new wxStringProperty(_property.getName(), name.str(), _property.getValue()));
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::onValueModified(I_PropertiesPublisher& _publisher, I_Property& _property)
{
    // The name is a concatenation of the property set name and the property name.
    std::stringstream name;
    name << _publisher.getPropertiesName();

    if (_property.getParent() != NULL)
    {
        // Append the parent name.  This gives us the fully qualified
        // parent name.
        name << "\\" << _property.getParent()->getFullName();
    }

    // Now create the child name.
    name << "\\" << _property.getName();

    wxPGProperty* pProperty = m_pPropertyGrid->GetPropertyByName(std2wx(name.str()));

    if (pProperty != NULL)
    {
        pProperty->SetValueFromString(std2wx(_property.getValue()));
    }
    else
    {
        // Assert?  Shouldn't get here.
        throw Zen::Utility::runtime_exception("PropertiesView::onValueModified(): Property name not found.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::onRemoveProperty(I_PropertiesPublisher& _publisher, I_Property& _property)
{
    // TODO Implement
    throw Zen::Utility::runtime_exception("PropertiesView::onRemoveProperty(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::createGUIControls()
{
	m_pBoxSizer = new wxGridSizer(1, 1, 5, 5);
	SetSizer(m_pBoxSizer);
	SetAutoLayout(true);

    m_pPropertyGrid = new wxPropertyGrid(this, ID_WXPROPERTYGRID1,
        wxDefaultPosition, wxDefaultSize,
        wxPG_AUTO_SORT | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);


    m_pBoxSizer->Add(m_pPropertyGrid, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 5);

	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PropertiesView::pPropertiesHandle_type
PropertiesView::registerProperties(const std::string& _propertySetName, I_PropertiesPublisher& _publisher)
{
    // TODO Add the property but keep it hidden.

    // Create a handle for this registration.
    PropertiesHandle* pPropertiesHandle = new PropertiesHandle(*this, _publisher);

    // Keep a raw pointer to this handle so we can reference it later.
    m_propertiesHandles[pPropertiesHandle] = I_PropertiesPublisher::pSubscription_type();

    // Index the handle by the publisher.
    m_propertiesMap[&_publisher] = pPropertiesHandle;

    return pPropertiesHandle_type(pPropertiesHandle, &PropertiesView::destroy);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::destroy(wpPropertiesHandle_type _pHandle)
{
    PropertiesHandle* pHandle = dynamic_cast<PropertiesHandle*>(_pHandle.get());

    if (pHandle)
    {
        // Hide it in case it's not hidden.
        pHandle->hide();

        delete pHandle;
    }
    // TODO Implement
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::unregisterProperties(PropertiesHandle* _pHandle)
{
    // Erase the handle and the index of the handle.
    m_propertiesHandles.erase(_pHandle);
    m_propertiesMap.erase(&_pHandle->getPublisher());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::show(PropertiesHandle& _handle)
{
    // If the handle already has been shown (i.e. the handle has a valid category)
    // don't do this.

    if (_handle.getCategory() == NULL)
    {
        wxPropertyCategory* const pCategory =
            new wxPropertyCategory(std2wx(_handle.getPublisher().getPropertiesName()));

        wxPGProperty* pID = m_pPropertyGrid->Append(pCategory);

        _handle.setCategory(pCategory);

        // Subscribe to the publisher
        m_propertiesHandles[&_handle] = _handle.getPublisher().subscribe(this);

        // Refresh the grid
        m_pPropertyGrid->Refresh();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::hide(PropertiesHandle& _handle)
{
    // This is a
    wxPropertyCategory* pCategory = _handle.getCategory();

    if (pCategory)
    {
        m_pPropertyGrid->DeleteProperty(pCategory);
        _handle.setCategory(NULL);
    }

    // Unsubscribe from the publisher
    m_propertiesHandles[&_handle] = I_PropertiesPublisher::pSubscription_type();

    // Refresh the grid
    m_pPropertyGrid->Refresh();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxEnumProperty*
PropertiesView::createEnum(I_Property& _property, const std::string& _label)
{
    I_EnumProperty* pProperty = dynamic_cast<I_EnumProperty*>(&_property);

    assert(pProperty != NULL);

    wxPGChoices choices;

    struct EnumVisitor
        : public I_EnumProperty::I_EnumVisitor
    {

        virtual void visit(const std::string& _label)
        {
            m_choices.Add(std2wx(_label));
        }

        EnumVisitor(wxPGChoices& _choices)
        :   m_choices(_choices)
        {
        }

        wxPGChoices& m_choices;
    };

    EnumVisitor visitor(choices);
    pProperty->getEnums(visitor);

    return new wxEnumProperty(std2wx(_property.getName()), std2wx(_label), visitor.m_choices);
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxPropertyCategory*
PropertiesView::createCategory(I_Property& _property, const std::string& _label)
{
    return new wxPropertyCategory(std2wx(_property.getName()), std2wx(_label));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::onPropertyGridChanging(wxPropertyGridEvent& _event)
{
    wxPGProperty* const pWXProperty = _event.GetProperty();

    // TODO Don't assume this is a string.
    std::string newValue = wx2std(_event.GetValue().GetString());

    if (pWXProperty->GetClientData() != NULL)
    {
        I_Property* pProperty = static_cast<I_Property*>(pWXProperty->GetClientData());

        assert(pProperty != NULL);

        if (!pProperty->getPublisher().getController().canChangeProperty(*pProperty, newValue))
        {
            // Veto the event.
            _event.Veto();

            // Set the validation failure behavior.
            _event.SetValidationFailureBehavior(wxPG_VFB_STAY_IN_PROPERTY
                //| wxPG_VFB_BEEP
                //| wxPG_VFB_SHOW_MESSAGE
                );
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertiesView::onPropertyGridChanged(wxPropertyGridEvent& _event)
{
    wxPGProperty* const pWXProperty = _event.GetProperty();

    // TODO Don't assume this is a string.
    std::string newValue = wx2std(_event.GetProperty()->GetValueAsString());

    if (pWXProperty->GetClientData() != NULL)
    {
        I_Property* pProperty = static_cast<I_Property*>(pWXProperty->GetClientData());

        assert(pProperty != NULL);

        pProperty->getPublisher().getController().setPropertyValue(*pProperty, newValue);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

