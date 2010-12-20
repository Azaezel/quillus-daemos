cmake_minimum_required (VERSION 2.6) 

project(Tutorial5)

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
    Tutorial5\\
	REGULAR_EXPRESSION 
	Tutorial5/*.*
	)

source_group(
    Tutorial5\\src\\
	REGULAR_EXPRESSION 
	Tutorial5/src/*.*
	)


	
set ( Tutorial5_SRC
    Tutorial5/src/AvatarCollisionBehavior.cpp
    Tutorial5/src/AvatarCollisionBehavior.hpp
    Tutorial5/src/AvatarGameObject.cpp
    Tutorial5/src/AvatarGameObject.hpp
    Tutorial5/src/BehaviorGroup.cpp
    Tutorial5/src/BehaviorGroup.hpp
    Tutorial5/src/BehaviorManager.cpp
    Tutorial5/src/BehaviorManager.hpp
    Tutorial5/src/GameClient.cpp
    Tutorial5/src/GameClient.hpp
    Tutorial5/src/GameClientFactory.cpp
    Tutorial5/src/GameClientFactory.hpp
    Tutorial5/src/GameClientPlugin.cpp
    Tutorial5/src/GameObject.cpp
    Tutorial5/src/GameObject.hpp
    Tutorial5/src/GravityBehavior.cpp
    Tutorial5/src/GravityBehavior.hpp

    )

add_definitions( -DTUTORIAL5_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} ${LibXML2_LIBRARY_DIRS} )

add_library ( Tutorial5 ${LIB_TYPE} ${Tutorial5_SRC} )

add_dependencies( Tutorial5 ZenCore ZenEngine BaseClient BaseCommon )

target_link_libraries( Tutorial5 ZenCore ZenEngine BaseClient BaseCommon ${Boost_LIBRARIES} ${LIBXML2_LIBRARIES} )

set ( CLIENT_LIBRARIES_PATH "${Boost_LIBRARY_DIRS};${PC_LIBXML_LIBDIR};${OGRE_LIBRARY_DIRS};${DEV_BIN}" )

create_zgameloader_vcproj_userfile(Tutorial5)

set_target_properties ( Tutorial5 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

configure_file(
	${INDIEZEN_PARENT}/Zen/tutorials/Tutorial5/client.xml.template
	${INDIEZEN_PARENT}/Zen/tutorials/Tutorial5/client.xml
	@ONLY
)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( Tutorial5 PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( Tutorial5 PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)
