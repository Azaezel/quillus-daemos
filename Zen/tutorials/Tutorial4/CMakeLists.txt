cmake_minimum_required (VERSION 2.6) 

project(Tutorial4)

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
    Tutorial4\\
	REGULAR_EXPRESSION 
	Tutorial4/*.*
	)

source_group(
    Tutorial4\\src\\
	REGULAR_EXPRESSION 
	Tutorial4/src/*.*
	)

source_group(
    scripts\\
	REGULAR_EXPRESSION 
	scripts/*.*
	)

source_group(
    scripts\\core\\
	REGULAR_EXPRESSION 
	scripts/core/*.*
	)

source_group(
    scripts\\client\\
	REGULAR_EXPRESSION 
	scripts/client/*.*
	)

source_group(
    resources\\
	REGULAR_EXPRESSION 
	resources/*.*
	)

source_group(
    resources\\Hydrax\\
	REGULAR_EXPRESSION 
	resources/Hydrax/*.*
	)


	
set ( Tutorial4_SRC
    Tutorial4/src/GameClient.hpp
    Tutorial4/src/GameClientFactory.hpp
    Tutorial4/src/AvatarGameObject.hpp
    Tutorial4/src/BehaviorGroup.cpp
    Tutorial4/src/BehaviorManager.cpp
    Tutorial4/src/WidgetManager.hpp
    Tutorial4/src/BehaviorManager.hpp
    Tutorial4/src/AvatarCollisionBehavior.hpp
    Tutorial4/src/GameClientFactory.cpp
    Tutorial4/src/GravityBehavior.cpp
    Tutorial4/src/GameClientPlugin.cpp
    Tutorial4/src/GameObject.hpp
    Tutorial4/src/GameClient.cpp
    Tutorial4/src/AvatarCollisionBehavior.cpp
    Tutorial4/src/AvatarGameObject.cpp
    Tutorial4/src/BehaviorGroup.hpp
    Tutorial4/src/GravityBehavior.hpp
    Tutorial4/src/WidgetManager.cpp
    Tutorial4/src/GameObject.cpp

    )

add_definitions( -DTUTORIAL4_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} ${LibXML2_LIBRARY_DIRS} )

add_library ( Tutorial4 ${LIB_TYPE} ${Tutorial4_SRC} )

add_dependencies( Tutorial4 ZenCore ZenEngine BaseClient BaseCommon )

target_link_libraries( Tutorial4 ZenCore ZenEngine BaseClient BaseCommon ${Boost_LIBRARIES} ${LIBXML2_LIBRARIES} )

set ( CLIENT_LIBRARIES_PATH "${Boost_LIBRARY_DIRS};${PC_LIBXML_LIBDIR};${OGRE_LIBRARY_DIRS};${DEV_BIN}" )

create_zgameloader_vcproj_userfile(Tutorial4)

set_target_properties ( Tutorial4 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

configure_file(
	${INDIEZEN_PARENT}/Zen/tutorials/Tutorial4/client.xml.template
	${INDIEZEN_PARENT}/Zen/tutorials/Tutorial4/client.xml
	@ONLY
)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( Tutorial4 PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( Tutorial4 PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)
