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

#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H
#include <QObject>
#include <memory>
#include "SaveManager.h"
#include "DataTypes.h"
#include <QQmlApplicationEngine>

class StudentManager: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int selectGroupId READ selectGroupId WRITE setSelectGroupId NOTIFY selectGroupIdChanged)

public:
    explicit StudentManager(QObject *parent = nullptr);

    // singleton type provider function
    static QObject* studentManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    static void setSaveManager(std::shared_ptr<SaveManager> saveMgr);
    Q_INVOKABLE void clearComponentCache();

    Q_INVOKABLE bool existsStudent(const QString& name, const QString& group);

    Q_INVOKABLE void updateStudent(int id, QString name, QString phone, QString group, QString email, QString photo);
    Q_INVOKABLE void addStudent(QString name, QString phone, QString group, QString email, QString photo);
    Q_INVOKABLE void deleteStudent(int id);

    Q_INVOKABLE QStringList getGroups();
    Q_INVOKABLE QList<QObject*> getAllStudents();
    Q_INVOKABLE QList<QObject*> getStudentsByGroup(const QString& groupName);
    Q_INVOKABLE QList<QObject*> getStudentsByName(const QString& partOfName);

    Q_INVOKABLE void selectedStudent(const int& id);
    Q_INVOKABLE QObject* selectedStudent() const;

    Q_INVOKABLE void loadStudentsFromDB();

    Q_INVOKABLE QString copyExternalPhoto(QString path);

    void setSelectGroupId(int);
    int selectGroupId() const;

signals:
    void selectGroupIdChanged();

private:
    void checkGroup(const QString& name);

protected:
    QList<Student> mStudentList;
    QMap<int, QString> mGroupMap;
    Student mSelectedStudent;
    int mSelectedGroupIdx;
    static SaveManager::Ptr mSaveManager;
    static QQmlApplicationEngine *m_qmlEngine;

    QList<QObject*> model;
};

#endif // STUDENTMANAGER_H
