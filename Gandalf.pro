QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
win32: RC_ICONS = $$PWD/Images/gandalf.ico


SOURCES += \
    AppParameters/AppParameters.cpp \
    AppParameters/criptpass.cpp \
    AppParameters/parametersdialog.cpp \
    AppParameters/qaesencryption.cpp \
    DbSettingsDialog/dbsettingsdialog.cpp \
    LogginCategories/loggincategories.cpp \
    Terminals/searchform.cpp \
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
    LogginCategories/loggincategories.h \
    Terminals/searchform.h \
    Terminals/termdata.h \
    Users/userprofiledialog.h \
    mainwindow.h

FORMS += \
    AppParameters/parametersdialog.ui \
    DbSettingsDialog/dbsettingsdialog.ui \
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

