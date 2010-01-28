#	ZEN_PARENT 	- Parent of the Zen directory
#	DEV_LIB		- lib input and output directory
#	DEV_BIN		- bin output directory
#
#	(Win32 only - assuming other platforms can find these)
#
#       BOOST_ROOT 			- Boost Root 
#       BOOST_LIBRARYDIR 	- Boost Library 
#		PC_LIBXML_INCLUDE_DIRS
#		PC_LIBXML_LIBDIR

include(FindPkgMacros)

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ${ZEN_HOME}/.. ENV )
	message ( "-- Zen Parent: ${INDIEZEN_PARENT} ")

endif ( NOT INDIEZEN_PARENT)

set (ZEN_HOME "${INDIEZEN_PARENT}/Zen" )

if (NOT DEV_BIN)
	set ( DEV_BIN "${INDIEZEN_PARENT}/bin/" )
	message ( "-- Dev Bin: ${DEV_BIN}" )
endif (NOT DEV_BIN)
	
if (NOT DEV_LIB)
	set ( DEV_LIB "${INDIEZEN_PARENT}/lib/" )
	message ( "-- Dev Lib: ${DEV_LIB}" )
endif (NOT DEV_LIB)

if (NOT DEPENDENCIES_HOME)
    # If DEPENDENCIES_HOME environment is set, use it, otherwise default to 
	# "${INDIEZEN_PARENT}/dependencies/"
	message("DEPENDENCIES_HOME environment variable $ENV{DEPENDENCIES_HOME}")
	#if (NOT "$ENV{DEPENDENCIES_HOME}") # -- this line doesn't work... fix it and we fix the problem
	#	message("Defaulting DEPENDENCIES_HOME")
	#	set ( DEPENDENCIES_HOME "${INDIEZEN_PARENT}/dependencies/" )
	set (DEPENDENCIES_HOME "$ENV{DEPENDENCIES_HOME}")
	if(NOT DEPENDENCIES_HOME)
		set ( DEPENDENCIES_HOME "${INDIEZEN_PARENT}/dependencies/" )
	endif()
	message ( "-- Dependencies Home: ${DEPENDENCIES_HOME}" )		
endif (NOT DEPENDENCIES_HOME)

# Boost overrides
SET( Boost_USE_MULTITHREAD On )
SET( Boost_USE_STATIC_LIBS Off )
#SET( Boost_DEBUG On)

IF (WIN32)
	IF (NOT BOOST_ROOT)
		# Hack for BOOST_ROOT
		FIND_PATH(BOOST_ROOT 
			NAMES include/boost-1_35 include/boost-1_39
			HINTS "${INDIEZEN_PARENT}Boost" "${DEPENDENCIES_HOME}Boost"
		)

		FIND_PATH(BOOST_LIBRARYDIR
			NAMES libboost_date_time-vc80-mt-1_35.lib
			HINTS "${DEV_LIB}"
		)

		#message ("-- Boost Root: ${BOOST_ROOT}")
		#message ("-- Boost Lib: ${BOOST_LIBRARYDIR}")
	
	ENDIF(NOT BOOST_ROOT)


	set (PC_LIBXML_INCLUDE_DIRS 
        "${INDIEZEN_PARENT}/libxml2-2.6.30+.win32/include" 
        "${INDIEZEN_PARENT}/libxml2/include" 
        "${DEPENDENCIES_HOME}libxml2/include"
        )
	set (PC_LIBXML_LIBDIR 
        "${INDIEZEN_PARENT}/libxml2-2.6.30+.win32/lib"
        "${INDIEZEN_PARENT}/libxml2/lib"
        "${DEPENDENCIES_HOME}libxml2/lib"
        "${INDIEZEN_PARENT}/lib"
        )
        
    set (PC_LIBXML_BINDIR
        "${INDIEZEN_PARENT}/libxml2-2.6.30+.win32/bin"
        "${INDIEZEN_PARENT}/libxml2/bin"
        "${DEPENDENCIES_HOME}libxml2/bin"
        "${INDIEZEN_PARENT}/bin"
        )

	set(BUILD_EXE_TYPE "WIN32")
	set(Zen_DEFINITIONS "-D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE")

ENDIF (WIN32)
	
find_file ( THREAD_H thread.h ENV )
if ( SYNCH_H AND THREAD_H )
    add_definitions( -DHOST_SOLARIS )
endif ( SYNCH_H AND THREAD_H )

find_file ( UNISTD_H unistd.h ENV )
if ( UNISTD_H )
    add_definitions( -DHOST_POSIX )
    add_definitions( -DTHREADMODEL_POSIX )
endif ( UNISTD_H )

find_file ( WINDOWS_H windows.h ENV )
if ( WINDOWS_H )
    add_definitions( -DHOST_WIN32 )
	add_definitions( -DTHREADMODEL_WIN32 )
endif ( WINDOWS_H )

find_file (ZPKG_SCRIPT zpkg.lua 
	HINTS 
	${INDIEZEN_PARENT}
	${INDIEZEN_PARENT}/Zen/Setup/Package
)

function(create_zenstudio_vcproj_userfile TARGETNAME)
  if (MSVC)
    configure_file(
	  ${INDIEZEN_PARENT}/Zen/build/CMake/Templates/ZenStudio.vcproj.user.in
	  ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcproj.user
	  @ONLY
	)
  endif ()
endfunction(create_zenstudio_vcproj_userfile)

function(create_zgameloader_vcproj_userfile TARGETNAME)
	if (MSVC)
		configure_file(
			${INDIEZEN_PARENT}/Zen/build/CMake/Templates/ZGameLoader.vcproj.user.in
			${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcproj.user
			@ONLY
		)
	endif ()
endfunction(create_zgameloader_vcproj_userfile TARGETNAME CLIENT_PATH)

# Adding some more stuff for supporting "using" Zen (as opposed to building it)
FOREACH(COMPONENT ${IndieZen_FIND_COMPONENTS})
   STRING(TOUPPER ${COMPONENT} UPPER_COMPONENT)
   message("Looking for ${COMPONENT}")

   find_library(INDIEZEN_${UPPER_COMPONENT}_LIBRARY_DBG NAMES ${COMPONENT}_d HINTS ${DEV_LIB} )
   find_library(INDIEZEN_${UPPER_COMPONENT}_LIBRARY_REL NAMES ${COMPONENT} HINTS ${DEV_LIB} )
   message("Found ${INDIEZEN_${UPPER_COMPONENT}_LIBRARY_DBG}")
   message("Found ${INDIEZEN_${UPPER_COMPONENT}_LIBRARY_REL}")
ENDFOREACH(COMPONENT)

