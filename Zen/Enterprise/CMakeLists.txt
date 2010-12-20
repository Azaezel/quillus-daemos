cmake_minimum_required (VERSION 2.6) 

project (ZenEnterprise)
set ( PROJECT_PREFIX "Enterprise" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED program_options)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    Account\\
	REGULAR_EXPRESSION 
	Account/*.*
	)

source_group(
    Account\\src\\
	REGULAR_EXPRESSION 
	Account/src/*.*
	)

source_group(
    AppServer\\
	REGULAR_EXPRESSION 
	AppServer/*.*
	)

source_group(
    AppServer\\src\\
	REGULAR_EXPRESSION 
	AppServer/src/*.*
	)

source_group(
    Database\\
	REGULAR_EXPRESSION 
	Database/*.*
	)

source_group(
    Database\\src\\
	REGULAR_EXPRESSION 
	Database/src/*.*
	)

source_group(
    DataModel\\
	REGULAR_EXPRESSION 
	DataModel/*.*
	)

source_group(
    DataModel\\src\\
	REGULAR_EXPRESSION 
	DataModel/src/*.*
	)

source_group(
    Networking\\
	REGULAR_EXPRESSION 
	Networking/*.*
	)

source_group(
    Networking\\src\\
	REGULAR_EXPRESSION 
	Networking/src/*.*
	)

source_group(
    Permission\\
	REGULAR_EXPRESSION 
	Permission/*.*
	)

source_group(
    Permission\\src\\
	REGULAR_EXPRESSION 
	Permission/src/*.*
	)

source_group(
    Session\\
	REGULAR_EXPRESSION 
	Session/*.*
	)

source_group(
    Session\\src\\
	REGULAR_EXPRESSION 
	Session/src/*.*
	)


	
set ( ZenEnterprise_SRC
    Account/Configuration.hpp
    Account/I_Account.hpp
    Account/I_AccountService.hpp
    Account/I_Group.hpp
    Account/src/I_Account.cpp
    Account/src/I_AccountService.cpp
    Account/src/I_Group.cpp
    AppServer/Configuration.hpp
    AppServer/I_ApplicationServer.hpp
    AppServer/I_ApplicationServerManager.hpp
    AppServer/I_ApplicationService.hpp
    AppServer/I_ApplicationServiceFactory.hpp
    AppServer/I_Container.hpp
    AppServer/I_Message.hpp
    AppServer/I_MessageFactory.hpp
    AppServer/I_MessageHandler.hpp
    AppServer/I_MessageHeader.hpp
    AppServer/I_MessageRegistry.hpp
    AppServer/I_MessageType.hpp
    AppServer/I_NumericTypeMessageRegistry.hpp
    AppServer/I_ProtocolManager.hpp
    AppServer/I_ProtocolService.hpp
    AppServer/I_ProtocolServiceFactory.hpp
    AppServer/I_Request.hpp
    AppServer/I_RequestHandler.hpp
    AppServer/I_ResourceLocation.hpp
    AppServer/I_Response.hpp
    AppServer/I_ResponseHandler.hpp
    AppServer/I_SessionEvent.hpp
    AppServer/scriptable_generic_service.hpp
    AppServer/src/ApplicationServer.cpp
    AppServer/src/ApplicationServer.hpp
    AppServer/src/ApplicationServerManager.cpp
    AppServer/src/ApplicationServerManager.hpp
    AppServer/src/Container.cpp
    AppServer/src/Container.hpp
    AppServer/src/I_ApplicationServer.cpp
    AppServer/src/I_ApplicationServerManager.cpp
    AppServer/src/I_ApplicationService.cpp
    AppServer/src/I_ApplicationServiceFactory.cpp
    AppServer/src/I_Container.cpp
    AppServer/src/I_Message.cpp
    AppServer/src/I_MessageFactory.cpp
    AppServer/src/I_MessageHandler.cpp
    AppServer/src/I_MessageHeader.cpp
    AppServer/src/I_MessageRegistry.cpp
    AppServer/src/I_MessageType.cpp
    AppServer/src/I_NumericTypeMessageRegistry.cpp
    AppServer/src/I_ProtocolManager.cpp
    AppServer/src/I_ProtocolService.cpp
    AppServer/src/I_ProtocolServiceFactory.cpp
    AppServer/src/I_Request.cpp
    AppServer/src/I_RequestHandler.cpp
    AppServer/src/I_ResourceLocation.cpp
    AppServer/src/I_Response.cpp
    AppServer/src/I_ResponseHandler.cpp
    AppServer/src/NumericTypeMessageRegistry.cpp
    AppServer/src/NumericTypeMessageRegistry.hpp
    AppServer/src/QueuedRequestHandler.hpp
    AppServer/src/ResourceLocation.cpp
    AppServer/src/ResourceLocation.hpp
    Database/Configuration.hpp
    Database/I_DatabaseClause.hpp
    Database/I_DatabaseColumn.hpp
    Database/I_DatabaseCommand.hpp
    Database/I_DatabaseConnection.hpp
    Database/I_DatabaseDynamicQuery.hpp
    Database/I_DatabaseManager.hpp
    Database/I_DatabaseQuery.hpp
    Database/I_DatabaseResult.hpp
    Database/I_DatabaseRow.hpp
    Database/I_DatabaseSchema.hpp
    Database/I_DatabaseSchemaReader.hpp
    Database/I_DatabaseSchemaReaderFactory.hpp
    Database/I_DatabaseSchemaWriter.hpp
    Database/I_DatabaseSchemaWriterFactory.hpp
    Database/I_DatabaseService.hpp
    Database/I_DatabaseServiceFactory.hpp
    Database/I_DatabaseStatement.hpp
    Database/I_DatabaseStaticQuery.hpp
    Database/I_DatabaseTransaction.hpp
    Database/I_FilterClause.hpp
    Database/I_ImmutableDatabaseSchema.hpp
    Database/I_JoinClause.hpp
    Database/I_MetaAttribute.hpp
    Database/I_MetaAttributeIterator.hpp
    Database/I_MetaField.hpp
    Database/I_MetaFieldIterator.hpp
    Database/I_MetaKey.hpp
    Database/I_MetaTable.hpp
    Database/I_MetaTableIterator.hpp
    Database/I_MetaView.hpp
    Database/I_MetaViewIterator.hpp
    Database/I_MutableDatabaseSchema.hpp
    Database/I_Persistable.hpp
    Database/I_SchemaService.hpp
    Database/I_SchemaServiceFactory.hpp
    Database/I_SelectClause.hpp
    Database/I_SortClause.hpp
    Database/src/DatabaseManager.cpp
    Database/src/DatabaseManager.hpp
    Database/src/I_DatabaseClause.cpp
    Database/src/I_DatabaseColumn.cpp
    Database/src/I_DatabaseCommand.cpp
    Database/src/I_DatabaseConnection.cpp
    Database/src/I_DatabaseDynamicQuery.cpp
    Database/src/I_DatabaseManager.cpp
    Database/src/I_DatabaseQuery.cpp
    Database/src/I_DatabaseResult.cpp
    Database/src/I_DatabaseRow.cpp
    Database/src/I_DatabaseSchema.cpp
    Database/src/I_DatabaseSchemaReader.cpp
    Database/src/I_DatabaseSchemaReaderFactory.cpp
    Database/src/I_DatabaseSchemaWriter.cpp
    Database/src/I_DatabaseSchemaWriterFactory.cpp
    Database/src/I_DatabaseService.cpp
    Database/src/I_DatabaseServiceFactory.cpp
    Database/src/I_DatabaseStatement.cpp
    Database/src/I_DatabaseStaticQuery.cpp
    Database/src/I_DatabaseTransaction.cpp
    Database/src/I_FilterClause.cpp
    Database/src/I_ImmutableDatabaseSchema.cpp
    Database/src/I_JoinClause.cpp
    Database/src/I_MetaAttribute.cpp
    Database/src/I_MetaAttributeIterator.cpp
    Database/src/I_MetaField.cpp
    Database/src/I_MetaFieldIterator.cpp
    Database/src/I_MetaKey.cpp
    Database/src/I_MetaTable.cpp
    Database/src/I_MetaTableIterator.cpp
    Database/src/I_MetaView.cpp
    Database/src/I_MetaViewIterator.cpp
    Database/src/I_MutableDatabaseSchema.cpp
    Database/src/I_Persistable.cpp
    Database/src/I_SchemaService.cpp
    Database/src/I_SchemaServiceFactory.cpp
    Database/src/I_SelectClause.cpp
    Database/src/I_SortClause.cpp
    DataModel/Configuration.hpp
    DataModel/element_container.hpp
    DataModel/I_DataElement.hpp
    DataModel/I_DataElementCollection.hpp
    DataModel/I_DataElementListener.hpp
    DataModel/I_DataStructure.hpp
    DataModel/I_DataStructureCollection.hpp
    DataModel/src/DataElement.cpp
    DataModel/src/DataElement.hpp
    DataModel/src/I_DataElement.cpp
    DataModel/src/I_DataElementCollection.cpp
    DataModel/src/I_DataElementListener.cpp
    DataModel/src/I_DataStructure.cpp
    DataModel/src/I_DataStructureCollection.cpp
    Networking/Configuration.hpp
    Networking/Exceptions.hpp
    Networking/I_AbstractNetworkAddressFactory.hpp
    Networking/I_AbstractNetworkServiceFactory.hpp
    Networking/I_Address.hpp
    Networking/I_Endpoint.hpp
    Networking/I_GameConnection.hpp
    Networking/I_NetworkAddress.hpp
    Networking/I_NetworkAddressFactory.hpp
    Networking/I_NetworkConnection.hpp
    Networking/I_NetworkConnectionListener.hpp
    Networking/I_NetworkManager.hpp
    Networking/I_NetworkReadReadyListener.hpp
    Networking/I_NetworkService.hpp
    Networking/I_NetworkServiceFactory.hpp
    Networking/I_NetworkWriteReadyListener.hpp
    Networking/src/AbstractNetworkAddressFactory.cpp
    Networking/src/AbstractNetworkAddressFactory.hpp
    Networking/src/I_AbstractNetworkAddressFactory.cpp
    Networking/src/I_Address.cpp
    Networking/src/I_Endpoint.cpp
    Networking/src/I_NetworkAddress.cpp
    Networking/src/I_NetworkAddressFactory.cpp
    Networking/src/I_NetworkConnection.cpp
    Networking/src/I_NetworkManager.cpp
    Networking/src/I_NetworkService.cpp
    Networking/src/I_NetworkServiceFactory.cpp
    Networking/src/NetworkManager.cpp
    Networking/src/NetworkManager.hpp
    Permission/Configuration.hpp
    Permission/I_PermissibleResource.hpp
    Permission/I_Permission.hpp
    Permission/I_PermissionService.hpp
    Permission/src/I_PermissibleResource.cpp
    Permission/src/I_Permission.cpp
    Permission/src/I_PermissionService.cpp
    Session/Configuration.hpp
    Session/I_Session.hpp
    Session/I_SessionService.hpp
    Session/src/I_Session.cpp
    Session/src/I_SessionService.cpp

    )

add_definitions( -DZENENTERPRISE_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZenEnterprise ${LIB_TYPE} ${ZenEnterprise_SRC} )

add_dependencies(ZenEnterprise ZenCore)

message("Boost libraries: ${Boost_LIBRARIES}")

target_link_libraries( ZenEnterprise ZenCore ${Boost_LIBRARIES} )

set_target_properties ( ZenEnterprise 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZenEnterprise PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZenEnterprise PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Account/Configuration.hpp
    Account/I_Account.hpp
    Account/I_AccountService.hpp
    Account/I_Group.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Account/
)

install ( FILES 
    AppServer/Configuration.hpp
    AppServer/I_ApplicationServer.hpp
    AppServer/I_ApplicationServerManager.hpp
    AppServer/I_ApplicationService.hpp
    AppServer/I_ApplicationServiceFactory.hpp
    AppServer/I_Container.hpp
    AppServer/I_Message.hpp
    AppServer/I_MessageFactory.hpp
    AppServer/I_MessageHandler.hpp
    AppServer/I_MessageHeader.hpp
    AppServer/I_MessageRegistry.hpp
    AppServer/I_MessageType.hpp
    AppServer/I_NumericTypeMessageRegistry.hpp
    AppServer/I_ProtocolManager.hpp
    AppServer/I_ProtocolService.hpp
    AppServer/I_ProtocolServiceFactory.hpp
    AppServer/I_Request.hpp
    AppServer/I_RequestHandler.hpp
    AppServer/I_ResourceLocation.hpp
    AppServer/I_Response.hpp
    AppServer/I_ResponseHandler.hpp
    AppServer/I_SessionEvent.hpp
    AppServer/scriptable_generic_service.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/AppServer/
)

install ( FILES 
    Database/Configuration.hpp
    Database/I_DatabaseClause.hpp
    Database/I_DatabaseColumn.hpp
    Database/I_DatabaseCommand.hpp
    Database/I_DatabaseConnection.hpp
    Database/I_DatabaseDynamicQuery.hpp
    Database/I_DatabaseManager.hpp
    Database/I_DatabaseQuery.hpp
    Database/I_DatabaseResult.hpp
    Database/I_DatabaseRow.hpp
    Database/I_DatabaseSchema.hpp
    Database/I_DatabaseSchemaReader.hpp
    Database/I_DatabaseSchemaReaderFactory.hpp
    Database/I_DatabaseSchemaWriter.hpp
    Database/I_DatabaseSchemaWriterFactory.hpp
    Database/I_DatabaseService.hpp
    Database/I_DatabaseServiceFactory.hpp
    Database/I_DatabaseStatement.hpp
    Database/I_DatabaseStaticQuery.hpp
    Database/I_DatabaseTransaction.hpp
    Database/I_FilterClause.hpp
    Database/I_ImmutableDatabaseSchema.hpp
    Database/I_JoinClause.hpp
    Database/I_MetaAttribute.hpp
    Database/I_MetaAttributeIterator.hpp
    Database/I_MetaField.hpp
    Database/I_MetaFieldIterator.hpp
    Database/I_MetaKey.hpp
    Database/I_MetaTable.hpp
    Database/I_MetaTableIterator.hpp
    Database/I_MetaView.hpp
    Database/I_MetaViewIterator.hpp
    Database/I_MutableDatabaseSchema.hpp
    Database/I_Persistable.hpp
    Database/I_SchemaService.hpp
    Database/I_SchemaServiceFactory.hpp
    Database/I_SelectClause.hpp
    Database/I_SortClause.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Database/
)

install ( FILES 
    DataModel/Configuration.hpp
    DataModel/element_container.hpp
    DataModel/I_DataElement.hpp
    DataModel/I_DataElementCollection.hpp
    DataModel/I_DataElementListener.hpp
    DataModel/I_DataStructure.hpp
    DataModel/I_DataStructureCollection.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/DataModel/
)

install ( FILES 
    Networking/Configuration.hpp
    Networking/Exceptions.hpp
    Networking/I_AbstractNetworkAddressFactory.hpp
    Networking/I_AbstractNetworkServiceFactory.hpp
    Networking/I_Address.hpp
    Networking/I_Endpoint.hpp
    Networking/I_GameConnection.hpp
    Networking/I_NetworkAddress.hpp
    Networking/I_NetworkAddressFactory.hpp
    Networking/I_NetworkConnection.hpp
    Networking/I_NetworkConnectionListener.hpp
    Networking/I_NetworkManager.hpp
    Networking/I_NetworkReadReadyListener.hpp
    Networking/I_NetworkService.hpp
    Networking/I_NetworkServiceFactory.hpp
    Networking/I_NetworkWriteReadyListener.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Networking/
)

install ( FILES 
    Permission/Configuration.hpp
    Permission/I_PermissibleResource.hpp
    Permission/I_Permission.hpp
    Permission/I_PermissionService.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Permission/
)

install ( FILES 
    Session/Configuration.hpp
    Session/I_Session.hpp
    Session/I_SessionService.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Session/
)


install ( TARGETS
	ZenEnterprise
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
