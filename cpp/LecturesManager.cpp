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
#include "LecturesManager.h"
#include <QDebug>
#include <QApplication>
#include "TreeItem.h"
#include <QFile>
#include <QDir>

/*static*/ QQmlApplicationEngine *LecturesManager::m_qmlEngine = nullptr;
/*static*/ SaveManager::Ptr LecturesManager::mSaveManager;

LecturesManager::LecturesManager(QObject* parent)
    : QObject(parent)
{
#ifdef STUDENT_MODE
    mDisciplines = mSaveManager->loadStudDiscipline();
#else
    mDisciplines = mSaveManager->loadTeachDiscipline();
#endif

    mChapters    = mSaveManager->loadChapters();
    mThemes      = mSaveManager->loadTheme();
    mSubtheme    = mSaveManager->loadSubtheme();

    mThemeLectureFiles = mSaveManager->loadThemeLectureFile();
    mSubthemeLectureFiles = mSaveManager->loadSubthemeLectureFile();

    initLabsTree();
}

LecturesManager::~LecturesManager()
{
}

/*static*/ QObject* LecturesManager::lecturesManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    LecturesManager *lectureManager = new LecturesManager();
    return lectureManager;
}

/*static*/ void LecturesManager::setQmlEngine(QQmlApplicationEngine *engine)
{
    LecturesManager::m_qmlEngine = engine;
}

/*static*/ void LecturesManager::setSaveManager(std::shared_ptr<SaveManager> saveMgr)
{
    LecturesManager::mSaveManager = saveMgr;
}

void LecturesManager::clearComponentCache()
{
    LecturesManager::m_qmlEngine->clearComponentCache();
}

QList<QObject*> LecturesManager::labsTree()
{
    if (m_labsTree.size() <= 0)
    {
        TreeItem * d_item =  new TreeItem(tr("Add discipline"), -1, 0);
        m_labsTree << d_item;
    }

    return m_labsTree;
}

void LecturesManager::initLabsTree()
{
    m_labsTree.clear();

    for (auto disc : mDisciplines)
    {
        TreeItem * d_item = new TreeItem(disc.name, disc.id, 0);

        for (auto chapt : mChapters)
        {
            if (chapt.disciplineId != disc.id)
                continue;

            TreeItem * c_item = new TreeItem(chapt.name, chapt.id, 1);

            for (auto them : mThemes)
            {
                if (them.chapterId != chapt.id)
                    continue;

                TreeItem * t_item = new TreeItem(them.name, them.id, 2);

                for (auto sub : mSubtheme)
                {
                    if (sub.themeId != them.id)
                        continue;

                    TreeItem * s_item = new TreeItem(sub.name, sub.id, 3);
                    t_item->addChild(s_item);

                }
                c_item->addChild(t_item);
            }
            d_item->addChild(c_item);
        }

        m_labsTree << d_item;
    }

    emit labsTreeChanged();
}

void LecturesManager::updateDiscipline(const QString& name, const int& idx)
{
    qDebug() << "updateDiscipline: " << idx << " " << name;

    auto discIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& disc){ return disc.id == idx; });
    if (mDisciplines.end() == discIter)
    {
        qWarning() << "Cannot update discipline: " << name;
        return;
    }

    discIter->name = name;
    mSaveManager->updDiscipline(*discIter);
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::updateChapter(const QString& name, const int& idx)
{
    qDebug() << "updateChapter: " << idx << " " << name;

    auto chapIter = std::find_if(mChapters.begin(), mChapters.end(),
                                 [&idx](Chapter& ch){ return ch.id == idx; });
    if (mChapters.end() == chapIter)
    {
        qWarning() << "Cannot update chapter: " << name;
        return;
    }

    chapIter->name = name;
    mSaveManager->updChapter(*chapIter);
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::updateTheme(const QString& name, const int& idx)
{
    qDebug() << "updateTheme: " << idx << " " << name;

    auto themIter = std::find_if(mThemes.begin(), mThemes.end(),
                                 [&idx](Theme& th){ return th.id == idx; });
    if (mThemes.end() == themIter)
    {
        qWarning() << "Cannot update theme: " << name;
        return;
    }

    themIter->name = name;
    mSaveManager->updTheme(*themIter);
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::updateSubtheme(const QString& name, const int& idx)
{
    qDebug() << "updateSubtheme: " << idx << " " << name;

    auto subIter = std::find_if(mSubtheme.begin(), mSubtheme.end(),
                                [&idx](Subtheme& sub){ return sub.id == idx; });
    if (mSubtheme.end() == subIter)
    {
        qWarning() << "Cannot update subtheme: " << name;
        return;
    }

    subIter->name = name;
    mSaveManager->updSubtheme(*subIter);
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::removeDiscipline(const int& idx)
{
    qDebug() << "removeDiscipline: " << idx;

    mSaveManager->delDiscipline(idx);
    mDisciplines.clear();
    mDisciplines = mSaveManager->loadTeachDiscipline();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::removeChapter(const int& idx)
{
    qDebug() << "removeChapter: " << idx;

    mSaveManager->delChapter(idx);
    mChapters.clear();
    mChapters = mSaveManager->loadChapters();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::removeTheme(const int& idx)
{
    qDebug() << "removeTheme: " << idx;

    mSaveManager->delTheme(idx);
    mThemes.clear();
    mThemes = mSaveManager->loadTheme();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::removeSubtheme(const int& idx)
{
    qDebug() << "removeSubtheme: " << idx;

    mSaveManager->delSubtheme(idx);
    mSubtheme.clear();
    mSubtheme = mSaveManager->loadSubtheme();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::insertDiscipline(const QString& name, const int& idx)
{
    qDebug() << "insertDiscipline: " << idx << " " << name;

    auto it = mDisciplines.end();
    --it;

    while (mDisciplines.begin() != it)
    {
        if (it->id <= idx)
            break;

        int old = it->id;
        mSaveManager->updDisciplineIdx(old, (it->id = it->id + 1));
        --it;
    }

    DisciplineTeach add;
    add.name = name;
    add.literPath = "";
    add.educPlanPath = "";
    add.educProgPath = "";

    if (-1 == idx)
        add.id = 1;
    else
        add.id = idx + 1;

    mSaveManager->addDiscipline(add);

    mDisciplines.clear();
    mDisciplines = mSaveManager->loadTeachDiscipline();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::insertChapter(const QString& name, const int& idx)
{
    qDebug() << "insertChapter: " << idx << " " << name;

    auto it = mChapters.end();
    --it;

    int discIdx = -1;

    while (mChapters.begin() != it)
    {
        if (it->id == idx)
            discIdx = it->disciplineId;

        if (it->id <= idx)
            break;

        int old = it->id;
        mSaveManager->updChapterIdx(old, (it->id = it->id + 1));
        --it;
    }

    Chapter add;
    add.name = name;
    add.id = idx + 1;
    add.disciplineId = discIdx;
    add.orderId = 0;
    mSaveManager->addChapter(add);

    mChapters.clear();
    mChapters = mSaveManager->loadChapters();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::appendChapter(const QString& name, const int& idx)
{
    qDebug() << "insertFirstChapter: " << idx << " " << name;

    Chapter add;
    add.name = name;
    add.disciplineId = idx;
    add.orderId = 0;
    mSaveManager->appendChapter(add);

    mChapters.clear();
    mChapters = mSaveManager->loadChapters();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::insertTheme(const QString& name, const int& idx)
{
    qDebug() << "insertTheme: " << idx << " " << name;

    auto it = mThemes.end();
    --it;

    int chaptIdx = -1;

    while (mThemes.begin() != it)
    {
        if (it->id == idx)
            chaptIdx = it->chapterId;

        if (it->id <= idx)
            break;

        int old = it->id;
        mSaveManager->updThemeIdx(old, (it->id = it->id + 1));
        --it;
    }

    Theme add;
    add.name = name;
    add.id = idx + 1;
    add.chapterId = chaptIdx;
    add.orderId = 0;
    mSaveManager->addTheme(add);

    mThemes.clear();
    mThemes = mSaveManager->loadTheme();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::appendTheme(const QString& name, const int& idx)
{
    qDebug() << "appendTheme: " << idx << " " << name;

    Theme add;
    add.name = name;
    add.chapterId = idx;
    add.orderId = 0;
    mSaveManager->appendTheme(add);

    mThemes.clear();
    mThemes = mSaveManager->loadTheme();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::insertSubtheme(const QString& name, const int& idx)
{
    qDebug() << "insertSubtheme: " << idx << " " << name;

    auto it = mSubtheme.end();
    --it;

    int themeIdx = -1;

    while (mSubtheme.begin() != it)
    {
        if (it->id == idx)
            themeIdx = it->themeId;

        if (it->id <= idx)
            break;

        int old = it->id;
        mSaveManager->updSubthemeIdx(old, (it->id = it->id + 1));
        --it;
    }

    Subtheme add;
    add.name = name;
    add.id = idx + 1;
    add.themeId = themeIdx;
    add.orderId = 0;
    mSaveManager->addSubtheme(add);

    mSubtheme.clear();
    mSubtheme = mSaveManager->loadSubtheme();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::appendSubtheme(const QString& name, const int& idx)
{
    qDebug() << "appendSubtheme: " << idx << " " << name;

    Subtheme add;
    add.name = name;
    add.themeId = idx;
    add.orderId = 0;
    mSaveManager->appendSubtheme(add);

    mSubtheme.clear();
    mSubtheme = mSaveManager->loadSubtheme();
    initLabsTree();
    emit labsTreeChanged();
}

bool LecturesManager::fileExist(const LecturesManager::FileType& type, const int& idx, const int& nesting)
{
    qDebug() << "fileExist";

    switch(type)
    {
    case LectureFile: return lectureFileExist(nesting, idx);
    case LiteratureListFile: return literListFileExist(idx);
    case EducationPlanFile: return educPlanFileExist(idx);
    case EducationProgramFile: return educProgFileExist(idx);
    default: return false;
    }
}

QString LecturesManager::getFileContent(const LecturesManager::FileType& type, const int& idx, const int& nesting)
{
    qDebug() << "getFileContent";

    switch(type)
    {
    case LectureFile: return getLectureFileContent(nesting, idx);
    case LiteratureListFile: return getLiterListFileContent(idx);
    case EducationPlanFile: return getEducPlanFileContent(idx);
    case EducationProgramFile: return getEducProgFileContent(idx);
    default: return QString();
    }
}

bool LecturesManager::lectureFileExist(const int& nesting, const int& idx)
{
    qDebug() << "lectureFileExist";

    if (nesting == 3)
    {
        auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                     [&idx](SubthemeLectureFile& file){ return file.subthemeId == idx; });

        if (mSubthemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return false;
        return true;
    }
    else if (nesting == 2)
    {
        auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                     [&idx](ThemeLectureFile& file){ return file.themeId == idx; });

        if (mThemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return false;
        return true;
    }
    return false;
}

QString LecturesManager::getLectureFileContent(const int& nesting, const int& idx)
{
    qDebug() << "getLectureFileContent";
    QString path;

    if (nesting == 3)
    {
        auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                     [&idx](SubthemeLectureFile& file){ return file.subthemeId == idx; });

        if (mSubthemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return QString();

        path = fileIter->path;
    }
    else if (nesting == 2)
    {
        auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                     [&idx](ThemeLectureFile& file){ return file.themeId == idx; });

        if (mThemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return QString();

        path = fileIter->path;
    }

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << path << file.errorString();
        return QString();
    }

    QTextStream textStream(&file);
    QString fileContent = textStream.readAll().trimmed();
    file.close();
    return fileContent;
}

void LecturesManager::createFile(const int& nesting, const int& idx)
{
    qDebug() << "createFile";

    const QString fileExtension {"qppt"};
    QString baseFolder {"Lectures\\"};

    if (nesting == 3)
        baseFolder.append("Subthemes");
    else
        baseFolder.append("Themes");

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

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

    if (nesting == 3)
        saveSubthemeLectureFile(filePath, idx);
    else
        saveThemeLectureFile(filePath, idx);

    file.close();
}

void LecturesManager::saveFileContent(const QString& text, const int& nesting, const int& idx)
{
    qDebug() << "saveFileContent for id: " << idx;
    QString path;

    if (nesting == 3)
    {
        auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                     [&idx](SubthemeLectureFile& file){ return file.subthemeId == idx; });

        if (mSubthemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return;

        path = fileIter->path;
    }
    else
    {
        auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                     [&idx](ThemeLectureFile& file){ return file.themeId == idx; });

        if (mThemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return;

        path = fileIter->path;
    }

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << path << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << text;
    file.close();
}

void LecturesManager::copyLectureFile(QString path, const int& nesting, const int& idx)
{
    QString baseFolder {"Lectures\\"};

    if (nesting == 3)
        baseFolder.append("Subthemes");
    else
        baseFolder.append("Themes");

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString prefix("file:///");
    path.remove(0, prefix.size());
    QString fileName(path.right(path.size() - path.lastIndexOf('/') - 1));
    QString newPath(baseFolder + '\\' + fileName);
    QFile::copy(path, newPath);

    if (nesting == 3)
        saveSubthemeLectureFile(newPath, idx);
    else
        saveThemeLectureFile(newPath, idx);
}

bool LecturesManager::literListFileExist(const int& idx)
{
    qDebug() << "literListFileExist";

    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->literPath.isEmpty())
        return false;
    return true;
}

QString LecturesManager::getLiterListFileContent(const int& idx)
{
    qDebug() << "getLiterListFileContent";

    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->literPath.isEmpty())
        return QString();

    QFile file(fileIter->literPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->literPath << file.errorString();
        return QString();
    }

    QTextStream textStream(&file);
    QString fileContent = textStream.readAll().trimmed();
    file.close();
    return fileContent;
}

void LecturesManager::createLiterListFile(const int& idx)
{
    qDebug() << "createLiterListFile";

    const QString fileExtension {"qppt"};
    QString baseFolder {"Lectures\\Literatures"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString filePath(baseFolder + QDir::separator() + QString::number(idx) + "." + fileExtension);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to create file " << filePath << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << filePath;
    saveLiterListFile(filePath, idx);
    file.close();
}

void LecturesManager::saveLiterListFileContent(const QString& text, const int& idx)
{
    qDebug() << "saveLiterListFileContent for id: " << idx;
    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->literPath.isEmpty())
        return;

    QFile file(fileIter->literPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->literPath << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << text;
    file.close();
}

void LecturesManager::copyLiterListLectureFile(QString path, const int& idx)
{
    QString baseFolder {"Lectures\\Literatures"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString prefix("file:///");
    path.remove(0, prefix.size());
    QString fileName(path.right(path.size() - path.lastIndexOf('/') - 1));
    QString newPath(baseFolder + '\\' + fileName);
    QFile::copy(path, newPath);

    saveLiterListFile(newPath, idx);
}

bool LecturesManager::educPlanFileExist(const int& idx)
{
    qDebug() << "getEducPlanFileContent";

    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->educPlanPath.isEmpty())
        return false;
    return true;
}

QString LecturesManager::getEducPlanFileContent(const int& idx)
{
    qDebug() << "getEducPlanFileContent";

    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->educPlanPath.isEmpty())
        return QString();

    QFile file(fileIter->educPlanPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->educPlanPath << file.errorString();
        return QString();
    }

    QTextStream textStream(&file);
    QString fileContent = textStream.readAll().trimmed();
    file.close();
    return fileContent;
}

void LecturesManager::createEducPlanFile(const int& idx)
{
    qDebug() << "createLiterListFile";

    const QString fileExtension {"qppt"};
    QString baseFolder {"Lectures\\EducPlans"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString filePath(baseFolder + QDir::separator() + QString::number(idx) + "." + fileExtension);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to create file " << filePath << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << filePath;
    saveEducPlanFile(filePath, idx);
    file.close();
}

void LecturesManager::saveEducPlanFileContent(const QString& text, const int& idx)
{
    qDebug() << "saveEducPlanFileContent for id: " << idx;
    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->educPlanPath.isEmpty())
        return;

    QFile file(fileIter->educPlanPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->educPlanPath << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << text;
    file.close();
}

void LecturesManager::copyEducPlanLectureFile(QString path, const int& idx)
{
    QString baseFolder {"Lectures\\Literatures"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString prefix("file:///");
    path.remove(0, prefix.size());
    QString fileName(path.right(path.size() - path.lastIndexOf('/') - 1));
    QString newPath(baseFolder + '\\' + fileName);
    QFile::copy(path, newPath);

    saveEducPlanFileContent(newPath, idx);
}

bool LecturesManager::educProgFileExist(const int& idx)
{
    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->educProgPath.isEmpty())
        return false;
    return true;
}

QString LecturesManager::getEducProgFileContent(const int& idx)
{
    qDebug() << "getEducProgFileContent";

    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->educProgPath.isEmpty())
        return QString();

    QFile file(fileIter->educProgPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->educProgPath << file.errorString();
        return QString();
    }

    QTextStream textStream(&file);
    QString fileContent = textStream.readAll().trimmed();
    file.close();
    return fileContent;
}

void LecturesManager::createEducProgFile(const int& idx)
{
    qDebug() << "createEducProgFile";

    const QString fileExtension {"qppt"};
    QString baseFolder {"Lectures\\EducProgram"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString filePath(baseFolder + QDir::separator() + QString::number(idx) + "." + fileExtension);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to create file " << filePath << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << filePath;
    saveEducProgFile(filePath, idx);
    file.close();
}

void LecturesManager::saveEducProgFileContent(const QString& text, const int& idx)
{
    qDebug() << "saveEducProgFileContent for id: " << idx;
    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](DisciplineTeach& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->educProgPath.isEmpty())
        return;

    QFile file(fileIter->educProgPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->educProgPath << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << text;
    file.close();
}

void LecturesManager::copyEducProgLectureFile(QString path, const int& idx)
{
    QString baseFolder {"Lectures\\Literatures"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString prefix("file:///");
    path.remove(0, prefix.size());
    QString fileName(path.right(path.size() - path.lastIndexOf('/') - 1));
    QString newPath(baseFolder + '\\' + fileName);
    QFile::copy(path, newPath);

    saveEducProgFileContent(newPath, idx);
}

void LecturesManager::saveThemeLectureFile(const QString& path, const int& idx)
{
    qDebug() << "saveThemeLectureFile";

    auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                 [&idx](ThemeLectureFile& th){ return th.themeId == idx; });

    if (mThemeLectureFiles.end() == fileIter)
    {
        ThemeLectureFile add;
        add.path = path;
        add.themeId = idx;
        mSaveManager->addThemeLectureFile(add);
        mThemeLectureFiles = mSaveManager->loadThemeLectureFile();
    }
    else
    {
        fileIter->path = path;
        mSaveManager->updThemeLectureFile(*fileIter);
    }
}

void LecturesManager::saveSubthemeLectureFile(const QString& path, const int& idx)
{
    qDebug() << "saveSubthemeLectureFile";

    auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                 [&idx](SubthemeLectureFile& th){ return th.subthemeId == idx; });

    if (mSubthemeLectureFiles.end() == fileIter)
    {
        SubthemeLectureFile add;
        add.path = path;
        add.subthemeId = idx;
        mSaveManager->addSubthemeLectureFile(add);
        mSubthemeLectureFiles = mSaveManager->loadSubthemeLectureFile();
    }
    else
    {
        fileIter->path = path;
        mSaveManager->updSubthemeLectureFile(*fileIter);
    }
}

void LecturesManager::saveLiterListFile(const QString& path, const int& idx)
{
    qDebug() << "saveLiterListFile";

    auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                             [&idx](DisciplineTeach& d){ return d.id == idx; });

    if (mDisciplines.end() == disc)
    {
        qDebug() << "Cannot find discipline with Id " << idx;
        return;
    }

    disc->literPath = path;
    qDebug() << "New liter path: " << disc->literPath;
    mSaveManager->updDiscipline(*disc);
}

void LecturesManager::saveEducPlanFile(const QString& path, const int& idx)
{
    qDebug() << "saveEducPlanFile";

    auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                             [&idx](DisciplineTeach& d){ return d.id == idx; });

    if (mDisciplines.end() == disc)
    {
        qDebug() << "Cannot find discipline with Id " << idx;
        return;
    }

    disc->educPlanPath = path;
    qDebug() << "New education plan path: " << disc->educPlanPath;
    mSaveManager->updDiscipline(*disc);
}

void LecturesManager::saveEducProgFile(const QString& path, const int& idx)
{
    qDebug() << "saveEducProgFile";

    auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                             [&idx](DisciplineTeach& d){ return d.id == idx; });

    if (mDisciplines.end() == disc)
    {
        qDebug() << "Cannot find discipline with Id " << idx;
        return;
    }

    disc->educProgPath = path;
    qDebug() << "New education program path: " << disc->educProgPath;
    mSaveManager->updDiscipline(*disc);
}
