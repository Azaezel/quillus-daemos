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
#ifndef ZENSTUDIO_WORKBENCH_WORKBENCH_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_WORKBENCH_HPP_INCLUDED
#include "../I_Workbench.hpp"

#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ProjectTypes.hpp>

#include <Zen/Studio/Workbench/I_ContributorServiceFactory.hpp>

#include <boost/function.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class StudioTopFrame;
class View;
class I_Document;

class Workbench
:   public I_Workbench
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Enterprise::AppServer::I_ProtocolService>   pProtocolService_type;
    typedef Memory::managed_ptr<I_WorkbenchService>                         pWorkbenchService_type;
    /// @}

    /// @name I_Workbench implementation
    /// @{
public:
    virtual bool initialize(boost::filesystem::path& _workspacePath);
    virtual I_StudioTopFrame& getMainWindow();
    virtual I_ProjectTypes& getProjectTypes();
    virtual I_View& getView(const std::string& _viewType, pDocument_type _pDocument, bool _newInstance);
    virtual I_ProjectExplorerController& getProjectExplorerController();
    virtual I_ProjectExplorerWidget& getProjectExplorerWidget();
    virtual Enterprise::AppServer::I_ApplicationServer& getAppServer() const;
    virtual I_WorkbenchService& getWorkbenchService() const;
    virtual pRenderingService_type getRenderingService();
    virtual pPropertiesHandle_type registerProperties(const std::string& _propertySetName, I_PropertiesPublisher& _publisher);
    /// @}

    /// @name Workbench implementation
    /// @{
public:
    void createNewProject(const I_ProjectType& _projectType);

    /// Install a contributor service.
    /// @note Internal use only.  Public only because it's being used
    ///     by a visitor.
    /// @see installExplorerNodeActions()
    void installContributor(Plugins::I_ExtensionPoint::pExtension_type _pExtension, const std::string& _className);
private:
    /// Install the contributor extensions.
    void installContributors();

    /// Activate the contributors.
    void activateContributors();

    /// Create a new view.
    /// @param _viewType is the extension, although some view types are not dynamically loaded as plugins.
    /// @param _pDocument
    I_View& createView(const std::string& _viewType, pDocument_type _pDocument, const int _instance);

    /// Create a spreadsheet view.
    View* createSpreadSheetView(const std::string& _viewType, pDocument_type _pDocument, const int _instance);
    View* createOgreSceneView(const std::string& _viewType, pDocument_type _pDocument, const int _instance);    
    /// @}

    /// @name 'Structors
    /// @{
public:
             Workbench();
    virtual ~Workbench();
    /// @}

    /// @name Member Variables
    /// @{
private:
    StudioTopFrame*     m_pMainFrame;
    I_ProjectTypes&     m_projectTypes;

    boost::filesystem::path m_workspacePath;
    boost::filesystem::path m_controlPath;

    /// Primary application server
    Enterprise::AppServer::I_ApplicationServer*    m_pAppServer;

    /// Primary protocol adapter
    pProtocolService_type                           m_pProtocolService;

    /// Workbench Service
    pWorkbenchService_type                          m_pWorkbenchService;

    /// Map of views from an internal name (name:instance) to
    typedef std::map<std::string, I_View*>          Views_type;

    Views_type                                      m_views;

    typedef boost::function<View*(const std::string&, pDocument_type, const int)>
                                                    ViewFactoryMethod_type;

    typedef std::map<std::string, ViewFactoryMethod_type>
                                                    ViewFactories_type;

    ViewFactories_type                              m_viewFactories;

    typedef std::list<I_ContributorServiceFactory::pContributorService_type>    ContributorServices_type;
    ContributorServices_type                        m_contributors;
    /// @}

};  // class Workbench

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZENSTUDIO_WORKBENCH_WORKBENCH_MANAGER_HPP_INCLUDED
