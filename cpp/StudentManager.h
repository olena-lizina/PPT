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

class Student;
class SaveManager;

class StudentManager: public QObject
{
    Q_OBJECT

public:
    explicit StudentManager(std::weak_ptr<SaveManager>, QObject *parent = nullptr);

    Q_INVOKABLE void createStudent(const QString&, const QString&, const QString&, const QString&);
    Q_INVOKABLE void deleteStudent(const QString&, const QString&);
    Q_INVOKABLE void updateStudent(const QString&, const QString&, const QString&, const QString&, const QString&, const QString&);

    Q_INVOKABLE QStringList getStudentsByGroup(const QString&);
    Q_INVOKABLE QStringList getStudentsByName(const QString&);
    Q_INVOKABLE QStringList getStudentsByNameAndGroup(const QString&, const QString&);

private:
    void loadStudentsFromDB();

protected:
    QList<Student> mStudentList;
    std::weak_ptr<SaveManager> mSaveManager;
};

#endif // STUDENTMANAGER_H
