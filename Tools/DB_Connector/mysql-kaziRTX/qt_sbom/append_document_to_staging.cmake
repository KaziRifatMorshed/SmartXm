
        cmake_minimum_required(VERSION 3.16)
        message(STATUS "Starting SBOM generation in build dir: /home/noobcod3r-rtx/Documents/GitHub/SmartXm-Cross-Platform-Desktop-Exam-Management-with-Integrated-Judge/Tools/DB_Connector/mysql-kaziRTX/qt_sbom/staging-qsqlitedriverplugins.spdx.in")
        set(QT_SBOM_EXTERNAL_DOC_REFS "")
        file(READ "/home/noobcod3r-rtx/Documents/GitHub/SmartXm-Cross-Platform-Desktop-Exam-Management-with-Integrated-Judge/Tools/DB_Connector/mysql-kaziRTX/qt_sbom/SPDXRef-DOCUMENT-qsqlitedriverplugins.spdx.in" content)
        # Override any previous file because we're starting from scratch.
        file(WRITE "/home/noobcod3r-rtx/Documents/GitHub/SmartXm-Cross-Platform-Desktop-Exam-Management-with-Integrated-Judge/Tools/DB_Connector/mysql-kaziRTX/qt_sbom/staging-qsqlitedriverplugins.spdx.in" "${content}")
