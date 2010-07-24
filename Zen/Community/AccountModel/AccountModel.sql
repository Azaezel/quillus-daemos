DROP TABLE IF EXISTS "Account";

CREATE TABLE "Account"
(
    "accountId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
    "user" VARCHAR NOT NULL , 
    "password" VARCHAR NOT NULL 
);

DROP TABLE IF EXISTS "Group";

CREATE TABLE "Group"
(
    "groupId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL ,
    "groupName" VARCHAR NOT NULL ,
    "groupDescription" VARCHAR NOT NULL
);

DROP TABLE IF EXISTS "GroupMember";

CREATE TABLE "GroupMember"
(
	"memberId" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ,
    "groupId" INTEGER NOT NULL ,
    "accountId" INTEGER NOT NULL
);

DROP TABLE IF EXISTS "Permission";

CREATE TABLE "Permission"
(
	"permissionId" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ,
	"uri" VARCHAR NOT NULL ,
	"ownerId" INTEGER NOT NULL ,
	"groupId" INTEGER NOT NULL ,
	"flags" INTEGER NOT NULL
);