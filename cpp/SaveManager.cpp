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
#include <QVariant>
#include <QDebug>

//#define STUDENT_MODE
#define TEACHER_MODE

const QString getLastIdTemp = "SELECT MAX(Id) from '%1'";

SaveManager::SaveManager(): mSqlManager("ppt.db")
{
    initTables();
}

/*virtual*/ SaveManager::~SaveManager()
{
}

void SaveManager::appendChapter(const Chapter& info)
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

void SaveManager::addChapter(const Chapter& info)
{
    const QString tempAddChapter("INSERT INTO Chapter (Id,Name,Order_Id,Discipline_Id) VALUES ('%1',\"%2\",'%3','%4')");

    if (!mSqlManager.execute(tempAddChapter.arg(info.id).arg(info.name).arg(info.orderId).arg(info.disciplineId)).first)
        qDebug() << "Cannot save chapter";
}

void SaveManager::appendDiscipline(const DisciplineStud& info)
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

void SaveManager::appendDiscipline(const DisciplineTeach& info)
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

void SaveManager::addDiscipline(const DisciplineTeach& info)
{
    const QString tempAddDiscipline("INSERT INTO Discipline (Id,Name,Liter_Path,Educ_Plan_Path,Educ_Progr_Path) VALUES ('%1',\"%2\",\"%3\",\"%4\",\"%5\")");

    if (!mSqlManager.execute(tempAddDiscipline.arg(info.id).arg(info.name).arg(info.literPath).arg(info.educPlanPath).arg(info.educProgPath)).first)
        qDebug() << "Cannot save discipline";
}

void SaveManager::addGroup(const Group& info)
{
    auto res = mSqlManager.execute("SELECT MAX(Id) from 'Group'");

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

void SaveManager::appendSubtheme(const Subtheme& info)
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

void SaveManager::addSubtheme(const Subtheme& info)
{
    const QString tempAddSubtheme("INSERT INTO Subtheme(Id,Name,Order_Id,Theme_Id) VALUES ('%1',\"%2\",'%3','%4')");

    if (!mSqlManager.execute(tempAddSubtheme.arg(info.id).arg(info.name).arg(info.orderId).arg(info.themeId)).first)
        qDebug() << "Cannot save subtheme";
}

void SaveManager::appendTheme(const Theme& info)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("Theme"));

    if (!res.first)
    {
        qDebug() << "Cannot get last theme id";
        return;
    }

    int lastId = res.second.at(0).at(0).toInt();
    const QString tempAddTheme("INSERT INTO Theme(Id,Name,Order_Id,Chapter_Id) VALUES ('%1',\"%2\",'%3','%4')");

    if (!mSqlManager.execute(tempAddTheme.arg(++lastId).arg(info.name).arg(info.orderId).arg(info.chapterId)).first)
        qDebug() << "Cannot save theme";
}

void SaveManager::addTheme(const Theme& info)
{
    const QString tempAddTheme("INSERT INTO Theme(Id,Name,Order_Id,Chapter_Id) VALUES ('%1',\"%2\",'%3','%4')");

    if (!mSqlManager.execute(tempAddTheme.arg(info.id).arg(info.name).arg(info.orderId).arg(info.chapterId)).first)
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

void SaveManager::delStudent(const int& id)
{
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

void SaveManager::updChapterIdx(const int& oldIdx, const int& newIdx)
{
    const QString tempUpdStr("UPDATE Chapter SET Id='%1' WHERE Id='%2'");

    if (!mSqlManager.execute(tempUpdStr.arg(newIdx).arg(oldIdx)).first)
        qDebug() << "Cannot update chapter index";
}

void SaveManager::updDisciplineIdx(const int& oldIdx, const int& newIdx)
{
    const QString tempUpdStr("UPDATE Discipline SET Id='%1' WHERE Id='%2'");

    if (!mSqlManager.execute(tempUpdStr.arg(newIdx).arg(oldIdx)).first)
        qDebug() << "Cannot update discipline index";
}

void SaveManager::updSubthemeIdx(const int& oldIdx, const int& newIdx)
{
    const QString tempUpdStr("UPDATE Subtheme SET Id='%1' WHERE Id='%2'");

    if (!mSqlManager.execute(tempUpdStr.arg(newIdx).arg(oldIdx)).first)
        qDebug() << "Cannot update subtheme index";
}

void SaveManager::updThemeIdx(const int& oldIdx, const int& newIdx)
{
    const QString tempUpdStr("UPDATE Theme SET Id='%1' WHERE Id='%2'");

    if (!mSqlManager.execute(tempUpdStr.arg(newIdx).arg(oldIdx)).first)
        qDebug() << "Cannot update theme index";
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
        //qDebug() << tmp.name;
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
        //qDebug() << tmp.name;
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
        //qDebug() << tmp.name;
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
        //qDebug() << tmp.name;
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
        //qDebug() << tmp.name;
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
        //qDebug() << tmp.themeId;
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
        //qDebug() << tmp.subthemeId;
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
        //qDebug() << tmp.labId;
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
        //qDebug() << tmp.reportId;
    }

    return files;
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
        //qDebug() << tmp.name;
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
        //qDebug() << tmp.name;
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
        //qDebug() << tmp.name;
    }

    return themes;
}

void SaveManager::initTables()
{
    const QStringList commonValues {
        "CREATE TABLE IF NOT EXISTS Chapter (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Order_Id INTEGER, Discipline_Id INTEGER, FOREIGN KEY (Discipline_Id) REFERENCES Discipline(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Lab_Work (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Theme_Id INTEGER, Finish_Date INTEGER, Name TEXT, Path TEXT, FOREIGN KEY (Theme_Id) REFERENCES Theme(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Subtheme_Lecture_File (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Subtheme_Id INTEGER, Path TEXT, FOREIGN KEY (Subtheme_Id) REFERENCES Subtheme(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Theme_Lecture_File (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Theme_Id INTEGER, Path TEXT, FOREIGN KEY (Theme_Id) REFERENCES Theme(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Report (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Lab_Id INTEGER, Delivery_Date INTEGER, Mark TEXT, Evaluation_Date INTEGER, Stud_Id INTEGER, FOREIGN KEY (Lab_Id) REFERENCES Lab_Work(Id) ON DELETE CASCADE, FOREIGN KEY (Stud_Id) REFERENCES Student(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Report_File (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Report_Id INTEGER, Path TEXT, FOREIGN KEY (Report_Id) REFERENCES Report(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Subtheme (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Order_Id INTEGER, Theme_Id INTEGER, FOREIGN KEY (Theme_Id) REFERENCES Theme(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Theme (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Order_Id INTEGER, Chapter_Id INTEGER, FOREIGN KEY (Chapter_Id) REFERENCES Chapter(Id) ON DELETE CASCADE)"
    };

    const QStringList teacherValues {
        "CREATE TABLE IF NOT EXISTS Discipline (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Liter_Path TEXT, Educ_Plan_Path TEXT, Educ_Progr_Path TEXT)",
        "CREATE TABLE IF NOT EXISTS 'Group' (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT)",
        "CREATE TABLE IF NOT EXISTS Student (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Phone TEXT, Email TEXT, Photo_Path TEXT, Group_Id INTEGER, FOREIGN KEY (Group_Id) REFERENCES 'Group'(Id) ON DELETE CASCADE)",
    };

    const QStringList studentValues {
        "CREATE TABLE IF NOT EXISTS Discipline (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Liter_Path	TEXT)"
    };

    for (auto val : commonValues)
        if (!mSqlManager.execute(val).first)
            qDebug() << "Cannot init table: " << val;

#ifdef STUDENT_MODE
    for (auto val : studentValues)
        if (!mSqlManager.execute(val).first)
            qDebug() << "Cannot init table: " << val;
#else
    for (auto val : teacherValues)
        if (!mSqlManager.execute(val).first)
            qDebug() << "Cannot init table: " << val;
#endif
}
