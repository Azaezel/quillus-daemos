cmake_minimum_required (VERSION 2.6) 

project (SessionProtocol)
set ( PROJECT_PREFIX "Community/SessionProtocol" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( SessionProtocol_SRC
    Configuration.hpp
    I_LoginRequest.hpp
    I_LoginResponse.hpp
    I_Message.hpp
    I_SessionProtocolManager.hpp
    src/I_LoginRequest.cpp
    src/I_LoginResponse.cpp
    src/I_Message.cpp
    src/I_SessionProtocolManager.cpp
    src/LoginRequest.cpp
    src/LoginRequest.hpp
    src/LoginResponse.cpp
    src/LoginResponse.hpp
    src/Message.cpp
    src/Message.hpp
    src/MessageFactory.cpp
    src/MessageFactory.hpp
    src/Request.cpp
    src/Request.hpp
    src/Response.cpp
    src/Response.hpp
    src/SessionProtocolManager.cpp
    src/SessionProtocolManager.hpp
    src/SessionProtocolPlugin.cpp

    )

add_definitions( -DSESSIONPROTOCOL_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( SessionProtocol ${LIB_TYPE} ${SessionProtocol_SRC} )

add_dependencies(SessionProtocol ZenCore ZenEngine ZenEnterprise SessionCommon)

target_link_libraries(SessionProtocol ZenCore ZenEngine ZenEnterprise SessionCommon ${Boost_LIBRARIES} )

set_target_properties ( SessionProtocol
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( SessionProtocol PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( SessionProtocol PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_LoginRequest.hpp
    I_LoginResponse.hpp
    I_Message.hpp
    I_SessionProtocolManager.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	SessionProtocol
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)