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
#include <QObject>
#include <QQmlApplicationEngine>
#include <memory>
#include "SaveManager.h"
#include "DataTypes.h"
#include <QList>

class LecturesManager: public QObject
{
    Q_OBJECT

    Q_ENUMS(FileType)

    Q_PROPERTY(QList<QObject*> labsTree READ labsTree NOTIFY labsTreeChanged)
    Q_PROPERTY(QString teacherEmail READ teacherEmail WRITE setTeacherEmail NOTIFY teacherEmailChanged)

public:

    enum FileType {
        LectureFile,
        LiteratureListFile,
        EducationPlanFile,
        EducationProgramFile
    };

    explicit LecturesManager(QObject* parent = nullptr);
    ~LecturesManager();

    // singleton type provider function
    static QObject* lecturesManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    static void setSaveManager(std::shared_ptr<SaveManager> saveMgr);
    Q_INVOKABLE void clearComponentCache();

public:
    Q_INVOKABLE void updateDiscipline(const QString& name, const int& idx);
    Q_INVOKABLE void updateChapter(const QString& name, const int& idx);
    Q_INVOKABLE void updateTheme(const QString& name, const int& idx);
    Q_INVOKABLE void updateSubtheme(const QString& name, const int& idx);

    Q_INVOKABLE void removeDiscipline(const int& idx);
    Q_INVOKABLE void removeChapter(const int& idx);
    Q_INVOKABLE void removeTheme(const int& idx);
    Q_INVOKABLE void removeSubtheme(const int& idx);
    Q_INVOKABLE void removeFile(const LecturesManager::FileType& type, const int& idx, const int& nesting = 0);

    Q_INVOKABLE void insertDiscipline(const QString& name, const int& idx);
    Q_INVOKABLE void insertChapter(const QString& name, const int& idx);
    Q_INVOKABLE void appendChapter(const QString& name, const int& idx);
    Q_INVOKABLE void insertTheme(const QString& name, const int& idx);
    Q_INVOKABLE void appendTheme(const QString& name, const int& idx);
    Q_INVOKABLE void insertSubtheme(const QString& name, const int& idx);
    Q_INVOKABLE void appendSubtheme(const QString& name, const int& idx);

    Q_INVOKABLE bool fileExist(const LecturesManager::FileType& type, const int& idx, const int& nesting = 0);
    Q_INVOKABLE QString getFileContent(const LecturesManager::FileType& type, const int& idx, const int& nesting = 0);
    Q_INVOKABLE void createFile(const int& nesting, const int& idx);
    Q_INVOKABLE void saveFileContent(const QString& text, const int& nesting, const int& idx);
    Q_INVOKABLE void copyLectureFile(QString path, const int& nesting, const int& idx);

    Q_INVOKABLE void createLiterListFile(const int& idx);
    Q_INVOKABLE void saveLiterListFileContent(const QString& text, const int& idx);
    Q_INVOKABLE void copyLiterListFile(QString path, const int& idx);

    Q_INVOKABLE void createEducPlanFile(const int& idx);
    Q_INVOKABLE void saveEducPlanFileContent(const QString& text, const int& idx);
    Q_INVOKABLE void copyEducPlanFile(QString path, const int& idx);

    Q_INVOKABLE void createEducProgFile(const int& idx);
    Q_INVOKABLE void saveEducProgFileContent(const QString& text, const int& idx);
    Q_INVOKABLE void copyEducProgFile(QString path, const int& idx);

    Q_INVOKABLE void saveTeacherEmail(const QString& email);

    QString teacherEmail() const;
    void setTeacherEmail(const QString& email);

    Q_INVOKABLE void saveStudentCourses(QString courseName, int groupId);

    Q_INVOKABLE QStringList getCourses();

public:
    QList<QObject*> labsTree();
    Q_INVOKABLE bool isEmptyTree() { return m_labsTree.size() <= 0; }

signals:
    void labsTreeChanged();
    void fileContentChanged();
    void teacherEmailChanged();

private:
    void initLabsTree();

    QString getLectureFileContent(const int& nesting, const int& idx);
    QString getLiterListFileContent(const int& idx);
    QString getEducPlanFileContent(const int& idx);
    QString getEducProgFileContent(const int& idx);

    void saveThemeLectureFile(const QString& path, const int& idx);
    void saveSubthemeLectureFile(const QString& path, const int& idx);
    void saveLiterListFile(const QString& path, const int& idx);
    void saveEducPlanFile(const QString& path, const int& idx);
    void saveEducProgFile(const QString& path, const int& idx);

    bool lectureFileExist(const int& nesting, const int& idx);
    bool literListFileExist(const int& idx);
    bool educPlanFileExist(const int& idx);
    bool educProgFileExist(const int& idx);

    void removeLectureFile(const int& nesting, const int& idx);
    void removeDisciplineFiles(const int& idx, const LecturesManager::FileType& type);

private:
    static SaveManager::Ptr mSaveManager;
    static QQmlApplicationEngine *m_qmlEngine;
    QList<QObject*> m_labsTree;

    QList<DisciplineTeach> mDisciplines;
    QList<Chapter> mChapters;
    QList<Theme> mThemes;
    QList<Subtheme> mSubtheme;

    QList<ThemeLectureFile> mThemeLectureFiles;
    QList<SubthemeLectureFile> mSubthemeLectureFiles;

    QString mTeacherEmail;
};

#endif // LECTURESMANAGER_H
