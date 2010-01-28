cmake_minimum_required (VERSION 2.4) 

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Newer CMake Find system for locating Boost includes and libraries

SET( Boost_USE_MULTITHREAD On )
SET( Boost_USE_STATIC_LIBS Off )

Find_Package( Boost REQUIRED COMPONENTS system )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# We want this to be a shared lib (.so, .dylib, or .dll)
SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Look for libraries in frameworks last. Should help find the 
#  correct (Zen version) of libScripting.
if( APPLE )
    SET( CMAKE_FIND_FRAMEWORK LAST )
endif( APPLE )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
project ( ZenEvent )
set ( Resource_SRC
    src/CompoundResource.cpp
    src/FileStream.cpp
    src/I_CompoundResource.cpp
    src/I_Exporter.cpp
    src/I_ExporterFactory.cpp
    src/I_Importer.cpp
    src/I_ImporterFactory.cpp
    src/I_Resource.cpp
    src/I_ResourceFactory.cpp
    src/I_ResourceManager.cpp
    src/I_ResourceService.cpp
    src/I_ResourceServiceFactory.cpp
    src/I_ResourceStream.cpp
    src/I_ResourceStreamFactory.cpp
    src/ResourceManager.cpp
    )

find_file ( SYNCH_H synch.h ENV )
if ( SYNCH_H )
    add_definitions( -DTHREADMODEL_SOLARIS )
endif( SYNCH_H )

find_file ( PTHREAD_H pthread.h ENV )
if ( PTHREAD_H )
    add_definitions( -DTHREADMODEL_POSIX )

    set ( CPPFLAGS
        ${CPPFLAGS}
        -pthread
        )

    set ( LDFLAGS 
        -pthread
        )
endif ( PTHREAD_H )

find_file ( WINDOWS_H windows.h ENV )
if ( WINDOWS_H )
    add_definitions( -DTHREADMODEL_WIN32 )
endif ( WINDOWS_H )

find_file ( THREAD_H thread.h ENV )
if ( SYNCH_H AND THREAD_H )
    add_definitions( -DHOST_SOLARIS )
endif ( SYNCH_H AND THREAD_H )

find_file ( UNISTD_H unistd.h ENV )
if ( UNISTD_H )
    add_definitions( -DHOST_POSIX )
endif ( UNISTD_H )

find_file ( WINDOWS_H windows.h ENV )
if ( WINDOWS_H )
    add_definitions( -DHOST_WIN32 )
endif ( WINDOWS_H )

find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )

find_library( ZEN_THREADING_LIBRARY Threading ${CMAKE_BINARY_DIR}/../Core/Threading ${INDIEZEN_PARENT}/Zen/Core/Threading )
if( NOT ZEN_THREADING_LIBRARY )
    message( "Could not find the path to libThreading! Please set ZEN_THREADING_LIBRARY to the proper value." )
elseif( ZEN_THREADING_LIBRARY )
    message( "-- Found libThreading at ${ZEN_THREADING_LIBRARY}" )
endif( NOT ZEN_THREADING_LIBRARY )

find_library( ZEN_PLUGINS_LIBRARY Plugins ${CMAKE_BINARY_DIR}/../Core/Plugins ${INDIEZEN_PARENT}/Zen/Core/Plugins )
if( NOT ZEN_PLUGINS_LIBRARY )
    message( "Could not find the path to libPlugins! Please set ZEN_PLUGINS_LIBRARY to the proper value." )
elseif( ZEN_PLUGINS_LIBRARY )
    message( "-- Found libPlugins at ${ZEN_PLUGINS_LIBRARY}" )
endif( NOT ZEN_PLUGINS_LIBRARY )

find_library( ZEN_SCRIPTING_LIBRARY Scripting ${CMAKE_BINARY_DIR}/../Core/Scripting ${INDIEZEN_PARENT}/Zen/Core/Scripting )
if( NOT ZEN_SCRIPTING_LIBRARY )
    message( "Could not find the path to libScripting! Please set ZEN_SCRIPTING_LIBRARY to the proper value." )
elseif( ZEN_SCRIPTING_LIBRARY )
    message( "-- Found libScripting at ${ZEN_SCRIPTING_LIBRARY}" )
endif( NOT ZEN_SCRIPTING_LIBRARY )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ../../.. )

add_library ( Resource ${LIB_TYPE} ${Resource_SRC} )

TARGET_LINK_LIBRARIES( Resource ${ZEN_THREADING_LIBRARY} ${ZEN_PLUGINS_LIBRARY} ${ZEN_SCRIPTING_LIBRARY} ${Boost_SYSTEM_LIBRARY} )

