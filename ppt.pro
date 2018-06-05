QT += \
    core gui qml quick sql widgets

TARGET = ppt
TEMPLATE = app

CONFIG += mobility
MOBILITY =

RESOURCES += \
    ppt_resources.qrc

RC_ICONS = $$PWD/resources/images/48icon.ico

HEADERS += \
    cpp/StudentManager.h \
    cpp/SaveManager.h \
    cpp/LecturesManager.h \
    cpp/DataTypes.h \
    cpp/StudentModel.h \
    cpp/TreeItem.h \
    cpp/ScreenContextBuffer.h \
    cpp/MailServiceManager.h \
    cpp/LabsManager.h \
    cpp/ReportInfoModel.h \
    cpp/ManagerInterface.h \
    cpp/GradesManager.h \
    cpp/SQLiteManager.h

SOURCES += \
    cpp/main.cpp \
    cpp/StudentManager.cpp \
    cpp/SaveManager.cpp \
    cpp/LecturesManager.cpp \
    cpp/TreeItem.cpp \
    cpp/ScreenContextBuffer.cpp \
    cpp/MailServiceManager.cpp \
    cpp/LabsManager.cpp \
    cpp/ManagerInterface.cpp \
    cpp/GradesManager.cpp \
    cpp/SQLiteManager.cpp

lupdate_only {
SOURCES += \
    qml/components/*.qml \
    qml/components/dialogs/*.qml \
    qml/components/palettes/*.qml \
    qml/screens/*.qml \
    qml/screens/admins/*.qml \
    qml/screens/education/lectures/*.qml \
    qml/screens/education/labs/*.qml \
    qml/*.qml
}

TRANSLATIONS = resources/translations/ppt_ru.ts \
               resources/translations/ppt_uk_UA.ts

# Location of SMTP Library
SMTP_LIBRARY_LOCATION = $$PWD/Libs

win32:CONFIG(release, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/release/ -lSMTPEmail
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/debug/ -lSMTPEmail
else:unix: LIBS += -L$$SMTP_LIBRARY_LOCATION/release -lSMTPEmail

INCLUDEPATH += $$SMTP_LIBRARY_LOCATION/include/
DEPENDPATH += $$SMTP_LIBRARY_LOCATION/include/
