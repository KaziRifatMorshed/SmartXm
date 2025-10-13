
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin" false)

        file(APPEND "/home/noobcod3r-rtx/Documents/GitHub/SmartXm-Cross-Platform-Desktop-Exam-Management-with-Integrated-Judge/Tools/DB_Connector/mysql-kaziRTX/qt_sbom/staging-qsqlitedriverplugins.spdx.in"
"
PackageName: QSQLiteDriverPlugin
SPDXID: SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin
PackageDownloadLocation: git://code.qt.io/qt/qsqlitedriverplugins.git
PackageVersion: 6.9.3
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qsqlitedriverplugins?library_name=QSQLiteDriverPlugin#sqlite
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qsqlitedriverplugins-QSQLiteDriverPlugin?vcs_url=https://code.qt.io/qt/qsqlitedriverplugins.git&library_name=QSQLiteDriverPlugin#sqlite
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: QSQLiteDriverPlugin
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.9.3:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qsqlitedriverplugins:6.9.3:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin}
Relationship: SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin DEPENDS_ON DocumentRef-qtbase:SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin DEPENDS_ON DocumentRef-qtbase:SPDXRef-Package-qtbase-qt-module-Sql
Relationship: SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin DEPENDS_ON DocumentRef-qtbase:SPDXRef-Package-qtbase-qt-module-PlatformPluginInternal
Relationship: SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin DEPENDS_ON SPDXRef-Package-qsqlitedriverplugins-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite
Relationship: SPDXRef-Package-qsqlitedriverplugins CONTAINS SPDXRef-Package-qsqlitedriverplugins-qt-plugin-QSQLiteDriverPlugin
"
        )
