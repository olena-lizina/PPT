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

    enum LecturePartType {
        Discipline,
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
    void saveLecturePart(const LecturePart& lecture, const SaveManager::LecturePartType& type);
    void updateLecturePart(const LecturePart& oldLecture, const LecturePart& newLecture, const SaveManager::LecturePartType& type);
    void deleteLecturePart(const int& id, const int& parentId, const SaveManager::LecturePartType& type);
    void deleteLecturePartByParentId(const int& parentId, const SaveManager::LecturePartType& type);

    std::list<LecturePart> getLectureParts(const SaveManager::LecturePartType& type);

protected:
    void initTables();

    int getPartId(const QString& part);
    int getChapterId(const int& partId, const QString& chapter);
    int getThemeId(const int& chapterId, const QString& theme);

protected:
    std::shared_ptr<QSqlDatabase> mDbConnection;
    std::shared_ptr<QSqlQuery> mQuery;
    bool isInitialized = { false };
};

#endif // SAVEMANAGER_H
