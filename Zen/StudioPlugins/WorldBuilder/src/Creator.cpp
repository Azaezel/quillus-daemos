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
#include "Creator.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/wxUtilities.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectType.hpp>
#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectElement.hpp>
#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectTypeDocument.hpp>

#include <wx/imaglist.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BEGIN_EVENT_TABLE(Creator,wxPanel)
END_EVENT_TABLE()
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Creator::Creator(GameBuilder::I_Project& _project, wxWindow* _pParent)
:   wxPanel(_pParent)
{
    createGUIControls();

    m_parentItemId = m_pTreeCtrl->AddRoot(wxT("Root Group"));
    setImage("./icons/folder.png", m_parentItemId);

    // TODO _project needs to be a managed_ptr so we can retain it.

    // TODO This Creator needs to be created upon project opening, not
    // delayed the way it currently is, otherwise it will miss some GOT
    // events.
    m_pSubscription = _project.subscribe(this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Creator::~Creator()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::createGUIControls()
{
    // Copied from ProjectExplorerView::CreateGUIControls().
    // See it for more references.

	m_pBoxSizer = new wxGridSizer(1, 1, 5, 5);
	SetSizer(m_pBoxSizer);
	SetAutoLayout(true);

	m_pTreeCtrl = new wxTreeCtrl(this, ID_WXTREECTRL1, wxPoint(5,5), wxSize(208,240),
        wxTR_HAS_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT | wxTR_LINES_AT_ROOT);

    m_pTreeCtrl->AssignImageList(new wxImageList(16, 16, false, 2));
	m_pTreeCtrl->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	m_pBoxSizer->Add(m_pTreeCtrl, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 5);

	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::gameObjectTypeCreated(GameBuilder::I_GameObjectType& _gameObjectType)
{
    // Get the document and temporarily subscribe to it
    GameBuilder::I_GameObjectTypeDocument* pDoc =
        dynamic_cast<GameBuilder::I_GameObjectTypeDocument*>(_gameObjectType.getDocument().get());

    if (pDoc)
    {
        updateFromDocument(*pDoc);
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::gameObjectTypeDocumentModified(GameBuilder::I_GameObjectTypeDocument& _document)
{
    updateFromDocument(_document);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::gameObjectTypeDeleted(GameBuilder::I_GameObjectType& _gameObjectType)
{
    throw Zen::Utility::runtime_exception("Creator::gameObjectTypeDeleted(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::updateFromDocument(GameBuilder::I_GameObjectTypeDocument& _document)
{
    // Subscribe, but since we don't retain a reference, the subscription will
    // immediately unsubscribe when it goes out of scope.
    _document.subscribe(this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::onNewElement(GameBuilder::I_GameObjectElement& _element, int _position)
{
    handleElement(_element);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::onElementRemoved(int _position)
{
    // TODO How to handle?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::onElementModified(GameBuilder::I_GameObjectElement& _element, int _row)
{
    handleElement(_element);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::handleElement(GameBuilder::I_GameObjectElement& _element)
{
    GameBuilder::I_GameObjectTypeDocument*
        pDoc = dynamic_cast<GameBuilder::I_GameObjectTypeDocument*>(&_element.getDocument());

    const boost::uint64_t gameObjectTypeId = pDoc->getNode()->getGameObjectTypeId();

    bool isCreatorObject = false;
    std::string creatorCategory;
    std::string creatorIcon;

    if (_element.getElementName() == "creatorCategory")
    {
        creatorCategory = _element.getDefaultValue();
        isCreatorObject = true;
    }
    else if (_element.getElementName() == "creatorIcon")
    {
        creatorIcon = _element.getDefaultValue();
        isCreatorObject = true;
    }

    // If it's a creator object, update the element.
    if (isCreatorObject)
    {
        CreatorItem* pItem = NULL;

        CreatorItems_type::iterator iter = m_creatorItems.find(gameObjectTypeId);

        if (iter == m_creatorItems.end())
        {
            pItem = new CreatorItem;
            pItem->gameObjectTypeId = gameObjectTypeId;
            m_creatorItems[gameObjectTypeId] = pItem;
        }
        else
        {
            pItem = iter->second;
        }

        std::string oldCategory = pItem->category;
        std::string oldIcon = pItem->icon;

        pItem->category = creatorCategory;
        pItem->icon = creatorIcon;

        if (oldCategory != pItem->category)
        {
            // New category so need to re-parent or create a new tree item.
            if (pItem->id.IsOk())
            {
                // Prune

                // First, make sure it doesn't delete the item data.
                m_pTreeCtrl->SetItemData(pItem->id, 0);

                // Delete the item so we can create a new one.
                m_pTreeCtrl->Delete(pItem->id);
                pItem->id = 0;

                // TODO Delete the parent folder if it's empty now.
            }

            // Find / create the appropriate folder(s)
            wxTreeItemId folder = findFolder(pItem->category);

            // Make sure the folder was valid.  If not, make the root item
            // be the parent.
            if (!folder.IsOk())
            {
                folder = m_parentItemId;
            }

            // Append a new item
            pItem->id = m_pTreeCtrl->AppendItem(folder, wxT(""));

            m_pTreeCtrl->SetItemData(pItem->id, pItem);
        }


        // Set / update the item text if the item exists.
        if (pItem->id.IsOk())
        {

            m_pTreeCtrl->SetItemText(pItem->id, std2wx(pDoc->getNode()->getName()));

            // TODO: Umm, what happens if it changes?  Does that
            // change get propagated all the way to here?  If not, need to fix it.
        }

    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Creator::setImage(const std::string& _imageName, const wxTreeItemId& _id)
{
    ImageListMap_type::iterator iter = m_imageList.find(_imageName);

    if (iter == m_imageList.end())
    {

        const int i = m_pTreeCtrl->GetImageList()->Add(wxIcon(std2wx(_imageName), wxBITMAP_TYPE_PNG));

        m_imageList[_imageName] = i;
        m_pTreeCtrl->SetItemImage(_id, i);
    }
    else
    {
        m_pTreeCtrl->SetItemImage(_id, iter->second);
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxTreeItemId
Creator::findFolder(const std::string& _path)
{
    // Parse the string into tokens using "/" as the delimiter.
    std::string delimiters = "/";

    // Skip the optional delimiters at the start of the string.
    std::string::size_type lastPosition = _path.find_first_not_of(delimiters, 0);

    // Find the first character that's not a delimiter.
    std::string::size_type curPosition = _path.find_first_of(delimiters, lastPosition);

    wxTreeItemId currentParent = m_parentItemId;
    wxTreeItemId child;

    // Now that we're set up, loop through and find all of the strings
    while (curPosition != std::string::npos || lastPosition != std::string::npos)
    {
        std::string folderName = _path.substr(lastPosition, curPosition - lastPosition);

        wxTreeItemIdValue cursor;
        child = m_pTreeCtrl->GetFirstChild(currentParent, cursor);

        while(child.IsOk())
        {
            // Skip non-folders (folders don't have item data)
            if (m_pTreeCtrl->GetItemData(child) == NULL)
            {
                wxString textValue = m_pTreeCtrl->GetItemText(child);

                // Check to see if this is the right folder name.
                if (wx2std(textValue) == folderName)
                {
                    break;
                }
            }

            child = m_pTreeCtrl->GetNextChild(currentParent, cursor);
        }

        // If the folder wasn't found, create one.
        // TODO Group the folders at the top?
        if (!child.IsOk())
        {
            child = m_pTreeCtrl->AppendItem(currentParent, std2wx(folderName));
            setImage("./icons/folder.png", child);
        }

        // Set the next currentParent to be the folder we just
        // found or created.
        currentParent = child;

        // Skip the delimiter(s)
        lastPosition = _path.find_first_not_of(delimiters, curPosition);

        // Find the next character that's not a delimiter
        curPosition = _path.find_first_of(delimiters, lastPosition);
    }

    // Return the leaf folder that was found or created.
    return currentParent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
