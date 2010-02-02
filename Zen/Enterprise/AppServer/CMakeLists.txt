# Zen/Enterprise/AppServer CMakeLists.txt
#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Newer CMake Find system for locating Boost includes and libraries
cmake_minimum_required(VERSION 2.6)

SET( Boost_USE_MULTITHREAD On )
SET( Boost_USE_STATIC_LIBS Off )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# We want this to be a shared lib (.so, .dylib, or .dll)
SET (LIB_TYPE SHARED)
#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Look for libraries in frameworks last. Should help find the 
#  correct (Zen version) of libScripting.
if( APPLE )
    SET( CMAKE_FIND_FRAMEWORK LAST )
endif( APPLE )
#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
Add_Library( AppServer_SRC
    #in AppServer
    Configuration.hpp
    I_ApplicationServer.hpp
    I_ApplicationServerManager.hpp
    I_ApplicationService.hpp
    I_ApplicationServiceFactory.hpp
    I_Message.hpp
    I_ProtocolService.hpp
    I_ProtocolServiceFactory.hpp
    I_Request.hpp
    I_RequestHandler.hpp
    I_ResourceLocation.hpp
    I_Response.hpp
    I_ResponseHandler.hpp
    I_SessionEvent.hpp
    #in AppServer/src
    src/ApplicationServer.hpp
    src/ApplicationServer.cpp
    src/ApplicationServerManager.hpp
    src/ApplicationServerManager.cpp
    src/I_ApplicationServer.cpp
    src/I_ApplicationServerManager.cpp
    src/I_ApplicationService.cpp
    src/I_ApplicationServiceFactory.cpp
    src/I_Message.cpp
    src/I_ProtocolService.cpp
    src/I_ProtocolServiceFactory.cpp
    src/I_Request.cpp
    src/I_RequestHandler.cpp
    src/I_ResourceLocation.cpp
    src/I_Response.cpp
    src/I_ResponseHandler.cpp
    src/QueuedRequestHandler.hpp
    src/ResourceLocation.hpp
    src/ResourceLocation.cpp
)
