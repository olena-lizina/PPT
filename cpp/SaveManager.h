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

#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H
#include <QList>
#include <QMap>
#include <memory>
#include "DataTypes.h"
#include "SQLiteManager.h"

class SaveManager
{
public:

    enum ItemType {
        TYPE_CHAPTER,
        TYPE_DISCIPLINE,
        TYPE_THEME,
        TYPE_SUBTHEME,
        TYPE_LAB_WORK,
        TYPE_THEME_LECTURE,
        TYPE_SUBTHEME_LECTURE,
        TYPE_REPORT,
        TYPE_REPORT_FILE,
        TYPE_GROUP,
        TYPE_STUDENT,
        TYPE_STUDENTS_COURSES
    };

public:

    using WPtr = std::weak_ptr<SaveManager>;
    using Ptr = std::shared_ptr<SaveManager>;

    explicit SaveManager();
    virtual ~SaveManager();

    void appendItem(BaseItem* item, ItemType type);
    void insertItem(BaseItem* item, ItemType type);
    void editItem(BaseItem* item, ItemType type);
    void deleteItem(const int& id, ItemType type);

//    void updChapter(const Chapter& info);
//    void updDiscipline(const DisciplineStud& info);
//    void updDiscipline(const DisciplineTeach& info);
//    void updGroup(const Group& info);
//    void updLabWork(const LabWork& info);
//    void updThemeLectureFile(const ThemeLectureFile& info);
//    void updSubthemeLectureFile(const SubthemeLectureFile& info);
//    void updReport(const Report& info);
//    void updReportFile(const ReportFile& info);
//    void updStudent(const Student& info);
//    void updSubtheme(const Subtheme& info);
//    void updTheme(const Theme& info);
//    void updTeacherMail(const QString& mail);
//    void updStudentsCourses(const int& courseId, int studIds);

    void updChapterIdx(const int& oldIdx, const int& newIdx);
    void updDisciplineIdx(const int& oldIdx, const int& newIdx);
    void updSubthemeIdx(const int& oldIdx, const int& newIdx);
    void updThemeIdx(const int& oldIdx, const int& newIdx);

    QList<Chapter> loadChapters();
    QList<Discipline> loadDiscipline();
    QList<Group> loadGroup();
    QList<LabWork> loadLabWork();
    QList<ThemeLectureFile> loadThemeLectureFile();
    QList<SubthemeLectureFile> loadSubthemeLectureFile();
    QList<Report> loadReport();
    QList<ReportFile> loadReportFile();
    QList<Student> loadStudent();
    QList<Subtheme> loadSubtheme();
    QList<Theme> loadTheme();
    QString loadTeacherMail();

    QStringList studentsEmails(const int& courseId);

protected:
    void initTypeNames();
    void initInsertStrs();
    void initDeleteStrs();
    void initEditStrs();

    void saveItem(BaseItem* item, ItemType type, int index);


    void initTables();


    void addTeacherMail(const QString& mail);

protected:
    SQLiteManager mSqlManager;

    QMap<ItemType, QString> mTypeNames;
    QMap<ItemType, QString> mInsertStr;
    QMap<ItemType, QString> mDeleteStr;
    QMap<ItemType, QString> mEditStr;
};

#endif // SAVEMANAGER_H
