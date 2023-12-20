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
    AppParameters/parametersdialog.cpp \
    AppParameters/qaesencryption.cpp \
    DbSettingsDialog/dbsettingsdialog.cpp \
    Deploys/deploydata.cpp \
    Deploys/deploysmodel.cpp \
    Deploys/getdeploys.cpp \
    LogginCategories/loggincategories.cpp \
    Logs/logdata.cpp \
    Logs/logger.cpp \
    Terminals/buttonvnc.cpp \
    Terminals/connectiondata.cpp \
    Terminals/connstatusvnc.cpp \
    Terminals/dispenserinfo.cpp \
    Terminals/dispenserproperty.cpp \
    Terminals/objectform.cpp \
    Terminals/pingmodel.cpp \
    Terminals/punpproperty.cpp \
    Terminals/searchform.cpp \
    Terminals/tankproperty.cpp \
    Terminals/tanksinfo.cpp \
    Terminals/tanksinfomodel.cpp \
    Terminals/termdata.cpp \
    Users/userprofiledialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AppParameters/AppParameters.h \
    AppParameters/criptpass.h \
    AppParameters/parametersdialog.h \
    AppParameters/qaesencryption.h \
    DbSettingsDialog/dbsettingsdialog.h \
    Deploys/deploydata.h \
    Deploys/deploysmodel.h \
    Deploys/getdeploys.h \
    LogginCategories/loggincategories.h \
    Logs/logdata.h \
    Logs/logger.h \
    Terminals/buttonvnc.h \
    Terminals/connectiondata.h \
    Terminals/connstatusvnc.h \
    Terminals/dispenserinfo.h \
    Terminals/dispenserproperty.h \
    Terminals/objectform.h \
    Terminals/pingmodel.h \
    Terminals/punpproperty.h \
    Terminals/searchform.h \
    Terminals/tankproperty.h \
    Terminals/tanksinfo.h \
    Terminals/tanksinfomodel.h \
    Terminals/termdata.h \
    Users/userprofiledialog.h \
    mainwindow.h

FORMS += \
    AppParameters/parametersdialog.ui \
    DbSettingsDialog/dbsettingsdialog.ui \
    Terminals/objectform.ui \
    Terminals/searchform.ui \
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


