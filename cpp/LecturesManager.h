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
public:
    explicit LecturesManager(QObject* parent = nullptr);

    // singleton type provider function
    static QObject* lecturesManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    static void setSaveManager(std::shared_ptr<SaveManager> saveMgr);
    Q_INVOKABLE void clearComponentCache();

    Q_INVOKABLE QStringList getParts();
    Q_INVOKABLE QStringList getChapters();
    Q_INVOKABLE QStringList getThemes();
    Q_INVOKABLE QStringList getSubThemes();

    Q_INVOKABLE bool partExists(const QString& part);
    Q_INVOKABLE void addPart(const QString& part);
    Q_INVOKABLE void editPart(const QString& part);
    Q_INVOKABLE void deletePart();

    Q_INVOKABLE bool chapterExists(const QString& chapter);
    Q_INVOKABLE void addChapter(const QString& chapter);
    Q_INVOKABLE void editChapter(const QString& chapter);
    Q_INVOKABLE void deleteChapter();

    Q_INVOKABLE bool themesExists(const QString& theme);
    Q_INVOKABLE void addTheme(const QString& theme);
    Q_INVOKABLE void editTheme(const QString& theme);
    Q_INVOKABLE void deleteTheme();
    Q_INVOKABLE bool hasSubThemes(const QString& theme);

    Q_INVOKABLE bool subThemesExists(const QString& subTheme);
    Q_INVOKABLE void addSubTheme(const QString& subTheme);
    Q_INVOKABLE void editSubTheme(const QString& subTheme);
    Q_INVOKABLE void deleteSubTheme();

    Q_INVOKABLE void selectedPart(const QString& part);
    Q_INVOKABLE QString selectedPart();
    Q_INVOKABLE void selectedChapter(const QString& chapter);
    Q_INVOKABLE QString selectedChapter();
    Q_INVOKABLE void selectedTheme(const QString& theme);
    Q_INVOKABLE QString selectedTheme();
    Q_INVOKABLE void selectedSubTheme(const QString& subTheme);
    Q_INVOKABLE QString selectedSubTheme();

    Q_INVOKABLE bool hasLecture(const QString& theme);

signals:
    void signalUpdateThemes();

private:
    static SaveManager::Ptr mSaveManager;
    static QQmlApplicationEngine *m_qmlEngine;
    QList<Lecture> mLectureList;

    QString mSelectedPart;
    QString mSelectedChapter;
    QString mSelectedTheme;
    QString mSelectedSubTheme;
};

#endif // LECTURESMANAGER_H
