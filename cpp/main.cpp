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
#include <QTranslator>
//#include "MessageHandler.h"
//#include "ProjectManager.h"
#include "StudentManager.h"
#include "LecturesManager.h"
#include "SyntaxHighlighter.h"

int main(int argc, char *argv[])
{
//    qInstallMessageHandler(&MessageHandler::handler);
    QGuiApplication app(argc, argv);
    app.setApplicationName("Pedagogical program tool");
    app.setApplicationVersion("1.0.1");
    app.setOrganizationName("lizinaolena");
    app.setOrganizationDomain("com.lizinaolena.ppz");

    QTranslator translator;
    translator.load("ppt_" + QLocale::system().name(), ":/resources/translations");
    app.installTranslator(&translator);

//    qmlRegisterSingletonType<ProjectManager>("ProjectManager", 1, 1, "ProjectManager", &ProjectManager::projectManagerProvider);

    qmlRegisterSingletonType<StudentManager>("StudentManager", 1, 1, "StudentManager", &StudentManager::studentManagerProvider);
    qmlRegisterSingletonType<LecturesManager>("LecturesManager", 1, 1, "LecturesManager", &LecturesManager::lecturesManagerProvider);

    SaveManager::Ptr saveManager(new SaveManager());
    StudentManager::setSaveManager(saveManager);
    LecturesManager::setSaveManager(saveManager);

    qmlRegisterType<SyntaxHighlighter>("SyntaxHighlighter", 1, 1, "SyntaxHighlighter");

    QQmlApplicationEngine engine(QUrl("qrc:/qml/main.qml"));
//    ProjectManager::setQmlEngine(&engine);
    StudentManager::setQmlEngine(&engine);
//    MessageHandler::setQmlEngine(&engine);
    LecturesManager::setQmlEngine(&engine);

    return app.exec();
}
