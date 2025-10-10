
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qsqlitedriverplugins-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qsqlitedriverplugins-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite" false)

        file(APPEND "/home/noobcod3r-rtx/Documents/GitHub/SmartXm-Cross-Platform-Desktop-Exam-Management-with-Integrated-Judge/Tools/DB_Connector/mysql-kaziRTX/qt_sbom/staging-qsqlitedriverplugins.spdx.in"
"
PackageName: QSQLiteDriverPlugin_Attribution_sqlite
SPDXID: SPDXRef-Package-qsqlitedriverplugins-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite
PackageDownloadLocation: https://www.sqlite.org/2025/sqlite-amalgamation-3500400.zip
PackageVersion: 3.50.4
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: blessing
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/sqlite/sqlite@version-3.50.4
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qsqlitedriverplugins?library_name=QSQLiteDriverPlugin_Attribution_sqlite#sqlite
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qsqlitedriverplugins-QSQLiteDriverPlugin_Attribution_sqlite?vcs_url=https://code.qt.io/qt/qsqlitedriverplugins.git&library_name=QSQLiteDriverPlugin_Attribution_sqlite#sqlite
PackageCopyrightText: <text>The authors disclaim copyright to the source code. However, a license can be obtained if needed.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: QSQLiteDriverPlugin_Attribution_sqlite
    Information extracted from:
     /home/noobcod3r-rtx/Qt/6.9.3/Src/qtbase/src/3rdparty/sqlite/qt_attribution.json
    Entry index: 0
    Id: sqlite
    Name: SQLite
    Description: SQLite is a small C library that implements a self-contained, embeddable, zero-configuration SQL database engine.
    Qt usage: Used in Qt SQL Lite plugin. Configure Qt with -system-sqlite or -no-sqlite to avoid.
    License: SQLite Blessing
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:sqlite:sqlite:3.50.4:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qsqlitedriverplugins-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qsqlitedriverplugins-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite}
Relationship: SPDXRef-Package-qsqlitedriverplugins CONTAINS SPDXRef-Package-qsqlitedriverplugins-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite
"
        )
