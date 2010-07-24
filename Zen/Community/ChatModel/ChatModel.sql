DROP TABLE IF EXISTS "ChatChannel";

CREATE TABLE "ChatChannel"
(
    "chatChannelId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
    "name" VARCHAR NOT NULL , 
    "description" VARCHAR ,
	"ownerId" INTEGER NOT NULL ,
	"groupId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "AllowUser";

CREATE TABLE "AllowUser"
(
    "allowUserId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL ,
    "chatChannelId" INTEGER NOT NULL ,
    "accountId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "AllowGroup";

CREATE TABLE "AllowGroup"
(
    "allowGroupId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL ,
    "chatChannelId" INTEGER NOT NULL ,
    "groupId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "DenyUser";

CREATE TABLE "DenyUser"
(
    "denyUserId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL ,
    "chatChannelId" INTEGER NOT NULL ,
    "accountId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "DenyGroup";

CREATE TABLE "DenyGroup"
(
    "denyGroupId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL ,
    "chatChannelId" INTEGER NOT NULL ,
    "groupId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "ChatEvent";

CREATE TABLE "ChatEvent"
(
	"chatEventId" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ,
	"event" VARCHAR NOT NULL ,
	"response" VARCHAR NOT NULL ,
	"flags" INTEGER NOT NULL ,
    "ownerId" INTEGER NOT NULL ,
    "groupId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "ChatCommand";

CREATE TABLE "ChatCommand"
(
	"chatCommandId" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ,
	"token" VARCHAR NOT NULL ,
	"action" VARCHAR ,
	"response" VARCHAR ,
	"flags" INTEGER NOT NULL ,
    "ownerId" INTEGER NOT NULL ,
    "groupId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "ChatLog";

CREATE TABLE "ChatLog"
(
	"chatLogId" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ,
	"channelId" INTEGER NOT NULL ,
	"accountId" INTEGER NOT NULL ,
	"messageType" VARCHAR NOT NULL ,
	"message" VARCHAR NOT NULL ,
	"logTime" DATETIME NOT NULL
);

