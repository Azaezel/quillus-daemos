DROP TABLE IF EXISTS "Account";

CREATE TABLE "Account"
(
    "accountId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
    "userId" VARCHAR NOT NULL , 
    "password" VARCHAR NOT NULL 
);

INSERT INTO "Account" ("accountId", "userId", "password") VALUES (0,'testUser','testPassword');
