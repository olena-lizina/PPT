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

class Student;
class QSqlDatabase;
class QSqlQuery;

class SaveManager
{
public:
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
    QStringList getParts();
    QStringList getChapters(const QString& part);
    QStringList getThemes(const QString& part, const QString& chapter);
    QStringList getSubThemes(const QString& part, const QString& chapter, const QString& theme);

    void savePart(const QString& part);
    void updatePart(const QString& oldPart, const QString& newPart);
    void deletePart(const QString& part);

    void saveChapter(const QString& part, const QString& chapter);
    void updateChapter(const QString& oldChapter, const QString& newChapter, const QString& part);
    void deleteChapter(const QString& chapter, const QString& part);

    void saveTheme(const QString& part, const QString& chapter, const QString& theme);
    void updateTheme(const QString& oldTheme, const QString& newTheme, const QString& part, const QString& chapter);
    void deleteTheme(const QString& part, const QString& chapter, const QString& theme);

    void saveSubTheme(const QString& part, const QString& chapter, const QString& theme, const QString& subTheme);
    void updateSubTheme(const QString& oldSubTheme, const QString& newSubTheme, const QString& part, const QString& chapter, const QString& theme);
    void deleteSubTheme(const QString& part, const QString& chapter, const QString& theme, const QString& subTheme);

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
