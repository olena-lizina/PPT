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
#include "Lecture.h"

class LecturesManager: public QObject
{
    Q_OBJECT

    Q_ENUMS(Type)

    struct selectedIt {
        QString name;
        int id;
    };

public:

    enum Type {
        Disciplines,
        Parts,
        Chapters,
        Themes,
        SubThemes
    };

    explicit LecturesManager(QObject* parent = nullptr);

    // singleton type provider function
    static QObject* lecturesManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    static void setSaveManager(std::shared_ptr<SaveManager> saveMgr);
    Q_INVOKABLE void clearComponentCache();

    Q_INVOKABLE QStringList getListModel(const LecturesManager::Type&);
    Q_INVOKABLE bool itemExists(const QString& name, const LecturesManager::Type&);
    Q_INVOKABLE void addItem(const QString& name, const LecturesManager::Type&, const QString& fileName = "");
    Q_INVOKABLE void editItem(const QString& name, const LecturesManager::Type&, const QString& fileName = "");
    Q_INVOKABLE void deleteItem(const LecturesManager::Type& type);

    Q_INVOKABLE bool hasSubThemes(const QString& theme);

    Q_INVOKABLE void selectedItem(const QString& name, const LecturesManager::Type& type);
    Q_INVOKABLE QString selectedItem(const LecturesManager::Type& type);

    Q_INVOKABLE bool itemHasFile(const QString& name, const LecturesManager::Type& type);

    Q_INVOKABLE void loadAllLectures();

    // work with files
    Q_INVOKABLE void createFile();
    Q_INVOKABLE void selectFile();
    Q_INVOKABLE void saveFileContent(QString);
    Q_INVOKABLE QString getFileContent();
private:
    static SaveManager::Ptr mSaveManager;
    static QQmlApplicationEngine *m_qmlEngine;

    std::list<LecturePart> mDisciplines;
    std::list<LecturePart> mParts;
    std::list<LecturePart> mChapters;
    std::list<LecturePart> mThemes;
    std::list<LecturePart> mSubThemes;

    selectedIt mSelectedDisciplines;
    selectedIt mSelectedPart;
    selectedIt mSelectedChapter;
    selectedIt mSelectedTheme;
    selectedIt mSelectedSubTheme;

    QString mSelectedFile;
};

#endif // LECTURESMANAGER_H
