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

#ifndef LECTURESMANAGER_H
#define LECTURESMANAGER_H
#include "ManagerInterface.h"
#include "DataTypes.h"

#include <memory>
#include <QList>

class LecturesManager: public ManagerInterface
{
    Q_OBJECT

    Q_ENUMS(FileType)
    Q_ENUMS(ItemType)

    Q_PROPERTY(QList<QObject*> lecturesTree READ lecturesTree NOTIFY lecturesTreeChanged)
    Q_PROPERTY(QString teacherEmail READ teacherEmail WRITE setTeacherEmail NOTIFY teacherEmailChanged)

public:

    enum ItemType {
        DISCIPLINE_ITEM,
        CHAPTER_ITEM,
        THEME_ITEM,
        SUBTHEME_ITEM
    };

    enum FileType {
        ThemeLectureFileType,
        SubthemeLectureFileType,
        LiteratureListFileType,
        EducationPlanFileType,
        EducationProgramFileType
    };

    explicit LecturesManager(QObject* parent = nullptr);
    static QObject* managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

public:
    Q_INVOKABLE void insertItem(const QString& name, const int& idx, ItemType type);
    Q_INVOKABLE void appendItem(const QString& name, const int& idx, ItemType type);
    Q_INVOKABLE void updateItem(const QString& name, const int& idx, ItemType type);
    Q_INVOKABLE void removeItem(const int& idx, ItemType type);
    Q_INVOKABLE void removeFile(const LecturesManager::FileType& type, const int& idx);
    Q_INVOKABLE bool fileExist(const LecturesManager::FileType& type, const int& idx);
    Q_INVOKABLE QString getFileContent(const LecturesManager::FileType& type, const int& idx);
    Q_INVOKABLE void createFile(const int& idx, FileType type);
    Q_INVOKABLE void saveFileContent(const QString& text, const int& idx, FileType type);
    Q_INVOKABLE void copyFile(QString path, const int& idx, FileType type);

    QString teacherEmail() const;
    void setTeacherEmail(const QString& email);

    Q_INVOKABLE void saveStudentCourses(QString courseName, int groupId);

    Q_INVOKABLE QStringList getCourses();

    QList<QObject*> lecturesTree();
    Q_INVOKABLE bool isEmptyTree();

signals:
    void lecturesTreeChanged();
    void fileContentChanged();
    void teacherEmailChanged();

private:
    void initLecturesTree();

    void saveThemeLectureFile(const QString& path, const int& idx);
    void saveSubthemeLectureFile(const QString& path, const int& idx);

private:
    QString                    mTeacherEmail;
    QList<Theme>               mThemes;
    QList<Chapter>             mChapters;
    QList<QObject*>            mLecturesTree;
    QList<Subtheme>            mSubtheme;
    QList<Discipline>          mDisciplines;
    QList<ThemeLectureFile>    mThemeLectureFiles;
    QList<SubthemeLectureFile> mSubthemeLectureFiles;
};

#endif // LECTURESMANAGER_H
