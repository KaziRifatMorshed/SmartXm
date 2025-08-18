include("D:/230201/QT GUI/Notepad/build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Notepad-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE D:/230201/QT GUI/Notepad/build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/Notepad.exe
    GENERATE_QT_CONF
)
