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
#include "TreeItem.h"
#include <QFile>
#include <QDir>

/*static*/ QQmlApplicationEngine *LecturesManager::m_qmlEngine = nullptr;
/*static*/ SaveManager::Ptr LecturesManager::mSaveManager;

LecturesManager::LecturesManager(QObject* parent)
    : ManagerInterface(parent)
{
    mDisciplines = mSaveManager->loadDiscipline();
    mChapters    = mSaveManager->loadChapters();
    mThemes      = mSaveManager->loadTheme();
    mSubtheme    = mSaveManager->loadSubtheme();

    mThemeLectureFiles = mSaveManager->loadThemeLectureFile();
    mSubthemeLectureFiles = mSaveManager->loadSubthemeLectureFile();

    mTeacherEmail = mSaveManager->loadTeacherMail();

    initLabsTree();
}

/*static*/ QObject* LecturesManager::managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    LecturesManager *manager = new LecturesManager();
    return manager;
}

QList<QObject*> LecturesManager::labsTree()
{
    if (m_labsTree.size() <= 0)
    {
        TreeItem * d_item =  new TreeItem("Додати дисципліну", -1, 0);
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
                                 [&idx](Discipline& disc){ return disc.id == idx; });
    if (mDisciplines.end() == discIter)
    {
        qWarning() << "Cannot update discipline: " << name;
        return;
    }

    discIter->name = name;

    BaseItem * edit = new Discipline(discIter->id, discIter->name, discIter->literPath, discIter->educPlanPath, discIter->educProgPath);

    if (!edit)
    {
        qDebug() << "Cannot create instance of Discipline";
        return;
    }

    mSaveManager->editItem(edit, SaveManager::TYPE_DISCIPLINE);

    if (edit)
        delete edit;

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


    BaseItem * edit = new Chapter(chapIter->id, chapIter->name, chapIter->disciplineId);

    if (!edit)
    {
        qDebug() << "Cannot create instance of Chapter";
        return;
    }

    mSaveManager->editItem(edit, SaveManager::TYPE_CHAPTER);

    if (edit)
        delete edit;

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

    BaseItem * edit = new Theme(themIter->id, themIter->name, themIter->chapterId);

    if (!edit)
    {
        qDebug() << "Cannot create instance of Theme";
        return;
    }

    mSaveManager->editItem(edit, SaveManager::TYPE_THEME);

    if (edit)
        delete edit;

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

    BaseItem * edit = new Subtheme(subIter->id, subIter->name, subIter->themeId);

    if (!edit)
    {
        qDebug() << "Cannot create instance of Subtheme";
        return;
    }

    mSaveManager->editItem(edit, SaveManager::TYPE_SUBTHEME);

    if (edit)
        delete edit;

    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::removeDiscipline(const int& idx)
{
    qDebug() << "removeDiscipline: " << idx;

    mSaveManager->deleteItem(idx, SaveManager::TYPE_DISCIPLINE);
    mDisciplines.clear();
    mDisciplines = mSaveManager->loadDiscipline();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::removeChapter(const int& idx)
{
    qDebug() << "removeChapter: " << idx;

    mSaveManager->deleteItem(idx, SaveManager::TYPE_CHAPTER);
    mChapters.clear();
    mChapters = mSaveManager->loadChapters();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::removeTheme(const int& idx)
{
    qDebug() << "removeTheme: " << idx;

    mSaveManager->deleteItem(idx, SaveManager::TYPE_THEME);
    mThemes.clear();
    mThemes = mSaveManager->loadTheme();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::removeSubtheme(const int& idx)
{
    qDebug() << "removeSubtheme: " << idx;

    mSaveManager->deleteItem(idx, SaveManager::TYPE_SUBTHEME);
    mSubtheme.clear();
    mSubtheme = mSaveManager->loadSubtheme();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::removeFile(const LecturesManager::FileType& type, const int& idx, const int& nesting)
{
    qDebug() << "removeFile: " << idx;
    switch(type)
    {
    case LectureFile: return removeLectureFile(nesting, idx);
    case LiteratureListFile: return removeDisciplineFiles(idx, LiteratureListFile);
    case EducationPlanFile: return removeDisciplineFiles(idx, EducationPlanFile);
    case EducationProgramFile: return removeDisciplineFiles(idx, EducationProgramFile);
    default: break;
    }
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

    int id = 0;

    if (-1 == idx)
        id = 1;
    else
        id = idx + 1;

    BaseItem * add = new Discipline(id, name, "", "", "");

    if (!add)
    {
        qDebug() << "Cannot create instance of Discipline";
        return;
    }

    mSaveManager->insertItem(add, SaveManager::TYPE_DISCIPLINE);

    if (add)
        delete add;

    mDisciplines.clear();
    mDisciplines = mSaveManager->loadDiscipline();
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

    BaseItem * add = new Chapter(idx + 1, name, idx);

    if (!add)
    {
        qDebug() << "Cannot create instance of Chapter";
        return;
    }

    mSaveManager->insertItem(add, SaveManager::TYPE_CHAPTER);

    if (add)
        delete add;

    mChapters.clear();
    mChapters = mSaveManager->loadChapters();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::appendChapter(const QString& name, const int& idx)
{
    qDebug() << "appendChapter: " << idx << " " << name;

    BaseItem * add = new Chapter(0, name, idx);

    if (!add)
    {
        qDebug() << "Cannot create instance of Chapter";
        return;
    }

    mSaveManager->appendItem(add, SaveManager::TYPE_CHAPTER);

    if (add)
        delete add;

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

    BaseItem * add = new Theme(idx + 1, name, chaptIdx);

    if (!add)
    {
        qDebug() << "Cannot create instance of Theme";
        return;
    }

    mSaveManager->insertItem(add, SaveManager::TYPE_THEME);

    if (add)
        delete add;

    mThemes.clear();
    mThemes = mSaveManager->loadTheme();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::appendTheme(const QString& name, const int& idx)
{
    qDebug() << "appendTheme: " << idx << " " << name;

    BaseItem * add = new Theme(0, name, idx);

    if (!add)
    {
        qDebug() << "Cannot create instance of Theme";
        return;
    }

    mSaveManager->appendItem(add, SaveManager::TYPE_THEME);

    if (add)
        delete add;

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

    BaseItem * add = new Subtheme(idx + 1, name, themeIdx);

    if (!add)
    {
        qDebug() << "Cannot create instance of Subtheme";
        return;
    }

    mSaveManager->insertItem(add, SaveManager::TYPE_SUBTHEME);

    if (add)
        delete add;

    mSubtheme.clear();
    mSubtheme = mSaveManager->loadSubtheme();
    initLabsTree();
    emit labsTreeChanged();
}

void LecturesManager::appendSubtheme(const QString& name, const int& idx)
{
    qDebug() << "appendSubtheme: " << idx << " " << name;

    BaseItem * add = new Subtheme(0, name, idx);

    if (!add)
    {
        qDebug() << "Cannot create instance of Subtheme";
        return;
    }

    mSaveManager->appendItem(add, SaveManager::TYPE_SUBTHEME);

    if (add)
        delete add;

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

void LecturesManager::removeLectureFile(const int& nesting, const int& idx)
{
    qDebug() << "removeLectureFile";

    if (nesting == 3)
    {
        auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                     [&idx](SubthemeLectureFile& file){ return file.subthemeId == idx; });

        if (mSubthemeLectureFiles.end() == fileIter)
        {
            qDebug() << "Unable to delete file";
            return;
        }

        mSaveManager->deleteItem(fileIter->id, SaveManager::TYPE_SUBTHEME_LECTURE);
        QFile::remove(fileIter->path);
        mSubthemeLectureFiles = mSaveManager->loadSubthemeLectureFile();
    }
    else if (nesting == 2)
    {
        auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                     [&idx](ThemeLectureFile& file){ return file.themeId == idx; });

        if (mThemeLectureFiles.end() == fileIter)
        {
            qDebug() << "Unable to delete file";
            return;
        }

        mSaveManager->deleteItem(fileIter->id, SaveManager::TYPE_THEME_LECTURE);
        QFile::remove(fileIter->path);
        mThemeLectureFiles = mSaveManager->loadThemeLectureFile();
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

        QFile file(fileIter->path);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Unable to open file " << fileIter->path << file.errorString();
            return false;
        }

        file.close();
        return true;
    }
    else if (nesting == 2)
    {
        auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                     [&idx](ThemeLectureFile& file){ return file.themeId == idx; });

        if (mThemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return false;

        QFile file(fileIter->path);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Unable to open file " << fileIter->path << file.errorString();
            return false;
        }

        file.close();
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

    qDebug() << "file path: " << path;

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

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

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
    emit fileContentChanged();
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
                                 [&idx](Discipline& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->literPath.isEmpty())
        return false;

    QFile file(fileIter->literPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->literPath << file.errorString();
        return false;
    }
    file.close();
    return true;
}

QString LecturesManager::getLiterListFileContent(const int& idx)
{
    qDebug() << "getLiterListFileContent";

    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](Discipline& file){ return file.id == idx; });

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
                                 [&idx](Discipline& file){ return file.id == idx; });

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

void LecturesManager::copyLiterListFile(QString path, const int& idx)
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
                                 [&idx](Discipline& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->educPlanPath.isEmpty())
        return false;

    QFile file(fileIter->educPlanPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->educPlanPath << file.errorString();
        return false;
    }
    file.close();
    return true;
}

QString LecturesManager::getEducPlanFileContent(const int& idx)
{
    qDebug() << "getEducPlanFileContent";

    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](Discipline& file){ return file.id == idx; });

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
                                 [&idx](Discipline& file){ return file.id == idx; });

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

void LecturesManager::copyEducPlanFile(QString path, const int& idx)
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
                                 [&idx](Discipline& file){ return file.id == idx; });

    if (mDisciplines.end() == fileIter || fileIter->educProgPath.isEmpty())
        return false;

    QFile file(fileIter->educProgPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << fileIter->educProgPath << file.errorString();
        return false;
    }
    file.close();
    return true;
}

QString LecturesManager::getEducProgFileContent(const int& idx)
{
    qDebug() << "getEducProgFileContent";

    auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](Discipline& file){ return file.id == idx; });

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
                                 [&idx](Discipline& file){ return file.id == idx; });

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

void LecturesManager::copyEducProgFile(QString path, const int& idx)
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
        BaseItem * add = new ThemeLectureFile(0, idx, path);

        if (!add)
        {
            qDebug() << "Cannot create instance of ThemeLectureFile";
            return;
        }

        mSaveManager->appendItem(add, SaveManager::TYPE_THEME_LECTURE);

        if (add)
            delete add;

        mThemeLectureFiles = mSaveManager->loadThemeLectureFile();
    }
    else
    {
        fileIter->path = path;

        BaseItem * edit = new ThemeLectureFile(fileIter->id, fileIter->themeId, fileIter->path);

        if (!edit)
        {
            qDebug() << "Cannot create instance of ThemeLectureFile";
            return;
        }

        mSaveManager->editItem(edit, SaveManager::TYPE_THEME_LECTURE);

        if (edit)
            delete edit;
    }
}

void LecturesManager::saveSubthemeLectureFile(const QString& path, const int& idx)
{
    qDebug() << "saveSubthemeLectureFile";

    auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                 [&idx](SubthemeLectureFile& th){ return th.subthemeId == idx; });

    if (mSubthemeLectureFiles.end() == fileIter)
    {
        BaseItem * add = new SubthemeLectureFile(0, idx, path);

        if (!add)
        {
            qDebug() << "Cannot create instance of SubthemeLectureFile";
            return;
        }

        mSaveManager->appendItem(add, SaveManager::TYPE_SUBTHEME_LECTURE);

        if (add)
            delete add;

        mSubthemeLectureFiles = mSaveManager->loadSubthemeLectureFile();
    }
    else
    {
        fileIter->path = path;

        BaseItem * edit = new SubthemeLectureFile(fileIter->id, fileIter->subthemeId, fileIter->path);

        if (!edit)
        {
            qDebug() << "Cannot create instance of SubthemeLectureFile";
            return;
        }

        mSaveManager->editItem(edit, SaveManager::TYPE_SUBTHEME_LECTURE);

        if (edit)
            delete edit;
    }
}

void LecturesManager::saveLiterListFile(const QString& path, const int& idx)
{
    qDebug() << "saveLiterListFile";

    auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                             [&idx](Discipline& d){ return d.id == idx; });

    if (mDisciplines.end() == disc)
    {
        qDebug() << "Cannot find discipline with Id " << idx;
        return;
    }

    disc->literPath = path;

    BaseItem * edit = new Discipline(disc->id, disc->name, disc->literPath, disc->educPlanPath, disc->educProgPath);

    if (!edit)
    {
        qDebug() << "Cannot create instance of Discipline";
        return;
    }

    mSaveManager->editItem(edit, SaveManager::TYPE_DISCIPLINE);

    if (edit)
        delete edit;
}

void LecturesManager::saveEducPlanFile(const QString& path, const int& idx)
{
    qDebug() << "saveEducPlanFile";

    auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                             [&idx](Discipline& d){ return d.id == idx; });

    if (mDisciplines.end() == disc)
    {
        qDebug() << "Cannot find discipline with Id " << idx;
        return;
    }

    disc->educPlanPath = path;

    BaseItem * edit = new Discipline(disc->id, disc->name, disc->literPath, disc->educPlanPath, disc->educProgPath);

    if (!edit)
    {
        qDebug() << "Cannot create instance of Discipline";
        return;
    }

    mSaveManager->editItem(edit, SaveManager::TYPE_DISCIPLINE);

    if (edit)
        delete edit;
}

void LecturesManager::saveEducProgFile(const QString& path, const int& idx)
{
    qDebug() << "saveEducProgFile";

    auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                             [&idx](Discipline& d){ return d.id == idx; });

    if (mDisciplines.end() == disc)
    {
        qDebug() << "Cannot find discipline with Id " << idx;
        return;
    }

    disc->educProgPath = path;

    BaseItem * edit = new Discipline(disc->id, disc->name, disc->literPath, disc->educPlanPath, disc->educProgPath);

    if (!edit)
    {
        qDebug() << "Cannot create instance of Discipline";
        return;
    }

    mSaveManager->editItem(edit, SaveManager::TYPE_DISCIPLINE);

    if (edit)
        delete edit;
}

void LecturesManager::removeDisciplineFiles(const int& idx, const LecturesManager::FileType& type)
{
    qDebug() << "removeLiteratureListFile";

    auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                             [&idx](Discipline& d){ return d.id == idx; });

    if (mDisciplines.end() == disc)
    {
        qDebug() << "Cannot remove literature list file";
        return;
    }

    switch(type)
    {
    case LiteratureListFile: disc->literPath = ""; break;
    case EducationPlanFile: disc->educPlanPath = ""; break;
    case EducationProgramFile: disc->educProgPath = ""; break;
    }

    BaseItem * edit = new Discipline(disc->id, disc->name, disc->literPath, disc->educPlanPath, disc->educProgPath);

    if (!edit)
    {
        qDebug() << "Cannot create instance of Discipline";
        return;
    }

    mSaveManager->editItem(edit, SaveManager::TYPE_DISCIPLINE);

    if (edit)
        delete edit;
}

QString LecturesManager::teacherEmail() const
{
    return mTeacherEmail;
}

void LecturesManager::setTeacherEmail(const QString& email)
{
    qDebug() << "setTeacherEmail: " << email;
    mTeacherEmail = email;
    mSaveManager->updTeacherMail(email);
    emit teacherEmailChanged();
}

void LecturesManager::saveStudentCourses(QString courseName, int groupId)
{
    auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                 [&courseName](Discipline& d){ return !d.name.compare(courseName); });

    if (mDisciplines.end() == disc)
    {
        qDebug() << "cannot find discipline with name " << courseName;
        return;
    }

    mSaveManager->updStudentsCourses(disc->id, groupId);
}

void LecturesManager::saveTeacherEmail(const QString& email)
{
    qDebug() << "saveTeacherEmail: " << email;
    mTeacherEmail = email;
    mSaveManager->updTeacherMail(email);
    emit teacherEmailChanged();
}

QStringList LecturesManager::getCourses()
{
    QStringList result;

    for(auto& it: mDisciplines)
        result << it.name;

    return result;
}
