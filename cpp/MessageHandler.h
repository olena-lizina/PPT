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

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextStream>

class MessageHandler : public QObject
{
    Q_OBJECT

public:
    explicit MessageHandler(QObject *parent = 0);

    // QML engine stuff
    static void setQmlEngine(QQmlApplicationEngine *engine);

    static void handler(QtMsgType messageType, const QMessageLogContext &context, const QString &message);

private:
    // QML engine stuff
    static QQmlApplicationEngine *m_qmlEngine;

    static QObject *m_qmlMessageHandler;
};

#endif // MESSAGEHANDLER_H
