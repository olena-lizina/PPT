QT += \
    core gui qml quick \
    multimedia sql \
    network websockets \
    xml svg \
    sensors bluetooth nfc \
    positioning location

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
    cpp/DBManager.h \
    cpp/DataTypes.h \
    cpp/StudentModel.h \
    cpp/TreeItem.h \
    cpp/ScreenContextBuffer.h \
    cpp/MailServiceManager.h

SOURCES += \
    cpp/main.cpp \
    cpp/StudentManager.cpp \
    cpp/SaveManager.cpp \
    cpp/LecturesManager.cpp \
    cpp/DBManager.cpp \
    cpp/TreeItem.cpp \
    cpp/ScreenContextBuffer.cpp \
    cpp/MailServiceManager.cpp

lupdate_only {
SOURCES += \
    qml/components/*.qml \
    qml/components/dialogs/*.qml \
    qml/components/palettes/*.qml \
    qml/modules/*.qml \
    qml/screens/*.qml \
    qml/screens/admins/*.qml \
    qml/screens/education/lectures/*.qml \
    qml/*.qml
}

TRANSLATIONS = resources/translations/ppt_ru.ts \
               resources/translations/ppt_uk_UA.ts

android {
    OTHER_FILES += platform-specific/android/AndroidManifest.xml
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/platform-specific/android
}

ios {
    ICON_DATA.files = \
        $$PWD/platform-specific/ios/Icon.png \
        $$PWD/platform-specific/ios/Icon@2x.png \
        $$PWD/platform-specific/ios/Icon-60.png \
        $$PWD/platform-specific/ios/Icon-60@2x.png \
        $$PWD/platform-specific/ios/Icon-72.png \
        $$PWD/platform-specific/ios/Icon-72@2x.png \
        $$PWD/platform-specific/ios/Icon-76.png \
        $$PWD/platform-specific/ios/Icon-76@2x.png \
        $$PWD/platform-specific/ios/Def.png \
        $$PWD/platform-specific/ios/Def@2x.png \
        $$PWD/platform-specific/ios/Def-Portrait.png \
        $$PWD/platform-specific/ios/Def-568h@2x.png
    QMAKE_BUNDLE_DATA += ICON_DATA

    QMAKE_INFO_PLIST = $$PWD/platform-specific/ios/Project-Info.plist
    OTHER_FILES += $$QMAKE_INFO_PLIST
}

# Location of SMTP Library
SMTP_LIBRARY_LOCATION = $$PWD/Libs

win32:CONFIG(release, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/release/ -lSMTPEmail
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/debug/ -lSMTPEmail
else:unix: LIBS += -L$$SMTP_LIBRARY_LOCATION/release -lSMTPEmail

INCLUDEPATH += $$SMTP_LIBRARY_LOCATION/include/
DEPENDPATH += $$SMTP_LIBRARY_LOCATION/include/
