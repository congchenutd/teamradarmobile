# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xE34B1D0A

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices ReadUserData WriteUserData

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += sensors

QT += network sql

SOURCES += \
    Main.cpp \
    MainWindow.cpp \
    SettingDlg.cpp \
	../QtCreator/src/plugins/TeamRadar/Connection.cpp \
	../QtCreator/src/plugins/TeamRadar/PeerModel.cpp \
	../QtCreator/src/plugins/TeamRadar/PeerManager.cpp \
	../QtCreator/src/plugins/TeamRadar/Utility.cpp \
	../QtCreator/src/plugins/TeamRadar/TeamRadarView.cpp \
	../QtCreator/src/plugins/TeamRadar/Node.cpp \
	../QtCreator/src/plugins/TeamRadar/ModeTag.cpp \
	../QtCreator/src/plugins/TeamRadar/LightTrail.cpp \
	../QtCreator/src/plugins/TeamRadar/Label.cpp \
	../QtCreator/src/plugins/TeamRadar/Engine.cpp \
	../QtCreator/src/plugins/TeamRadar/Edge.cpp \
    SpeedDlg.cpp \
	../QtCreator/src/plugins/TeamRadar/Setting.cpp \
    ProjectsDlg.cpp \
    WelcomeDlg.cpp
HEADERS += \
    MainWindow.h \
    SettingDlg.h \
	../QtCreator/src/plugins/TeamRadar/Connection.h \
	../QtCreator/src/plugins/TeamRadar/TeamRadarEvent.h \
	../QtCreator/src/plugins/TeamRadar/MySetting.h \
	../QtCreator/src/plugins/TeamRadar/PeerModel.h \
	../QtCreator/src/plugins/TeamRadar/PeerManager.h \
	../QtCreator/src/plugins/TeamRadar/Utility.h \
	../QtCreator/src/plugins/TeamRadar/TeamRadarView.h \
	../QtCreator/src/plugins/TeamRadar/Node.h \
	../QtCreator/src/plugins/TeamRadar/ModeTag.h \
	../QtCreator/src/plugins/TeamRadar/LightTrail.h \
	../QtCreator/src/plugins/TeamRadar/Label.h \
	../QtCreator/src/plugins/TeamRadar/Engine.h \
	../QtCreator/src/plugins/TeamRadar/Edge.h \
    SpeedDlg.h \
	../QtCreator/src/plugins/TeamRadar/Setting.h \
    ProjectsDlg.h \
    WelcomeDlg.h
FORMS += \
    MainWindow.ui \
    SettingDlg.ui \
    SpeedDlg.ui \
    ProjectsDlg.ui \
    WelcomeDlg.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES += \
    Resource.qrc

OTHER_FILES +=





















































