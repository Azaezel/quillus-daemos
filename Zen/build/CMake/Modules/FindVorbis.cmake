include(FindPkgMacros)

FIND_PATH(VORBIS_INCLUDE_DIR vorbis/vorbisfile.h
  HINTS
  PATH_SUFFIXES include/vorbis include
  PATHS
  ${INDIEZEN_PARENT}/vorbis
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

FIND_LIBRARY(VORBIS_DEBUG_LIBRARY libvorbis
  HINTS
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 Win32/Debug Win64/Debug
  PATHS
  ${INDIEZEN_PARENT}/vorbis/win32/VS6/Win32/Debug
  ${INDIEZEN_PARENT}/vorbis/win32/VS2003/Win32/Debug
  ${INDIEZEN_PARENT}/vorbis/win32/VS2005/Win32/Debug
  ${INDIEZEN_PARENT}/vorbis/win32/VS2008/Win32/Debug
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

FIND_LIBRARY(VORBIS_RELEASE_LIBRARY libvorbis
  HINTS
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 Win32/Release Win64/Release
  PATHS
  ${INDIEZEN_PARENT}/vorbis/win32/VS6/Win32/Release
  ${INDIEZEN_PARENT}/vorbis/win32/VS2003/Win32/Release
  ${INDIEZEN_PARENT}/vorbis/win32/VS2005/Win32/Release
  ${INDIEZEN_PARENT}/vorbis/win32/VS2008/Win32/Release
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

FIND_LIBRARY(VORBISFILE_DEBUG_LIBRARY libvorbisfile
  HINTS
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 Win32/Debug Win64/Debug
  PATHS
  ${INDIEZEN_PARENT}/vorbis/win32/VS6/Win32/Debug
  ${INDIEZEN_PARENT}/vorbis/win32/VS2003/Win32/Debug
  ${INDIEZEN_PARENT}/vorbis/win32/VS2005/Win32/Debug
  ${INDIEZEN_PARENT}/vorbis/win32/VS2008/Win32/Debug
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

FIND_LIBRARY(VORBISFILE_RELEASE_LIBRARY libvorbisfile
  HINTS
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 Win32/Release Win64/Release
  PATHS
  ${INDIEZEN_PARENT}/vorbis/win32/VS6/Win32/Release
  ${INDIEZEN_PARENT}/vorbis/win32/VS2003/Win32/Release
  ${INDIEZEN_PARENT}/vorbis/win32/VS2005/Win32/Release
  ${INDIEZEN_PARENT}/vorbis/win32/VS2008/Win32/Release
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(VORBIS_FOUND "NO")
IF(VORBIS_DEBUG_LIBRARY AND VORBIS_RELEASE_LIBRARY AND VORBISFILE_DEBUG_LIBRARY AND VORBISFILE_RELEASE_LIBRARY AND VORBIS_INCLUDE_DIR)
  SET(VORBIS_FOUND "YES")
  SET(VORBIS_LIBRARIES debug ${VORBIS_DEBUG_LIBRARY} optimized ${VORBIS_RELEASE_LIBRARY} debug ${VORBISFILE_DEBUG_LIBRARY} optimized ${VORBISFILE_RELEASE_LIBRARY})
ENDIF(VORBIS_DEBUG_LIBRARY AND VORBIS_RELEASE_LIBRARY AND VORBISFILE_DEBUG_LIBRARY AND VORBISFILE_RELEASE_LIBRARY AND VORBIS_INCLUDE_DIR)

