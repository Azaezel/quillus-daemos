include(FindPkgMacros)

FIND_PATH(OGG_INCLUDE_DIR ogg/ogg.h
  HINTS
  PATH_SUFFIXES include/ogg include
  PATHS
  c:/dev/ogg
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

FIND_LIBRARY(OGG_DEBUG_LIBRARY libogg
  HINTS
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 Win32/Debug Win64/Debug
  PATHS
  C:/dev/ogg/win32/VS6/Win32/Debug
  C:/dev/ogg/win32/VS2003/Win32/Debug
  C:/dev/ogg/win32/VS2005/Win32/Debug
  C:/dev/ogg/win32/VS2008/Win32/Debug
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

FIND_LIBRARY(OGG_RELEASE_LIBRARY libogg
  HINTS
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 Win32/Release Win64/Release
  PATHS
  C:/dev/ogg/win32/VS6/Win32/Release
  C:/dev/ogg/win32/VS2003/Win32/Release
  C:/dev/ogg/win32/VS2005/Win32/Release
  C:/dev/ogg/win32/VS2008/Win32/Release
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(OGG_FOUND "NO")
IF(OGG_DEBUG_LIBRARY AND OGG_RELEASE_LIBRARY AND OGG_INCLUDE_DIR)
  SET(OGG_FOUND "YES")
  SET(OGG_LIBRARIES debug ${OGG_DEBUG_LIBRARY} optimized ${OGG_RELEASE_LIBRARY})
ENDIF(OGG_DEBUG_LIBRARY AND OGG_RELEASE_LIBRARY AND OGG_INCLUDE_DIR)

