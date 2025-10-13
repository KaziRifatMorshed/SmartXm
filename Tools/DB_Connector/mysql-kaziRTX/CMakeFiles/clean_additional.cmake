# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "RelWithDebInfo")
  file(REMOVE_RECURSE
  "mysql/CMakeFiles/QMYSQLDriverPlugin_autogen.dir/AutogenUsed.txt"
  "mysql/CMakeFiles/QMYSQLDriverPlugin_autogen.dir/ParseCache.txt"
  "mysql/QMYSQLDriverPlugin_autogen"
  "psql/CMakeFiles/QPSQLDriverPlugin_autogen.dir/AutogenUsed.txt"
  "psql/CMakeFiles/QPSQLDriverPlugin_autogen.dir/ParseCache.txt"
  "psql/QPSQLDriverPlugin_autogen"
  "sqlite/CMakeFiles/QSQLiteDriverPlugin_autogen.dir/AutogenUsed.txt"
  "sqlite/CMakeFiles/QSQLiteDriverPlugin_autogen.dir/ParseCache.txt"
  "sqlite/QSQLiteDriverPlugin_autogen"
  )
endif()
