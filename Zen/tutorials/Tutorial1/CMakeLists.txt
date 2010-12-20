cmake_minimum_required (VERSION 2.6) 

project(Tutorial1)

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED COMPONENTS system filesystem )
Find_Package(LibXml2 REQUIRED)

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
    Tutorial1\\
	REGULAR_EXPRESSION 
	Tutorial1/*.*
	)

source_group(
    Tutorial1\\src\\
	REGULAR_EXPRESSION 
	Tutorial1/src/*.*
	)


	
set ( Tutorial1_SRC
    Tutorial1/src/GameClient.cpp
    Tutorial1/src/GameClient.hpp
    Tutorial1/src/GameClientFactory.cpp
    Tutorial1/src/GameClientFactory.hpp
    Tutorial1/src/GameClientPlugin.cpp

    )

add_definitions( -DTutorial1_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} ${LibXML2_LIBRARY_DIRS} )

add_library ( Tutorial1 ${LIB_TYPE} ${Tutorial1_SRC} )

add_dependencies( Tutorial1 ZenCore ZenEngine BaseClient BaseCommon )

target_link_libraries( Tutorial1 ZenCore ZenEngine BaseClient BaseCommon ${Boost_LIBRARIES} ${LIBXML2_LIBRARIES} )

set ( CLIENT_PATH "${INDIEZEN_PARENT}/Zen/tutorials/Tutorial1" )

set ( CLIENT_LIBRARIES_PATH "${Boost_LIBRARY_DIRS};${PC_LIBXML_LIBDIR};${DEV_BIN}" )

create_zgameloader_vcproj_userfile(Tutorial1)

set_target_properties ( Tutorial1 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

configure_file(
	${INDIEZEN_PARENT}/Zen/tutorials/Tutorial1/client.xml.template
	${INDIEZEN_PARENT}/Zen/tutorials/Tutorial1/client.xml
	@ONLY
)					
					
if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( Tutorial1 PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( Tutorial1 PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)
