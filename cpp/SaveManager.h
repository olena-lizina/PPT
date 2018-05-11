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
#include "Lecture.h"

#include "DBManager.h"

class QSqlDatabase;
class QSqlQuery;

class SaveManager
{
public:

    enum LecturePartType {
        Discipline,
        Part,
        ChapterType,
        ThemeType,
        SubTheme
    };

    using WPtr = std::weak_ptr<SaveManager>;
    using Ptr = std::shared_ptr<SaveManager>;

    explicit SaveManager();
    virtual ~SaveManager();

    // work for students
    void saveStudent(const Student&);
    void removeStudent(const Student&);
    void updateStudent(const Student& oldStudent, const Student& newStudent);
    QList<Student> loadAllStudents();
    QStringList getGroups();

    // work for lectures
    void saveLecturePart(const LecturePart& lecture, const SaveManager::LecturePartType& type);
    void updateLecturePart(const LecturePart& oldLecture, const LecturePart& newLecture, const SaveManager::LecturePartType& type);
    void deleteLecturePart(const int& id, const int& parentId, const SaveManager::LecturePartType& type);
    void deleteLecturePartByParentId(const int& parentId, const SaveManager::LecturePartType& type);

    std::list<LecturePart> getLectureParts(const SaveManager::LecturePartType& type);

    void clearTable(const SaveManager::LecturePartType& type);

    void addChapter(const Chapter& info);
    void addDiscipline(const DisciplineStud& info);
    void addDiscipline(const DisciplineTeach& info);
    void addGroup(const Group& info);
    void addLabWork(const LabWork& info);
    void addThemeLectureFile(const ThemeLectureFile& info);
    void addSubthemeLectureFile(const SubthemeLectureFile& info);
    void addReport(const Report& info);
    void addReportFile(const ReportFile& info);
    void addReportStudent(const ReportStudent& info);
    void addStudent(const Student& info);
    void addSubtheme(const Subtheme& info);
    void addTheme(const Theme& info);

    void delChapter(const int& id);
    void delDiscipline(const int& id);
    void delGroup(const int& id);
    void delLabWork(const int& id);
    void delThemeLectureFile(const int& id);
    void delSubthemeLectureFile(const int& id);
    void delReport(const int& id);
    void delReportFile(const int& id);
    void delStudent(const int& id, const int& groupId);
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
    void initTables(); // OK

    int getPartId(const QString& part);
    int getChapterId(const int& partId, const QString& chapter);
    int getThemeId(const int& chapterId, const QString& theme);

protected:
    std::shared_ptr<QSqlDatabase> mDbConnection;
    std::shared_ptr<QSqlQuery> mQuery;
    bool isInitialized = { false };

    SQLiteManager mSqlManager;
};

#endif // SAVEMANAGER_H
