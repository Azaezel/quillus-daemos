cmake_minimum_required (VERSION 2.6) 

project ( ZenPlugins )
#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Newer CMake Find system for locating Boost includes and libraries

SET( Boost_USE_MULTITHREAD On )
SET( Boost_USE_STATIC_LIBS Off )

Find_Package( Boost REQUIRED COMPONENTS filesystem system )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Newer CMake Find system for locating LibXml2 includes and libs

Find_Package(LibXml2)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# We want this to be a shared lib (.so, .dylib, or .dll)
SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

set ( Plugins_SRC
    src/Application.cpp
    src/Extension.cpp
    src/ExtensionPoint.cpp
    src/ExtensionQuery.cpp
    src/ExtensionRegistry.cpp
    src/I_Application.cpp
    src/I_ClassFactory.cpp
    src/I_Configuration.cpp
    src/I_ConfigurationElement.cpp
    src/I_Extension.cpp
    src/I_ExtensionPoint.cpp
    src/I_ExtensionQuery.cpp
    src/I_ExtensionRegistry.cpp
    src/I_Module.cpp
    src/I_ModuleInfo.cpp
    src/I_ModuleManager.cpp
    src/I_ModuleService.cpp
    src/I_Plugin.cpp
    src/I_PluginInfo.cpp
    src/I_PluginManager.cpp
    src/I_StartupShutdownParticipant.cpp
    src/ModuleInfo.cpp
    src/ModuleManager.cpp
    src/ModuleService.cpp
    src/PluginInfo.cpp
    src/PluginManager.cpp
    src/XMLConfiguration.cpp
    src/XMLConfigurationElement.cpp
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

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

add_library ( Plugins ${LIB_TYPE} ${Plugins_SRC} )

TARGET_LINK_LIBRARIES( Plugins Threading Utility ${LIBXML2_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} )

