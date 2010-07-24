include(FindPkgMacros)

FIND_PATH(ALUT_INCLUDE_DIR AL/alut.h
  HINTS
  PATH_SUFFIXES include/AL include
  PATHS
  c:/Program Files/Alut
  c:/dev/alut
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

FIND_LIBRARY(ALUT_LIBRARY alut
  HINTS
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
  PATHS
  c:/Program Files/Alut
  c:/dev/alut
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(ALUT_FOUND "NO")
IF(ALUT_LIBRARY AND ALUT_INCLUDE_DIR)
  SET(ALUT_FOUND "YES")
ENDIF(ALUT_LIBRARY AND ALUT_INCLUDE_DIR)

