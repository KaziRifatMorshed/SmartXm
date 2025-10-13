
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/sqldrivers/libqsqlpsql.so"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT FALSE)
                message(FATAL_ERROR "Cannot find 'plugins/sqldrivers/libqsqlpsql.so' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/sqldrivers/libqsqlpsql.so' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/sqldrivers/libqsqlpsql.so" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QPSQLDriverPlugin" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QPSQLDriverPlugin")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QPSQLDriverPlugin"
                    "${sha1}")
            endif()
            file(APPEND "/home/noobcod3r-rtx/Documents/GitHub/SmartXm-Cross-Platform-Desktop-Exam-Management-with-Integrated-Judge/Tools/DB_Connector/mysql-kaziRTX/qt_sbom/staging-qsqlitedriverplugins.spdx.in"
"
FileName: ./plugins/sqldrivers/libqsqlpsql.so
SPDXID: SPDXRef-PackagedFile-qt-plugin-QPSQLDriverPlugin
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QPSQLDriverPlugin CONTAINS SPDXRef-PackagedFile-qt-plugin-QPSQLDriverPlugin
Relationship: SPDXRef-PackagedFile-qt-plugin-QPSQLDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/psql/main.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QPSQLDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/psql/qsql_psql.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QPSQLDriverPlugin GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qsqlitedriverplugins/psql/qsql_psql_p.h
"
                )
        endif()
