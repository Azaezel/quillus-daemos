//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// World Builder
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
#ifndef WORLDBUILDER_ZONE_LAYER_VIEW_HPP_INCLUDED
#define WORLDBUILDER_ZONE_LAYER_VIEW_HPP_INCLUDED

#include <Zen/StudioPlugins/GameBuilderCommon/I_ProjectListener.hpp>

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>

#include <wx/laywin.h>
#include <wx/scrolwin.h>

//#include <wx/foldbar/foldpanelbar.h>
#include <wx/collpane.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Zone;

class ZoneLayerView
:   public wxScrolledWindow
{
	DECLARE_EVENT_TABLE();

    /// @name ZoneLayerView implementation
    /// @{
public:
    /// Set the active zone that's being edited.
    void setActiveZone(I_Zone* _pZone);

protected:
    void createGUIControls();
    void onPaneChanged(wxCollapsiblePaneEvent& _event);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ZoneLayerView(wxWindow* _pParent);
    virtual ~ZoneLayerView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    //wxCollapsiblePane*  m_pFoldPanelBar;

    wxFlexGridSizer*        m_pSizer;

	enum
	{
		////GUI Enum Control ID Start
		ID_WXTREECTRL1 = 1002,
        ID_WXFOLDPANELBAR1,
        ID_COLLAPSEME,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};

    /// Active zone being displayed in this view.
    I_Zone*                 m_pActiveZone;
    /// @}

};  // class Creator

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_CREATOR_HPP_INCLUDED
