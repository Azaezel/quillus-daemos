//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// World Builder
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
#ifndef WORLDBUILDER_ZONE_OGITORS_SYSTEM_HPP_INCLUDED
#define WORLDBUILDER_ZONE_OGITORS_SYSTEM_HPP_INCLUDED

#include <Ogre.h>
#include <Ogitors.h>

#include <Zen/Studio/Workbench/I_SceneView.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class OgitorsController;

/// Zen Ogitors System.
/// This class implements all of the callbacks necessary to be handled for 
/// integrating with Ogitor
class ZenOgitorsSystem
:   public Ogitors::OgitorsSystem
{
    /// @name Typedefs
    /// @{
public:
    /// @}

    /// @name OgitorsSystem implementation
    /// @{
public:
    virtual bool CopyFilesEx(Ogre::String source, Ogre::String destination);
    virtual bool MakeDirectory(Ogre::String dirname);
    virtual Ogre::String getProjectsDirectory();
    virtual void DeleteFile(const Ogre::String &file);
    virtual void RenameFile(const Ogre::String &oldname, const Ogre::String &newname);
    virtual void GetFileList(Ogre::String path, Ogre::StringVector &list);
    virtual Ogre::String DisplayOpenDialog(Ogre::UTFString title, Ogitors::UTFStringVector ExtensionList);
    virtual Ogre::String DisplaySaveDialog(Ogre::UTFString title, Ogitors::UTFStringVector ExtensionList);
    virtual Ogitors::DIALOGRET  DisplayMessageDialog(Ogre::UTFString msg, Ogitors::DIALOGTYPE dlgType);
    virtual bool         DisplayTerrainDialog(Ogre::NameValuePairList &params);
    virtual bool         DisplayImportHeightMapDialog(Ogre::NameValuePairList &params);
    virtual void         UpdateLoadProgress(float percentage, Ogre::UTFString msg);
    virtual Ogre::UTFString Translate(Ogre::String& str);
    virtual Ogre::UTFString Translate(const char * str);
    virtual bool         HasTreeView();
    virtual bool         HasPropertiesView();
    virtual void         PresentPropertiesView(Ogitors::CBaseEditor* object);
    virtual void         SelectTreeItem(void *handle);
    virtual void *       InsertTreeItem(Ogre::String parent,Ogre::String name,int iconid, unsigned int colour);
    virtual void *       InsertTreeItem(void *parent,Ogre::String name,int iconid, unsigned int colour);
    virtual void *       MoveTreeItem(void *newparent, void *object);
    virtual void         SetTreeItemText(Ogre::String oldname, Ogre::String newname);
    virtual void         DeleteTreeItem(Ogre::String name);
    virtual void *       CreateTreeRoot(Ogre::String name);
    virtual void         SetTreeItemColour(Ogre::String name, unsigned int colour);
    virtual void         ClearTreeItems();
    virtual void         SetMouseCursor(unsigned int cursor);
    virtual void         SetMousePosition(Ogre::Vector2 position);
    virtual void         ShowMouseCursor(bool bShow);
    /// @}

    /// @name ZenOgitorsSystem implementation
    /// @{
public:
    /// Set the current scene view.
    void setCurrentSceneView(Zen::Studio::Workbench::I_SceneView* _pSceneView);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ZenOgitorsSystem();
    virtual ~ZenOgitorsSystem();
    /// @}

    /// @name Member Variables
    /// @{
private:
    OgitorsController*          m_pController;
    /// @}

};  // class ZenOgitorsSystem

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_ZONE_OGITORS_SYSTEM_HPP_INCLUDED

