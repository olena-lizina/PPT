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

#include "SaveManager.h"
#include "Student.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

SaveManager::SaveManager()
{
    mDbConnection.reset(new QSqlDatabase());
    *mDbConnection = QSqlDatabase::addDatabase("QSQLITE");
    mDbConnection->setDatabaseName("students.db");

    if (mDbConnection->open())
    {
        mQuery.reset(new QSqlQuery(*mDbConnection));
        isInitialized = true;
    }
    else
        isInitialized = false;

    mQuery->prepare("CREATE TABLE IF NOT EXISTS Students (Id int, Name string, Phone string, Email string, Group string, PRIMARY KEY(Id))");
    mQuery->exec();
}

/*virtual*/ SaveManager::~SaveManager()
{
    if (mDbConnection)
    {
        // we need to remove query first because possible memory leak
        mQuery.reset();
        mDbConnection->close();
        mDbConnection->removeDatabase("QSQLITE");
        mDbConnection.reset();
    }
}

void SaveManager::saveStudent(const Student& student)
{
    if (!isInitialized)
        return;

    // get the current student index
    mQuery->prepare("SELECT COUNT(*) FROM Students");
    mQuery->exec();

    int idx = 0;

    if (mQuery->next())
        idx = mQuery->value(0).toInt();

    // get the current student index

    mQuery->prepare(QString("INSERT INTO Students (Id, Name, Phone, Email, Group) VALUES (%1, '%2', '%3', '%4', '%5')")
                    .arg(idx).arg(student.name()).arg(student.phone()).arg(student.email()).arg(student.group()));
    mQuery->exec();
}

void SaveManager::removeStudent(const Student&)
{
    if (!isInitialized)
        return;
}

QList<Student> SaveManager::loadAllStudents()
{
    if (!isInitialized)
        return;
}
