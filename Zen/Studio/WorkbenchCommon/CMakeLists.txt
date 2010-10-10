cmake_minimum_required (VERSION 2.6) 

project (WorkbenchCommon)
set ( PROJECT_PREFIX "Studio/WorkbenchCommon" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( WorkbenchCommon_SRC
    I_PropertiesPublisher.hpp
    I_ProjectServiceFactory.hpp
    I_ProjectWizardType.hpp
    I_SceneProperties.hpp
    I_SpreadSheetProperties.hpp
    I_SceneEntityProperties.hpp
    generic_scene_model_type.hpp
    I_ExplorerNodeFactory.hpp
    I_SpreadSheetCellProperties.hpp
    I_ProjectType.hpp
    I_ProjectAction.hpp
    I_ProjectWizardFactory.hpp
    I_SpreadSheetController.hpp
    I_SpreadSheetDocumentType.hpp
    I_ExplorerNodeType.hpp
    I_ProjectLocation.hpp
    I_SpreadSheetViewable.hpp
    II_ProjectTypes.hpp
    I_SpreadSheetAction.hpp
    I_View.hpp
    Project.hpp
    I_ProjectWizardManager.hpp
    I_SceneEntityFactory.hpp
    I_SceneNode.hpp
    generic_explorer_node_type.hpp
    I_ProjectTypes.hpp
    I_ProjectExplorerView.hpp
    generic_spreadsheet_document_type.hpp
    I_SpreadSheetCellFactory.hpp
    I_SpreadSheetView.hpp
    I_DocumentWizardService.hpp
    I_ExplorerNodeAction.hpp
    I_Project.hpp
    subscription.hpp
    src/DocumentWizardManager.cpp
    src/I_SpreadSheetController.cpp
    src/I_SceneEntity.cpp
    src/I_WorkbenchService.cpp
    src/I_ProjectWizardFactory.cpp
    src/I_ProjectWizardManager.cpp
    src/I_ProjectServiceFactory.cpp
    src/I_SpreadSheetProperties.cpp
    src/I_SceneEntityProperties.cpp
    src/I_Viewable.cpp
    src/ProjectType.hpp
    src/DocumentWizardManager.hpp
    src/I_ProjectExplorerController.cpp
    src/ProjectServiceManager.hpp
    src/I_SpreadSheetDocumentType.cpp
    src/I_SceneModel.cpp
    src/I_SpreadSheetCellProperties.cpp
    src/I_DocumentWizardFactory.cpp
    src/II_ExplorerNode.cpp
    src/I_SceneProperties.cpp
    src/I_ProjectTypes.cpp
    src/ProjectWizardType.cpp
    src/I_PropertiesListener.cpp
    src/ProjectWizardType.hpp
    src/I_ProjectType.cpp
    src/I_ProjectWizardType.cpp
    src/ProjectType.cpp
    src/I_ProjectService.cpp
    src/I_SceneEntityFactory.cpp
    src/I_ProjectServiceManager.cpp
    src/I_EnumProperty.cpp
    src/ProjectWizardManager.cpp
    src/I_ExplorerModel.cpp
    src/I_SceneListener.cpp
    src/I_ExplorerNodeType.cpp
    src/I_ExplorerNodeAction.cpp
    src/I_ProjectExplorerView.cpp
    src/I_ProjectWizardService.cpp
    src/I_SpreadSheetViewable.cpp
    src/ProjectExplorerModel.hpp
    src/I_SpreadSheetCellFactory.cpp
    src/I_View.cpp
    src/I_DocumentWizardService.cpp
    src/Project.cpp
    src/DocumentType.hpp
    src/I_Project.cpp
    src/I_SpreadSheetAction.cpp
    src/ProjectTypes.hpp
    src/ProjectServiceManager.cpp
    src/I_ProjectAction.cpp
    src/I_ProjectLocation.cpp
    src/ProjectWizardManager.hpp
    src/I_SpreadSheetDocument.cpp
    src/I_PropertiesController.cpp
    src/I_DocumentWizardManager.cpp
    src/I_SpreadSheetCell.cpp
    src/II_ProjectTypes.cpp
    src/I_ExplorerNode.cpp
    src/I_ProjectExplorerModel.cpp
    src/I_PropertiesPublisher.cpp
    src/I_Property.cpp
    src/I_DocumentType.cpp
    src/DocumentType.cpp
    src/I_Document.cpp
    src/I_SceneController.cpp
    src/I_SpreadSheetView.cpp
    src/I_ExplorerNodeFactory.cpp
    src/I_SceneViewable.cpp
    src/I_SceneModelType.cpp
    src/ProjectTypes.cpp
    I_ProjectWizardService.hpp
    I_SceneModel.hpp
    I_DocumentType.hpp
    I_DocumentWizardFactory.hpp
    indirect_explorer_node_action.hpp
    I_Property.hpp
    I_SceneController.hpp
    I_SceneAction.hpp
    I_PropertiesController.hpp
    I_SpreadSheetDocument.hpp
    I_ProjectExplorerController.hpp
    I_ProjectServiceManager.hpp
    I_PropertiesListener.hpp
    I_Viewable.hpp
    I_ProjectService.hpp
    II_SpreadSheetDocument.hpp
    I_ExplorerModel.hpp
    I_DocumentWizardManager.hpp
    Configuration.hpp
    I_ExplorerNode.hpp
    direct_explorer_node_action.hpp
    I_EnumProperty.hpp
    indirect_spread_sheet_action.hpp
    I_SpreadSheetDocumentFactory.hpp
    I_ProjectExplorerModel.hpp
    I_Document.hpp
    I_SceneEntity.hpp
    II_ExplorerNode.hpp
    I_SceneListener.hpp
    I_WorkbenchService.hpp
    I_SpreadSheetCell.hpp
    I_SceneViewable.hpp
    I_SceneModelType.hpp

    )

add_definitions( -DWORKBENCHCOMMON_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${wxWidgets_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS} )

add_library ( WorkbenchCommon ${LIB_TYPE} ${WorkbenchCommon_SRC} )

add_dependencies(WorkbenchCommon ZenCore ZenEnterprise ${Boost_LIBRARIES} )

target_link_libraries( WorkbenchCommon ZenCore ZenEnterprise ${Boost_LIBRARIES} )

set_target_properties ( WorkbenchCommon 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( WorkbenchCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( WorkbenchCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_PropertiesPublisher.hpp
    I_ProjectServiceFactory.hpp
    I_ProjectWizardType.hpp
    I_SceneProperties.hpp
    I_SpreadSheetProperties.hpp
    I_SceneEntityProperties.hpp
    generic_scene_model_type.hpp
    I_ExplorerNodeFactory.hpp
    I_SpreadSheetCellProperties.hpp
    I_ProjectType.hpp
    I_ProjectAction.hpp
    I_ProjectWizardFactory.hpp
    I_SpreadSheetController.hpp
    I_SpreadSheetDocumentType.hpp
    I_ExplorerNodeType.hpp
    I_ProjectLocation.hpp
    I_SpreadSheetViewable.hpp
    II_ProjectTypes.hpp
    I_SpreadSheetAction.hpp
    I_View.hpp
    Project.hpp
    I_ProjectWizardManager.hpp
    I_SceneEntityFactory.hpp
    I_SceneNode.hpp
    generic_explorer_node_type.hpp
    I_ProjectTypes.hpp
    I_ProjectExplorerView.hpp
    generic_spreadsheet_document_type.hpp
    I_SpreadSheetCellFactory.hpp
    I_SpreadSheetView.hpp
    I_DocumentWizardService.hpp
    I_ExplorerNodeAction.hpp
    I_Project.hpp
    subscription.hpp
    I_ProjectWizardService.hpp
    I_SceneModel.hpp
    I_DocumentType.hpp
    I_DocumentWizardFactory.hpp
    indirect_explorer_node_action.hpp
    I_Property.hpp
    I_SceneController.hpp
    I_SceneAction.hpp
    I_PropertiesController.hpp
    I_SpreadSheetDocument.hpp
    I_ProjectExplorerController.hpp
    I_ProjectServiceManager.hpp
    I_PropertiesListener.hpp
    I_Viewable.hpp
    I_ProjectService.hpp
    II_SpreadSheetDocument.hpp
    I_ExplorerModel.hpp
    I_DocumentWizardManager.hpp
    Configuration.hpp
    I_ExplorerNode.hpp
    direct_explorer_node_action.hpp
    I_EnumProperty.hpp
    indirect_spread_sheet_action.hpp
    I_SpreadSheetDocumentFactory.hpp
    I_ProjectExplorerModel.hpp
    I_Document.hpp
    I_SceneEntity.hpp
    II_ExplorerNode.hpp
    I_SceneListener.hpp
    I_WorkbenchService.hpp
    I_SpreadSheetCell.hpp
    I_SceneViewable.hpp
    I_SceneModelType.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorkbenchCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)