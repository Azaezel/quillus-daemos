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
#ifndef WORLDBUILDER_SIMPLE_LAYER_PANEL_HPP_INCLUDED
#define WORLDBUILDER_SIMPLE_LAYER_PANEL_HPP_INCLUDED

#include <Zen/StudioPlugins/GameBuilderCommon/I_ProjectListener.hpp>

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>

#include <wx/laywin.h>

// TR - Move to a collabible pane instead of a foldbar
//#include <wx/foldbar/foldpanelbar.h>

#include <wx/collpane.h>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Simple Layer Panel for the Zone Layer View.
/// This layer panel presents a simple hierarchical interface much
/// like most World Builder / Mission Editor applications.
class SimpleLayerPanel
:   public wxPanel
{
	DECLARE_EVENT_TABLE();

    /// @name SimpleLayerPanel implementation
    /// @{
public:

protected:
    void createGUIControls();
    void setImage(const std::string& _imageName, const wxTreeItemId& _id);
    /// @}

    /// @name 'Structors
    /// @{
public:
             SimpleLayerPanel(wxWindow* _pParent);
    virtual ~SimpleLayerPanel();
    /// @}

    /// @name Member Variables
    /// @{
private:
    wxGridSizer*        m_pBoxSizer;
	wxTreeCtrl*         m_pTreeCtrl;

    wxTreeItemId        m_parentItemId;

    typedef std::map<std::string, int>                  ImageListMap_type;
    ImageListMap_type                                   m_imageList;

	enum
	{
		////GUI Enum Control ID Start
		ID_WXTREECTRL1 = 1002,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};
    /// @}

};  // class SimpleLayerPanel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_SIMPLE_LAYER_PANEL_HPP_INCLUDED
