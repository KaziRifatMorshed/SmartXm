
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/sqldrivers/libqsqlmysql.so"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT FALSE)
                message(FATAL_ERROR "Cannot find 'plugins/sqldrivers/libqsqlmysql.so' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/sqldrivers/libqsqlmysql.so' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/sqldrivers/libqsqlmysql.so" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QMYSQLDriverPlugin" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QMYSQLDriverPlugin")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QMYSQLDriverPlugin"
                    "${sha1}")
            endif()
            file(APPEND "/home/noobcod3r-rtx/Documents/GitHub/SmartXm-Cross-Platform-Desktop-Exam-Management-with-Integrated-Judge/Tools/DB_Connector/mysql-kaziRTX/qt_sbom/staging-qsqlitedriverplugins.spdx.in"
"
FileName: ./plugins/sqldrivers/libqsqlmysql.so
SPDXID: SPDXRef-PackagedFile-qt-plugin-QMYSQLDriverPlugin
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QMYSQLDriverPlugin CONTAINS SPDXRef-PackagedFile-qt-plugin-QMYSQLDriverPlugin
Relationship: SPDXRef-PackagedFile-qt-plugin-QMYSQLDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/mysql/main.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QMYSQLDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/mysql/qsql_mysql.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QMYSQLDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/mysql/qsql_mysql_p.h
"
                )
        endif()
