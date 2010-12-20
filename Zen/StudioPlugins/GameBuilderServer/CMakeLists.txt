cmake_minimum_required (VERSION 2.6) 

project (GameBuilderServer)
set ( PROJECT_PREFIX "StudioPlugins/GameBuilderServer" )

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


	
set ( GameBuilderServer_SRC
    src/CreatorCategoryProperty.cpp
    src/CreatorCategoryProperty.hpp
    src/CreatorIconProperty.cpp
    src/CreatorIconProperty.hpp
    src/CreatorPropertyGroup.cpp
    src/CreatorPropertyGroup.hpp
    src/DocumentProperties.cpp
    src/DocumentProperties.hpp
    src/ElementProperties.cpp
    src/ElementProperties.hpp
    src/ExplorerNodeFactory.cpp
    src/ExplorerNodeFactory.hpp
    src/ExtensionPointProperty.cpp
    src/ExtensionPointProperty.hpp
    src/Folder.cpp
    src/Folder.hpp
    src/GameBuilderPlugin.cpp
    src/GameObjectElement.cpp
    src/GameObjectElement.hpp
    src/GameObjectType.cpp
    src/GameObjectType.hpp
    src/GameObjectTypeController.cpp
    src/GameObjectTypeController.hpp
    src/GameObjectTypeDocument.cpp
    src/GameObjectTypeDocument.hpp
    src/PersistenceProperty.cpp
    src/PersistenceProperty.hpp
    src/Project.cpp
    src/Project.hpp
    src/ProjectService.cpp
    src/ProjectService.hpp
    src/ProjectServiceFactory.cpp
    src/ProjectServiceFactory.hpp
    src/Properties.cpp
    src/Properties.hpp
    src/Property.cpp
    src/Property.hpp
    src/PropertyGroup.cpp
    src/PropertyGroup.hpp
    src/ReplicationDirectionProperty.cpp
    src/ReplicationDirectionProperty.hpp
    src/ReplicationPropertyGroup.cpp
    src/ReplicationPropertyGroup.hpp
    src/ReplicationTypeProperty.cpp
    src/ReplicationTypeProperty.hpp
    src/TextCell.cpp
    src/TextCell.hpp
    src/TextCellData.cpp
    src/TextCellData.hpp

    )

add_definitions( -DGAMEBUILDERSERVER_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${wxWidgets_LIB_DIR} ${Boost_LIBRARY_DIRS} )

add_library ( GameBuilderServer ${LIB_TYPE} ${GameBuilderServer_SRC} )

add_dependencies(GameBuilderServer ZenCore ZenEngine ZenEnterprise ZenObjectModel WorkbenchCommon WorkbenchModel GameBuilderCommon GameBuilderModel )

target_link_libraries( GameBuilderServer ZenCore ZenEngine ZenEnterprise ZenObjectModel WorkbenchCommon WorkbenchModel GameBuilderCommon GameBuilderModel ${Boost_LIBRARIES} )

create_zenstudio_vcproj_userfile(GameBuilderServer)

set_target_properties ( GameBuilderServer 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( GameBuilderServer PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( GameBuilderServer PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	GameBuilderServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)