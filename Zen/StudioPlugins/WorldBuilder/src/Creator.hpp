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
#ifndef WORLDBUILDER_CREATOR_HPP_INCLUDED
#define WORLDBUILDER_CREATOR_HPP_INCLUDED

#include <Zen/StudioPlugins/GameBuilderCommon/I_Project.hpp>
#include <Zen/StudioPlugins/GameBuilderCommon/I_ProjectListener.hpp>
#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectTypeView.hpp>

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// The Creator view and model.
/// This implementation is a quick and dirty implementation.
/// Eventually it should be split into a model / view with the model
/// subscribing to all open GOT documents and updating the Creator view
/// when a GOT has a creatorCategory element modified or a creatorPreview modified.
class Creator
:   public wxPanel
,   public GameBuilder::I_ProjectListener
,   public GameBuilder::I_GameObjectTypeView
{
	DECLARE_EVENT_TABLE();

    /// @name GameBuilder::I_ProjectListener implementation
    /// @{
public:
    virtual void onDocumentModified(GameBuilder::I_GameObjectTypeDocument& _gameObjectTypeDocument);
    virtual void gameObjectTypeCreated(GameBuilder::I_GameObjectType& _gameObjectType);
    virtual void gameObjectTypeDocumentModified(GameBuilder::I_GameObjectTypeDocument& _document);
    virtual void gameObjectTypeDeleted(GameBuilder::I_GameObjectType& _gameObjectType);
    /// @}

    /// @name I_GameObjectTypeView implementation
    /// @{
public:
    virtual void onNewElement(GameBuilder::I_GameObjectElement& _element, int _position);
    virtual void onElementRemoved(int _position);
    virtual void onElementModified(GameBuilder::I_GameObjectElement& _element, int _row);
    /// @}

    /// @name Creator implementation
    /// @{
public:
    /// Create the Ogitors objects in the Creator view after the Ogitor view has
    /// been initialized.
    ///
    /// @see ZoneContributor::open()
    void createOgitorObjects();

protected:
    void createGUIControls();

    void setImage(const std::string& _imageName, const wxTreeItemId& _id);

    void updateFromDocument(GameBuilder::I_GameObjectTypeDocument& _document);

    void handleElement(GameBuilder::I_GameObjectElement& _element);
    void handleDocument(GameBuilder::I_GameObjectTypeDocument& _document);

    /// @param _path "/" separated list of folders.
    /// @return wxTreeItemId of the folder that was created or found
    ///     along the path of _path.
    wxTreeItemId findFolder(const std::string& _path);
    /// @}

    /// @name 'Structors
    /// @{
public:
             Creator(GameBuilder::I_Project& _project, wxWindow* _pParent);
    virtual ~Creator();
    /// @}

    /// @name Member Variables
    /// @{
private:
    GameBuilder::I_Project::pSubscription_type          m_pSubscription;
	wxTreeCtrl*     m_pTreeCtrl;
    wxGridSizer*    m_pBoxSizer;

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

    struct CreatorItem
    :   public wxTreeItemData
    ,   public boost::noncopyable
    {
        boost::uint64_t gameObjectTypeId;
        std::string     typeName;
        std::string     category;
        std::string     icon;

        wxTreeItemId    id;

        CreatorItem()
        :   id(0)
        {
        }
    };

    /// Map of gameObjectTypeId to CreatorItem
    typedef std::map<boost::uint64_t, CreatorItem*>     CreatorItems_type;
    CreatorItems_type       m_creatorItems;
    /// @}

};  // class Creator

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_CREATOR_HPP_INCLUDED
