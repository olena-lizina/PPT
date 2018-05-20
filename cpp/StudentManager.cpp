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
#include "StudentModel.h"

#include <QDebug>
#include <QFile>
#include <QDir>

/*static*/ QQmlApplicationEngine *StudentManager::m_qmlEngine = nullptr;
/*static*/ SaveManager::Ptr StudentManager::mSaveManager;

StudentManager::StudentManager(QObject *parent)
    : QObject(parent)
    , mSelectedGroupIdx(0)
{
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

void StudentManager::selectedStudent(const int& id)
{
    auto studIter = std::find_if(mStudentList.cbegin(), mStudentList.cend(),
                                 [&id](const Student& stud){ return stud.id == id; });
    mSelectedStudent = mStudentList.cend() == studIter ? Student() : *studIter;
}

QObject* StudentManager::selectedStudent() const
{
    return new StudentModel(mSelectedStudent, mGroupMap[mSelectedStudent.groupId]);
}

bool StudentManager::existsStudent(const QString&, const QString&)
{
    return false;
}

void StudentManager::updateStudent(int id, QString name, QString phone, QString group, QString email, QString photo)
{
    checkGroup(group);

    Student edit;
    edit.id = id;
    edit.name = name;
    edit.email = email;
    edit.phone = phone;
    edit.photoPath = photo;
    edit.groupId = mGroupMap.key(group);

    auto studIter = std::find_if(mStudentList.begin(), mStudentList.end(),
                                 [&id](Student& stud){ return stud.id == id; });
    if (mStudentList.end() == studIter)
    {
        qWarning() << "Cannot update student: " << name;
        return;
    }

    mSaveManager->updStudent(edit);
    *studIter = edit;
}

void StudentManager::checkGroup(const QString& name)
{
    const int defaultGroup = -1;
    if (defaultGroup == mGroupMap.key(name, defaultGroup))
    {
        // no such group
        Group tmp;
        tmp.name = name;
        mSaveManager->addGroup(tmp);

        mGroupMap.clear();
        auto groups = mSaveManager->loadGroup();

        for (auto it : groups)
            mGroupMap.insert(it.id, it.name);
    }
}

void StudentManager::addStudent(QString name, QString phone, QString group, QString email, QString photo)
{
    checkGroup(group);

    Student add;
    add.name = name;
    add.email = email;
    add.phone = phone;
    add.photoPath = photo;
    add.groupId = mGroupMap.key(group);
    mSaveManager->addStudent(add);

    mStudentList.clear();
    mStudentList = mSaveManager->loadStudent();
}

void StudentManager::deleteStudent(int id)
{
    auto studIter = std::find_if(mStudentList.begin(), mStudentList.end(),
                                 [&id](Student& stud){ return stud.id == id; });
    if (mStudentList.end() == studIter)
    {
        qWarning() << "Cannot delete student with id: " << id;
        return;
    }

    int groupId = studIter->groupId;
    mSaveManager->delStudent(id);
    mStudentList.erase(studIter);

    auto hasGroup = std::find_if(mStudentList.begin(), mStudentList.end(),
                 [groupId](Student& it){ return it.groupId == groupId; });

    if (hasGroup == mStudentList.end())
        mSaveManager->delGroup(groupId);

    loadStudentsFromDB();
}

QStringList StudentManager::getGroups()
{
    QStringList result;    
    result << tr("All groups");

    for (auto& it: mGroupMap)
        result << it;

    return result;
}

int StudentManager::getGroupIdx(QString name)
{
    if (!name.compare("All groups"))
        return 0;

    return mGroupMap.key(name);
}

QList<QObject*> StudentManager::getAllStudents()
{
    QList<QObject*> result;

    for (auto& stud : mStudentList)
        result.append(new StudentModel(stud, mGroupMap[stud.groupId]));

    return result;
}

QList<QObject*> StudentManager::getStudentsByGroup(const QString& group)
{
    QList<QObject*> result;
    int groupId = mGroupMap.key(group);

    for (auto& stud : mStudentList)
        if (stud.groupId == groupId)
            result.append(new StudentModel(stud, group));

    return result;
}

QList<QObject*> StudentManager::getStudentsByName(const QString& partOfName)
{
    QList<QObject*> result;

    for (auto& stud : mStudentList)
        if (stud.name.contains(partOfName))
            result.append(new StudentModel(stud, mGroupMap[stud.id]));

    return result;
}

void StudentManager::loadStudentsFromDB()
{
    if (mSaveManager)
    {
        mStudentList.clear();
        mGroupMap.clear();
        mStudentList = mSaveManager->loadStudent();
        auto groups = mSaveManager->loadGroup();

        for (auto it : groups)
            mGroupMap.insert(it.id, it.name);
    }
}
QString StudentManager::copyExternalPhoto(QString path)
{
    QDir dir("photos");
    if (!dir.exists())
        dir.mkpath("photos");

    QString prefix("file:///");
    path.remove(0, prefix.size());
    QString fileName(path.right(path.size() - path.lastIndexOf('/') - 1));
    QString newPath(QDir::currentPath() + '/' + "photos" + '/' + fileName);
    QFile::copy(path, newPath);

    qDebug() << "file: " << fileName;
    return fileName;
}

void StudentManager::setSelectGroupId(int group)
{
    mSelectedGroupIdx = group;
}

int StudentManager::selectGroupId() const
{
    return mSelectedGroupIdx;
}
