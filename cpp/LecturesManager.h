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

    Q_PROPERTY(QList<QObject*> labsTree READ labsTree NOTIFY labsTreeChanged)

public:

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

    Q_INVOKABLE void insertDiscipline(const QString& name, const int& idx);
    Q_INVOKABLE void insertChapter(const QString& name, const int& idx);
    Q_INVOKABLE void appendChapter(const QString& name, const int& idx);
    Q_INVOKABLE void insertTheme(const QString& name, const int& idx);
    Q_INVOKABLE void appendTheme(const QString& name, const int& idx);
    Q_INVOKABLE void insertSubtheme(const QString& name, const int& idx);
    Q_INVOKABLE void appendSubtheme(const QString& name, const int& idx);

public:
    QList<QObject*> labsTree();
    Q_INVOKABLE bool isEmptyTree() { return m_labsTree.size() <= 0; }

signals:
    void labsTreeChanged();

private:
    void initLabsTree();

private:
    static SaveManager::Ptr mSaveManager;
    static QQmlApplicationEngine *m_qmlEngine;
    QList<QObject*> m_labsTree;

    QList<DisciplineTeach> mDisciplines;
    QList<Chapter> mChapters;
    QList<Theme> mThemes;
    QList<Subtheme> mSubtheme;
};

#endif // LECTURESMANAGER_H
