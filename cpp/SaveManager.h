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

class Student;
class QSqlDatabase;
class QSqlQuery;

class SaveManager
{
public:

    enum LecturePart {
        Part,
        Chapter,
        Theme,
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

    void saveLecturePart(const LecturePart& lecture, const SaveManager::LecturePart& part);
    void updateLecturePart(const LecturePart& oldLecture, const LecturePart& newLecture, const SaveManager::LecturePart& part);













    QStringList getParts();
    QStringList getChapters(const QString& part);
    QStringList getThemes(const QString& part, const QString& chapter);
    QStringList getSubThemes(const QString& part, const QString& chapter, const QString& theme);

    void savePart(const Part& part);
    void updatePart(const Part& oldPart, const Part& newPart);
    void deletePart(const Part& part);

    void saveChapter(const Chapter& chapter);
    void updateChapter(const Chapter& oldChapter, const Chapter& newChapter);
    void deleteChapter(const Chapter& chapter);

    void saveTheme(const Theme& theme);
    void updateTheme(const Theme& oldTheme, const Theme& newTheme);
    void deleteTheme(const Theme& theme);
    QString getThemeFile(const Theme& theme);

    void saveSubTheme(const SubTheme& subTheme);
    void updateSubTheme(const SubTheme& oldSubTheme, const SubTheme& newSubTheme);
    void deleteSubTheme(const SubTheme& subTheme);
    QString getSubThemeFile(const SubTheme& subTheme);

protected:
    void initTables();

    int getPartId(const QString& part);
    int getChapterId(const int& partId, const QString& chapter);
    int getThemeId(const int& chapterId, const QString& theme);

    int getNextPartId();
    int getNextChapterId();
    int getNextThemeId();
    int getNextSubThemeId();

protected:
    std::shared_ptr<QSqlDatabase> mDbConnection;
    std::shared_ptr<QSqlQuery> mQuery;
    bool isInitialized = { false };
};

#endif // SAVEMANAGER_H
