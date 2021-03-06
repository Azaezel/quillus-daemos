cmake_minimum_required (VERSION 2.6) 

project (GameBuilder)
set ( PROJECT_PREFIX "StudioPlugins/GameBuilder" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

Find_Package(wxWidgets COMPONENTS aui adv base core REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( GameBuilder_SRC
    Configuration.hpp
    src/ContributorService.cpp
    src/ContributorService.hpp
    src/ContributorServiceFactory.cpp
    src/ContributorServiceFactory.hpp
    src/FolderContributor.cpp
    src/FolderContributor.hpp
    src/GameBuilderPlugin.cpp
    src/GameObjectTypeContributor.cpp
    src/GameObjectTypeContributor.hpp

    )
# TR - I removed ${wxWidgets_DEFINITIONS}  from add_definitions 
# This made it work on Linux, but does it break other OS?
add_definitions( -DGAMEBUILDER_EXPORTS ${wxWidgets_CXX_FLAGS} ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${wxWidgets_INCLUDE_DIRS} )

link_directories ( ${DEV_LIB} ${wxWidgets_LIB_DIR} ${Boost_LIBRARY_DIRS} )

add_library ( GameBuilder ${LIB_TYPE} ${GameBuilder_SRC} )

add_dependencies(GameBuilder ZenCore ZenEngine ZenEnterprise Workbench GameBuilderCommon)

message("wxWidgets Libs: ${wxWidgets_LIBRARIES}")

target_link_libraries( GameBuilder ZenCore ZenEngine ZenEnterprise Workbench GameBuilderCommon ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES} )

set_target_properties ( GameBuilder 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( GameBuilder PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( GameBuilder PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	GameBuilder
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
