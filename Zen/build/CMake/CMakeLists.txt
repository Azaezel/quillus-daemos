cmake_minimum_required (VERSION 2.6) 

project (IndieZen)

set ( INDIEZEN_VERSION_MAJOR "0" )
set ( INDIEZEN_VERSION_MINOR "6" )
set ( INDIEZEN_VERSION_PATCH "1" )
set ( INDIEZEN_VERSION "${INDIEZEN_VERSION_MAJOR}.${INDIEZEN_VERSION_MINOR}.${INDIEZEN_VERSION_PATCH}" )


if ( WIN32 )
	set(BUILD_EXE_TYPE "WIN32")
	set(Zen_DEFINITIONS "-D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE")

    # TODO This should only be on MSVC not specifically WIN32.
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
endif ( WIN32 )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)

# Generate the cmake files using the zpkg
message("DEBUG: Running lua ${ZPKG_SCRIPT} from ${INDIEZEN_PARENT}")
execute_process(COMMAND lua ${ZPKG_SCRIPT} build generate ${INDIEZEN_PARENT}
	WORKING_DIRECTORY ${INDIEZEN_PARENT}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DEV_BIN}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DEV_LIB}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DEV_BIN}")

# Do the major Zen projects
#set ( ZEN_PROJECTS "Core;Engine;Enterprise;Studio/Workbench;Studio/WorkbenchCommon;Studio/WorkbenchModel;Studio/WorkbenchProtocol;Studio/WorkbenchServer;Studio/ZenStudio" )

set ( ZEN_PROJECTS
	Core
	Engine
	Enterprise
	Starter/Base/BaseCommon
	Starter/Base/BaseClient
	Starter/Base/ZGameLoader
	Spaces/ObjectModel
	plugins
)

foreach(PROJECT_PATH ${ZEN_PROJECTS})
	message("-- ${PROJECT_PATH}") 
	add_subdirectory("${ZEN_HOME}/${PROJECT_PATH}" "${PROJECT_PATH}")
endforeach(PROJECT_PATH)

option(ZEN_BUILD_TESTS "Build Tests" FALSE)
if (ZEN_BUILD_TESTS)

	set ( ZEN_TEST_PROJECTS
		tests/EventTest
	)

    foreach(PROJECT_PATH ${ZEN_TEST_PROJECTS})
        message("-- ${PROJECT_PATH}") 
        add_subdirectory("${ZEN_HOME}/${PROJECT_PATH}" "${PROJECT_PATH}")
    endforeach(PROJECT_PATH)
	
endif(ZEN_BUILD_TESTS)

option(ZEN_BUILD_TUTORIALS "Build Tutorials" TRUE)
if (ZEN_BUILD_TUTORIALS)

    set ( ZEN_TUTORIAL_PROJECTS
        tutorials/Tutorial1
        tutorials/Tutorial2
        tutorials/Tutorial3
        tutorials/Tutorial4
        tutorials/Tutorial5
    )

    foreach(PROJECT_PATH ${ZEN_TUTORIAL_PROJECTS})
        message("-- ${PROJECT_PATH}") 
        add_subdirectory("${ZEN_HOME}/${PROJECT_PATH}" "${PROJECT_PATH}")
    endforeach(PROJECT_PATH)    
endif(ZEN_BUILD_TUTORIALS)

# Do the major Studio projects
option(ZEN_BUILD_ZEN_STUDIO "Build Zen Studio" TRUE)
if (ZEN_BUILD_ZEN_STUDIO)

    set ( ZEN_STUDIO_PROJECTS
        Studio/WorkbenchCommon
        Studio/Workbench
        Studio/WorkbenchModel
        Studio/WorkbenchProtocol
        Studio/WorkbenchServer
        Studio/ZenStudio
		StudioPlugins/Workbench
    )

    foreach(PROJECT_PATH ${ZEN_STUDIO_PROJECTS})
        message("-- ${PROJECT_PATH}") 
        add_subdirectory("${ZEN_HOME}/${PROJECT_PATH}" "${PROJECT_PATH}")
    endforeach(PROJECT_PATH)

    # Do the Game Builder plugin
    option(ZEN_BUILD_GAME_BUILDER "Build Zen Studio Game Builder plugin" FALSE)
    if (ZEN_BUILD_GAME_BUILDER)

        set ( ZEN_GAME_BUILDER_PROJECTS
            StudioPlugins/GameBuilderCommon
            StudioPlugins/GameBuilder
            StudioPlugins/GameBuilderModel
        #	StudioPlugins/GameBuilderProtocol
            StudioPlugins/GameBuilderServer
        )

        foreach(PROJECT_PATH ${ZEN_GAME_BUILDER_PROJECTS})
            message("-- ${PROJECT_PATH}") 
            add_subdirectory("${ZEN_HOME}/${PROJECT_PATH}" "${PROJECT_PATH}")
        endforeach(PROJECT_PATH)
    endif()

    # Do the World Builder plugin
    option(ZEN_BUILD_WORLD_BUILDER "Build Zen Studio World Builder plugin" FALSE)
    if (ZEN_BUILD_WORLD_BUILDER)

        set ( ZEN_WORLD_BUILDER_PROJECTS
            StudioPlugins/WorldBuilderCommon
            StudioPlugins/WorldBuilder
            StudioPlugins/WorldBuilderModel
       		#StudioPlugins/WorldBuilderProtocol
            StudioPlugins/WorldBuilderServer
        )

        foreach(PROJECT_PATH ${ZEN_WORLD_BUILDER_PROJECTS})
            message("-- ${PROJECT_PATH}") 
            add_subdirectory("${ZEN_HOME}/${PROJECT_PATH}" "${PROJECT_PATH}")
        endforeach(PROJECT_PATH)
    endif()

    # Do the Art Library plugin
    option(ZEN_BUILD_ART_LIBRARY "Build Zen Studio Art Library plugin" FALSE)
    if (ZEN_BUILD_ART_LIBRARY)

        set ( ZEN_ART_LIBRARY_PROJECTS
            StudioPlugins/ArtLibraryCommon
            StudioPlugins/ArtLibrary
            StudioPlugins/ArtLibraryModel
        #   StudioPlugins/ArtLibraryProtocol
            StudioPlugins/ArtLibraryServer
        )

        foreach(PROJECT_PATH ${ZEN_ART_LIBRARY_PROJECTS})
            message("-- ${PROJECT_PATH}") 
            add_subdirectory("${ZEN_HOME}/${PROJECT_PATH}" "${PROJECT_PATH}")
        endforeach(PROJECT_PATH)
    endif()
    
endif()

# Do the experimental tests
option(ZEN_BUILD_EXPERIMENTAL_TESTS "Build Experimental Tests" FALSE)
if(ZEN_BUILD_EXPERIMENTAL_TESTS)

	set(ZEN_EXPERIMENTAL_TESTS_PROJECTS
		tests/ScriptTest
	)
	
	foreach(PROJECT_PATH ${ZEN_EXPERIMENTAL_TESTS_PROJECTS})
		message("-- ${PROJECT_PATH}")
		add_subdirectory("${ZEN_HOME}/${PROJECT_PATH}" "${PROJECT_PATH}")
	endforeach(PROJECT_PATH)
endif()

# Do Zen Worlds (optionally)
option(ZEN_BUILD_ZEN_WORLDS "Build Zen Worlds" FALSE)

if (ZEN_BUILD_ZEN_WORLDS)
    add_subdirectory("${ZEN_HOME}/Worlds" "Worlds")
endif()


# Do Zen Community (optionally)
option(ZEN_BUILD_ZEN_COMMUNITY "Build Zen Community" FALSE)

if (ZEN_BUILD_ZEN_COMMUNITY)
    add_subdirectory("${ZEN_HOME}/Community" "Community")
endif()

include(Packaging)
