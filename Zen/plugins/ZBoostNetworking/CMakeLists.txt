cmake_minimum_required (VERSION 2.6) 

project (ZBoostNetworking)
set ( PROJECT_PREFIX "plugins/ZBoostNetworking" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED serialization )

message("Libraries: ${Boost_LIBRARIES}")

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZBoostNetworking source file

source_group(
    UDP\\
	REGULAR_EXPRESSION 
	UDP/*.*
	)

source_group(
    XML\\
	REGULAR_EXPRESSION 
	XML/*.*
	)

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)

source_group(
    src\\UDP\\
	REGULAR_EXPRESSION 
	src/UDP/*.*
	)

source_group(
    src\\XML\\
	REGULAR_EXPRESSION 
	src/XML/*.*
	)

source_group(
    src\\HTTP\\
	REGULAR_EXPRESSION 
	src/HTTP/*.*
	)

source_group(
    src\\TCP\\
	REGULAR_EXPRESSION 
	src/TCP/*.*
	)


	
set ( ZBoostNetworking_SRC
    UDP/I_Message.hpp
    UDP/I_UDPService.hpp
    XML/I_XMLResponse.hpp
    XML/I_XMLRequest.hpp
    XML/I_XMLProtocolService.hpp
    I_WebProtocolService.hpp
    src/ProtocolServiceFactory.hpp
    src/UDP/SendTaskAllocator.cpp
    src/UDP/Address.hpp
    src/UDP/SendTask.hpp
    src/UDP/Message.hpp
    src/UDP/I_Message.cpp
    src/UDP/Message.cpp
    src/UDP/Address.cpp
    src/UDP/SendTaskAllocator.hpp
    src/UDP/Endpoint.cpp
    src/UDP/MessageBuffer.hpp
    src/UDP/I_UDPService.cpp
    src/UDP/Endpoint.hpp
    src/UDP/MessageBuffer.cpp
    src/XML/RequestParser.hpp
    src/XML/RequestGrammar.hpp
    src/XML/RequestParserState.cpp
    src/XML/Request.hpp
    src/XML/RequestParser.cpp
    src/XML/XMLProtocolService.cpp
    src/XML/Request.cpp
    src/XML/I_XMLResponse.cpp
    src/XML/Message.hpp
    src/XML/I_XMLProtocolService.cpp
    src/XML/Message.cpp
    src/XML/Endpoint.cpp
    src/XML/Endpoint.hpp
    src/XML/RequestHandler.hpp
    src/XML/RequestGrammar.cpp
    src/XML/Reply.cpp
    src/XML/Connection.hpp
    src/XML/Header.hpp
    src/XML/RequestParserState.hpp
    src/XML/XMLProtocolService.hpp
    src/XML/I_XMLRequest.cpp
    src/XML/RequestGrammar_impl.hpp
    src/XML/RequestHandler.cpp
    src/XML/Connection.cpp
    src/XML/Reply.hpp
    src/UserDatagramProtocolService.hpp
    src/TransmissionControlProtocolService.cpp
    src/HTTP/RequestParser.hpp
    src/HTTP/RequestGrammar.hpp
    src/HTTP/RequestParserState.cpp
    src/HTTP/Request.hpp
    src/HTTP/RequestParser.cpp
    src/HTTP/Request.cpp
    src/HTTP/Message.hpp
    src/HTTP/I_WebProtocolService.cpp
    src/HTTP/Message.cpp
    src/HTTP/RequestHandler.hpp
    src/HTTP/RequestGrammar.cpp
    src/HTTP/Reply.cpp
    src/HTTP/Connection.hpp
    src/HTTP/Header.hpp
    src/HTTP/RequestParserState.hpp
    src/HTTP/RequestGrammar_impl.hpp
    src/HTTP/RequestHandler.cpp
    src/HTTP/Connection.cpp
    src/HTTP/I_WebResponse.cpp
    src/HTTP/Reply.hpp
    src/HTTP/I_WebRequest.cpp
    src/HyperTextTransportProtocolService.cpp
    src/Endpoint.cpp
    src/ProtocolServiceFactory.cpp
    src/HyperTextTransportProtocolService.hpp
    src/Endpoint.hpp
    src/UserDatagramProtocolService.cpp
    src/TCP/Message.hpp
    src/TCP/Message.cpp
    src/TCP/Connection.hpp
    src/TCP/Connection.cpp
    src/ZBoostNetworkingPlugin.cpp
    src/TransmissionControlProtocolService.hpp
    I_WebRequest.hpp
    message_factory.hpp
    Configuration.hpp
    I_WebResponse.hpp
    message_serialization_adapter.hpp

    )

add_definitions( -DZBOOSTNETWORKING_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIRS} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS}  )

add_library ( ZBoostNetworking ${LIB_TYPE} ${ZBoostNetworking_SRC} )

add_dependencies(ZBoostNetworking ZenCore ZenEnterprise ZenObjectModel )

target_link_libraries( ZBoostNetworking ZenCore ZenEnterprise ZenObjectModel ${Boost_LIBRARIES} )

set_target_properties ( ZBoostNetworking 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZBoostNetworking PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZBoostNetworking PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    UDP/I_Message.hpp
    UDP/I_UDPService.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/UDP/
)

install ( FILES 
    XML/I_XMLResponse.hpp
    XML/I_XMLRequest.hpp
    XML/I_XMLProtocolService.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/XML/
)

install ( FILES 
    plugin.xml
    I_WebProtocolService.hpp
    I_WebRequest.hpp
    message_factory.hpp
    Configuration.hpp
    I_WebResponse.hpp
    message_serialization_adapter.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZBoostNetworking
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)