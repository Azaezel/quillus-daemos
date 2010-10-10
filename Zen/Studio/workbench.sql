DROP TABLE IF EXISTS "WorkbenchAttribute";

CREATE TABLE "WorkbenchAttribute"
(
    "workBenchAttributeId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
    "name" VARCHAR NOT NULL , 
    "value" VARCHAR
);

DROP TABLE IF EXISTS "ExplorerNode";

CREATE TABLE "ExplorerNode" 
(
    "explorerNodeId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
    "nodeType" VARCHAR NOT NULL , 
    "parentNodeId" INTEGER
);

DROP TABLE IF EXISTS "Project";

CREATE TABLE "Project" 
(
	"projectId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL ,
	"name" VARCHAR NOT NULL 
);

DROP TABLE IF EXISTS "GameObjectFolder";

CREATE TABLE "GameObjectFolder" 
(
    "gameObjectFolderId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL 
);

DROP TABLE IF EXISTS "GameObjectType";

CREATE TABLE "GameObjectType" 
(
    "gameObjectTypeId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL ,
    "propertiesId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "GameObjectElement";

CREATE TABLE "GameObjectElement" 
(
    "gameObjectElementId" INTEGER PRIMARY KEY  NOT NULL , 
    "gameObjectTypeId" INTEGER NOT NULL , 
    "overrideParent" INTEGER NOT NULL ,
    "sequenceNumber" INTEGER NOT NULL ,
    "name" VARCHAR NOT NULL ,
    "dataType" VARCHAR NOT NULL ,
    "defaultValue" VARCHAR NOT NULL ,
    "propertiesId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "Properties";

CREATE TABLE "Properties" 
(
    "propertiesId" INTEGER PRIMARY KEY  NOT NULL,
    "emptyValue" INTEGER DEFAULT 0
);

DROP TABLE IF EXISTS "Property";

CREATE TABLE "Property" 
(
    "propertyId" INTEGER PRIMARY KEY  NOT NULL , 
    "propertiesId" INTEGER NOT NULL ,
    "name" VARCHAR NOT NULL ,
    "value" VARCHAR NOT NULL
);

DROP TABLE IF EXISTS "SpreadSheetDocument";

CREATE TABLE "SpreadSheetDocument" 
(
    "spreadSheetDocumentId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL ,
    "explorerNodeId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "SpreadSheetColumn";

CREATE TABLE "SpreadSheetColumn" 
(
    "spreadSheetColumnId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL ,
    "spreadSheetDocumentId" INTEGER NOT NULL ,
    "columnNumber" INTEGER NOT NULL ,
    "columnText" VARCHAR NOT NULL 
);


DROP TABLE IF EXISTS "SpreadSheetRow";

CREATE TABLE "SpreadSheetRow" 
(
    "spreadSheetRowId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL ,
    "spreadSheetDocumentId" INTEGER NOT NULL ,
    "rowNumber" INTEGER NOT NULL ,
    "rowText" VARCHAR NOT NULL 
);

DROP TABLE IF EXISTS "SpreadSheetCell";

CREATE TABLE "SpreadSheetCell" 
(
    "spreadSheetCellId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
    "spreadSheetDocumentId" INTEGER NOT NULL ,
    "spreadSheetRowId" INTEGER NOT NULL ,
    "spreadSheetColumnId" INTEGER NOT NULL ,
    "valueType" VARCHAR NOT NULL ,
    "value" VARCHAR NOT NULL
);

DROP TABLE IF EXISTS "SceneModel";

CREATE TABLE "SceneModel" 
(
    "sceneModelId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL ,
    "explorerNodeId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "SceneEntity";

CREATE TABLE "SceneEntity"
(
    "sceneEntityId" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ,
    "sceneModelId" INTEGER NOT NULL ,
    "parentEntityId" INTEGER ,
    "propertiesId" INTEGER NOT NULL,
    "name" VARCHAR NOT NULL
);

DROP TABLE IF EXISTS "ZoneFolder";

CREATE TABLE "ZoneFolder" 
(
    "zoneFolderId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL 
);


DROP TABLE IF EXISTS "Zone";

CREATE TABLE "Zone" 
(
    "zoneId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL 
);

DROP TABLE IF EXISTS "ZoneLayer";

CREATE TABLE "ZoneLayer" 
(
    "zoneLayerId" INTEGER PRIMARY KEY  NOT NULL , 
    "name" VARCHAR NOT NULL 
);

DROP TABLE IF EXISTS "ZoneObject";

CREATE TABLE "ZoneObject" 
(
    "zoneObjectId" INTEGER PRIMARY KEY  NOT NULL , 
    "zoneLayerId" INTEGER NOT NULL ,
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL 
);


DROP TABLE IF EXISTS "ArtFolder";

CREATE TABLE "ArtFolder" 
(
    "artFolderId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL 
);

DROP TABLE IF EXISTS "ArtAssetExplorerNode";

CREATE TABLE "ArtAssetExplorerNode" 
(
    "artAssetExplorerNodeId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL 
);


DROP TABLE IF EXISTS "ArtAssetDocument";

CREATE TABLE "ArtAssetDocument" 
(
    "artAssetDocumentId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL 
);


DROP TABLE IF EXISTS "TerrainFolder";

CREATE TABLE "TerrainFolder" 
(
    "terrainFolderId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL 
);

DROP TABLE IF EXISTS "TerrainGroup";

CREATE TABLE "TerrainGroup" 
(
    "terrainGroupId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL ,
    "propertiesId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "TerrainChunk";

CREATE TABLE "TerrainChunk" 
(
    "terrainChunkId" INTEGER PRIMARY KEY  NOT NULL , 
    "explorerNodeId" INTEGER NOT NULL , 
    "name" VARCHAR NOT NULL ,
    "propertiesId" INTEGER NOT NULL
);
