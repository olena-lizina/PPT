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

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QDebug>
#include <QList>

#define STUDENT_MODE
//#define TEACHER_MODE

const QString getLastIdTemp = "SELECT MAX(Id) from '%1'";

SaveManager::SaveManager(): mSqlManager("ppt.db")
{
    //    mDbConnection.reset(new QSqlDatabase());
    //    *mDbConnection = QSqlDatabase::addDatabase("QSQLITE");
    //    mDbConnection->setDatabaseName("ppt.db");

    //    if (mDbConnection->open())
    //    {
    //        mQuery.reset(new QSqlQuery(*mDbConnection));
    //        isInitialized = true;
    //    }
    //    else
    //        isInitialized = false;

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

void SaveManager::clearTable(const SaveManager::LecturePartType& type)
{
    if (!isInitialized)
        return;

    switch(type)
    {
    case Part:
        mQuery->prepare(QString("DELETE FROM Parts"));
        mQuery->exec();
        break;
    case ChapterType:
        mQuery->prepare(QString("DELETE FROM Chapters"));
        mQuery->exec();
        break;
    case ThemeType:
        mQuery->prepare(QString("DELETE FROM Themes"));
        mQuery->exec();
        break;
    case SubTheme:
        mQuery->prepare(QString("DELETE FROM SubThemes"));
        mQuery->exec();
        break;
    }
}

void SaveManager::saveStudent(const Student& student)
{
    if (!isInitialized)
        return;

    mQuery->prepare(QString("INSERT INTO Students (Name, Phone, Email, InstGroup) VALUES ('%1', '%2', '%3', '%4')")
                    .arg(student.name).arg(student.phone).arg(student.email).arg(student.groupId));
    mQuery->exec();
}

void SaveManager::removeStudent(const Student& student)
{
    if (!isInitialized)
        return;

    mQuery->prepare(QString("DELETE FROM Students WHERE Name=\"%1\" AND InstGroup='%2'").arg(student.name).arg(student.groupId));
    mQuery->exec();
}

void SaveManager::updateStudent(const Student& oldStudent, const Student& newStudent)
{
    if (!isInitialized)
        return;

    mQuery->prepare(QString("UPDATE Students SET Name='%1', Phone='%2', Email='%3', InstGroup='%4' WHERE Name='%5' AND InstGroup='%6'")
                    .arg(newStudent.name).arg(newStudent.phone).arg(newStudent.email).arg(newStudent.groupId).arg(oldStudent.name).arg(oldStudent.groupId));
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

        //students << Student(name, phone, email, group);
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
    case Discipline:
    {
        mQuery->prepare(QString("INSERT INTO Disciplines (Id, Name) VALUES ('%1', \"%2\")")
                        .arg(lecture.getId()).arg(lecture.getName()));
        mQuery->exec();
        break;
    }
    case Part:
    {
        mQuery->prepare(QString("INSERT INTO Parts (Id, Name, DisciplineId) VALUES ('%1', \"%2\", '%3')")
                        .arg(lecture.getId()).arg(lecture.getName()).arg(lecture.getParentId()));
        mQuery->exec();
        break;
    }
    case ChapterType:
    {
        mQuery->prepare(QString("INSERT INTO Chapters (Id, Name, PartId) VALUES ('%1', \"%2\", '%3')")
                        .arg(lecture.getId()).arg(lecture.getName()).arg(lecture.getParentId()));
        mQuery->exec();
        break;
    }
    case ThemeType:
    {
        mQuery->prepare(QString("INSERT INTO Themes (Id, Name, ChapterId, File) VALUES ('%1', \"%2\", '%3', \"%4\")")
                        .arg(lecture.getId()).arg(lecture.getName()).arg(lecture.getParentId()).arg(lecture.getFileName()));
        mQuery->exec();
        break;
    }
    case SubTheme:
    {
        mQuery->prepare(QString("INSERT INTO SubThemes (Id, Name, ThemeId, File) VALUES ('%1', \"%2\", '%3', \"%4\")")
                        .arg(lecture.getId()).arg(lecture.getName()).arg(lecture.getParentId()).arg(lecture.getFileName()));
        mQuery->exec();
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
    case Discipline:
    {
        mQuery->prepare(QString("UPDATE Disciplines Set Name=\"%1\", Id='%2' WHERE Name=\"%3\"")
                        .arg(newLecture.getName()).arg(newLecture.getId()).arg(oldLecture.getName()));
        mQuery->exec();
        break;
    }
    case Part:
    {
        mQuery->prepare(QString("UPDATE Parts Set Name=\"%1\", Id='%2', DisciplineId='%3' WHERE Id='%4' AND DisciplineId='%5'")
                        .arg(newLecture.getName()).arg(newLecture.getId()).arg(newLecture.getParentId()).arg(oldLecture.getId()).arg(oldLecture.getParentId()));
        mQuery->exec();
        break;
    }
    case ChapterType:
    {
        mQuery->prepare(QString("UPDATE Chapters Set Name=\"%1\", Id='%2', PartId='%3' WHERE Id='%4' AND PartId='%5'")
                        .arg(newLecture.getName()).arg(newLecture.getId()).arg(newLecture.getParentId()).arg(oldLecture.getId()).arg(oldLecture.getParentId()));
        mQuery->exec();
        break;
    }
    case ThemeType:
    {
        mQuery->prepare(QString("UPDATE Themes Set Name=\"%1\", Id='%2', ChapterId='%3', File=\"%4\" WHERE Id='%5' AND ChapterId='%6'")
                        .arg(newLecture.getName()).arg(newLecture.getId()).arg(newLecture.getParentId()).arg(newLecture.getFileName()).arg(oldLecture.getId()).arg(oldLecture.getParentId()));
        mQuery->exec();
        break;
    }
    case SubTheme:
    {
        mQuery->prepare(QString("UPDATE SubThemes Set Name=\"%1\", Id='%2', ThemeId='%3', File=\"%4\" WHERE Id='%5' AND ThemeId='%6'")
                        .arg(newLecture.getName()).arg(newLecture.getId()).arg(newLecture.getParentId()).arg(newLecture.getFileName()).arg(oldLecture.getId()).arg(oldLecture.getParentId()));
        mQuery->exec();
        break;
    }
    }
}

void SaveManager::deleteLecturePart(const int& id, const int& parentId, const SaveManager::LecturePartType& type)
{
    if (!isInitialized)
        return;

    switch(type)
    {
    case Discipline:
        mQuery->prepare(QString("DELETE FROM Disciplines WHERE Id='%1'").arg(id));
        mQuery->exec();
        break;
    case Part:
        mQuery->prepare(QString("DELETE FROM Parts WHERE Id='%1' AND DisciplineId='%2'").arg(id).arg(parentId));
        mQuery->exec();
        break;
    case ChapterType:
        mQuery->prepare(QString("DELETE FROM Chapters WHERE Id='%1' AND PartId='%2'").arg(id).arg(parentId));
        mQuery->exec();
        break;
    case ThemeType:
        mQuery->prepare(QString("DELETE FROM Themes WHERE Id='%1' AND ChapterId='%2'").arg(id).arg(parentId));
        mQuery->exec();
        break;
    case SubTheme:
        mQuery->prepare(QString("DELETE FROM SubThemes WHERE Id='%1' AND ThemeId='%2'").arg(id).arg(parentId));
        mQuery->exec();
        break;
    }
}

void SaveManager::deleteLecturePartByParentId(const int& parentId, const SaveManager::LecturePartType& type)
{
    if (!isInitialized)
        return;

    switch(type)
    {
    case Part:
        mQuery->prepare(QString("DELETE FROM Parts WHERE DisciplineId='%1'").arg(parentId));
        mQuery->exec();
        break;
    case ChapterType:
        mQuery->prepare(QString("DELETE FROM Chapters WHERE PartId='%1'").arg(parentId));
        mQuery->exec();
        break;
    case ThemeType:
        mQuery->prepare(QString("DELETE FROM Themes WHERE ChapterId='%1'").arg(parentId));
        mQuery->exec();
        break;
    case SubTheme:
        mQuery->prepare(QString("DELETE FROM SubThemes WHERE ThemeId='%1'").arg(parentId));
        mQuery->exec();
        break;
    }
}

std::list<LecturePart> SaveManager::getLectureParts(const SaveManager::LecturePartType& type)
{
    if (!isInitialized)
        return std::list<LecturePart>();

    std::list<LecturePart> result;

    switch(type)
    {
    case Discipline:
    {
        mQuery->prepare("SELECT Id, Name FROM Disciplines");
        mQuery->exec();

        while(mQuery->next())
            result.push_back(LecturePart(mQuery->value(1).toString(), mQuery->value(0).toInt(), 0));

        break;
    }
    case Part:
    {
        mQuery->prepare("SELECT Id, Name, DisciplineId FROM Parts");
        mQuery->exec();

        while(mQuery->next())
            result.push_back(LecturePart(mQuery->value(1).toString(), mQuery->value(0).toInt(), mQuery->value(2).toInt()));

        break;
    }
    case ChapterType:
    {
        mQuery->prepare("SELECT Id, Name, PartId FROM Chapters");
        mQuery->exec();

        while(mQuery->next())
            result.push_back(LecturePart(mQuery->value(1).toString(), mQuery->value(0).toInt(), mQuery->value(2).toInt()));

        break;
    }
    case ThemeType:
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

void SaveManager::addChapter(const Chapter& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Chapter"));

    if (!res.first)
    {
        qDebug() << "Cannot get last chapter id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddChapter("INSERT INTO Chapter (Id,Name,Order_Id,Discipline_Id) VALUES ('%1',\"%2\",'%3','%4')");

    if (!mSqlManager.execute(tempAddChapter.arg(++lastId).arg(info.name).arg(info.orderId).arg(info.disciplineId)).first)
        qDebug() << "Cannot save chapter";
}

void SaveManager::addDiscipline(const DisciplineStud& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Discipline"));

    if (!res.first)
    {
        qDebug() << "Cannot get last discipline id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddDiscipline("INSERT INTO Discipline (Id,Name,Liter_Path) VALUES ('%1',\"%2\",\"%3\")");

    if (!mSqlManager.execute(tempAddDiscipline.arg(++lastId).arg(info.name).arg(info.literPath)).first)
        qDebug() << "Cannot save discipline";
}

void SaveManager::addDiscipline(const DisciplineTeach& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Discipline"));

    if (!res.first)
    {
        qDebug() << "Cannot get last discipline id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddDiscipline("INSERT INTO Discipline (Id,Name,Liter_Path,Educ_Plan_Path,Educ_Progr_Path) VALUES ('%1',\"%2\",\"%3\",\"%4\",\"%5\")");

    if (!mSqlManager.execute(tempAddDiscipline.arg(++lastId).arg(info.name).arg(info.literPath).arg(info.educPlanPath).arg(info.educProgPath)).first)
        qDebug() << "Cannot save discipline";
}

void SaveManager::addGroup(const Group& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("'Group'"));

    if (!res.first)
    {
        qDebug() << "Cannot get last group id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddGroup("INSERT INTO 'Group'(Id,Name) VALUES ('%1',\"%2\")");

    if (!mSqlManager.execute(tempAddGroup.arg(++lastId).arg(info.name)).first)
        qDebug() << "Cannot save group";
}

void SaveManager::addLabWork(const LabWork& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Lab_Work"));

    if (!res.first)
    {
        qDebug() << "Cannot get last lab_work id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddLabWork("INSERT INTO Lab_Work(Id,Theme_Id,Finish_Date,Name,Path) VALUES ('%1','%2','%3',\"%4\",\"%5\")");

    if (!mSqlManager.execute(tempAddLabWork.arg(++lastId).arg(info.themeId).arg(info.finishDate).arg(info.name).arg(info.path)).first)
        qDebug() << "Cannot save lab_work";
}

void SaveManager::addThemeLectureFile(const ThemeLectureFile& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Theme_Lecture_File"));

    if (!res.first)
    {
        qDebug() << "Cannot get last theme lecture file id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddLectFile("INSERT INTO Theme_Lecture_File(Id,Theme_Id,Path) VALUES ('%1','%2',\"%3\")");

    if (!mSqlManager.execute(tempAddLectFile.arg(++lastId).arg(info.themeId).arg(info.path)).first)
        qDebug() << "Cannot save theme lecture file";
}

void SaveManager::addSubthemeLectureFile(const SubthemeLectureFile& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Subtheme_Lecture_File"));

    if (!res.first)
    {
        qDebug() << "Cannot get last subtheme lecture file id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddLectFile("INSERT INTO Subtheme_Lecture_File(Id,Subtheme_Id,Path) VALUES ('%1','%2',\"%3\")");

    if (!mSqlManager.execute(tempAddLectFile.arg(++lastId).arg(info.subthemeId).arg(info.path)).first)
        qDebug() << "Cannot save subtheme lecture file";
}


void SaveManager::addReport(const Report& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Report"));

    if (!res.first)
    {
        qDebug() << "Cannot get last report id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddReport("INSERT INTO Report(Id,Lab_Id,Delivery_Date,Mark,Evaluation_Date) VALUES ('%1','%2','%3',\"%4\",'%5')");

    if (!mSqlManager.execute(tempAddReport.arg(++lastId).arg(info.labId).arg(info.delivDate).arg(info.mark).arg(info.evalDate)).first)
        qDebug() << "Cannot save report";
}

void SaveManager::addReportFile(const ReportFile& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Report_File"));

    if (!res.first)
    {
        qDebug() << "Cannot get last report file id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddReportFile("INSERT INTO Report_File(Id,Report_Id,Path) VALUES ('%1','%2',\"%3\")");

    if (!mSqlManager.execute(tempAddReportFile.arg(++lastId).arg(info.reportId).arg(info.path)).first)
        qDebug() << "Cannot save report file";
}

void SaveManager::addStudent(const Student& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Student"));

    if (!res.first)
    {
        qDebug() << "Cannot get last student id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddStud("INSERT INTO Student(Id,Name,Phone,Email,Photo_Path,Group_Id) VALUES ('%1',\"%2\",\"%3\",\"%4\",\"%5\",'%6')");

    if (!mSqlManager.execute(tempAddStud.arg(++lastId).arg(info.name).arg(info.phone).arg(info.email).arg(info.photoPath).arg(info.groupId)).first)
        qDebug() << "Cannot save student";
}

void SaveManager::addSubtheme(const Subtheme& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Subtheme"));

    if (!res.first)
    {
        qDebug() << "Cannot get last subtheme id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddSubtheme("INSERT INTO Subtheme(Id,Name,Order_Id,Theme_Id) VALUES ('%1',\"%2\",'%3','%4')");

    if (!mSqlManager.execute(tempAddSubtheme.arg(++lastId).arg(info.name).arg(info.orderId).arg(info.themeId)).first)
        qDebug() << "Cannot save subtheme";
}

void SaveManager::addTheme(const Theme& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Theme"));

    if (!res.first)
    {
        qDebug() << "Cannot get last theme id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddTheme("INSERT INTO Theme(Id,Name,Order_Id,Chapter_Id) VALUES ('%1',\"%2\",'%3','%3')");

    if (!mSqlManager.execute(tempAddTheme.arg(++lastId).arg(info.name).arg(info.orderId).arg(info.chapterId)).first)
        qDebug() << "Cannot save theme";
}

void SaveManager::delChapter(const int& id)
{
    const QString tempDelStr("DELETE FROM Chapter WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete chapter";
}

void SaveManager::delDiscipline(const int& id)
{
    const QString tempDelStr("DELETE FROM Discipline WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete student's discipline";
}

void SaveManager::delGroup(const int& id)
{
    const QString tempDelStr("DELETE FROM 'Group' WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete group";
}

void SaveManager::delLabWork(const int& id)
{
    const QString tempDelStr("DELETE FROM Lab_Work WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete lab work";
}

void SaveManager::delThemeLectureFile(const int& id)
{
    const QString tempDelStr("DELETE FROM Theme_Lecture_File WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete theme lecture file";
}

void SaveManager::delSubthemeLectureFile(const int& id)
{
    const QString tempDelStr("DELETE FROM Subtheme_Lecture_File WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete subtheme lecture file";
}

void SaveManager::delReport(const int& id)
{
    const QString tempDelStr("DELETE FROM Report WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete report";
}

void SaveManager::delReportFile(const int& id)
{
    const QString tempDelStr("DELETE FROM Report_File WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete report file";
}

void SaveManager::delStudent(const int& id, const int& groupId)
{
    /*
        To avoid keeping groups that doesn't have any student
        after each remove student we check does this group has
        any student. If no - we remove this group.

        After removing student we also remove all his reports
        and row in Report_Student table
    */

    const QString tempDelStr("DELETE FROM Student WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete student";
}

void SaveManager::delSubtheme(const int& id)
{
    const QString tempDelStr("DELETE FROM Subtheme WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete subtheme";
}

void SaveManager::delTheme(const int& id)
{
    const QString tempDelStr("DELETE FROM Theme WHERE Id='%1'");

    if (!mSqlManager.execute(tempDelStr.arg(id)).first)
        qDebug() << "Cannot delete theme";
}

void SaveManager::updChapter(const Chapter& info)
{
    const QString tempUpdStr("UPDATE Chapter SET Name=\"%1\",Order_Id='%2',Discipline_Id='%3' WHERE Id='%4'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.name).arg(info.orderId).arg(info.disciplineId).arg(info.id)).first)
        qDebug() << "Cannot update chapter";
}

void SaveManager::updDiscipline(const DisciplineStud& info)
{
    const QString tempUpdStr("UPDATE Discipline SET Name=\"%1\",Liter_Path=\"%2\" WHERE Id='%3'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.name).arg(info.literPath).arg(info.id)).first)
        qDebug() << "Cannot update discipline";
}

void SaveManager::updDiscipline(const DisciplineTeach& info)
{
    const QString tempUpdStr("UPDATE Discipline SET Name=\"%1\",Liter_Path=\"%2\",Educ_Plan_Path=\"%3\",Educ_Progr_Path=\"%4\" WHERE Id='%5'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.name).arg(info.literPath).arg(info.educPlanPath).arg(info.educProgPath).arg(info.id)).first)
        qDebug() << "Cannot update discipline";
}

void SaveManager::updGroup(const Group& info)
{
    const QString tempUpdStr("UPDATE 'Group' SET Name=\"%1\" WHERE Id='%2'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.name).arg(info.id)).first)
        qDebug() << "Cannot update group";
}

void SaveManager::updLabWork(const LabWork& info)
{
    const QString tempUpdStr("UPDATE Lab_Work SET Theme_Id='%1',Finish_Date='%2',Name=\"%3\",Path=\"%4\" WHERE Id='%5'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.themeId).arg(info.finishDate).arg(info.name).arg(info.path).arg(info.id)).first)
        qDebug() << "Cannot update lab_work";
}

void SaveManager::updThemeLectureFile(const ThemeLectureFile& info)
{
    const QString tempUpdStr("UPDATE Theme_Lecture_File SET Theme_Id='%1',Path=\"%2\" WHERE Id='%3'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.themeId).arg(info.path).arg(info.id)).first)
        qDebug() << "Cannot update theme lecture file";
}

void SaveManager::updSubthemeLectureFile(const SubthemeLectureFile& info)
{
    const QString tempUpdStr("UPDATE Subtheme_Lecture_File SET Subtheme_Id='%1',Path=\"%2\" WHERE Id='%3'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.subthemeId).arg(info.path).arg(info.id)).first)
        qDebug() << "Cannot update subtheme lecture file";
}

void SaveManager::updReport(const Report& info)
{
    const QString tempUpdStr("UPDATE Report SET Lab_Id='%1',Delivery_Date='%2',Mark=\"%3\",Evaluation_Date='%4',Stud_Id='%5' WHERE Id='%6'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.labId).arg(info.delivDate).arg(info.mark).arg(info.evalDate).arg(info.studId).arg(info.id)).first)
        qDebug() << "Cannot update report";
}

void SaveManager::updReportFile(const ReportFile& info)
{
    const QString tempUpdStr("UPDATE Report_File SET Report_Id='%1',Path=\"%2\" WHERE Id='%3'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.reportId).arg(info.path).arg(info.id)).first)
        qDebug() << "Cannot update report file";
}

void SaveManager::updStudent(const Student& info)
{
    const QString tempUpdStr("UPDATE Student SET Name=\"%1\",Phone=\"%2\",Email=\"%3\",Photo_Path=\"%4\",Group_Id='%5' WHERE Id='%6'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.name).arg(info.phone).arg(info.email).arg(info.photoPath).arg(info.groupId).arg(info.id)).first)
        qDebug() << "Cannot update student";
}

void SaveManager::updSubtheme(const Subtheme& info)
{
    const QString tempUpdStr("UPDATE Subtheme SET Name=\"%1\",Order_Id='%2',Theme_Id='%3' WHERE Id='%4'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.name).arg(info.orderId).arg(info.themeId).arg(info.id)).first)
        qDebug() << "Cannot update subtheme";
}

void SaveManager::updTheme(const Theme& info)
{
    const QString tempUpdStr("UPDATE Theme SET Name=\"%1\",Order_Id='%2',Chapter_Id='%3' WHERE Id='%4'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.name).arg(info.orderId).arg(info.chapterId).arg(info.id)).first)
        qDebug() << "Cannot update theme";
}

QList<Chapter> SaveManager::loadChapters()
{
    const QString tempLoadStr("SELECT Id,Name,Order_Id,Discipline_Id FROM Chapter");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load chapters";
        return QList<Chapter>();
    }

    QList<Chapter> chapters;

    for (auto chapt : res.second)
    {
        Chapter tmp;
        tmp.id = chapt.at(0).toInt();
        tmp.name = chapt.at(1).toString();
        tmp.orderId = chapt.at(2).toInt();
        tmp.disciplineId = chapt.at(3).toInt();
        chapters << tmp;
    }

    return chapters;
}

QList<DisciplineStud> SaveManager::loadStudDiscipline()
{
    const QString tempLoadStr("SELECT * FROM Discipline");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load student's disciplines";
        return QList<DisciplineStud>();
    }

    QList<DisciplineStud> disciplines;

    for (auto discipline : res.second)
    {
        DisciplineStud tmp;
        tmp.id = discipline.at(0).toInt();
        tmp.name = discipline.at(1).toString();
        tmp.literPath = discipline.at(2).toString();
        disciplines << tmp;
    }

    return disciplines;
}

QList<DisciplineTeach> SaveManager::loadTeachDiscipline()
{
    const QString tempLoadStr("SELECT * FROM Discipline");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load teacher's disciplines";
        return QList<DisciplineTeach>();
    }

    QList<DisciplineTeach> disciplines;

    for (auto discipline : res.second)
    {
        DisciplineTeach tmp;
        tmp.id = discipline.at(0).toInt();
        tmp.name = discipline.at(1).toString();
        tmp.literPath = discipline.at(2).toString();
        tmp.educPlanPath = discipline.at(3).toString();
        tmp.educProgPath = discipline.at(4).toString();
        disciplines << tmp;
    }

    return disciplines;
}

QList<Group> SaveManager::loadGroup()
{
    const QString tempLoadStr("SELECT * FROM 'Group'");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load groups";
        return QList<Group>();
    }

    QList<Group> groups;

    for (auto group : res.second)
    {
        Group tmp;
        tmp.id = group.at(0).toInt();
        tmp.name = group.at(1).toString();
        groups << tmp;
    }

    return groups;
}

QList<LabWork> SaveManager::loadLabWork()
{
    const QString tempLoadStr("SELECT * FROM Lab_Work");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load lab_works";
        return QList<LabWork>();
    }

    QList<LabWork> labWorks;

    for (auto work : res.second)
    {
        LabWork tmp;
        tmp.id = work.at(0).toInt();
        tmp.themeId = work.at(1).toInt();
        tmp.finishDate = work.at(2).toInt();
        tmp.name = work.at(3).toString();
        tmp.path = work.at(4).toString();
        labWorks << tmp;
    }

    return labWorks;
}

QList<ThemeLectureFile> SaveManager::loadThemeLectureFile()
{
    const QString tempLoadStr("SELECT * FROM Theme_Lecture_File");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load theme lecture files";
        return QList<ThemeLectureFile>();
    }

    QList<ThemeLectureFile> themeLectures;

    for (auto lect : res.second)
    {
        ThemeLectureFile tmp;
        tmp.id = lect.at(0).toInt();
        tmp.themeId = lect.at(1).toInt();
        tmp.path = lect.at(2).toString();
        themeLectures << tmp;
    }

    return themeLectures;
}

QList<SubthemeLectureFile> SaveManager::loadSubthemeLectureFile()
{
    const QString tempLoadStr("SELECT * FROM Subtheme_Lecture_File");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load subtheme lecture files";
        return QList<SubthemeLectureFile>();
    }

    QList<SubthemeLectureFile> subthemeLectures;

    for (auto lect : res.second)
    {
        SubthemeLectureFile tmp;
        tmp.id = lect.at(0).toInt();
        tmp.subthemeId = lect.at(1).toInt();
        tmp.path = lect.at(2).toString();
        subthemeLectures << tmp;
    }

    return subthemeLectures;
}

QList<Report> SaveManager::loadReport()
{
    const QString tempLoadStr("SELECT * FROM Report");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load reports";
        return QList<Report>();
    }

    QList<Report> reports;

    for (auto report : res.second)
    {
        Report tmp;
        tmp.id = report.at(0).toInt();
        tmp.labId = report.at(1).toInt();
        tmp.delivDate = report.at(2).toInt();
        tmp.mark = report.at(3).toString();
        tmp.evalDate = report.at(4).toInt();
        tmp.studId = report.at(5).toInt();
        reports << tmp;
    }

    return reports;
}

QList<ReportFile> SaveManager::loadReportFile()
{
    const QString tempLoadStr("SELECT * FROM Report_File");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load report files";
        return QList<ReportFile>();
    }

    QList<ReportFile> files;

    for (auto file : res.second)
    {
        ReportFile tmp;
        tmp.id = file.at(0).toInt();
        tmp.reportId = file.at(1).toInt();
        tmp.path = file.at(2).toString();
        files << tmp;
    }

    return files;
}

QList<ReportStudent> SaveManager::loadReportStudent()
{
    const QString tempLoadStr("SELECT * FROM Report_Student");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load report_students";
        return QList<ReportStudent>();
    }

    QList<ReportStudent> items;

    for (auto it : res.second)
    {
        ReportStudent tmp;
        tmp.id = it.at(0).toInt();
        tmp.studId = it.at(1).toInt();
        tmp.reportId = it.at(2).toInt();
        items << tmp;
    }

    return items;
}

QList<Student> SaveManager::loadStudent()
{
    const QString tempLoadStr("SELECT * FROM Student");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load students";
        return QList<Student>();
    }

    QList<Student> students;

    for (auto student : res.second)
    {
        Student tmp;
        tmp.id = student.at(0).toInt();
        tmp.name = student.at(1).toString();
        tmp.phone = student.at(2).toString();
        tmp.email = student.at(3).toString();
        tmp.photoPath = student.at(4).toString();
        tmp.groupId = student.at(5).toInt();
        students << tmp;
    }

    return students;
}

QList<Subtheme> SaveManager::loadSubtheme()
{
    const QString tempLoadStr("SELECT * FROM Subtheme");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load subthemes";
        return QList<Subtheme>();
    }

    QList<Subtheme> subthemes;

    for (auto subtheme : res.second)
    {
        Subtheme tmp;
        tmp.id = subtheme.at(0).toInt();
        tmp.name = subtheme.at(1).toString();
        tmp.orderId = subtheme.at(2).toInt();
        tmp.themeId = subtheme.at(3).toInt();
        subthemes << tmp;
    }

    return subthemes;
}

QList<Theme> SaveManager::loadTheme()
{
    const QString tempLoadStr("SELECT * FROM Theme");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first)
    {
        qDebug() << "Cannot load themes";
        return QList<Theme>();
    }

    QList<Theme> themes;

    for (auto subtheme : res.second)
    {
        Theme tmp;
        tmp.id = subtheme.at(0).toInt();
        tmp.name = subtheme.at(1).toString();
        tmp.orderId = subtheme.at(2).toInt();
        tmp.chapterId = subtheme.at(3).toInt();
        themes << tmp;
    }

    return themes;
}

void SaveManager::initTables()
{
    const QStringList commonValues {
        "CREATE TABLE IF NOT EXISTS Chapter (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Name TEXT, Order_Id	INTEGER, Discipline_Id INTEGER)",
        "CREATE TABLE IF NOT EXISTS Lab_Work (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Theme_Id INTEGER, Finish_Date INTEGER, Name TEXT, Path TEXT)",
        "CREATE TABLE IF NOT EXISTS Subtheme_Lecture_File (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Subtheme_Id INTEGER, Path TEXT)",
        "CREATE TABLE IF NOT EXISTS Theme_Lecture_File (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Theme_Id INTEGER, Path TEXT)",
        "CREATE TABLE IF NOT EXISTS Report (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Lab_Id INTEGER, Delivery_Date INTEGER, Mark TEXT, Evaluation_Date INTEGER, Stud_Id INTEGER)",
        "CREATE TABLE IF NOT EXISTS Report_File (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Report_Id INTEGER, Path TEXT)",
        "CREATE TABLE IF NOT EXISTS Subtheme (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Name TEXT, Order_Id INTEGER, Theme_Id INTEGER)",
        "CREATE TABLE IF NOT EXISTS Theme (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Name TEXT, Order_Id INTEGER, Chapter_Id INTEGER)"
    };

    const QStringList teacherValues {
        "CREATE TABLE IF NOT EXISTS Discipline (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Name TEXT, Liter_Path	TEXT, Educ_Plan_Path TEXT, Educ_Progr_Path TEXT)",
        "CREATE TABLE IF NOT EXISTS 'Group' (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Name TEXT)",
        "CREATE TABLE IF NOT EXISTS Student (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Name TEXT, Phone TEXT, Email TEXT, Photo_Path TEXT, Group_Id INTEGER)",
    };

    const QStringList studentValues {
        "CREATE TABLE IF NOT EXISTS Discipline (Id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, Name TEXT, Liter_Path	TEXT)"
    };

    for (auto val : commonValues)
        if (!mSqlManager.execute(val).first)
            qDebug() << "Cannot init table: " << val;

#ifdef STUDENT_MODE
    for (auto val : studentValues)
        if (!mSqlManager.execute(val).first)
            qDebug() << "Cannot init table: " << val;
#elif
    for (auto val : teacherValues)
        if (!mSqlManager.execute(val).first)
            qDebug() << "Cannot init table: " << val;
#endif
}
