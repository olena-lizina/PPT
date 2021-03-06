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
#include "ManagerInterface.h"
#include "DataTypes.h"

#include <memory>
#include <QList>

class LabsManager: public ManagerInterface
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> labsTree READ labsTree NOTIFY labsTreeChanged)

public:

    explicit LabsManager(QObject* parent = nullptr);
    static QObject* managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

public:
    QList<QObject*> labsTree();
    Q_INVOKABLE QList<QObject*> getExecutors(int id);
    Q_INVOKABLE QObject* getExecutor(int id);

    Q_INVOKABLE QStringList getReportFiles(int id);

    Q_INVOKABLE bool isEmptyTree() { return m_labsTree.size() <= 0; }

    Q_INVOKABLE void createLab(QString name, QString dueDate, int discipline);
    Q_INVOKABLE void editLab(QString name, QString dueDate, int labId);
    Q_INVOKABLE void importLab(QString name, QString date, int discipline, QString path);
    Q_INVOKABLE bool labExist(int id);
    Q_INVOKABLE void removeLab(int id);
    Q_INVOKABLE QString getLabFinishDate(int id);

    Q_INVOKABLE QString fileContent(int idx);
    Q_INVOKABLE void saveFileContent(QString text, int labId);
    Q_INVOKABLE QString getDisciplineName(int id);
    Q_INVOKABLE QString getLabName(int id);

    Q_INVOKABLE QString reportFileContent(QString path);
    Q_INVOKABLE void saveEvaluation(int id, QString mark, QString date);

signals:
    void labsTreeChanged();
    void reportsChanged();
private:
    void initLabsTree();    

private:
    QList<QObject*> m_labsTree;
    QList<LabWork> mLabWorks;
    QList<Report> mReports;
    QList<ReportFile> mReportFiles;
    QList<Discipline> mDisciplines;
};

#endif // LABSMANAGER_H
