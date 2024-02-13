QT       += core gui sql core5compat network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
win32: RC_ICONS = $$PWD/Images/gandalf.ico
# Додати макрос з датою збірки для використання у коді
VERSION = 0.0.0.1

SOURCES += \
    AppParameters/AppParameters.cpp \
    AppParameters/criptpass.cpp \
    AppParameters/editpassexcepdialog.cpp \
    AppParameters/impostpassvncdialog.cpp \
    AppParameters/parametersdialog.cpp \
    AppParameters/passmanagerdialog.cpp \
    AppParameters/qaesencryption.cpp \
    DbSettingsDialog/dbsettingsdialog.cpp \
    Deploys/deploydata.cpp \
    Deploys/deploysmodel.cpp \
    Deploys/getdeploys.cpp \
    Deploys/numericSortProxyModel.cpp \
    LogginCategories/loggincategories.cpp \
    Logs/connectionslistdialog.cpp \
    Logs/connectionsmodel.cpp \
    Logs/logdata.cpp \
    Logs/logger.cpp \
    SendMessage/sendmessagedialog.cpp \
    Terminals/TerminalListModel.cpp \
    Terminals/buttonvnc.cpp \
    Terminals/connectiondata.cpp \
    Terminals/connstatusvnc.cpp \
    Terminals/dispenserinfo.cpp \
    Terminals/dispenserproperty.cpp \
    Terminals/editworkplacedialog.cpp \
    Terminals/listazsdialog.cpp \
    Terminals/objectform.cpp \
    Terminals/pingmodel.cpp \
    Terminals/punpproperty.cpp \
    Terminals/searchform.cpp \
    Terminals/tankproperty.cpp \
    Terminals/tanksinfo.cpp \
    Terminals/tanksinfomodel.cpp \
    Terminals/termdata.cpp \
    Users/userlistdialog.cpp \
    Users/userprofiledialog.cpp \
    Users/usersmodel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AppParameters/AppParameters.h \
    AppParameters/criptpass.h \
    AppParameters/editpassexcepdialog.h \
    AppParameters/impostpassvncdialog.h \
    AppParameters/parametersdialog.h \
    AppParameters/passmanagerdialog.h \
    AppParameters/qaesencryption.h \
    DbSettingsDialog/dbsettingsdialog.h \
    Deploys/deploydata.h \
    Deploys/deploysmodel.h \
    Deploys/getdeploys.h \
    Deploys/numericSortProxyModel.h \
    LogginCategories/loggincategories.h \
    Logs/connectionslistdialog.h \
    Logs/connectionsmodel.h \
    Logs/logdata.h \
    Logs/logger.h \
    SendMessage/sendmessagedialog.h \
    Terminals/TerminalListModel.h \
    Terminals/buttonvnc.h \
    Terminals/connectiondata.h \
    Terminals/connstatusvnc.h \
    Terminals/dispenserinfo.h \
    Terminals/dispenserproperty.h \
    Terminals/editworkplacedialog.h \
    Terminals/listazsdialog.h \
    Terminals/objectform.h \
    Terminals/pingmodel.h \
    Terminals/punpproperty.h \
    Terminals/searchform.h \
    Terminals/tankproperty.h \
    Terminals/tanksinfo.h \
    Terminals/tanksinfomodel.h \
    Terminals/termdata.h \
    Users/UsersModel.h \
    Users/userlistdialog.h \
    Users/userprofiledialog.h \
    mainwindow.h

FORMS += \
    AppParameters/editpassexcepdialog.ui \
    AppParameters/impostpassvncdialog.ui \
    AppParameters/parametersdialog.ui \
    AppParameters/passmanagerdialog.ui \
    DbSettingsDialog/dbsettingsdialog.ui \
    Logs/connectionslistdialog.ui \
    SendMessage/sendmessagedialog.ui \
    Terminals/editworkplacedialog.ui \
    Terminals/listazsdialog.ui \
    Terminals/objectform.ui \
    Terminals/searchform.ui \
    Users/userlistdialog.ui \
    Users/userprofiledialog.ui \
    mainwindow.ui




CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

TRANSLATIONS += Gandalf_uk.ts \
                Gandalf_en.ts \
                Gandalf_ru.ts


