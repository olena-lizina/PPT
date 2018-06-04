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

StudentManager::StudentManager(QObject *parent)
    : ManagerInterface(parent)
    , mSelectedGroupIdx(0)
{
    loadStudentsFromDB();
}

/*static*/ QObject* StudentManager::managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    StudentManager *manager = new StudentManager();
    return manager;
}

bool StudentManager::existsStudent(const QString& name, const QString& group)
{
    const int defaultGroup = -1;
    const int groupId = mGroupMap.key(group, defaultGroup);

    if (defaultGroup == groupId)
    {
        qWarning() << "existsStudent: No such group";
        return false;
    }

    if (mStudentList.end() == std::find_if(mStudentList.begin(), mStudentList.end(),
                              [&name, &groupId](Student st){ return st.name.compare(name) && st.groupId == groupId; }))
        return false;

    return true;
}

void StudentManager::updateStudent(int id, QString name, QString phone, QString group, QString email, QString photo)
{
    if (!mSaveManager)
    {
        qWarning() << "updateStudent: Save manager is dead!";
        return;
    }

    checkGroup(group);

    BaseItem * edit = new Student(id, name, phone, email, photo, mGroupMap.key(group));

    if (!edit)
    {
        qWarning() << "updateStudent: Cannot create instance of Student";
        return;
    }

    auto studIter = std::find_if(mStudentList.begin(), mStudentList.end(),
                                 [&id](Student& stud){ return stud.id == id; });
    if (mStudentList.end() == studIter)
    {
        qWarning() << "updateStudent: Student does not exist: " << name;
        return;
    }

    mSaveManager->editItem(edit, SaveManager::TYPE_STUDENT);

    if (edit)
        delete edit;

    mStudentList.clear();
    mStudentList = mSaveManager->loadStudent();
}

void StudentManager::checkGroup(const QString& name)
{
    if (!mSaveManager)
    {
        qWarning() << "checkGroup: Save manager is dead!";
        return;
    }

    const int defaultGroup = -1;

    if (defaultGroup != mGroupMap.key(name, defaultGroup))
        return;

    // no such group
    BaseItem * add = new Group(0, name);

    if (!add)
    {
        qWarning() << "checkGroup: Cannot create instance of Group";
        return;
    }

    mSaveManager->appendItem(add, SaveManager::TYPE_GROUP);

    if (add)
        delete add;

    mGroupMap.clear();
    auto groups = mSaveManager->loadGroup();

    for (auto it : groups)
        mGroupMap.insert(it.id, it.name);
}

void StudentManager::addStudent(QString name, QString phone, QString group, QString email, QString photo)
{
    if (!mSaveManager)
    {
        qWarning() << "addStudent: Save manager is dead!";
        return;
    }

    checkGroup(group);

    BaseItem * add = new Student(0, name, email, phone, photo, mGroupMap.key(group));

    if (!add)
    {
        qDebug() << "addStudent: Cannot create instance of Student";
        return;
    }

    mSaveManager->appendItem(add, SaveManager::TYPE_STUDENT);

    if (add)
        delete add;

    mStudentList.clear();
    mStudentList = mSaveManager->loadStudent();
}

void StudentManager::deleteStudent(int id)
{
    if (!mSaveManager)
    {
        qWarning() << "addStudent: Save manager is dead!";
        return;
    }

    auto studIter = std::find_if(mStudentList.begin(), mStudentList.end(),
                                 [&id](Student& stud){ return stud.id == id; });
    if (mStudentList.end() == studIter)
    {
        qWarning() << "deleteStudent: Student does not exist: " << id;
        return;
    }

    int groupId = studIter->groupId;
    mSaveManager->deleteItem(id, SaveManager::TYPE_STUDENT);
    mStudentList.erase(studIter);

    auto hasGroup = std::find_if(mStudentList.begin(), mStudentList.end(),
                 [groupId](Student& it){ return it.groupId == groupId; });

    if (hasGroup == mStudentList.end())
        mSaveManager->deleteItem(groupId, SaveManager::TYPE_GROUP);

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

    return mGroupMap.key(name, 0);
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
    const int defaultGroup = -1;

    if (defaultGroup == mGroupMap.key(group, defaultGroup))
    {
        qWarning() << "getStudentsByGroup: no such group:" << group;
        return QList<QObject*>();
    }

    QList<QObject*> result;
    int groupId = mGroupMap.key(group, 0);

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
    else
        qWarning() << "Save manager is dead!";
}

QString StudentManager::copyExternalPhoto(QString path)
{
    if (!QFile::exists(path))
    {
        qWarning() << "copyExternalPhoto: path not exist: " << path;
        return QString();
    }

    QDir dir("photos");

    if (!dir.exists())
        dir.mkpath("photos");

    QString prefix("file:///");
    path.remove(0, prefix.size());
    QString fileName(path.right(path.size() - path.lastIndexOf('/') - 1));
    QString newPath(QDir::currentPath() + '/' + "photos" + '/' + fileName);

    if (!QFile::copy(path, newPath))
        qWarning() << "copyExternalPhoto: cannot copy file: " << path;

    return fileName;
}
