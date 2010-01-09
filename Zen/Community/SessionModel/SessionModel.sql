DROP TABLE IF EXISTS "Account";

CREATE TABLE "Account"
(
    "accountId" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 
    "user" VARCHAR NOT NULL , 
    "password" VARCHAR NOT NULL 
);
