cmake_minimum_required (VERSION 2.6) 

project (ZenCore)
set ( PROJECT_PREFIX "Core" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ${CMAKE_CURRENT_SOURCE_DIR}/../.. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(LibXml2 REQUIRED)
Find_Package(Boost REQUIRED serialization system filesystem)
Find_Package(Threads)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Core

source_group(
    Event\\
	REGULAR_EXPRESSION 
	Event/*.*
	)

source_group(
    Event\\src\\
	REGULAR_EXPRESSION 
	Event/src/*.*
	)

source_group(
    Math\\
	REGULAR_EXPRESSION 
	Math/*.*
	)

source_group(
    Math\\src\\
	REGULAR_EXPRESSION 
	Math/src/*.*
	)

source_group(
    Memory\\
	REGULAR_EXPRESSION 
	Memory/*.*
	)

source_group(
    Memory\\src\\
	REGULAR_EXPRESSION 
	Memory/src/*.*
	)

source_group(
    Plugins\\
	REGULAR_EXPRESSION 
	Plugins/*.*
	)

source_group(
    Plugins\\src\\
	REGULAR_EXPRESSION 
	Plugins/src/*.*
	)

source_group(
    Scripting\\
	REGULAR_EXPRESSION 
	Scripting/*.*
	)

source_group(
    Scripting\\src\\
	REGULAR_EXPRESSION 
	Scripting/src/*.*
	)

source_group(
    Threading\\
	REGULAR_EXPRESSION 
	Threading/*.*
	)

source_group(
    Threading\\src\\
	REGULAR_EXPRESSION 
	Threading/src/*.*
	)

source_group(
    Utility\\
	REGULAR_EXPRESSION 
	Utility/*.*
	)

source_group(
    Utility\\src\\
	REGULAR_EXPRESSION 
	Utility/src/*.*
	)



set ( ZenCore_SRC
    Event/Configuration.hpp
    Event/Connection.hpp
    Event/Event.hpp
    Event/future_return_value.hpp
    Event/I_Action.hpp
    Event/I_ActionMap.hpp
    Event/I_Connection.hpp
    Event/I_Event.hpp
    Event/I_EventManager.hpp
    Event/I_EventQueue.hpp
    Event/I_EventService.hpp
    Event/src/Action.cpp
    Event/src/Action.hpp
    Event/src/ActionMap.cpp
    Event/src/ActionMap.hpp
    Event/src/Connection_impl.cpp
    Event/src/Connection_impl.hpp
    Event/src/Event.cpp
    Event/src/EventManager.cpp
    Event/src/EventManager.hpp
    Event/src/EventQueue.cpp
    Event/src/EventQueue.hpp
    Event/src/EventService.cpp
    Event/src/EventService.hpp
    Event/src/Event_impl.hpp
    Event/src/I_Action.cpp
    Event/src/I_ActionMap.cpp
    Event/src/I_Connection.cpp
    Event/src/I_Event.cpp
    Event/src/I_EventManager.cpp
    Event/src/I_EventQueue.cpp
    Event/src/I_EventService.cpp
    Event/src/ScriptAction.cpp
    Event/src/ScriptAction.hpp
    Math/ComplexReal.hpp
    Math/Configuration.hpp
    Math/Degree.hpp
    Math/Math.hpp
    Math/Matrix3.hpp
    Math/Matrix4.hpp
    Math/Point3.hpp
    Math/Quaternion4.hpp
    Math/Radian.hpp
    Math/Ray.hpp
    Math/src/ComplexReal.cpp
    Math/src/Degree.cpp
    Math/src/Math.cpp
    Math/src/Matrix3.cpp
    Math/src/Matrix4.cpp
    Math/src/Point3.cpp
    Math/src/Quaternion4.cpp
    Math/src/Radian.cpp
    Math/src/Ray.cpp
    Math/src/Vector3.cpp
    Math/src/Vector4.cpp
    Math/Vector3.hpp
    Math/Vector4.hpp
    Memory/Configuration.hpp
    Memory/managed_iterator.hpp
    Memory/managed_payload.hpp
    Memory/managed_ptr.hpp
    Memory/managed_self_ref.hpp
    Memory/managed_weak_ptr.hpp
    Memory/src/managed_ptr.cpp
    Plugins/Configuration.hpp
    Plugins/I_Application.hpp
    Plugins/I_ClassFactory.hpp
    Plugins/I_Configuration.hpp
    Plugins/I_ConfigurationElement.hpp
    Plugins/I_Extension.hpp
    Plugins/I_ExtensionPoint.hpp
    Plugins/I_ExtensionQuery.hpp
    Plugins/I_ExtensionRegistry.hpp
    Plugins/I_Module.hpp
    Plugins/I_ModuleInfo.hpp
    Plugins/I_ModuleManager.hpp
    Plugins/I_ModuleService.hpp
    Plugins/I_Plugin.hpp
    Plugins/I_PluginInfo.hpp
    Plugins/I_PluginManager.hpp
    Plugins/I_Service.hpp
    Plugins/I_ServiceInfo.hpp
    Plugins/I_StartupShutdownManager.hpp
    Plugins/I_StartupShutdownParticipant.hpp
    Plugins/ServiceCache.hpp
    Plugins/src/Application.cpp
    Plugins/src/Application.hpp
    Plugins/src/Extension.cpp
    Plugins/src/Extension.hpp
    Plugins/src/ExtensionPoint.cpp
    Plugins/src/ExtensionPoint.hpp
    Plugins/src/ExtensionQuery.cpp
    Plugins/src/ExtensionQuery.hpp
    Plugins/src/ExtensionRegistry.cpp
    Plugins/src/ExtensionRegistry.hpp
    Plugins/src/I_Application.cpp
    Plugins/src/I_ClassFactory.cpp
    Plugins/src/I_Configuration.cpp
    Plugins/src/I_ConfigurationElement.cpp
    Plugins/src/I_Extension.cpp
    Plugins/src/I_ExtensionPoint.cpp
    Plugins/src/I_ExtensionQuery.cpp
    Plugins/src/I_ExtensionRegistry.cpp
    Plugins/src/I_Module.cpp
    Plugins/src/I_ModuleInfo.cpp
    Plugins/src/I_ModuleManager.cpp
    Plugins/src/I_ModuleService.cpp
    Plugins/src/I_Plugin.cpp
    Plugins/src/I_PluginInfo.cpp
    Plugins/src/I_PluginManager.cpp
    Plugins/src/I_StartupShutdownManager.cpp
    Plugins/src/I_StartupShutdownParticipant.cpp
    Plugins/src/ModuleInfo.cpp
    Plugins/src/ModuleInfo.hpp
    Plugins/src/ModuleManager.cpp
    Plugins/src/ModuleManager.hpp
    Plugins/src/ModuleService.cpp
    Plugins/src/ModuleService.hpp
    Plugins/src/PluginInfo.cpp
    Plugins/src/PluginInfo.hpp
    Plugins/src/PluginManager.cpp
    Plugins/src/PluginManager.hpp
    Plugins/src/StartupShutdownManager.cpp
    Plugins/src/StartupShutdownManager.hpp
    Plugins/src/XMLConfiguration.cpp
    Plugins/src/XMLConfiguration.hpp
    Plugins/src/XMLConfigurationElement.cpp
    Plugins/src/XMLConfigurationElement.hpp
    Plugins/Utilities.hpp
    Scripting/Configuration.hpp
    Scripting/forward_declarations.hpp
    Scripting/I_ObjectHeap.hpp
    Scripting/I_ObjectReference.hpp
    Scripting/I_ScriptableService.hpp
    Scripting/I_ScriptableType.hpp
    Scripting/I_ScriptEngine.hpp
    Scripting/I_ScriptEngineFactory.hpp
    Scripting/I_ScriptingManager.hpp
    Scripting/I_ScriptMethod.hpp
    Scripting/I_ScriptModule.hpp
    Scripting/I_ScriptObject.hpp
    Scripting/I_ScriptType.hpp
    Scripting/ObjectReference.hpp
    Scripting/script_arity_conversion.hpp
    Scripting/script_convert_argument.hpp
    Scripting/script_dispatch_helper.hpp
    Scripting/script_method.hpp
    Scripting/script_module.hpp
    Scripting/script_module_impl.hpp
    Scripting/script_type.hpp
    Scripting/script_type_impl.hpp
    Scripting/script_type_interface.hpp
    Scripting/script_type_traits.hpp
    Scripting/src/I_ObjectHeap.cpp
    Scripting/src/I_ObjectReference.cpp
    Scripting/src/I_ScriptableService.cpp
    Scripting/src/I_ScriptableType.cpp
    Scripting/src/I_ScriptEngine.cpp
    Scripting/src/I_ScriptEngineFactory.cpp
    Scripting/src/I_ScriptingManager.cpp
    Scripting/src/I_ScriptMethod.cpp
    Scripting/src/I_ScriptModule.cpp
    Scripting/src/I_ScriptObject.cpp
    Scripting/src/I_ScriptType.cpp
    Scripting/src/ScriptingManager.cpp
    Scripting/src/ScriptingManager.hpp
    Scripting/src/script_type.cpp
    Scripting.hpp
    Threading/ConditionFactory.hpp
    Threading/Configuration.hpp
    Threading/CriticalSection.hpp
    Threading/I_Callable.hpp
    Threading/I_Condition.hpp
    Threading/I_Mutex.hpp
    Threading/I_Runnable.hpp
    Threading/I_Thread.hpp
    Threading/MutexFactory.hpp
    Threading/SpinLock.hpp
    Threading/src/ConditionFactory.cpp
    Threading/src/Condition_posix.cpp
    Threading/src/Condition_posix.hpp
    Threading/src/Condition_solaris.cpp
    Threading/src/Condition_solaris.hpp
    Threading/src/Condition_win32.cpp
    Threading/src/Condition_win32.hpp
    Threading/src/CriticalSection.cpp
    Threading/src/I_Condition.cpp
    Threading/src/I_Mutex.cpp
    Threading/src/I_Thread.cpp
    Threading/src/MutexFactory.cpp
    Threading/src/Mutex_posix.cpp
    Threading/src/Mutex_posix.hpp
    Threading/src/Mutex_solaris.cpp
    Threading/src/Mutex_solaris.hpp
    Threading/src/Mutex_win32.cpp
    Threading/src/Mutex_win32.hpp
    Threading/src/SynchronizedQueue.cpp
    Threading/src/ThreadFactory.cpp
    Threading/src/ThreadPool.cpp
    Threading/src/Thread_posix.cpp
    Threading/src/Thread_posix.hpp
    Threading/src/Thread_solaris.cpp
    Threading/src/Thread_solaris.hpp
    Threading/src/Thread_win32.cpp
    Threading/src/Thread_win32.hpp
    Threading/SynchronizedQueue.hpp
    Threading/ThreadFactory.hpp
    Threading/ThreadPool.hpp
    Threading/ThreadSafeQueue.hpp
    Utility/Configuration.hpp
    Utility/GetLastError.hpp
    Utility/I_Configuration.hpp
    Utility/I_ConfigurationElement.hpp
    Utility/I_EnvironmentHandler.hpp
    Utility/I_Log.hpp
    Utility/I_LogManager.hpp
    Utility/I_LogService.hpp
    Utility/log_stream.hpp
    Utility/network_ordered.hpp
    Utility/runtime_exception.hpp
    Utility/src/EnvironmentHandler.cpp
    Utility/src/EnvironmentHandler.hpp
    Utility/src/GetLastError.cpp
    Utility/src/I_Configuration.cpp
    Utility/src/I_ConfigurationElement.cpp
    Utility/src/I_EnvironmentHandler.cpp
    Utility/src/I_Log.cpp
    Utility/src/I_LogManager.cpp
    Utility/src/I_LogService.cpp
    Utility/src/Log.cpp
    Utility/src/Log.hpp
    Utility/src/LogManager.cpp
    Utility/src/LogManager.hpp
    Utility/src/LogService.cpp
    Utility/src/LogService.hpp
    Utility/src/XMLConfiguration.cpp
    Utility/src/XMLConfiguration.hpp
    Utility/src/XMLConfigurationElement.cpp
    Utility/src/XMLConfigurationElement.hpp

    )
    
message("Boost defs: ${Boost_DEFINITIONS}")
    
add_definitions( -DZENCORE_EXPORTS ${LIBXML2_DEFINITIONS} ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB}  ${LibXML2_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS} )

add_library ( ZenCore ${LIB_TYPE} ${ZenCore_SRC} )

message("DEBUG: libxml2 libs: ${LIBXML2_LIBRARIES}")

target_link_libraries( ZenCore ${LIBXML2_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} )

set_target_properties ( ZenCore 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    add_definitions( -DTHREADMODEL_WIN32 )

    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZenCore PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZenCore PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Event/Configuration.hpp
    Event/Connection.hpp
    Event/Event.hpp
    Event/future_return_value.hpp
    Event/I_Action.hpp
    Event/I_ActionMap.hpp
    Event/I_Connection.hpp
    Event/I_Event.hpp
    Event/I_EventManager.hpp
    Event/I_EventQueue.hpp
    Event/I_EventService.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Event/
)

install ( FILES 
    Math/ComplexReal.hpp
    Math/Configuration.hpp
    Math/Degree.hpp
    Math/Math.hpp
    Math/Matrix3.hpp
    Math/Matrix4.hpp
    Math/Point3.hpp
    Math/Quaternion4.hpp
    Math/Radian.hpp
    Math/Ray.hpp
    Math/Vector3.hpp
    Math/Vector4.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Math/
)

install ( FILES 
    Memory/Configuration.hpp
    Memory/managed_iterator.hpp
    Memory/managed_payload.hpp
    Memory/managed_ptr.hpp
    Memory/managed_self_ref.hpp
    Memory/managed_weak_ptr.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Memory/
)

install ( FILES 
    Plugins/Configuration.hpp
    Plugins/I_Application.hpp
    Plugins/I_ClassFactory.hpp
    Plugins/I_Configuration.hpp
    Plugins/I_ConfigurationElement.hpp
    Plugins/I_Extension.hpp
    Plugins/I_ExtensionPoint.hpp
    Plugins/I_ExtensionQuery.hpp
    Plugins/I_ExtensionRegistry.hpp
    Plugins/I_Module.hpp
    Plugins/I_ModuleInfo.hpp
    Plugins/I_ModuleManager.hpp
    Plugins/I_ModuleService.hpp
    Plugins/I_Plugin.hpp
    Plugins/I_PluginInfo.hpp
    Plugins/I_PluginManager.hpp
    Plugins/I_Service.hpp
    Plugins/I_ServiceInfo.hpp
    Plugins/I_StartupShutdownManager.hpp
    Plugins/I_StartupShutdownParticipant.hpp
    Plugins/ServiceCache.hpp
    Plugins/Utilities.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Plugins/
)

install ( FILES 
    Scripting/Configuration.hpp
    Scripting/forward_declarations.hpp
    Scripting/I_ObjectHeap.hpp
    Scripting/I_ObjectReference.hpp
    Scripting/I_ScriptableService.hpp
    Scripting/I_ScriptableType.hpp
    Scripting/I_ScriptEngine.hpp
    Scripting/I_ScriptEngineFactory.hpp
    Scripting/I_ScriptingManager.hpp
    Scripting/I_ScriptMethod.hpp
    Scripting/I_ScriptModule.hpp
    Scripting/I_ScriptObject.hpp
    Scripting/I_ScriptType.hpp
    Scripting/ObjectReference.hpp
    Scripting/script_arity_conversion.hpp
    Scripting/script_convert_argument.hpp
    Scripting/script_dispatch_helper.hpp
    Scripting/script_method.hpp
    Scripting/script_module.hpp
    Scripting/script_module_impl.hpp
    Scripting/script_type.hpp
    Scripting/script_type_impl.hpp
    Scripting/script_type_interface.hpp
    Scripting/script_type_traits.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Scripting/
)

install ( FILES 
    Threading/ConditionFactory.hpp
    Threading/Configuration.hpp
    Threading/CriticalSection.hpp
    Threading/I_Callable.hpp
    Threading/I_Condition.hpp
    Threading/I_Mutex.hpp
    Threading/I_Runnable.hpp
    Threading/I_Thread.hpp
    Threading/MutexFactory.hpp
    Threading/SpinLock.hpp
    Threading/SynchronizedQueue.hpp
    Threading/ThreadFactory.hpp
    Threading/ThreadPool.hpp
    Threading/ThreadSafeQueue.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Threading/
)

install ( FILES 
    Utility/Configuration.hpp
    Utility/GetLastError.hpp
    Utility/I_Configuration.hpp
    Utility/I_ConfigurationElement.hpp
    Utility/I_EnvironmentHandler.hpp
    Utility/I_Log.hpp
    Utility/I_LogManager.hpp
    Utility/I_LogService.hpp
    Utility/log_stream.hpp
    Utility/network_ordered.hpp
    Utility/runtime_exception.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Utility/
)

install ( FILES 
    Scripting.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZenCore
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
