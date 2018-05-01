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

    mQuery->prepare(QString("DELETE FROM Students WHERE Name='%1' AND InstGroup='%2'").arg(student.name()).arg(student.group()));
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

QStringList SaveManager::getParts()
{
    if (!isInitialized)
        return QList<QString>();

    //mQuery->prepare("SELECT Part FROM Lectures");
    mQuery->prepare("SELECT Name FROM Parts");
    mQuery->exec();

    QStringList parts;

    while (mQuery->next())
        parts << mQuery->value(0).toString();

    return parts;
}

QStringList SaveManager::getChapters(const QString& part)
{
    if (!isInitialized)
        return QList<QString>();

    int partId = getPartId(part);
    if (-1 == partId)
        return QStringList();

    mQuery->prepare(QString("SELECT Name FROM Chapters WHERE PartId='%1'").arg(partId));
    mQuery->exec();

    QStringList chapters;

    while (mQuery->next())
        chapters << mQuery->value(0).toString();

    return chapters;
}

QStringList SaveManager::getThemes(const QString& part, const QString& chapter)
{
    if (!isInitialized)
        return QList<QString>();

    int partId = getPartId(part);
    if (-1 == partId)
        return QStringList();

    int chaptId = getChapterId(partId, chapter);
    if (-1 == chaptId)
        return QStringList();

    mQuery->prepare(QString("SELECT Name FROM Themes WHERE ChapterId='%1'").arg(chaptId));
    mQuery->exec();

    QStringList themes;

    while (mQuery->next())
            themes << mQuery->value(0).toString();

    return themes;
}

QStringList SaveManager::getSubThemes(const QString& part, const QString& chapter, const QString& theme)
{
    if (!isInitialized)
        return QList<QString>();

    int partId = getPartId(part);
    if (-1 == partId)
        return QStringList();

    int chaptId = getChapterId(partId, chapter);
    if (-1 == chaptId)
        return QStringList();

    int themeId = getThemeId(chaptId, theme);
    if (-1 == themeId)
        return QStringList();

    mQuery->prepare(QString("SELECT Name FROM SubThemes WHERE ThemeId='%1'").arg(themeId));
    mQuery->exec();

    QStringList subThemes;
    while (mQuery->next())
        subThemes << mQuery->value(0).toString();

    return subThemes;
}

int SaveManager::getPartId(const QString& part)
{
    mQuery->prepare(QString("SELECT Id FROM Parts WHERE Name='%1'").arg(part));
    mQuery->exec();

    if (mQuery->next())
        return mQuery->value(0).toInt();
    else
        return -1;
}

int SaveManager::getChapterId(const int& partId, const QString& chapter)
{
    mQuery->prepare(QString("SELECT Id FROM Chapters WHERE PartId='%1' AND Name='%2'").arg(partId).arg(chapter));
    mQuery->exec();

    if (mQuery->next())
        return mQuery->value(0).toInt();
    else
        return -1;
}

int SaveManager::getThemeId(const int& chapterId, const QString& theme)
{
    mQuery->prepare(QString("SELECT Id FROM Themes WHERE ChapterId='%1' AND Name=\"%2\"").arg(chapterId).arg(theme));
    mQuery->exec();

    if (mQuery->next())
        return mQuery->value(0).toInt();
    else
        return -1;
}

int SaveManager::getNextPartId()
{
    std::vector<int> partIds;
    mQuery->prepare("SELECT Id FROM Parts");
    mQuery->exec();

    while (mQuery->next())
        partIds.push_back(mQuery->value(0).toInt());

    std::sort(partIds.begin(), partIds.end());

    return partIds.back() + 1;
}

int SaveManager::getNextChapterId()
{
    std::vector<int> chapterIds;
    mQuery->prepare("SELECT Id FROM Chapters");
    mQuery->exec();

    while (mQuery->next())
        chapterIds.push_back(mQuery->value(0).toInt());

    std::sort(chapterIds.begin(), chapterIds.end());

    return chapterIds.back() + 1;
}

int SaveManager::getNextThemeId()
{
    std::vector<int> themeIds;
    mQuery->prepare("SELECT Id FROM Themes");
    mQuery->exec();

    while (mQuery->next())
        themeIds.push_back(mQuery->value(0).toInt());

    std::sort(themeIds.begin(), themeIds.end());

    return themeIds.back() + 1;
}
int SaveManager::getNextSubThemeId()
{
    std::vector<int> subThemeIds;
    mQuery->prepare("SELECT Id FROM SubThemes");
    mQuery->exec();

    while (mQuery->next())
        subThemeIds.push_back(mQuery->value(0).toInt());

    std::sort(subThemeIds.begin(), subThemeIds.end());

    return subThemeIds.back() + 1;
}

void SaveManager::savePart(const QString& part)
{
    if (!isInitialized)
        return;

    mQuery->prepare(QString("INSERT INTO Parts (Id, Name) VALUES ('%1', '%2')")
                    .arg(getNextPartId()).arg(part));
    mQuery->exec();
}

void SaveManager::saveChapter(const QString& part, const QString& chapter)
{
    if (!isInitialized)
        return;

    int partId = getPartId(part);
    if (-1 == partId)
        return;

    mQuery->prepare(QString("INSERT INTO Chapters (Id, Name, PartId) VALUES ('%1', '%2', '%3')")
                    .arg(getNextChapterId()).arg(chapter).arg(partId));
    mQuery->exec();
}

void SaveManager::updateChapter(const QString& oldChapter, const QString& newChapter, const QString& part)
{
    if (!isInitialized)
        return;

    int partId = getPartId(part);
    if (-1 == partId)
        return;

    mQuery->prepare(QString("UPDATE Chapters Set Name='%1' WHERE Name=\"%2\" AND PartId='%3'")
                    .arg(newChapter).arg(oldChapter).arg(partId));
    mQuery->exec();
}

void SaveManager::deleteChapter(const QString& chapter, const QString& part)
{
    if (!isInitialized)
        return;

    int partId = getPartId(part);
    if (-1 == partId)
        return;

    mQuery->prepare(QString("DELETE FROM Chapters WHERE Name=\"%1\" AND PartId='%2'").arg(chapter).arg(partId));
    mQuery->exec();
}

void SaveManager::saveTheme(const QString& part, const QString& chapter, const QString& theme)
{
    if (!isInitialized)
        return;

    int partId = getPartId(part);
    if (-1 == partId)
        return;
    int chapterId = getChapterId(partId, chapter);
    if (-1 == chapterId)
        return;

    mQuery->prepare(QString("INSERT INTO Themes (Id, Name, ChapterId) VALUES ('%1', '%2', '%3')")
                    .arg(getNextThemeId()).arg(theme).arg(chapterId));
    mQuery->exec();
}

void SaveManager::updateTheme(const QString& oldTheme, const QString& newTheme, const QString& part, const QString& chapter)
{
    if (!isInitialized)
        return;

    int partId = getPartId(part);
    if (-1 == partId)
        return;
    int chapterId = getChapterId(partId, chapter);
    if (-1 == chapterId)
        return;

    mQuery->prepare(QString("UPDATE Themes Set Name=\"%1\" WHERE Name=\"%2\" AND ChapterId='%3'")
                    .arg(newTheme).arg(oldTheme).arg(chapterId));
    mQuery->exec();
}

void SaveManager::deleteTheme(const QString& part, const QString& chapter, const QString& theme)
{
    if (!isInitialized)
        return;

    int partId = getPartId(part);
    if (-1 == partId)
        return;
    int chapterId = getChapterId(partId, chapter);
    if (-1 == chapterId)
        return;

    mQuery->prepare(QString("DELETE FROM Themes WHERE Name=\"%1\" AND ChapterId='%2'").arg(theme).arg(chapterId));
    mQuery->exec();
}

void SaveManager::saveSubTheme(const QString& part, const QString& chapter, const QString& theme, const QString& subTheme)
{
    if (!isInitialized)
        return;

    int partId = getPartId(part);
    if (-1 == partId)
        return;
    int chapterId = getChapterId(partId, chapter);
    if (-1 == chapterId)
        return;
    int themeId = getThemeId(chapterId, theme);
    if (-1 == themeId)
        return;

    mQuery->prepare(QString("INSERT INTO SubThemes (Id, Name, ThemeId) VALUES ('%1', '%2', '%3')")
                    .arg(getNextSubThemeId()).arg(subTheme).arg(themeId));
    mQuery->exec();
}

void SaveManager::updateSubTheme(const QString& oldSubTheme, const QString& newSubTheme, const QString& part, const QString& chapter, const QString& theme)
{
    if (!isInitialized)
        return;

    int partId = getPartId(part);
    if (-1 == partId)
        return;
    int chapterId = getChapterId(partId, chapter);
    if (-1 == chapterId)
        return;
    int themeId = getThemeId(chapterId, theme);
    if (-1 == themeId)
        return;

    mQuery->prepare(QString("UPDATE SubThemes Set Name=\"%1\" WHERE Name=\"%2\" AND ThemeId='%3'")
                    .arg(newSubTheme).arg(oldSubTheme).arg(themeId));
    mQuery->exec();
}

void SaveManager::deleteSubTheme(const QString& part, const QString& chapter, const QString& theme, const QString& subTheme)
{
    if (!isInitialized)
        return;

    int partId = getPartId(part);
    if (-1 == partId)
        return;
    int chapterId = getChapterId(partId, chapter);
    if (-1 == chapterId)
        return;
    int themeId = getThemeId(chapterId, theme);
    if (-1 == themeId)
        return;

    mQuery->prepare(QString("DELETE FROM SubThemes WHERE Name=\"%1\" AND ThemeId='%2'")
                    .arg(subTheme).arg(themeId));
    mQuery->exec();
}

void SaveManager::updatePart(const QString& oldPart, const QString& newPart)
{
    if (!isInitialized)
        return;

    mQuery->prepare(QString("UPDATE Parts Set Name=\"%1\" WHERE Name=\"%2\"")
                    .arg(newPart).arg(oldPart));
    mQuery->exec();
}

void SaveManager::deletePart(const QString& part)
{
    if (!isInitialized)
        return;

    mQuery->prepare(QString("DELETE FROM Parts WHERE Name=\"%1\"").arg(part));
    mQuery->exec();
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

    mQuery->prepare("CREATE TABLE IF NOT EXISTS Themes (Id int, Name string, ChapterId int)");
    mQuery->exec();

    mQuery->prepare("CREATE TABLE IF NOT EXISTS SubThemes (Id int, Name string, ThemeId int)");
    mQuery->exec();
}
