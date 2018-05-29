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

const QString getLastIdTemp = "SELECT MAX(Id) from '%1'";

SaveManager::SaveManager(): mSqlManager("ppt.db")
{
    initTypeNames();
    initTables();
}

/*virtual*/ SaveManager::~SaveManager()
{
}

void SaveManager::initTypeNames()
{
    mTypeNames[TYPE_CHAPTER] = "Chapter";
    mTypeNames[TYPE_DISCIPLINE] = "Discipline";
    mTypeNames[TYPE_THEME] = "Theme";
    mTypeNames[TYPE_SUBTHEME] = "Subtheme";
    mTypeNames[TYPE_LAB_WORK] = "Lab_Work";
    mTypeNames[TYPE_THEME_LECTURE] = "Theme_Lecture_File";
    mTypeNames[TYPE_SUBTHEME_LECTURE] = "Subtheme_Lecture_File";
    mTypeNames[TYPE_REPORT] = "Report";
    mTypeNames[TYPE_REPORT_FILE] = "Report_File";
    mTypeNames[TYPE_GROUP] = "'Group'";
    mTypeNames[TYPE_STUDENT] = "Student";
    mTypeNames[TYPE_STUDENTS_COURSES] = "StudentsCourses";
}

void SaveManager::initInsertStrs()
{
    mInsertStr[TYPE_CHAPTER] = "INSERT INTO Chapter (Id,Name,Discipline_Id) VALUES ('%1',\"%2\",'%3')";
    mInsertStr[TYPE_DISCIPLINE] = "INSERT INTO Discipline (Id,Name,Liter_Path) VALUES ('%1',\"%2\",\"%3\")";
    mInsertStr[TYPE_THEME] = "INSERT INTO Theme (Id,Name,Chapter_Id) VALUES ('%1',\"%2\",'%3')";
    mInsertStr[TYPE_SUBTHEME] = "INSERT INTO Subtheme(Id,Name,Theme_Id) VALUES ('%1',\"%2\",'%3')";
    mInsertStr[TYPE_LAB_WORK] = "INSERT INTO Lab_Work (Id,Discipline_Id,Finish_Date,Name,Path) VALUES ('%1','%2',\"%3\",\"%4\",\"%5\")";
    mInsertStr[TYPE_THEME_LECTURE] = "INSERT INTO Theme_Lecture_File (Id,Theme_Id,Path) VALUES ('%1','%2',\"%3\")";
    mInsertStr[TYPE_SUBTHEME_LECTURE] = "INSERT INTO Subtheme_Lecture_File (Id,Subtheme_Id,Path) VALUES ('%1','%2',\"%3\")";
    mInsertStr[TYPE_REPORT] = "INSERT INTO Report (Id,Lab_Id,Delivery_Date,Mark,Evaluation_Date) VALUES ('%1','%2',\"%3\",\"%4\",\"%5\")";
    mInsertStr[TYPE_REPORT_FILE] = "INSERT INTO Report_File (Id,Report_Id,Path) VALUES ('%1','%2',\"%3\")";
    mInsertStr[TYPE_GROUP] = "INSERT INTO 'Group' (Id,Name) VALUES ('%1',\"%2\")";
    mInsertStr[TYPE_STUDENT] = "INSERT INTO Student (Id,Name,Phone,Email,Photo_Path,Group_Id) VALUES ('%1',\"%2\",\"%3\",\"%4\",\"%5\",'%6')";
    mInsertStr[TYPE_STUDENTS_COURSES] = "INSERT INTO StudentsCourses (Id,StudId,CourseId) VALUES ";
}

void SaveManager::initDeleteStrs()
{
    mDeleteStr[TYPE_CHAPTER] = "DELETE FROM Chapter WHERE Id='%1'";
    mDeleteStr[TYPE_DISCIPLINE] = "DELETE FROM Discipline WHERE Id='%1'";
    mDeleteStr[TYPE_THEME] = "DELETE FROM Theme WHERE Id='%1'";
    mDeleteStr[TYPE_SUBTHEME] = "DELETE FROM Subtheme WHERE Id='%1'";
    mDeleteStr[TYPE_LAB_WORK] = "DELETE FROM Lab_Work WHERE Id='%1'";
    mDeleteStr[TYPE_THEME_LECTURE] = "DELETE FROM Theme_Lecture_File WHERE Id='%1'";
    mDeleteStr[TYPE_SUBTHEME_LECTURE] = "DELETE FROM Subtheme_Lecture_File WHERE Id='%1'";
    mDeleteStr[TYPE_REPORT] = "DELETE FROM Report WHERE Id='%1'";
    mDeleteStr[TYPE_REPORT_FILE] = "DELETE FROM Report_File WHERE Id='%1'";
    mDeleteStr[TYPE_GROUP] = "DELETE FROM 'Group' WHERE Id='%1'";
    mDeleteStr[TYPE_STUDENT] = "DELETE FROM Student WHERE Id='%1'";
    mDeleteStr[TYPE_STUDENTS_COURSES] = "DELETE FROM StudentsCourses WHERE Id='%1'";
}

void SaveManager::initEditStrs()
{
    mEditStr[TYPE_CHAPTER] = "UPDATE Chapter SET Name=\"%1\",Discipline_Id='%2' WHERE Id='%3'";
    mEditStr[TYPE_DISCIPLINE] = "UPDATE Discipline SET Name=\"%1\",Liter_Path=\"%2\" WHERE Id='%3'";
    mEditStr[TYPE_THEME] = "Theme";
    mEditStr[TYPE_SUBTHEME] = "Subtheme";
    mEditStr[TYPE_LAB_WORK] = "Lab_Work";
    mEditStr[TYPE_THEME_LECTURE] = "Theme_Lecture_File";
    mEditStr[TYPE_SUBTHEME_LECTURE] = "Subtheme_Lecture_File";
    mEditStr[TYPE_REPORT] = "Report";
    mEditStr[TYPE_REPORT_FILE] = "Report_File";
    mEditStr[TYPE_GROUP] = "'Group'";
    mEditStr[TYPE_STUDENT] = "Student";
    mEditStr[TYPE_STUDENTS_COURSES] = "StudentsCourses";
}

void SaveManager::saveItem(BaseItem* item, ItemType type, int index)
{
    switch (type)
    {
    case TYPE_CHAPTER:
    {
        Chapter *info = dynamic_cast<Chapter*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to Chapter";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_CHAPTER].arg(index).arg(info->name).arg(info->disciplineId)).first)
            qDebug() << "Cannot save chapter";
    }
        break;
    case TYPE_DISCIPLINE:
    {
        Discipline * info = dynamic_cast<Discipline*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to Discipline";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_DISCIPLINE].arg(index).arg(info->name).arg(info->literPath).arg(info->educPlanPath).arg(info->educProgPath)).first)
            qDebug() << "Cannot save discipline";
    }
        break;
    case TYPE_THEME:
    {
        Theme * info = dynamic_cast<Theme*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to Theme";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_THEME].arg(index).arg(info->name).arg(info->chapterId)).first)
            qDebug() << "Cannot save theme";
    }
        break;
    case TYPE_SUBTHEME:
    {
        Subtheme * info = dynamic_cast<Subtheme*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to Subtheme";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_SUBTHEME].arg(index).arg(info->name).arg(info->themeId)).first)
            qDebug() << "Cannot save subtheme";
    }
        break;
    case TYPE_LAB_WORK:
    {
        LabWork * info = dynamic_cast<LabWork*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to LabWork";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_LAB_WORK].arg(index).arg(info->disciplineId).arg(info->finishDate).arg(info->name).arg(info->path)).first)
            qDebug() << "Cannot save lab_work";
    }
        break;
    case TYPE_THEME_LECTURE:
    {
        ThemeLectureFile * info = dynamic_cast<ThemeLectureFile*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to ThemeLectureFile";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_THEME_LECTURE].arg(index).arg(info->themeId).arg(info->path)).first)
            qDebug() << "Cannot save theme lecture file";
    }
        break;
    case TYPE_SUBTHEME_LECTURE:
    {
        SubthemeLectureFile * info = dynamic_cast<SubthemeLectureFile*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to SubthemeLectureFile";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_SUBTHEME_LECTURE].arg(index).arg(info->subthemeId).arg(info->path)).first)
            qDebug() << "Cannot save subtheme lecture file";
    }
        break;
    case TYPE_REPORT:
    {
        Report * info = dynamic_cast<Report*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to Report";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_REPORT].arg(index).arg(info->labId).arg(info->delivDate).arg(info->mark).arg(info->evalDate)).first)
            qDebug() << "Cannot save report";
    }
        break;
    case TYPE_REPORT_FILE:
    {
        ReportFile * info = dynamic_cast<ReportFile*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to ReportFile";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_REPORT_FILE].arg(index).arg(info->reportId).arg(info->path)).first)
            qDebug() << "Cannot save report file";
    }
        break;
    case TYPE_GROUP:
    {
        Group * info = dynamic_cast<Group*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to Group";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_GROUP].arg(index).arg(info->name)).first)
            qDebug() << "Cannot save group";
    }
        break;
    case TYPE_STUDENT:
    {
        Student * info = dynamic_cast<Student*>(item);

        if (!info)
        {
            qDebug() << "Cannot dynamic cast BaseItem to Student";
            return;
        }

        if (!mSqlManager.execute(mInsertStr[TYPE_STUDENT].arg(index).arg(info->name).arg(info->phone).arg(info->email).arg(info->photoPath).arg(info->groupId)).first)
            qDebug() << "Cannot save student";
    }
        break;
    case TYPE_STUDENTS_COURSES:
        break;
    }
}

void SaveManager::appendItem(BaseItem* item, ItemType type)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg(mTypeNames[type]));

    if (!res.first || res.second.isEmpty() || res.second.at(0).isEmpty())
    {
        qDebug() << "Cannot get last " << name << " id";
        return;
    }
    int lastId = res.second.at(0).at(0).toInt();

    saveItem(item, type, ++lastId);
}

void SaveManager::insertItem(BaseItem* item, ItemType type)
{
    saveItem(item, type, item->id);
}

void SaveManager::editItem(BaseItem* item, ItemType type)
{
    switch (type)
    {
    case TYPE_CHAPTER:
        if (!mSqlManager.execute(tempUpdStr.arg(info.name).arg(info.orderId).arg(info.disciplineId).arg(info.id)).first)
            qDebug() << "Cannot update chapter";
        break;
    case TYPE_DISCIPLINE:
        if (!mSqlManager.execute(tempUpdStr.arg(info.name).arg(info.literPath).arg(info.id)).first)
            qDebug() << "Cannot update discipline";
        break;
    case TYPE_THEME: break;
    case TYPE_SUBTHEME: break;
    case TYPE_LAB_WORK: break;
    case TYPE_THEME_LECTURE: break;
    case TYPE_SUBTHEME_LECTURE: break;
    case TYPE_REPORT: break;
    case TYPE_REPORT_FILE: break;
    case TYPE_GROUP: break;
    case TYPE_STUDENT: break;
    case TYPE_STUDENTS_COURSES: break;
    }


}

void SaveManager::deleteItem(const int& id, ItemType type)
{
    if (!mSqlManager.execute(mDeleteStr[type].arg(id)).first)
        qDebug() << "Cannot delete item";
}

void SaveManager::updStudentsCourses(const int& courseId, int groupId)
{
    auto res = mSqlManager.execute(getLastIdTemp.arg("StudentsCourses"));
    int lastId = 0;

    if (res.first && !res.second.isEmpty() && !res.second.at(0).isEmpty())
    {
        qDebug() << "Cannot get last student course";
        lastId = res.second.at(0).at(0).toInt();
    }

    QString tempAddStr("INSERT INTO StudentsCourses(Id, StudId, CourseId) VALUES ");
    QString tempExStr("SELECT * FROM StudentsCourses WHERE StudId='%1' AND CourseId='%2'");

    QString selStudsStr;

    if (groupId > 0)
        selStudsStr = QString("SELECT Id FROM Student WHERE Group_Id='%1'").arg(groupId);
    else
        selStudsStr = "SELECT * FROM Student";

    auto studIds = mSqlManager.execute(selStudsStr);
    if (!studIds.first || studIds.second.isEmpty() || studIds.second.at(0).isEmpty())
    {
        qDebug() << "Cannot get last student ids";
        return;
    }

    for (auto stud : studIds.second)
    {
        auto tmp = mSqlManager.execute(tempExStr.arg(stud.at(0).toInt()).arg(courseId));
        if (tmp.first && !tmp.second.isEmpty() && !tmp.second.at(0).isEmpty())
            continue;
        tempAddStr.append(QString("(" + QString::number(++lastId) + "," + QString::number(stud.at(0).toInt()) + "," + QString::number(courseId) + "),"));
    }

    tempAddStr = tempAddStr.left(tempAddStr.length() - 1);

    if (!mSqlManager.execute(tempAddStr).first)
        qDebug() << "Cannot save addStudentsCourses";
}

void SaveManager::addTeacherMail(const QString& mail)
{
    const QString tempAddStr("INSERT INTO TeacherEmail(Id,Email) VALUES (1, \"%1\")");

    if (!mSqlManager.execute(tempAddStr.arg(mail)).first)
        qDebug() << "Cannot save teacher email";
}

void SaveManager::updDiscipline(const DisciplineStud& info)
{
    const QString tempUpdStr("UPDATE Discipline SET Name=\"%1\",Liter_Path=\"%2\" WHERE Id='%3'");


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
    const QString tempUpdStr("UPDATE Lab_Work SET Discipline_Id='%1',Finish_Date=\"%2\",Name=\"%3\",Path=\"%4\" WHERE Id='%5'");

    if (!mSqlManager.execute(tempUpdStr.arg(info.disciplineId).arg(info.finishDate).arg(info.name).arg(info.path).arg(info.id)).first)
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

void SaveManager::updTeacherMail(const QString& mail)
{
    auto exist = mSqlManager.execute("SELECT * FROM TeacherEmail");
    if (!exist.first || exist.second.isEmpty() || exist.second.at(0).isEmpty())
    {
        addTeacherMail(mail);
        return;
    }

    const QString tempAddStr("UPDATE TeacherEmail SET Email=\"%1\" WHERE Id=1");
    if (!mSqlManager.execute(tempAddStr.arg(mail)).first)
        qDebug() << "Cannot update teacher email";
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

QList<Discipline> SaveManager::loadDiscipline()
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
        tmp.disciplineId = work.at(1).toInt();
        tmp.finishDate = work.at(2).toString();
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

QString SaveManager::loadTeacherMail()
{
    const QString tempLoadStr("SELECT Email FROM TeacherEmail");
    auto res = mSqlManager.execute(tempLoadStr);

    if (!res.first || res.second.isEmpty() || res.second.at(0).isEmpty())
    {
        qDebug() << "Cannot load TeacherEmail";
        return QString();
    }

    qDebug() << res.second.at(0).at(0).toString();
    return res.second.at(0).at(0).toString();
}

QStringList SaveManager::studentsEmails(const int& courseId)
{
    const QString tempLoadStr("SELECT StudId FROM StudentsCourses WHERE CourseId='%1'");
    auto res = mSqlManager.execute(tempLoadStr.arg(courseId));

    if (!res.first || res.second.isEmpty() || res.second.at(0).isEmpty())
    {
        qDebug() << "Cannot load StudentsCourses";
        return QStringList();
    }

    QStringList emails;

    for (auto st : res.second)
    {
        const QString tempLoadStr("SELECT Email FROM Student WHERE Id='%1'");
        qDebug() << tempLoadStr.arg(st.at(0).toInt());

        auto em = mSqlManager.execute(tempLoadStr.arg(st.at(0).toInt()));

        if (!em.first || em.second.isEmpty() || em.second.at(0).isEmpty())
        {
            qDebug() << "Cannot load Student";
            continue;
        }
        emails << em.second.at(0).at(0).toString();
    }

    return emails;
}

void SaveManager::initTables()
{
    const QStringList values {
        "CREATE TABLE IF NOT EXISTS TeacherEmail (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Email TEXT)",
        "CREATE TABLE IF NOT EXISTS Chapter (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Discipline_Id INTEGER, FOREIGN KEY (Discipline_Id) REFERENCES Discipline(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Lab_Work (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Discipline_Id INTEGER, Finish_Date TEXT, Name TEXT, Path TEXT, FOREIGN KEY (Discipline_Id) REFERENCES Discipline(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Subtheme_Lecture_File (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Subtheme_Id INTEGER, Path TEXT, FOREIGN KEY (Subtheme_Id) REFERENCES Subtheme(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Theme_Lecture_File (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Theme_Id INTEGER, Path TEXT, FOREIGN KEY (Theme_Id) REFERENCES Theme(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Report (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Lab_Id INTEGER, Delivery_Date TEXT, Mark TEXT, Evaluation_Date TEXT, Stud_Id INTEGER, FOREIGN KEY (Lab_Id) REFERENCES Lab_Work(Id) ON DELETE CASCADE, FOREIGN KEY (Stud_Id) REFERENCES Student(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Report_File (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Report_Id INTEGER, Path TEXT, FOREIGN KEY (Report_Id) REFERENCES Report(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Subtheme (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Theme_Id INTEGER, FOREIGN KEY (Theme_Id) REFERENCES Theme(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Theme (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Chapter_Id INTEGER, FOREIGN KEY (Chapter_Id) REFERENCES Chapter(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS Discipline (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Liter_Path TEXT, Educ_Plan_Path TEXT, Educ_Progr_Path TEXT)",
        "CREATE TABLE IF NOT EXISTS 'Group' (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT)",
        "CREATE TABLE IF NOT EXISTS Student (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, Name TEXT, Phone TEXT, Email TEXT, Photo_Path TEXT, Group_Id INTEGER, FOREIGN KEY (Group_Id) REFERENCES 'Group'(Id) ON DELETE CASCADE)",
        "CREATE TABLE IF NOT EXISTS StudentsCourses (Id INTEGER NOT NULL PRIMARY KEY UNIQUE, StudId INTEGER, CourseId INTEGER, FOREIGN KEY (StudId) REFERENCES Student(Id) ON DELETE CASCADE, FOREIGN KEY (CourseId) REFERENCES Discipline(Id) ON DELETE CASCADE)"
    };

    for (auto val : values)
        if (!mSqlManager.execute(val).first)
            qDebug() << "Cannot init table: " << val;

}
