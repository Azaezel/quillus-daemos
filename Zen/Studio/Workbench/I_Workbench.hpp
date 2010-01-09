//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZENSTUDIO_WORKBENCH_I_WORKBENCH_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_WORKBENCH_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Engine/Rendering/I_RenderingService.hpp>

#include <boost/filesystem.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Enterprise {
        namespace AppServer {
            class I_ApplicationServer;
            class I_ProtocolService;
        }   // namespace AppServer
    }   // namespace Enterprise
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ProjectService;
class I_ProjectType;
class I_ProjectTypes;
class I_Project;
class I_View;
class I_ProjectExplorerController;
class I_ExplorerModel;
class I_WorkbenchService;
class I_Document;
class I_ProjectExplorerWidget;
class I_StudioTopFrame;
class I_PropertiesHandle;
class I_PropertiesPublisher;

class WORKBENCH_DLL_LINK I_Workbench
:   public Memory::managed_self_ref<I_Workbench>
{
    /// @name Types
    /// @{
public:
    typedef Event::Event<I_Project&>                        ProjectEvent_type;

    typedef Memory::managed_ptr<I_Document>                 pDocument_type;

    typedef Zen::Memory::managed_ptr<Engine::Rendering::I_RenderingService> 
                                                            pRenderingService_type;
    typedef Memory::managed_ptr<I_PropertiesHandle>         pPropertiesHandle_type;
    /// @}

    /// @name I_Workbench interface
    /// @{
public:
    /// Initialize the workbench.
    /// @param _workspacePath Path to the workspace.  This requires a
    ///         subdirectory ".workspace" with config files and 
    ///         the workbench.sqlite database.
    /// @return true if the initialization succeeded, otherwise false
    virtual bool initialize(boost::filesystem::path& _workspacePath) = 0;

    /// Get the main workbench window
    virtual I_StudioTopFrame& getMainWindow() = 0;

    /// Get the collection of project types
    virtual I_ProjectTypes& getProjectTypes() = 0;

    /// Get a View.
    /// @param _viewType type of view.  This is an extension point with several built-in
    ///     extensions.  "spreadsheet" will create an I_SpreadSheetView.  "ogrescene" will
    ///     create a I_SceneView backed by ZOgre.
    /// @param _newInstance true if requesting a new instance of the view.  If false, first
    ///     the workbench will attempt to find an existing view of the same type and name.
    ///     If one does not exist, it will create it.  If true, a new instance will always
    ///     be created.
    virtual I_View& getView(const std::string& _viewType, pDocument_type _pDocument, bool _newInstance) = 0;

    /// Get the project explorer controller.
    virtual I_ProjectExplorerController& getProjectExplorerController() = 0;

    /// Get the project explorer widget.
    virtual I_ProjectExplorerWidget& getProjectExplorerWidget() = 0;

    /// Get the main application server.
    virtual Enterprise::AppServer::I_ApplicationServer& getAppServer() const = 0;

    /// Get the main workbench service associated with this workbench.
    virtual I_WorkbenchService& getWorkbenchService() const = 0;

    /// Get the main rendering service.
    virtual pRenderingService_type getRenderingService() = 0;

    /// Register a set of properties.
    /// To make the properties be displayed, use I_PropertiesHandle::show() and 
    /// I_PropertiesHandle::hide().
    /// @return Properties handle.  When this object goes out of scope the property view
    ///     will destroy it's property cache and unsubscribe from the publisher.
    /// @todo Should this be moved to a I_PropertiesView? Probably.
    virtual pPropertiesHandle_type registerProperties(const std::string& _propertySetName, I_PropertiesPublisher& _publisher) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// This event is fired when a project is activated.
    /// The "active" project is the project that currently has focus
    /// due to an element or a sub element of the project gaining focus.
    ProjectEvent_type       onActivateProjectEvent;

    /// This event is fired when a project is deactivated
    ProjectEvent_type       onDeactivateProjectEvent;
    /// @}


    /// @name 'Structors
    /// @{
protected:
             I_Workbench();
    virtual ~I_Workbench();
    /// @}

};  // interface I_Workbench

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory 
{
    /// I_Workbench is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_Workbench> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_WORKBENCH_INCLUDED
