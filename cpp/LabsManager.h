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

#ifndef LABSMANAGER_H
#define LABSMANAGER_H
#include <QObject>
#include <QQmlApplicationEngine>
#include <memory>
#include "SaveManager.h"
#include "StudentManager.h"
#include "DataTypes.h"
#include <QList>

class LabsManager: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> labsTree READ labsTree NOTIFY labsTreeChanged)
    Q_PROPERTY(QString fileContent READ fileContent WRITE setFileContent NOTIFY fileContentChanged)
    Q_PROPERTY(int selectedLab READ selectedLab WRITE setSelectedLab NOTIFY selectedLabChanged)

public:

    explicit LabsManager(QObject* parent = nullptr);
    ~LabsManager();

    // singleton type provider function
    static QObject* labsManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    static void setSaveManager(std::shared_ptr<SaveManager> saveMgr);
    Q_INVOKABLE void clearComponentCache();

public:
    QList<QObject*> labsTree();
    Q_INVOKABLE QList<QObject*> getExecutors(int id);
    Q_INVOKABLE bool isEmptyTree() { return m_labsTree.size() <= 0; }

    Q_INVOKABLE void createLab(QString name, QString dueDate, int discipline);
    Q_INVOKABLE void editLab(QString name, QString dueDate, int labId);
    Q_INVOKABLE void importLab(QString name, QString date, int discipline, QString path);
    Q_INVOKABLE bool labExist(int id);
    Q_INVOKABLE void removeLab(int id);
    Q_INVOKABLE QString getLabFinishDate(int id);

    QString fileContent();
    int selectedLab() { return mSelectedLab; }

    void setSelectedLab(int id) { mSelectedLab = id; }
    void setFileContent(QString) {}

    Q_INVOKABLE void saveFileContent(QString text, int labId);
    Q_INVOKABLE QString getDisciplineName(int id);
    Q_INVOKABLE QString getLabName(int id);

signals:
    void labsTreeChanged();
    void fileContentChanged();
    void labChanged();
    void selectedLabChanged();

private:
    void initLabsTree();    

private:
    static SaveManager::Ptr mSaveManager;
    static QQmlApplicationEngine *m_qmlEngine;
    QList<QObject*> m_labsTree;

    QList<LabWork> mLabWorks;
    QList<Report> mReports;
    QList<ReportFile> mReportFiles;
    QList<DisciplineTeach> mDisciplines;

    int mSelectedLab;
};

#endif // LABSMANAGER_H
