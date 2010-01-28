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
#ifndef ZENSTUDIO_WORKBENCH_PROPERTIES_VIEW_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_PROPERTIES_VIEW_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include "View.hpp"

#include <Zen/Studio/WorkbenchCommon/I_PropertiesPublisher.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesListener.hpp>

#ifdef Yield
#undef Yield
#endif

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#ifndef wxUSE_PROPGRID
#error Property Grid was not enabled in your wxWidgets configuration.
#endif
#include <wx/propgrid/props.h>
#include <wx/propgrid/property.h>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_PropertiesHandle;
class I_PropertiesPublisher;
class PropertiesHandle;

/// Properties View.
/// This view displays a collection of property sets.
class PropertiesView
:   public wxPanel
,   public I_PropertiesListener
{
private:
		DECLARE_EVENT_TABLE();

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_PropertiesHandle>         pPropertiesHandle_type;
    typedef Memory::managed_weak_ptr<I_PropertiesHandle>    wpPropertiesHandle_type;
    /// @}

    /// @name I_PropertiesListener implementation
    /// @{
public:
    virtual void onAddProperty(I_PropertiesPublisher& _publisher, I_Property& _property);
    virtual void onValueModified(I_PropertiesPublisher& _publisher, I_Property& _property);
    virtual void onRemoveProperty(I_PropertiesPublisher& _publisher, I_Property& _property);
    /// @}

    /// @name PropertiesView implementation
    /// @{
    pPropertiesHandle_type registerProperties(const std::string& _propertySetName, I_PropertiesPublisher& _publisher);

    /// Show a panel.
    void show(PropertiesHandle& _handle);

    /// Hide a panel.
    void hide(PropertiesHandle& _handle);

protected:
    void createGUIControls();

    static void destroy(wpPropertiesHandle_type _pHandle);

    friend class PropertiesHandle;
    void unregisterProperties(PropertiesHandle* _pHandle);

    wxEnumProperty* createEnum(I_Property& _property, const std::string& _label);
    wxPropertyCategory* createCategory(I_Property& _property, const std::string& _label);

    void onPropertyGridChanging(wxPropertyGridEvent& _event);
    void onPropertyGridChanged(wxPropertyGridEvent& _event);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class StudioTopFrame;
             PropertiesView(wxWindow* _pParent, Workbench* _pWorkbench);
    virtual ~PropertiesView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Workbench*                                  m_pWorkbench;
    wxGridSizer*                                m_pBoxSizer;
    wxPropertyGrid*                             m_pPropertyGrid;

public:
	enum
	{
		////GUI Enum Control ID Start
        ID_WXPROPERTYGRID1 = 1002,
        ID_COLLAPSEME,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};
private:

    typedef std::map<PropertiesHandle*, I_PropertiesPublisher::pSubscription_type>  PropertiesHandles_type;

    /// Set of PropertiesHandle*.  The payload is a subscription, but the subscription
    /// is only valid if the properties are currently being displayed.
    PropertiesHandles_type  m_propertiesHandles;

    typedef std::map<I_PropertiesPublisher*, PropertiesHandle*>                     Properties_map;

    /// Map publishers to handlers.
    Properties_map          m_propertiesMap;
    /// @}

};  // class PropertiesView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_PROPERTIES_VIEW_HPP_INCLUDED
