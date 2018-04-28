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
/*static*/ std::shared_ptr<SaveManager> StudentManager::mSaveManager = nullptr;

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
    mSelectedStudent = Student(studentInfo.at(0).toLocal8Bit().constData(), studentInfo.at(2).toLocal8Bit().constData(),
                        studentInfo.at(3).toLocal8Bit().constData(), studentInfo.at(1).toLocal8Bit().constData());
}

void StudentManager::createStudent(const QString& name, const QString& phone, const QString& email, const QString& group)
{
    Student student(name, phone, email, group);
    mStudentList.push_back(student);

    if (mSaveManager)
        mSaveManager->saveStudent(student);
}

void StudentManager::deleteStudent(const QString& student)
{
    auto studentInfo = student.split(',');
    Student toRemove(studentInfo.at(0).toLocal8Bit().constData(), studentInfo.at(2).toLocal8Bit().constData(),
                        studentInfo.at(3).toLocal8Bit().constData(), studentInfo.at(1).toLocal8Bit().constData());
    mStudentList.removeOne(toRemove);

    if (mSaveManager)
        mSaveManager->removeStudent(toRemove);
}

void StudentManager::updateStudent(const QString& name, const QString& phone, const QString& email, const QString& group)
{
    auto student = std::find(mStudentList.begin(), mStudentList.end(), mSelectedStudent);

    if (mStudentList.cend() != student)
    {
        Student newStudent(name, phone, email, group);

        if (mSaveManager)
            mSaveManager->updateStudent(*student, newStudent);

        *student = newStudent;
    }
}

QStringList StudentManager::getGroups()
{
    return mSaveManager->getGroups();
}

QStringList StudentManager::getAllStudents()
{
    QStringList result;

    for (auto& stud : mStudentList)
        result << stud.name() + "," + stud.group() + "," + stud.phone() + "," + stud.email();

    return result;
}

QStringList StudentManager::getStudentsByGroup(const QString& group)
{
    QStringList result;

    for (auto& stud : mStudentList)
    {
        if (!group.compare(stud.group()))
            result << stud.name() + "," + stud.group() + "," + stud.phone() + "," + stud.email();
    }

    return result;
}

QStringList StudentManager::getStudentsByName(const QString& name)
{
    QStringList result;

    for (auto& stud : mStudentList)
    {
        if (stud.name().contains(name))
            result << stud.name() + "," + stud.group() + "," + stud.phone() + "," + stud.email();
    }

    return result;
}

QStringList StudentManager::getStudentsByNameAndGroup(const QString& name, const QString& group)
{
    QStringList result;

    for (auto& stud : mStudentList)
    {
        if (stud.name().contains(name) && !stud.group().compare(group))
            result << stud.name() + "," + stud.group() + "," + stud.phone() + "," + stud.email();
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
