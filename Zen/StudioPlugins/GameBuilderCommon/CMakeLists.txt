cmake_minimum_required (VERSION 2.6) 

project (GameBuilderCommon)
set ( PROJECT_PREFIX "StudioPlugins/GameBuilderCommon" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )
Find_Package(LibXml2 REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of GameBuilderCommon

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( GameBuilderCommon_SRC
    I_ProjectListener.hpp
    I_GameObjectTypeDocument.hpp
    I_GameBuilderService.hpp
    I_GameObjectTypeController.hpp
    I_Project.hpp
    src/I_GameObjectTypeController.cpp
    src/I_ProjectListener.cpp
    src/I_Folder.cpp
    src/I_GameObjectElement.cpp
    src/I_ProjectService.cpp
    src/I_GameBuilderService.cpp
    src/I_GameObjectTypeDocument.cpp
    src/I_GameObjectTypeView.cpp
    src/I_GameObjectType.cpp
    src/I_Project.cpp
    I_GameObjectElement.hpp
    I_GameObjectType.hpp
    I_GameObjectTypeView.hpp
    I_ProjectService.hpp
    Configuration.hpp
    I_Folder.hpp

    )

add_definitions( -DGAMEBUILDERCOMMON_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${wxWidgets_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS} )

add_library ( GameBuilderCommon ${LIB_TYPE} ${GameBuilderCommon_SRC} )

add_dependencies(GameBuilderCommon ZenCore ZenEnterprise WorkbenchCommon ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES} )

target_link_libraries( GameBuilderCommon ZenCore ZenEnterprise WorkbenchCommon ${Boost_LIBRARIES} )

set_target_properties ( GameBuilderCommon 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( GameBuilderCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( GameBuilderCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_ProjectListener.hpp
    I_GameObjectTypeDocument.hpp
    I_GameBuilderService.hpp
    I_GameObjectTypeController.hpp
    I_Project.hpp
    I_GameObjectElement.hpp
    I_GameObjectType.hpp
    I_GameObjectTypeView.hpp
    I_ProjectService.hpp
    Configuration.hpp
    I_Folder.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	GameBuilderCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)