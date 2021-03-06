cmake_minimum_required (VERSION 2.6) 

project (ZFMOD)
set ( PROJECT_PREFIX "plugins/ZFMOD" )
if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED system )
Find_Package(FMOD REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZOpenAL source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZFMOD_SRC
    Configuration.hpp
    src/FMODService.cpp
    src/FMODService.hpp
    src/ResourceService.cpp
    src/ResourceService.hpp
    src/ResourceServiceFactory.cpp
    src/ResourceServiceFactory.hpp
    src/SoundResource.cpp
    src/SoundResource.hpp
    src/SoundService.cpp
    src/SoundService.hpp
    src/SoundServiceFactory.cpp
    src/SoundServiceFactory.hpp
    src/Source.cpp
    src/Source.hpp
    src/ZFMODModule.cpp
    src/ZFMODPlugin.cpp
    src/ZFMODPlugin.hpp
    ZFMODModule.hpp

    )

add_definitions( -DZFMOD_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${FMOD_INCLUDE_DIR})

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZFMOD ${LIB_TYPE} ${ZFMOD_SRC} )

add_dependencies( ZFMOD ZenCore ZenEngine )

target_link_libraries( ZFMOD ZenCore ZenEngine optimized ${FMOD_LIBRARY} debug ${FMOD_DEBUG_LIBRARY})

set_target_properties ( ZFMOD 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZFMOD PROPERTIES PREFIX "../../bin/" )
    set_target_properties ( ZFMOD PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    plugin.xml
    ZFMODModule.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZFMOD
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)