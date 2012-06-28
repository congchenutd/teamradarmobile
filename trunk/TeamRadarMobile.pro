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
INCLUDEPATH += ../TeamRadar \
			   ../ImageColorBoolModel

SOURCES += \
	Main.cpp \
	MainWindow.cpp \
	SettingDlg.cpp \
	../TeamRadar/Connection.cpp \
	../TeamRadar/PeerModel.cpp \
	../TeamRadar/PeerManager.cpp \
	../TeamRadar/Utility.cpp \
	../TeamRadar/TeamRadarView.cpp \
	../TeamRadar/Node.cpp \
	../TeamRadar/ModeTag.cpp \
	../TeamRadar/LightTrail.cpp \
	../TeamRadar/NodeLabel.cpp \
	../TeamRadar/Engine.cpp \
	../TeamRadar/Edge.cpp \
	../TeamRadar/RequestEventsDlg.cpp \
	SpeedDlg.cpp \
	../TeamRadar/Setting.cpp \
	ProjectsDlg.cpp \
	WelcomeDlg.cpp \
	../ImageColorBoolModel/ImageColorBoolProxy.cpp \
	../ImageColorBoolModel/ImageColorBoolDelegate.cpp \
	PlaylistDlg.cpp
HEADERS += \
	MainWindow.h \
	SettingDlg.h \
	../TeamRadar/Connection.h \
	../TeamRadar/TeamRadarEvent.h \
	../TeamRadar/MySetting.h \
	../TeamRadar/PeerModel.h \
	../TeamRadar/PeerManager.h \
	../TeamRadar/Utility.h \
	../TeamRadar/TeamRadarView.h \
	../TeamRadar/Node.h \
	../TeamRadar/ModeTag.h \
	../TeamRadar/LightTrail.h \
	../TeamRadar/NodeLabel.h \
	../TeamRadar/Engine.h \
	../TeamRadar/Edge.h \
	../TeamRadar/RequestEventsDlg.h \
	SpeedDlg.h \
	../TeamRadar/Setting.h \
	ProjectsDlg.h \
	WelcomeDlg.h \
	../ImageColorBoolModel/ImageColorBoolProxy.h \
	../ImageColorBoolModel/ImageColorBoolDelegate.h \
	PlaylistDlg.h \
    ../TeamRadar/Defines.h
FORMS += \
	MainWindow.ui \
	SettingDlg.ui \
	SpeedDlg.ui \
	ProjectsDlg.ui \
	WelcomeDlg.ui \
	../TeamRadar/RequestEventsDlg.ui \
	PlaylistDlg.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES += \
	Resource.qrc

OTHER_FILES +=














































































