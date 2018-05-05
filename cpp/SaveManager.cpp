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
#include <QVariant>
#include <QDebug>
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

    initTables();
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

    mQuery->prepare(QString("INSERT INTO Students (Name, Phone, Email, InstGroup) VALUES ('%1', '%2', '%3', '%4')")
                    .arg(student.name()).arg(student.phone()).arg(student.email()).arg(student.group()));
    mQuery->exec();
}

void SaveManager::removeStudent(const Student& student)
{
    if (!isInitialized)
        return;

    mQuery->prepare(QString("DELETE FROM Students WHERE Name=\"%1\" AND InstGroup='%2'").arg(student.name()).arg(student.group()));
    mQuery->exec();
}

void SaveManager::updateStudent(const Student& oldStudent, const Student& newStudent)
{
    if (!isInitialized)
        return;

    mQuery->prepare(QString("UPDATE Students SET Name='%1', Phone='%2', Email='%3', InstGroup='%4' WHERE Name='%5' AND InstGroup='%6'")
                    .arg(newStudent.name()).arg(newStudent.phone()).arg(newStudent.email()).arg(newStudent.group()).arg(oldStudent.name()).arg(oldStudent.group()));
    mQuery->exec();
}

QList<Student> SaveManager::loadAllStudents()
{
    if (!isInitialized)
        return QList<Student>();

    mQuery->prepare("SELECT * FROM Students");
    mQuery->exec();

    QList<Student> students;

    while (mQuery->next())
    {
        QString name(mQuery->value(0).toString());
        QString phone(mQuery->value(1).toString());
        QString email(mQuery->value(2).toString());
        QString group(mQuery->value(3).toString());

        students << Student(name, phone, email, group);
    }

    return students;
}

QStringList SaveManager::getGroups()
{
    if (!isInitialized)
        return QList<QString>();

    mQuery->prepare("SELECT InstGroup FROM Students");
    mQuery->exec();

    QStringList groups;

    groups << "------";

    while (mQuery->next())
    {
        QString group(mQuery->value(0).toString());

        if (!groups.count(group))
            groups << group;
    }

    return groups;
}

void SaveManager::saveLecturePart(const LecturePart& lecture, const SaveManager::LecturePartType& type)
{
    if (!isInitialized)
        return;

    switch(type)
    {
    case Part:
    {
        mQuery->prepare(QString("INSERT INTO Parts (Id, Name) VALUES ('%1', \"%2\")")
                        .arg(lecture.getId()).arg(lecture.getName()));
        if (!mQuery->exec())
            qDebug() << "Add Part failed with error: " << mQuery->lastError().text();
        break;
    }
    case Chapter:
    {
        mQuery->prepare(QString("INSERT INTO Chapters (Id, Name, PartId) VALUES ('%1', \"%2\", '%3')")
                        .arg(lecture.getId()).arg(lecture.getName()).arg(lecture.getParentId()));
        if (!mQuery->exec())
            qDebug() << "Add Chapter failed with error: " << mQuery->lastError().text();
        break;
    }
    case Theme:
    {
        mQuery->prepare(QString("INSERT INTO Themes (Id, Name, ChapterId, File) VALUES ('%1', \"%2\", '%3', \"%4\")")
                        .arg(lecture.getId()).arg(lecture.getName()).arg(lecture.getParentId()).arg(lecture.getFileName()));
        if (!mQuery->exec())
            qDebug() << "Add Theme failed with error: " << mQuery->lastError().text();
        break;
    }
    case SubTheme:
    {
        mQuery->prepare(QString("INSERT INTO SubThemes (Id, Name, ThemeId, File) VALUES ('%1', \"%2\", '%3', \"%4\")")
                        .arg(lecture.getId()).arg(lecture.getName()).arg(lecture.getParentId()).arg(lecture.getFileName()));
        if (!mQuery->exec())
            qDebug() << "Add SubTheme failed with error: " << mQuery->lastError().text();
        break;
    }
    }
}

void SaveManager::updateLecturePart(const LecturePart& oldLecture, const LecturePart& newLecture, const SaveManager::LecturePartType& type)
{
    if (!isInitialized)
        return;

    switch(type)
    {
    case Part:
    {
        mQuery->prepare(QString("UPDATE Parts Set Name=\"%1\" WHERE Name=\"%2\"")
                        .arg(newLecture.getName()).arg(oldLecture.getName()));
        mQuery->exec();
        break;
    }
    case Chapter:
    {
        mQuery->prepare(QString("UPDATE Chapters Set Name=\"%1\" WHERE Name=\"%2\" AND PartId='%3'")
                        .arg(newLecture.getName()).arg(oldLecture.getName()).arg(newLecture.getParentId()));
        mQuery->exec();
        break;
    }
    case Theme:
    {
        mQuery->prepare(QString("UPDATE Themes Set Name=\"%1\", File=\"%2\" WHERE Name=\"%3\" AND ChapterId='%4'")
                        .arg(newLecture.getName()).arg(oldLecture.getFileName()).arg(oldLecture.getName()).arg(oldLecture.getParentId()));
        mQuery->exec();
        break;
    }
    case SubTheme:
    {
        mQuery->prepare(QString("UPDATE SubThemes Set Name=\"%1\", File=\"%2\" WHERE Name=\"%3\" AND ThemeId='%4'")
                        .arg(newLecture.getName()).arg(oldLecture.getFileName()).arg(oldLecture.getName()).arg(oldLecture.getParentId()));
        mQuery->exec();
        break;
    }
    }
}

void SaveManager::deleteLecturePart(const LecturePart& lecture, const SaveManager::LecturePartType& type)
{
    if (!isInitialized)
        return;

    switch(type)
    {
    case Part:
    {
        mQuery->prepare(QString("DELETE FROM Parts WHERE Name=\"%1\"").arg(lecture.getName()));
        mQuery->exec();
        break;
    }
    case Chapter:
    {
        mQuery->prepare(QString("DELETE FROM Chapters WHERE Name=\"%1\" AND PartId='%2'")
                        .arg(lecture.getName()).arg(lecture.getParentId()));
        mQuery->exec();
        break;
    }
    case Theme:
    {
        mQuery->prepare(QString("DELETE FROM Themes WHERE Name=\"%1\" AND ChapterId='%2'")
                        .arg(lecture.getName()).arg(lecture.getParentId()));
        mQuery->exec();
        break;
    }
    case SubTheme:
    {
        mQuery->prepare(QString("DELETE FROM SubThemes WHERE Name=\"%1\" AND ThemeId='%2'")
                        .arg(lecture.getName()).arg(lecture.getParentId()));
        mQuery->exec();
        break;
    }
    }
}

std::list<LecturePart> SaveManager::getLectureParts(const SaveManager::LecturePartType& type)
{
    if (!isInitialized)
        return std::list<LecturePart>();

    std::list<LecturePart> result;

    switch(type)
    {
    case Part:
    {
        mQuery->prepare("SELECT Id, Name FROM Parts");
        mQuery->exec();

        while(mQuery->next())
            result.push_back(LecturePart(mQuery->value(1).toString(), mQuery->value(0).toInt(), 0));

        break;
    }
    case Chapter:
    {
        mQuery->prepare("SELECT Id, Name, PartId FROM Chapters");
        mQuery->exec();

        while(mQuery->next())
            result.push_back(LecturePart(mQuery->value(1).toString(), mQuery->value(0).toInt(), mQuery->value(2).toInt()));

        break;
    }
    case Theme:
    {
        mQuery->prepare("SELECT Id, Name, ChapterId, File FROM Themes");
        mQuery->exec();

        while(mQuery->next())
        {
            LecturePart lect(mQuery->value(1).toString(), mQuery->value(0).toInt(), mQuery->value(2).toInt());
            lect.setFileName(mQuery->value(3).toString());
            result.push_back(lect);
        }

        break;
    }
    case SubTheme:
    {
        mQuery->prepare("SELECT Id, Name, ThemeId, File FROM SubThemes");
        mQuery->exec();

        while(mQuery->next())
        {
            LecturePart lect(mQuery->value(1).toString(), mQuery->value(0).toInt(), mQuery->value(2).toInt());
            lect.setFileName(mQuery->value(3).toString());
            result.push_back(lect);
        }

        break;
    }
    }

    return result;
}

void SaveManager::initTables()
{
    if (!isInitialized)
        return;

    mQuery->prepare("CREATE TABLE IF NOT EXISTS Students (Name string, Phone string, Email string, InstGroup string)");
    mQuery->exec();

    mQuery->prepare("CREATE TABLE IF NOT EXISTS Parts (Id int, Name string)");
    mQuery->exec();

    mQuery->prepare("CREATE TABLE IF NOT EXISTS Chapters (Id int, Name string, PartId int)");
    mQuery->exec();

    mQuery->prepare("CREATE TABLE IF NOT EXISTS Themes (Id int, Name string, ChapterId int, File string)");
    mQuery->exec();

    mQuery->prepare("CREATE TABLE IF NOT EXISTS SubThemes (Id int, Name string, ThemeId int, File string)");
    mQuery->exec();
}
