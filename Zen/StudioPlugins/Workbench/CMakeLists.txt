cmake_minimum_required (VERSION 2.6)

project(WorkbenchFramework)
set ( PROJECT_PREFIX "StudioPlugins/Workbench" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)


install ( FILES 
    schema/contributor.exsd
    schema/node.exsd
    schema/nodeAction.exsd
    schema/project.exsd
    schema/projectWizard.exsd
    schema/sscell.exsd
    DESTINATION
    Zen/${PROJECT_PREFIX}/schema/
)

install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)

