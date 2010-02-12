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
#ifndef WORLDBUILDER_TERRAIN_LAYER_PANEL_HPP_INCLUDED
#define WORLDBUILDER_TERRAIN_LAYER_PANEL_HPP_INCLUDED

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
class TerrainLayerPanel
:   public wxPanel
{
	DECLARE_EVENT_TABLE();

    /// @name TerrainLayerPanel implementation
    /// @{
public:

protected:
    void createGUIControls();
    void setImage(const std::string& _imageName, const wxTreeItemId& _id);
    /// @}

    /// @name 'Structors
    /// @{
public:
             TerrainLayerPanel(wxWindow* _pParent);
    virtual ~TerrainLayerPanel();
    /// @}

    /// @name Member Variables
    /// @{
private:
    wxFlexGridSizer*    m_flexGridSizer;

    wxStaticText*       m_staticText1;
    wxTextCtrl*         m_bitsPerEntryTE;
    wxButton*           m_selectFileBTN;
    wxStaticText*       m_staticText2;
    wxTextCtrl*         m_fileNameTE;

	enum
	{
		////GUI Enum Control ID Start
		ID_STATICTEXT1 = 1002,
        ID_TEXTCTRL1,
        ID_BUTTON1,
        ID_STATICTEXT2,
        ID_TEXTCTRL2,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};
    /// @}

};  // class TerrainLayerPanel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_TERRAIN_LAYER_PANEL_HPP_INCLUDED

