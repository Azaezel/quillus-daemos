cmake_minimum_required (VERSION 2.6) 

project (Workbench)
set ( PROJECT_PREFIX "Studio/Workbench" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

# Only use this stuff on UNIX platforms
if (UNIX)
	find_package(PkgConfig)
	pkg_check_modules(GTK REQUIRED gtk+-2.0)
endif (UNIX)

Find_Package(wxWidgets COMPONENTS aui adv base core propgrid REQUIRED)
INCLUDE(${wxWidgets_USE_FILE})

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( Workbench_SRC
    Configuration.hpp
    I_Contributor.hpp
    I_ContributorService.hpp
    I_ContributorServiceFactory.hpp
    I_DocumentReader.hpp
    I_DocumentService.hpp
    I_DocumentServiceFactory.hpp
    I_DocumentStoreService.hpp
    I_DocumentType.hpp
    I_DocumentWizardFactory.hpp
    I_DocumentWizardManager.hpp
    I_DocumentWizardService.hpp
    I_DocumentWriter.hpp
    I_Explorer.hpp
    I_ExplorerNodeContributor.hpp
    I_ExplorerNodeDecoration.hpp
    I_Notebook.hpp
    I_ProjectExplorerWidget.hpp
    I_ProjectWizardFactory.hpp
    I_ProjectWizardManager.hpp
    I_ProjectWizardService.hpp
    I_ProjectWizardType.hpp
    I_PropertiesHandle.hpp
    I_PropertyContributor.hpp
    I_SceneContributor.hpp
    I_SceneView.hpp
    I_SpreadSheetContributor.hpp
    I_StudioTopFrame.hpp
    I_ToolBarContributor.hpp
    I_ToolViewContributor.hpp
    I_Workbench.hpp
    I_WorkbenchManager.hpp
    src/action_visitor.hpp
    src/contributor_visitor.hpp
    src/Explorer.cpp
    src/Explorer.hpp
    src/ExplorerNotebook.cpp
    src/ExplorerNotebook.hpp
    src/FileWizardDialog.cpp
    src/FileWizardDialog.hpp
    src/GridCellEditor.cpp
    src/GridCellEditor.hpp
    src/I_Contributor.cpp
    src/I_ContributorService.cpp
    src/I_ContributorServiceFactory.cpp
    src/I_DocumentReader.cpp
    src/I_DocumentService.cpp
    src/I_DocumentServiceFactory.cpp
    src/I_DocumentStoreService.cpp
    src/I_DocumentWriter.cpp
    src/I_Explorer.cpp
    src/I_ExplorerNodeContributor.cpp
    src/I_ExplorerNodeDecoration.cpp
    src/I_Notebook.cpp
    src/I_ProjectExplorerWidget.cpp
    src/I_PropertiesHandle.cpp
    src/I_PropertyContributor.cpp
    src/I_SceneContributor.cpp
    src/I_SceneView.cpp
    src/I_SpreadSheetContributor.cpp
    src/I_StudioTopFrame.cpp
    src/I_ToolBarContributor.cpp
    src/I_ToolViewContributor.cpp
    src/I_Workbench.cpp
    src/I_WorkbenchManager.cpp
    src/menu_contributor_visitor.hpp
    src/Notebook.cpp
    src/Notebook.hpp
    src/ProjectExplorerView.cpp
    src/ProjectExplorerView.h
    src/ProjectWizardDialog.cpp
    src/ProjectWizardDialog.hpp
    src/PropertiesHandle.cpp
    src/PropertiesHandle.hpp
    src/PropertiesView.cpp
    src/PropertiesView.hpp
    src/SceneViewWidget.cpp
    src/SceneViewWidget.hpp
    src/SpreadSheetView.cpp
    src/SpreadSheetView.hpp
    src/StatusNotebook.cpp
    src/StatusNotebook.hpp
    src/StudioTopFrame.cpp
    src/StudioTopFrame.hpp
    src/ToolNotebook.cpp
    src/ToolNotebook.hpp
    src/View.cpp
    src/View.hpp
    src/Workbench.cpp
    src/Workbench.hpp
    src/WorkbenchManager.cpp
    src/WorkbenchManager.hpp
    src/WorkspaceExplorer.cpp
    src/WorkspaceExplorer.hpp
    src/WorkspaceNotebook.cpp
    src/WorkspaceNotebook.hpp
    src/wxUtilities.cpp
    wxUtilities.hpp

    )

set (wxWidgets_USE_UNICODE TRUE)

message("wx Flag: ${wxWidgets_DEFINITIONS} + ${wxWidgets_CXX_FLAGS} + ${wxWidgets_INCLUDE_DIRS}")

# TR - I removed ${wxWidgets_DEFINITIONS}  from add_definitions 
# This made it work on Linux, but does it break other OS?
add_definitions( -DWORKBENCH_EXPORTS ${wxWidgets_CXX_FLAGS} ${Zen_DEFINITIONS} )

message("GTK: ${GTK_INCLUDE_DIR}")

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${wxWidgets_INCLUDE_DIRS} ${GTK_INCLUDE_DIRS} )

link_directories ( ${DEV_LIB} ${wxWidgets_LIB_DIR} ${Boost_LIBRARY_DIRS} )

add_library ( Workbench ${LIB_TYPE} ${Workbench_SRC} )

add_dependencies(Workbench ZenCore ZenEngine ZenEnterprise WorkbenchCommon)

target_link_libraries( Workbench ZenCore ZenEngine ZenEnterprise WorkbenchCommon ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES} )

# TODO Only if using 2.8
#target_link_libraries( Workbench debug wxcode_${wxWidgets_CONFIGURATION}28d_propgrid )
#target_link_libraries( Workbench optimized wxcode_${wxWidgets_CONFIGURATION}28_propgrid )

set_target_properties ( Workbench 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( Workbench PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( Workbench PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_Contributor.hpp
    I_ContributorService.hpp
    I_ContributorServiceFactory.hpp
    I_DocumentReader.hpp
    I_DocumentService.hpp
    I_DocumentServiceFactory.hpp
    I_DocumentStoreService.hpp
    I_DocumentType.hpp
    I_DocumentWizardFactory.hpp
    I_DocumentWizardManager.hpp
    I_DocumentWizardService.hpp
    I_DocumentWriter.hpp
    I_Explorer.hpp
    I_ExplorerNodeContributor.hpp
    I_ExplorerNodeDecoration.hpp
    I_Notebook.hpp
    I_ProjectExplorerWidget.hpp
    I_ProjectWizardFactory.hpp
    I_ProjectWizardManager.hpp
    I_ProjectWizardService.hpp
    I_ProjectWizardType.hpp
    I_PropertiesHandle.hpp
    I_PropertyContributor.hpp
    I_SceneContributor.hpp
    I_SceneView.hpp
    I_SpreadSheetContributor.hpp
    I_StudioTopFrame.hpp
    I_ToolBarContributor.hpp
    I_ToolViewContributor.hpp
    I_Workbench.hpp
    I_WorkbenchManager.hpp
    wxUtilities.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	Workbench
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
