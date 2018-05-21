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
#include "LabsManager.h"

#include <QApplication>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QDir>

#include "ReportInfoModel.h"
#include "TreeItem.h"


/*static*/ QQmlApplicationEngine *LabsManager::m_qmlEngine = nullptr;
/*static*/ SaveManager::Ptr LabsManager::mSaveManager;

LabsManager::LabsManager(QObject* parent)
    : QObject(parent)
{
    mLabWorks = mSaveManager->loadLabWork();
    mReports = mSaveManager->loadReport();
    mReportFiles = mSaveManager->loadReportFile();
    mDisciplines = mSaveManager->loadTeachDiscipline();
    initLabsTree();
}

LabsManager::~LabsManager()
{
}

/*static*/ QObject* LabsManager::labsManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    LabsManager *lectureManager = new LabsManager();
    return lectureManager;
}

/*static*/ void LabsManager::setQmlEngine(QQmlApplicationEngine *engine)
{
    LabsManager::m_qmlEngine = engine;
}

/*static*/ void LabsManager::setSaveManager(std::shared_ptr<SaveManager> saveMgr)
{
    LabsManager::mSaveManager = saveMgr;
}

void LabsManager::clearComponentCache()
{
    LabsManager::m_qmlEngine->clearComponentCache();
}

QList<QObject*> LabsManager::labsTree()
{
    if (m_labsTree.size() <= 0)
    {
        TreeItem * d_item =  new TreeItem(tr("Спочатку створіть дисципліну"), -1, 0);
        m_labsTree << d_item;
    }

    return m_labsTree;
}

QList<QObject*> LabsManager::getExecutors(int id)
{
    QList<QObject*> result;

    auto students = mSaveManager->loadStudent();

    for (auto work : mReports)
    {
        if (id != work.labId)
            continue;

        auto stud = std::find_if(students.begin(), students.end(),
                                 [work](Student& st){ return work.studId == st.id; });

        if (students.end() == stud)
            continue;

        auto rep = std::find_if(mReportFiles.begin(), mReportFiles.end(),
                                [work](ReportFile& st){ return work.id == st.reportId; });

        if (mReportFiles.end() == rep)
            continue;

        result.append(new ReportInfoModel(stud->name, work, rep->path));
    }

    return result;
}

void LabsManager::initLabsTree()
{
    m_labsTree.clear();

    for (auto di : mDisciplines)
    {
        TreeItem* disc = new TreeItem(di.name, di.id, 0);

        for (auto it : mLabWorks)
            if (it.disciplineId == di.id)
                disc->addChild(new TreeItem(it.name, it.id, 1));

        m_labsTree << disc;
    }
    emit labsTreeChanged();
}

void LabsManager::createLab(QString name, QString dueDate, int discipline)
{
    qDebug() << "createLab";

    const QString fileExtension {"qppt"};
    QString baseFolder {"Labs"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    int idx = mLabWorks.isEmpty() ? 1 : mLabWorks.last().id + 1;
    QString filePath(baseFolder + QDir::separator() + QString::number(idx) + "." + fileExtension);
    qDebug() << filePath;

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to create file " << filePath << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << filePath;
    file.close();

    LabWork work;
    work.id = idx;
    work.disciplineId = discipline;
    work.finishDate = dueDate;
    work.name = name;
    work.path = filePath;

    mSaveManager->addLabWork(work);
    mLabWorks.append(work);

    initLabsTree();
}

void LabsManager::editLab(QString name, QString dueDate, int labId)
{
    qDebug() << "editLab";

    auto lab = std::find_if(mLabWorks.begin(), mLabWorks.end(),
                            [&labId](LabWork l){ return l.id == labId; });

    if (lab == mLabWorks.end())
    {
        qDebug() << "Cannot find such lab with id: " << labId;
        return;
    }

    lab->name = name;
    lab->finishDate = dueDate;
    mSaveManager->updLabWork(*lab);
    initLabsTree();
    emit labChanged();
}

void LabsManager::importLab(QString name, QString date, int discipline, QString path)
{
    QString baseFolder {"Labs"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString prefix("file:///");
    path.remove(0, prefix.size());
    QString fileName(path.right(path.size() - path.lastIndexOf('/') - 1));
    QString newPath(baseFolder + '\\' + fileName);
    QFile::copy(path, newPath);

    int idx = mLabWorks.isEmpty() ? 1 : mLabWorks.last().id + 1;

    LabWork work;
    work.id = idx;
    work.disciplineId = discipline;
    work.finishDate = date;
    work.name = name;
    work.path = newPath;

    mSaveManager->addLabWork(work);
    mLabWorks.append(work);

    initLabsTree();
}

bool LabsManager::labExist(int id)
{
    auto res = std::find_if(mLabWorks.begin(), mLabWorks.end(),
                            [&id](LabWork& l){ return l.id == id; });

    if (mLabWorks.end() == res)
        return false;

    QFile file(res->path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    file.close();
    return true;
}

QString LabsManager::getLabFinishDate(int id)
{
    auto res = std::find_if(mLabWorks.begin(), mLabWorks.end(),
                            [&id](LabWork& l){ return l.id == id; });

    if (mLabWorks.end() == res)
        return QString();

    return res->finishDate;
}

QString LabsManager::fileContent()
{
    qDebug() << "fileContent";

    int idx = mSelectedLab;
    auto fileIter = std::find_if(mLabWorks.begin(), mLabWorks.end(),
                                 [&idx](LabWork& file){ return file.id == idx; });

    if (mLabWorks.end() == fileIter || fileIter->path.isEmpty())
        return QString();

    QFile file(fileIter->path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->path << file.errorString();
        return QString();
    }

    QTextStream textStream(&file);
    QString fileText = textStream.readAll().trimmed();
    file.close();
    return fileText;
}

void LabsManager::saveFileContent(QString text, int labId)
{
    qDebug() << "saveFileContent for id: " << labId;
    auto fileIter = std::find_if(mLabWorks.begin(), mLabWorks.end(),
                                     [&labId](LabWork& file){ return file.id == labId; });

    if (mLabWorks.end() == fileIter)
        return;
    QFile file(fileIter->path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->path << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << text;
    file.close();
    emit fileContentChanged();
}

QString LabsManager::getDisciplineName(int id)
{
    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&id](DisciplineTeach& file){ return file.id == id; });

    if (mDisciplines.end() == fileIter)
        return QString();

    return fileIter->name;
}

QString LabsManager::getLabName(int id)
{
    auto fileIter = std::find_if(mLabWorks.begin(), mLabWorks.end(),
                                     [&id](LabWork& file){ return file.id == id; });

    if (mLabWorks.end() == fileIter)
        return QString();

    return fileIter->name;
}

void LabsManager::removeLab(int id)
{
    mSaveManager->delLabWork(id);
    auto fileIter = std::find_if(mLabWorks.begin(), mLabWorks.end(),
                                     [&id](LabWork& file){ return file.id == id; });

    if (mLabWorks.end() == fileIter)
        return;
    mLabWorks.erase(fileIter);
    initLabsTree();
    emit labsTreeChanged();
}
