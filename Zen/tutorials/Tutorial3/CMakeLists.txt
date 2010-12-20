cmake_minimum_required (VERSION 2.6) 

project(Tutorial3)

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED COMPONENTS system filesystem )
Find_Package(LibXml2 REQUIRED)
Find_Package(OGRE REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    resources\\
	REGULAR_EXPRESSION 
	resources/*.*
	)

source_group(
    scripts\\
	REGULAR_EXPRESSION 
	scripts/*.*
	)

source_group(
    scripts\\client\\
	REGULAR_EXPRESSION 
	scripts/client/*.*
	)

source_group(
    scripts\\core\\
	REGULAR_EXPRESSION 
	scripts/core/*.*
	)

source_group(
    Tutorial3\\
	REGULAR_EXPRESSION 
	Tutorial3/*.*
	)

source_group(
    Tutorial3\\src\\
	REGULAR_EXPRESSION 
	Tutorial3/src/*.*
	)


	
set ( Tutorial3_SRC
    Tutorial3/src/GameClient.cpp
    Tutorial3/src/GameClient.hpp
    Tutorial3/src/GameClientFactory.cpp
    Tutorial3/src/GameClientFactory.hpp
    Tutorial3/src/GameClientPlugin.cpp
    Tutorial3/src/WidgetManager.cpp
    Tutorial3/src/WidgetManager.hpp

    )

add_definitions( -DTUTORIAL3_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )
#${OGRE_INCLUDE_DIR} 

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} ${LibXML2_LIBRARY_DIRS} )

add_library ( Tutorial3 ${LIB_TYPE} ${Tutorial3_SRC} )

add_dependencies( Tutorial3 ZenCore ZenEngine BaseClient BaseCommon )

target_link_libraries( Tutorial3 ZenCore ZenEngine BaseClient BaseCommon ${Boost_LIBRARIES} ${LIBXML2_LIBRARIES} )

set ( CLIENT_LIBRARIES_PATH "${Boost_LIBRARY_DIRS};${PC_LIBXML_LIBDIR};${OGRE_LIBRARY_DIRS};${DEV_BIN}" )

create_zgameloader_vcproj_userfile(Tutorial3)

set_target_properties ( Tutorial3 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

configure_file(
	${INDIEZEN_PARENT}/Zen/tutorials/Tutorial3/client.xml.template
	${INDIEZEN_PARENT}/Zen/tutorials/Tutorial3/client.xml
	@ONLY
)
					
if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( Tutorial3 PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( Tutorial3 PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)
