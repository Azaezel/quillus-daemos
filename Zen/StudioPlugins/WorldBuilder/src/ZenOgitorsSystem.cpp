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
#include "ZenOgitorsSystem.hpp"
#include "OgitorsController.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static bool sm_viewKeyboard[1024];
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

ZenOgitorsSystem::ZenOgitorsSystem()
:   m_pController(NULL)
{
    // TODO Get this key setup from a configuration.
    Ogitors::OgitorsSpecialKeys keys;
    keys.SPK_FORWARD = 'W';
    keys.SPK_LEFT = 'A';
    keys.SPK_BACKWARD = 'S';
    keys.SPK_RIGHT = 'D';

    // SetKeyboard can be called more than once, but sm_viewKeyboard should 
    // always be used as the first parameter.
    Ogitors::CViewportEditor::SetKeyboard(sm_viewKeyboard, keys);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZenOgitorsSystem::~ZenOgitorsSystem()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenOgitorsSystem::CopyFilesEx(Ogre::String source, Ogre::String destination)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::CopyFilesEx(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenOgitorsSystem::MakeDirectory(Ogre::String dirname)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::MakeDirectory(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::String
ZenOgitorsSystem::getProjectsDirectory()
{
    // HACK Don't hard-code this
    return "C:/dev/ogitor-build/RunPath/Projects/";
    //throw Zen::Utility::runtime_exception("ZenOgitorsSystem::getProjectsDirectory(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::DeleteFile(const Ogre::String &file)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::DeleteFile(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::RenameFile(const Ogre::String &oldname, const Ogre::String &newname)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::RenameFile(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::GetFileList(Ogre::String path, Ogre::StringVector &list)
{
    // Clear the list.
    list.clear();

    std::string srcPath(Ogitors::OgitorsUtils::ExtractFilePath(path).c_str());
    std::string fileSpec(path.c_str());
    fileSpec = fileSpec.substr(srcPath.length(), fileSpec.length() - srcPath.length());

    if( fileSpec.empty() )
    {
        fileSpec = ".*\..*";
    }
    else
    {
        boost::algorithm::replace_all(fileSpec, "\.", "\\.");
        boost::algorithm::replace_all(fileSpec, "*", ".*");
    }

    boost::filesystem::path p(srcPath);

    // Make sure the source path exists.
    if (boost::filesystem::exists(p))
    {
        boost::regex e(fileSpec);

        boost::filesystem::directory_iterator dIter(p), dIterEnd;
        while( dIter != dIterEnd )
        {
            std::string filename = dIter->leaf();
            if( boost::regex_search(filename, e) )
            {
                list.push_back(srcPath + filename);
            }
            dIter++;
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::String
ZenOgitorsSystem::DisplayOpenDialog(Ogre::UTFString title, Ogitors::UTFStringVector ExtensionList)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::DisplayOpenDialog(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::String
ZenOgitorsSystem::DisplaySaveDialog(Ogre::UTFString title, Ogitors::UTFStringVector ExtensionList)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::DisplaySaveDialog(): Error, not implemented.");
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogitors::DIALOGRET
ZenOgitorsSystem::DisplayMessageDialog(Ogre::UTFString msg, Ogitors::DIALOGTYPE dlgType)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::DisplayMessageDialog(): Error, not implemented.");
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenOgitorsSystem::DisplayTerrainDialog(Ogre::NameValuePairList &params)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::DisplayTerrainDialog(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenOgitorsSystem::DisplayImportHeightMapDialog(Ogre::NameValuePairList &params)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::DisplayImportHeightMapDialog(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::UpdateLoadProgress(float percentage, Ogre::UTFString msg)
{
    // HACK (for now lets just ignore it)
    //throw Zen::Utility::runtime_exception("ZenOgitorsSystem::UpdateLoadProgress(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::UTFString
ZenOgitorsSystem::Translate(Ogre::String& str) 
{
    return str;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::UTFString
ZenOgitorsSystem::Translate(const char * str) 
{
    return Ogre::String(str);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenOgitorsSystem::HasTreeView()
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::HasTreeView(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenOgitorsSystem::HasPropertiesView()
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::HasPropertiesView(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::PresentPropertiesView(Ogitors::CBaseEditor* object)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::PresentPropertiesView(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::SelectTreeItem(void* handle)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::SelectTreeItem(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void*
ZenOgitorsSystem::InsertTreeItem(Ogre::String parent,Ogre::String name,int iconid, unsigned int colour)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::InsertTreeItem(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void*
ZenOgitorsSystem::InsertTreeItem(void* parent,Ogre::String name,int iconid, unsigned int colour)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::InsertTreeItem(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void*
ZenOgitorsSystem::MoveTreeItem(void* newparent, void* object)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::MoveTreeItem(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::SetTreeItemText(Ogre::String oldname, Ogre::String newname)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::SetTreeItemText(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::DeleteTreeItem(Ogre::String name)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::DeleteTreeItem(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void*
ZenOgitorsSystem::CreateTreeRoot(Ogre::String name)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::CreateTreeRoot(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::SetTreeItemColour(Ogre::String name, unsigned int colour)
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::SetTreeItemColour(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::ClearTreeItems()
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::ClearTreeItems(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::SetMouseCursor(unsigned int cursor) 
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::SetMouseCursor(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::SetMousePosition(Ogre::Vector2 position) 
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::SetMousePosition(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::ShowMouseCursor(bool bShow) 
{
    throw Zen::Utility::runtime_exception("ZenOgitorsSystem::ShowMouseCursor(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static const char*
resourcePath()
{
    // HACK this shouldn't be hardcoded
    return "C:/dev/ogitor-build/RunPath/bin/";
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenOgitorsSystem::setCurrentSceneView(Zen::Studio::Workbench::I_SceneView* _pSceneView)
{
    // TODO Cache multiple controllers if there are multiple scene views?
    if (m_pController)
    {
        delete m_pController;
    }

    // Attach a new controller to this scene view.
    // Each controller links a view with an ZenOgitorsSystem / OgitorsRoot.
    new OgitorsController(*this, *_pSceneView);

    // TODO Copied from Ogitor, give the author credit.
    Ogre::ConfigFile cf;

    // TODO Use boost::filesystem::path instead of std::string
    std::string cfPath = resourcePath();
    cfPath.append("resources.cfg");
    cf.load(cfPath);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = resourcePath() + i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

