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
#include "SaveManager.h"
#include "Student.h"

StudentManager::StudentManager(std::weak_ptr<SaveManager> saveMgr, QObject *parent)
    : QObject(parent)
    , mSaveManager(saveMgr)
{

}

void StudentManager::createStudent(const QString& name, const QString& phone, const QString& email, const QString& group)
{
    Student student(name, phone, email, group);
    mStudentList.push_back(student);

    auto saveMgr = mSaveManager.lock();

    if (saveMgr)
        mSaveManager->saveStudent(student);
}

void StudentManager::deleteStudent(const QString& name, const QString& group)
{
    auto student = std::find_if(mStudentList.cbegin(), mStudentList.cend(),
                                [&name, &group](const Student& student)
    { return !name.compare(student.name()) && !group.compare(student.group()); });

    if (mStudentList.cend() != student)
        mStudentList.removeOne(*student);

    auto saveMgr = mSaveManager.lock();

    if (saveMgr)
        mSaveManager->removeStudent(*student);
}

void StudentManager::updateStudent(const QString& oldName, const QString& oldGroup, const QString& name, const QString& phone, const QString& email, const QString& group)
{
    auto student = std::find_if(mStudentList.cbegin(), mStudentList.cend(),
                                [&oldName, &oldGroup](const Student& student)
    { return !oldName.compare(student.name()) && !oldGroup.compare(student.group()); });

    if (mStudentList.cend() != student)
        mStudentList.removeOne(*student);

    auto saveMgr = mSaveManager.lock();

    if (saveMgr)
        mSaveManager->removeStudent(*student);

    createStudent(name, phone, email, group);
}

QStringList StudentManager::getStudentsByGroup(const QString& group)
{
    QStringList result;

    QList<Student>::iterator iter = std::find_if(mStudentList.cbegin(), mStudentList.cend(),
                                                 [&group](const Student& student)
    { return !group.compare(student.group()); });

    while (mStudentList.cend() != iter)
    {
        result << *iter.name() + "," + *iter.group();

        iter = std::find_if(mStudentList.cbegin(), mStudentList.cend(),
                            [&group](const Student& student)
        { return !group.compare(student.group()); });
    }

    return result;
}

QStringList StudentManager::getStudentsByName(const QString& name)
{
    QStringList result;

    QList<Student>::iterator iter = std::find_if(mStudentList.cbegin(), mStudentList.cend(),
                                                 [&name](const Student& student)
    { return !name.compare(student.name()); });

    while (mStudentList.cend() != iter)
    {
        result << *iter.name() + "," + *iter.group();

        iter = std::find_if(mStudentList.cbegin(), mStudentList.cend(),
                            [&name](const Student& student)
        { return !name.compare(student.name()); });
    }

    return result;
}

QStringList StudentManager::getStudentsByNameAndGroup(const QString& name, const QString& group)
{
    QStringList result;

    QList<Student>::iterator iter = std::find_if(mStudentList.cbegin(), mStudentList.cend(),
                                                 [&name, &group](const Student& student)
    { return !name.compare(student.name()) && !group.compare(student.group()); });

    while (mStudentList.cend() != iter)
    {
        result << *iter.name() + "," + *iter.group();

        iter = std::find_if(mStudentList.cbegin(), mStudentList.cend(),
                            [&name](const Student& student)
        { return !name.compare(student.group()); });
    }

    return result;
}

void StudentManager::loadStudentsFromDB()
{
    auto saveMgr = mSaveManager.lock();

    if (saveMgr)
    {
        mStudentList.clear();
        mStudentList = saveMgr->loadAllStudents();
    }
}
