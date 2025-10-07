
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/sqldrivers/libqsqlite.so"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT FALSE)
                message(FATAL_ERROR "Cannot find 'plugins/sqldrivers/libqsqlite.so' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/sqldrivers/libqsqlite.so' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/sqldrivers/libqsqlite.so" sha1)
                endif()
                list(APPEND QT_SBOM_VERIFICATION_CODES ${sha1})
            endif()
            file(APPEND "/home/noobcod3r-rtx/Documents/GitHub/SmartXm-Cross-Platform-Desktop-Exam-Management-with-Integrated-Judge/Tools/DB_Connector/mysql-kaziRTX/qt_sbom/staging-qsqlitedriverplugins.spdx.in"
"
FileName: ./plugins/sqldrivers/libqsqlite.so
SPDXID: SPDXRef-PackagedFile-qt-plugin-QSQLiteDriverPlugin
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin CONTAINS SPDXRef-PackagedFile-qt-plugin-QSQLiteDriverPlugin
Relationship: SPDXRef-PackagedFile-qt-plugin-QSQLiteDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/sqlite/qsql_sqlite.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSQLiteDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/sqlite/qsql_sqlite_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSQLiteDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/sqlite/qsql_sqlite_vfs.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSQLiteDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/sqlite/qsql_sqlite_vfs_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSQLiteDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/sqlite/smain.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSQLiteDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /home/noobcod3r-rtx/Qt/6.9.2/Src/qtbase/src/3rdparty/sqlite/sqlite3.c
"
                )
        endif()
