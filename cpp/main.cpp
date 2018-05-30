/****************************************************************************
**
** Copyright (C) 2017-2018 Olena Lizina
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QIcon>
#include <QTranslator>
#include "StudentManager.h"
#include "LecturesManager.h"
#include "ScreenContextBuffer.h"
#include "MailServiceManager.h"
#include "LabsManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QTranslator translator;
    translator.load("ppt_" + QLocale::system().name(), QCoreApplication::applicationDirPath());
    app.installTranslator(&translator);

    app.setApplicationName("Pedagogical program tool");
    app.setApplicationVersion("1.0.1");
    app.setOrganizationName("lizinaolena");
    app.setOrganizationDomain("com.lizinaolena.ppz");

    qmlRegisterSingletonType<StudentManager>("StudentManager", 1, 1, "StudentManager", &StudentManager::managerProvider);
    qmlRegisterSingletonType<LecturesManager>("LecturesManager", 1, 1, "LecturesManager", &LecturesManager::managerProvider);
    qmlRegisterSingletonType<ScreenContextBuffer>("ScreenContextBuffer", 1, 1, "ScreenContextBuffer", &ScreenContextBuffer::screenContextBufferProvider);
    qmlRegisterSingletonType<MailServiceManager>("MailServiceManager", 1, 1, "MailServiceManager", &MailServiceManager::managerProvider);
    qmlRegisterSingletonType<LabsManager>("LabsManager", 1, 1, "LabsManager", &LabsManager::managerProvider);

    SaveManager::Ptr saveManager(new SaveManager());
    StudentManager::setSaveManager(saveManager);
    LecturesManager::setSaveManager(saveManager);
    MailServiceManager::setSaveManager(saveManager);
    LabsManager::setSaveManager(saveManager);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("applicationDirPath", QGuiApplication::applicationDirPath());
    engine.load(QUrl("qrc:/qml/main.qml"));

    StudentManager::setQmlEngine(&engine);
    LecturesManager::setQmlEngine(&engine);
    ScreenContextBuffer::setQmlEngine(&engine);
    MailServiceManager::setQmlEngine(&engine);
    LabsManager::setQmlEngine(&engine);

    return app.exec();
}
