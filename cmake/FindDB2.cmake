#.rst:
# FindDB2
# ---------
#
# Try to locate the db2 client library.
# If found, this will define the following variables:
#
# ``DB2_FOUND``
#     True if the db2 library is available
# ``DB2_INCLUDE_DIRS``
#     The db2 include directories
# ``DB2_LIBRARIES``
#     The db2 libraries for linking
#
# If ``DB2_FOUND`` is TRUE, it will also define the following
# imported target:
#
# ``DB2::DB2``
#     The db2 client library

if (NOT DEFINED DB2_INCLUDE_DIR)
  find_path(DB2_INCLUDE_DIRS
    NAMES sqlcli1.h
    HINTS ENV DB2_HOME
    PATH_SUFFIXES include)
else()
  find_path(DB2_INCLUDE_DIRS
    NAMES sqlcli1.h
    HINTS ${DB2_INCLUDE_DIR})
endif()

if (NOT DEFINED DB2_LIBRARY_DIR)
  find_library(DB2_LIBRARIES
    NAMES db2
    HINTS ENV DB2LIB)
else()
  find_library(DB2_LIBRARIES
    NAMES db2
    HINTS ${DB2_LIBRARY_DIR})
endif()

if (NOT DB2_INCLUDE_DIRS STREQUAL "DB2_INCLUDE_DIRS-NOTFOUND" AND NOT DB2_LIBRARIES STREQUAL "DB2_LIBRARIES-NOTFOUND")
  set(DB2_FOUND ON)
endif()

if(DB2_FOUND AND NOT TARGET DB2::DB2)
  add_library(DB2::DB2 UNKNOWN IMPORTED)
  set_target_properties(DB2::DB2 PROPERTIES
                        IMPORTED_LOCATION "${DB2_LIBRARIES}"
                        INTERFACE_INCLUDE_DIRECTORIES "${DB2_INCLUDE_DIRS}")
endif()

mark_as_advanced(DB2_INCLUDE_DIRS DB2_LIBRARIES)

include(FeatureSummary)
set_package_properties(DB2 PROPERTIES
  URL "https://www.ibm.com"
  DESCRIPTION "IBM DB2 client library")

