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

#include "StudentManager.h"
#include <QDebug>

/*static*/ QQmlApplicationEngine *StudentManager::m_qmlEngine = nullptr;
/*static*/ SaveManager::Ptr StudentManager::mSaveManager;

StudentManager::StudentManager(QObject *parent)
    : QObject(parent)
    , mSelectedStudent("", "", "", "")
    , mSelectedGroupIdx(0)
{
    loadStudentsFromDB();
}

/*static*/ QObject* StudentManager::studentManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    StudentManager *studentManager = new StudentManager();
    return studentManager;
}

/*static*/ void StudentManager::setQmlEngine(QQmlApplicationEngine *engine)
{
    StudentManager::m_qmlEngine = engine;
}

/*static*/ void StudentManager::setSaveManager(std::shared_ptr<SaveManager> saveMgr)
{
    StudentManager::mSaveManager = saveMgr;
}

void StudentManager::clearComponentCache()
{
    StudentManager::m_qmlEngine->clearComponentCache();
}

void StudentManager::selectedStudent(const QString& student)
{
    auto studentInfo = student.split(',');
    mSelectedStudent = Student(studentInfo.at(0), studentInfo.at(1),
                        studentInfo.at(2), studentInfo.at(3));
}

void StudentManager::selectedStudent(const QString& name, const QString& phone, const QString& email, const QString& group)
{
    mSelectedStudent = Student(name, phone, email, group);
}

bool StudentManager::existsStudent(const QString& name, const QString& group)
{
    Student stud(name, "", "", group);
    auto student = std::find(mStudentList.begin(), mStudentList.end(), stud);

    if (mStudentList.cend() != student)
        return true;
    return false;
}

void StudentManager::createStudent(const QString& name, const QString& phone, const QString& email, const QString& group)
{
    Student stud(name, phone, email, group);
    mStudentList.push_back(stud);

    if (mSaveManager)
        mSaveManager->saveStudent(stud);
}

void StudentManager::deleteStudent(const QString& name, const QString& phone, const QString& email, const QString& group)
{
    Student stud(name, phone, email, group);
    mStudentList.removeOne(stud);

    if (mSaveManager)
        mSaveManager->removeStudent(stud);
}

QStringList StudentManager::getGroups()
{
    return mSaveManager->getGroups();
}

QStringList StudentManager::getAllStudents()
{
    QStringList result;

    for (auto& stud : mStudentList)
        result << stud.name() + "," + stud.phone() + "," + stud.email() + "," + stud.group();

    return result;
}

QStringList StudentManager::getStudentsByGroup(const QString& group)
{
    QStringList result;

    for (auto& stud : mStudentList)
    {
        if (!group.compare(stud.group()))
            result << stud.name() + "," + stud.phone() + "," + stud.email() + "," + stud.group();
    }

    return result;
}

QStringList StudentManager::getStudentsByName(const QString& name)
{
    QStringList result;

    for (auto& stud : mStudentList)
    {
        if (stud.name().contains(name))
            result << stud.name() + "," + stud.phone() + "," + stud.email() + "," + stud.group();
    }

    return result;
}

QStringList StudentManager::getStudentsByNameAndGroup(const QString& name, const QString& group)
{
    QStringList result;

    for (auto& stud : mStudentList)
    {
        if (stud.name().contains(name) && !stud.group().compare(group))
            result << stud.name() + "," + stud.phone() + "," + stud.email() + "," + stud.group();
    }

    return result;
}

void StudentManager::loadStudentsFromDB()
{
    if (mSaveManager)
    {
        mStudentList.clear();
        mStudentList = mSaveManager->loadAllStudents();
    }
}

QString StudentManager::selectedStudentName() const
{
    return mSelectedStudent.name();
}

QString StudentManager::selectedStudentPhone() const
{
    return mSelectedStudent.phone();
}

QString StudentManager::selectedStudentEmail() const
{
    return mSelectedStudent.email();
}

QString StudentManager::selectedStudentGroup() const
{
    return mSelectedStudent.group();
}

void StudentManager::selectedGroupIdx(const int& group)
{
    mSelectedGroupIdx = group;
}

int StudentManager::selectedGroupIdx() const
{
    return mSelectedGroupIdx;
}
