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
#include "Student.h"
#include <QQmlApplicationEngine>

class StudentManager: public QObject
{
    Q_OBJECT

public:
    explicit StudentManager(QObject *parent = nullptr);

    // singleton type provider function
    static QObject* studentManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    static void setSaveManager(std::shared_ptr<SaveManager> saveMgr);
    Q_INVOKABLE void clearComponentCache();

    Q_INVOKABLE bool existsStudent(const QString& name, const QString& group);
    Q_INVOKABLE void createStudent(const QString& name, const QString& phone, const QString& email, const QString& group);
    Q_INVOKABLE void deleteStudent(const QString& name, const QString& phone, const QString& email, const QString& group);

    Q_INVOKABLE QStringList getGroups();
    Q_INVOKABLE QStringList getAllStudents();
    Q_INVOKABLE QStringList getStudentsByGroup(const QString& groupName);
    Q_INVOKABLE QStringList getStudentsByName(const QString&);
    Q_INVOKABLE QStringList getStudentsByNameAndGroup(const QString& name, const QString& group);

    Q_INVOKABLE void selectedStudent(const QString&);
    Q_INVOKABLE void selectedStudent(const QString& name, const QString& phone, const QString& email, const QString& group);
    Q_INVOKABLE QString selectedStudentName() const;
    Q_INVOKABLE QString selectedStudentPhone() const;
    Q_INVOKABLE QString selectedStudentEmail() const;
    Q_INVOKABLE QString selectedStudentGroup() const;

    Q_INVOKABLE void selectedGroupIdx(const int&);
    Q_INVOKABLE int selectedGroupIdx() const;

private:
    void loadStudentsFromDB();

protected:
    QList<Student> mStudentList;
    Student mSelectedStudent;
    int mSelectedGroupIdx;
    static SaveManager::Ptr mSaveManager;
    static QQmlApplicationEngine *m_qmlEngine;
};

#endif // STUDENTMANAGER_H
