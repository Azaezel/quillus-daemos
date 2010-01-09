# Zen/Enterprise/Database CMakeLists.txt
#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Newer CMake Find system for locating Boost includes and libraries
cmake_minimum_required(VERSION 2.6)
SET( Boost_USE_MULTITHREAD On )
SET( Boost_USE_STATIC_LIBS Off )

Find_Package( Boost REQUIRED )

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
Add_Library( Database_SRC
    
    #in Database
    Configuration.hpp   
	I_DatabaseClause.hpp   
	I_DatabaseColumn.hpp   
	I_DatabaseCommand.hpp   
	I_DatabaseConnection.hpp   
	I_DatabaseDynamicQuery.hpp   
	I_DatabaseManager.hpp   
	I_DatabaseQuery.hpp   
	I_DatabaseResult.hpp   
	I_DatabaseRow.hpp   
	I_DatabaseSchema.hpp   
	I_DatabaseSchemaReader.hpp   
	I_DatabaseSchemaReaderFactory.hpp   
	I_DatabaseSchemaWriter.hpp   
	I_DatabaseSchemaWriterFactory.hpp   
	I_DatabaseService.hpp   
	I_DatabaseServiceFactory.hpp   
	I_DatabaseStatement.hpp   
	I_DatabaseStaticQuery.hpp   
	I_DatabaseTransaction.hpp   
	I_FilterClause.hpp   
	I_ImmutableDatabaseSchema.hpp   
	I_JoinClause.hpp   
	I_MetaAttribute.hpp   
	I_MetaAttributeIterator.hpp   
	I_MetaField.hpp   
	I_MetaFieldIterator.hpp   
	I_MetaKey.hpp   
	I_MetaTable.hpp   
	I_MetaTableIterator.hpp   
	I_MetaView.hpp   
	I_MetaViewIterator.hpp   
	I_MutableDatabaseSchema.hpp   
	I_SchemaService.hpp   
	I_SchemaServiceFactory.hpp   
	I_SelectClause.hpp   
	I_SortClause.hpp
    
    #in Database/src
    src/DatabaseManager.hpp
    src/DatabaseManager.cpp 
	src/I_DatabaseClause.cpp 
	src/I_DatabaseColumn.cpp 
	src/I_DatabaseCommand.cpp 
	src/I_DatabaseConnection.cpp 
	src/I_DatabaseDynamicQuery.cpp 
	src/I_DatabaseManager.cpp 
	src/I_DatabaseQuery.cpp 
	src/I_DatabaseResult.cpp 
	src/I_DatabaseRow.cpp 
	src/I_DatabaseSchema.cpp 
	src/I_DatabaseSchemaReader.cpp 
	src/I_DatabaseSchemaReaderFactory.cpp 
	src/I_DatabaseSchemaWriter.cpp 
	src/I_DatabaseSchemaWriterFactory.cpp 
	src/I_DatabaseService.cpp 
	src/I_DatabaseServiceFactory.cpp 
	src/I_DatabaseStatement.cpp 
	src/I_DatabaseStaticQuery.cpp 
	src/I_DatabaseTransaction.cpp 
	src/I_FilterClause.cpp 
	src/I_ImmutableDatabaseSchema.cpp 
	src/I_JoinClause.cpp 
	src/I_MetaAttribute.cpp 
	src/I_MetaAttributeIterator.cpp 
	src/I_MetaField.cpp 
	src/I_MetaFieldIterator.cpp 
	src/I_MetaKey.cpp 
	src/I_MetaTable.cpp 
	src/I_MetaTableIterator.cpp 
	src/I_MetaView.cpp 
	src/I_MetaViewIterator.cpp 
	src/I_MutableDatabaseSchema.cpp 
	src/I_SchemaService.cpp 
	src/I_SchemaServiceFactory.cpp 
	src/I_SelectClause.cpp 
	src/I_SortClause.cpp
)
