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
#include "ManagerInterface.h"
#include "DataTypes.h"

#include <memory>

class StudentManager: public ManagerInterface
{
    Q_OBJECT

public:

    using WPtr = std::weak_ptr<StudentManager>;
    using Ptr = std::shared_ptr<StudentManager>;

    explicit StudentManager(QObject *parent = nullptr);
    static QObject* managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE bool existsStudent(const QString& name, const QString& group);
    Q_INVOKABLE void updateStudent(int id, QString name, QString phone, QString group, QString email, QString photo);
    Q_INVOKABLE void addStudent(QString name, QString phone, QString group, QString email, QString photo);
    Q_INVOKABLE void deleteStudent(int id);

    Q_INVOKABLE QStringList getGroups();
    Q_INVOKABLE int getGroupIdx(QString name);
    Q_INVOKABLE QList<QObject*> getAllStudents();
    Q_INVOKABLE QList<QObject*> getStudentsByGroup(const QString& groupName);
    Q_INVOKABLE QList<QObject*> getStudentsByName(const QString& partOfName);    

    Q_INVOKABLE QString copyExternalPhoto(QString path);

private:
    void checkGroup(const QString& name);
    void loadStudentsFromDB();

protected:
    QList<Student> mStudentList;
    QMap<int, QString> mGroupMap;
    Student mSelectedStudent;
    int mSelectedGroupIdx;

    QList<QObject*> model;
};

#endif // STUDENTMANAGER_H
