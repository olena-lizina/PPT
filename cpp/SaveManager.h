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
#include <memory>
#include "DataTypes.h"
#include "DBManager.h"

class SaveManager
{
public:

    using WPtr = std::weak_ptr<SaveManager>;
    using Ptr = std::shared_ptr<SaveManager>;

    explicit SaveManager();
    virtual ~SaveManager();

    void appendChapter(const Chapter& info);
    void addChapter(const Chapter& info);
    void appendDiscipline(const DisciplineStud& info);
    void appendDiscipline(const DisciplineTeach& info);
    void addDiscipline(const DisciplineTeach& info);
    void addGroup(const Group& info);
    void addLabWork(const LabWork& info);
    void addThemeLectureFile(const ThemeLectureFile& info);
    void addSubthemeLectureFile(const SubthemeLectureFile& info);
    void addReport(const Report& info);
    void addReportFile(const ReportFile& info);
    void addStudent(const Student& info);
    void appendSubtheme(const Subtheme& info);
    void addSubtheme(const Subtheme& info);
    void appendTheme(const Theme& info);
    void addTheme(const Theme& info);

    void delChapter(const int& id);
    void delDiscipline(const int& id);
    void delGroup(const int& id);
    void delLabWork(const int& id);
    void delThemeLectureFile(const int& id);
    void delSubthemeLectureFile(const int& id);
    void delReport(const int& id);
    void delReportFile(const int& id);
    void delStudent(const int& id);
    void delSubtheme(const int& id);
    void delTheme(const int& id);

    void updChapter(const Chapter& info);
    void updDiscipline(const DisciplineStud& info);
    void updDiscipline(const DisciplineTeach& info);
    void updGroup(const Group& info);
    void updLabWork(const LabWork& info);
    void updThemeLectureFile(const ThemeLectureFile& info);
    void updSubthemeLectureFile(const SubthemeLectureFile& info);
    void updReport(const Report& info);
    void updReportFile(const ReportFile& info);
    void updStudent(const Student& info);
    void updSubtheme(const Subtheme& info);
    void updTheme(const Theme& info);

    void updChapterIdx(const int& oldIdx, const int& newIdx);
    void updDisciplineIdx(const int& oldIdx, const int& newIdx);
    void updSubthemeIdx(const int& oldIdx, const int& newIdx);
    void updThemeIdx(const int& oldIdx, const int& newIdx);

    QList<Chapter> loadChapters();
    QList<DisciplineStud> loadStudDiscipline();
    QList<DisciplineTeach> loadTeachDiscipline();
    QList<Group> loadGroup();
    QList<LabWork> loadLabWork();
    QList<ThemeLectureFile> loadThemeLectureFile();
    QList<SubthemeLectureFile> loadSubthemeLectureFile();
    QList<Report> loadReport();
    QList<ReportFile> loadReportFile();
    QList<Student> loadStudent();
    QList<Subtheme> loadSubtheme();
    QList<Theme> loadTheme();

protected:
    void initTables();

protected:
    SQLiteManager mSqlManager;
};

#endif // SAVEMANAGER_H
