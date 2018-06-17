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
#include "TreeItem.h"

#include <QGuiApplication>
#include <QDebug>
#include <QFile>
#include <QDir>

namespace {
    const QString fileExtension {"qppt"};

    SaveManager::ItemType cast(LecturesManager::ItemType type)
    {
        switch (type)
        {
        case LecturesManager::DISCIPLINE_ITEM: return SaveManager::TYPE_DISCIPLINE;
        case LecturesManager::CHAPTER_ITEM: return SaveManager::TYPE_CHAPTER;
        case LecturesManager::THEME_ITEM: return SaveManager::TYPE_THEME;
        case LecturesManager::SUBTHEME_ITEM: return SaveManager::TYPE_SUBTHEME;
        default: return SaveManager::TYPE_UNKNOWN;
        }
    }

    SaveManager::ItemType cast(LecturesManager::FileType type)
    {
        switch (type)
        {
        case LecturesManager::ThemeLectureFileType: return SaveManager::TYPE_THEME_LECTURE;
        case LecturesManager::SubthemeLectureFileType: return SaveManager::TYPE_SUBTHEME_LECTURE;
        case LecturesManager::LiteratureListFileType: return SaveManager::TYPE_DISCIPLINE;
        case LecturesManager::EducationPlanFileType: return SaveManager::TYPE_DISCIPLINE;
        case LecturesManager::EducationProgramFileType: return SaveManager::TYPE_DISCIPLINE;
        default: return SaveManager::TYPE_UNKNOWN;
        }
    }    
} // anonymous

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

    initLecturesTree();
}

/*static*/ QObject* LecturesManager::managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    LecturesManager *manager = new LecturesManager();
    return manager;
}

void LecturesManager::insertItem(const QString& name, const int& idx, ItemType type)
{
    qDebug() << "insertItem: id:" << idx << ", name: " << name;

    if (!mSaveManager)
    {
        qWarning() << "Cannot insert item due to invalid save manager";
        return;
    }

    BaseItem * add = nullptr;

    switch (type)
    {
    case DISCIPLINE_ITEM:
    {
        if (-1 != idx)
        {
            auto it = mDisciplines.end();
            --it;

            while (mDisciplines.begin() <= it)
            {
                if (it->id <= idx)
                    break;

                int old = it->id;
                mSaveManager->updDisciplineIdx(old, (it->id = it->id + 1));
                --it;
            }
        }

        add = new Discipline(-1 == idx ? 1 : idx + 1, name, "", "", "");

        if (!add)
        {
            qDebug() << "Cannot create instance of Discipline";
            return;
        }

        mSaveManager->insertItem(add, cast(type));
        mDisciplines.clear();
        mDisciplines = mSaveManager->loadDiscipline();
    }
        break;
    case CHAPTER_ITEM:
    {
        auto it = mChapters.end();
        --it;

        int discIdx = -1;

        while (mChapters.begin() <= it)
        {
            if (it->id == idx)
                discIdx = it->disciplineId;

            if (it->id <= idx)
                break;

            int old = it->id;
            mSaveManager->updChapterIdx(old, (it->id = it->id + 1));
            --it;
        }

        if (discIdx == -1)
        {
            qWarning() << "Cannot find chapter with idx: " << idx;
            return;
        }

        add = new Chapter(idx + 1, name, idx);

        if (!add)
        {
            qDebug() << "Cannot create instance of Chapter";
            return;
        }

        mSaveManager->insertItem(add, cast(type));
        mChapters.clear();
        mChapters = mSaveManager->loadChapters();
    }
        break;
    case THEME_ITEM:
    {
        auto it = mThemes.end();
        --it;

        int chaptIdx = -1;

        while (mThemes.begin() <= it)
        {
            if (it->id == idx)
                chaptIdx = it->chapterId;

            if (it->id <= idx)
                break;

            int old = it->id;
            mSaveManager->updThemeIdx(old, (it->id = it->id + 1));
            --it;
        }

        if (chaptIdx == -1)
        {
            qWarning() << "Cannot find theme with idx: " << idx;
            return;
        }

        add = new Theme(idx + 1, name, chaptIdx);

        if (!add)
        {
            qDebug() << "Cannot create instance of Theme";
            return;
        }

        mSaveManager->insertItem(add, cast(type));
        mThemes.clear();
        mThemes = mSaveManager->loadTheme();
    }
        break;
    case SUBTHEME_ITEM:
    {
        auto it = mSubtheme.end();
        --it;

        int themeIdx = -1;

        while (mSubtheme.begin() <= it)
        {
            if (it->id == idx)
                themeIdx = it->themeId;

            if (it->id <= idx)
                break;

            int old = it->id;
            mSaveManager->updSubthemeIdx(old, (it->id = it->id + 1));
            --it;
        }

        if (themeIdx == -1)
        {
            qWarning() << "Cannot find subtheme with idx: " << idx;
            return;
        }

        add = new Subtheme(idx + 1, name, themeIdx);

        if (!add)
        {
            qDebug() << "Cannot create instance of Subtheme";
            return;
        }

        mSaveManager->insertItem(add, cast(type));
        mSubtheme.clear();
        mSubtheme = mSaveManager->loadSubtheme();
    }
        break;
    }

    if (add)
        delete add;

    initLecturesTree();
}

void LecturesManager::appendItem(const QString& name, const int& idx, ItemType type)
{
    qDebug() << "appendItem: id: " << idx << ", name: " << name;

    if (!mSaveManager)
    {
        qWarning() << "Cannot append item due to invalid save manager";
        return;
    }

    BaseItem * add = nullptr;

    switch (type)
    {
    case CHAPTER_ITEM:
    {
        add = new Chapter(0, name, idx);

        if (!add)
        {
            qDebug() << "Cannot create instance of Chapter";
            return;
        }

        mSaveManager->appendItem(add, cast(type));
        mChapters.clear();
        mChapters = mSaveManager->loadChapters();
    }
        break;
    case THEME_ITEM:
    {
        add = new Theme(0, name, idx);

        if (!add)
        {
            qDebug() << "Cannot create instance of Theme";
            return;
        }

        mSaveManager->appendItem(add, cast(type));
        mThemes.clear();
        mThemes = mSaveManager->loadTheme();
    }
        break;
    case SUBTHEME_ITEM:
    {
        add = new Subtheme(0, name, idx);

        if (!add)
        {
            qDebug() << "Cannot create instance of Subtheme";
            return;
        }

        mSaveManager->appendItem(add, cast(type));
        mSubtheme.clear();
        mSubtheme = mSaveManager->loadSubtheme();
    }
        break;
    }

    if (add)
        delete add;

    initLecturesTree();
}

void LecturesManager::updateItem(const QString& name, const int& idx, ItemType type)
{
    qDebug() << "updateItem: id: " << idx << ", name: " << name;

    if (!mSaveManager)
    {
        qWarning() << "Cannot remove item due to invalid save manager";
        return;
    }

    BaseItem * edit = nullptr;

    switch (type)
    {
    case DISCIPLINE_ITEM:
    {
        auto discIter = std::find_if(mDisciplines.begin(), mDisciplines.end(), [&idx](Discipline& disc){ return disc.id == idx; });

        if (mDisciplines.end() == discIter)
        {
            qWarning() << "Cannot update discipline: " << name;
            return;
        }

        edit = new Discipline(discIter->id, name, discIter->literPath, discIter->educPlanPath, discIter->educProgPath);

        if (!edit)
        {
            qDebug() << "Cannot create instance of Discipline";
            return;
        }

        discIter->name = name;
    }
        break;
    case CHAPTER_ITEM:
    {
        auto chapIter = std::find_if(mChapters.begin(), mChapters.end(), [&idx](Chapter& ch){ return ch.id == idx; });

        if (mChapters.end() == chapIter)
        {
            qWarning() << "Cannot update chapter: " << name;
            return;
        }

        edit = new Chapter(chapIter->id, chapIter->name, chapIter->disciplineId);

        if (!edit)
        {
            qDebug() << "Cannot create instance of Chapter";
            return;
        }

        chapIter->name = name;
    }
        break;
    case THEME_ITEM:
    {
        auto themIter = std::find_if(mThemes.begin(), mThemes.end(), [&idx](Theme& th){ return th.id == idx; });

        if (mThemes.end() == themIter)
        {
            qWarning() << "Cannot update theme: " << name;
            return;
        }

        edit = new Theme(themIter->id, themIter->name, themIter->chapterId);

        if (!edit)
        {
            qDebug() << "Cannot create instance of Theme";
            return;
        }

        themIter->name = name;
    }
        break;
    case SUBTHEME_ITEM:
    {
        auto subIter = std::find_if(mSubtheme.begin(), mSubtheme.end(),
                                    [&idx](Subtheme& sub){ return sub.id == idx; });
        if (mSubtheme.end() == subIter)
        {
            qWarning() << "Cannot update subtheme: " << name;
            return;
        }

        edit = new Subtheme(subIter->id, subIter->name, subIter->themeId);

        if (!edit)
        {
            qDebug() << "Cannot create instance of Subtheme";
            return;
        }

        subIter->name = name;
    }
        break;
    }

    mSaveManager->editItem(edit, cast(type));

    if (edit)
        delete edit;

    initLecturesTree();
}

void LecturesManager::removeItem(const int& idx, ItemType type)
{
    qDebug() << "removeItem: id: " << idx;

    if (!mSaveManager)
    {
        qWarning() << "Cannot remove item due to invalid save manager";
        return;
    }

    mSaveManager->deleteItem(idx, cast(type));

    switch (type)
    {
    case DISCIPLINE_ITEM:
        removeFile(LiteratureListFileType, idx);
        removeFile(EducationPlanFileType, idx);
        removeFile(EducationProgramFileType, idx);
        mDisciplines.clear();
        mDisciplines = mSaveManager->loadDiscipline();

        for (auto it: mChapters)
        {
            if (it.disciplineId != idx)
                continue;

            removeThemeFiles(it.id);
        }
        if (mDisciplines.isEmpty())
            QDir(QGuiApplication::applicationDirPath() + QDir::separator() + "\\Lectures").removeRecursively();
        break;
    case CHAPTER_ITEM:
        mChapters.clear();
        mChapters = mSaveManager->loadChapters();
        removeThemeFiles(idx);
        break;
    case THEME_ITEM:
        removeFile(ThemeLectureFileType, idx);
        mThemes.clear();
        mThemes = mSaveManager->loadTheme();
        removeSubthemeFiles(idx);
        break;
    case SUBTHEME_ITEM:
        removeFile(SubthemeLectureFileType, idx);
        mSubtheme.clear();
        mSubtheme = mSaveManager->loadSubtheme();
        break;
    }
    initLecturesTree();
}

void LecturesManager::removeFile(const LecturesManager::FileType& type, const int& idx)
{
    qDebug() << "removeFile: " << idx;

    if (!mSaveManager)
    {
        qWarning() << "Cannot insert item due to invalid save manager";
        return;
    }

    switch(type)
    {
    case SubthemeLectureFileType:
    {
        auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                     [&idx](SubthemeLectureFile& file){ return file.subthemeId == idx; });

        if (mSubthemeLectureFiles.end() == fileIter)
        {
            qDebug() << "Unable to delete file: file not exists";
            return;
        }

        mSaveManager->deleteItem(fileIter->id, cast(type));

        if (QFile::remove(QGuiApplication::applicationDirPath() + QDir::separator() + fileIter->path))
            qWarning() << "Cannot remove file on file system: " << fileIter->path;

        mSubthemeLectureFiles = mSaveManager->loadSubthemeLectureFile();
    }
    case ThemeLectureFileType:
    {
        auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                     [&idx](ThemeLectureFile& file){ return file.themeId == idx; });

        if (mThemeLectureFiles.end() == fileIter)
        {
            qDebug() << "Unable to delete file";
            return;
        }

        mSaveManager->deleteItem(fileIter->id, cast(type));

        if (QFile::remove(QGuiApplication::applicationDirPath() + QDir::separator() + fileIter->path))
            qWarning() << "Cannot remove file on file system: " << fileIter->path;

        mThemeLectureFiles = mSaveManager->loadThemeLectureFile();
    }
    case LiteratureListFileType:
    case EducationPlanFileType:
    case EducationProgramFileType:
    {
        auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                 [&idx](Discipline& d){ return d.id == idx; });

        if (mDisciplines.end() == disc)
        {
            qDebug() << "Cannot remove literature list file";
            return;
        }

        QString path;

        switch(type)
        {
        case LiteratureListFileType: path += disc->literPath; disc->literPath = ""; break;
        case EducationPlanFileType: path += disc->educPlanPath; disc->educPlanPath = ""; break;
        case EducationProgramFileType: path += disc->educProgPath; disc->educProgPath = ""; break;
        }

        if (QFile::remove(QGuiApplication::applicationDirPath() + QDir::separator() + path))
            qWarning() << "Cannot remove file on file system: " << path;

        BaseItem * edit = new Discipline(disc->id, disc->name, disc->literPath, disc->educPlanPath, disc->educProgPath);

        if (!edit)
        {
            qDebug() << "Cannot create instance of Discipline";
            return;
        }

        mSaveManager->editItem(edit, cast(type));

        if (edit)
            delete edit;
    }
    default: break;
    }
}

bool LecturesManager::fileExist(const LecturesManager::FileType& type, const int& idx)
{
    qDebug() << "fileExist";

    switch(type)
    {
    case SubthemeLectureFileType:
    {
        auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                     [&idx](SubthemeLectureFile& file){ return file.subthemeId == idx; });

        if (mSubthemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return false;

        if (!QFile::exists(QGuiApplication::applicationDirPath() + QDir::separator() + fileIter->path))
            return false;

        qDebug() << "fileExist: true";
        return true;
    }
        break;
    case ThemeLectureFileType:
    {
        auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                     [&idx](ThemeLectureFile& file){ return file.themeId == idx; });

        if (mThemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return false;

        if (!QFile::exists(QGuiApplication::applicationDirPath() + QDir::separator() + fileIter->path))
            return false;

        qDebug() << "fileExist: true";
        return true;
    }
    case LiteratureListFileType:
    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter || fileIter->literPath.isEmpty())
            return false;

        if (!QFile::exists(QGuiApplication::applicationDirPath() + QDir::separator() + fileIter->literPath))
            return false;

        qDebug() << "fileExist: true";
        return true;
    }
    case EducationPlanFileType:
    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter || fileIter->educPlanPath.isEmpty())
            return false;

        if (!QFile::exists(QGuiApplication::applicationDirPath() + QDir::separator() + fileIter->educPlanPath))
            return false;

        qDebug() << "fileExist: true";
        return true;
    }
    case EducationProgramFileType:
    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter || fileIter->educProgPath.isEmpty())
            return false;

        if (!QFile::exists(QGuiApplication::applicationDirPath() + QDir::separator() + fileIter->educProgPath))
            return false;

        qDebug() << "fileExist: true";
        return true;
    }
    default: return false;
    }
}

QString LecturesManager::getFileContent(const LecturesManager::FileType& type, const int& idx)
{
    qDebug() << "getFileContent: " << idx;

    QString path;

    switch(type)
    {
    case ThemeLectureFileType:
    {
        auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                     [&idx](ThemeLectureFile& file){ return file.themeId == idx; });

        if (mThemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return QString();

        path = fileIter->path;
    }
        break;
    case SubthemeLectureFileType:
    {
        auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                     [&idx](SubthemeLectureFile& file){ return file.subthemeId == idx; });

        if (mSubthemeLectureFiles.end() == fileIter || fileIter->path.isEmpty())
            return QString();

        path = fileIter->path;
    }
        break;
    case LiteratureListFileType:
    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter || fileIter->literPath.isEmpty())
            return QString();

        path = fileIter->literPath;
    }
        break;
    case EducationPlanFileType:
    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter || fileIter->educPlanPath.isEmpty())
            return QString();

        path = fileIter->educPlanPath;
    }
        break;
    case EducationProgramFileType:
    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter || fileIter->educProgPath.isEmpty())
            return QString();

        path = fileIter->educProgPath;
    }
        break;
    default: return QString();
    }

    QFile file(QGuiApplication::applicationDirPath() + QDir::separator() + path);

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

void LecturesManager::createFile(const int& idx, FileType type)
{
    qDebug() << "createFile " << idx;

    if (!mSaveManager)
    {
        qWarning() << "Cannot insert item due to invalid save manager";
        return;
    }

    QString baseFolder("Lectures\\");

    if (!QDir(QGuiApplication::applicationDirPath() + QDir::separator() + baseFolder).exists())
        QDir().mkdir(QGuiApplication::applicationDirPath() + QDir::separator() + baseFolder);

    switch (type)
    {
    case ThemeLectureFileType:
        baseFolder.append("Themes");
        break;
    case SubthemeLectureFileType:
        baseFolder.append("Subthemes");
        break;
    case LiteratureListFileType:
        baseFolder.append("Literatures");
        break;
    case EducationPlanFileType:
        baseFolder.append("EducPlans");
        break;
    case EducationProgramFileType:
        baseFolder.append("EducPrograms");
        break;
    }

    if (!QDir(QGuiApplication::applicationDirPath() + QDir::separator() + baseFolder).exists())
        QDir().mkdir(QGuiApplication::applicationDirPath() + QDir::separator() + baseFolder);

    QString filePath(baseFolder + QDir::separator() + QString::number(idx) + "." + fileExtension);
    QFile file(QGuiApplication::applicationDirPath() + QDir::separator() + filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to create file " << filePath << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << filePath;
    file.close();

    switch (type)
    {
    case ThemeLectureFileType:
        saveThemeLectureFile(filePath, idx);
        break;
    case SubthemeLectureFileType:
        saveSubthemeLectureFile(filePath, idx);
        break;
    case LiteratureListFileType:
    case EducationPlanFileType:
    case EducationProgramFileType:
        saveDisciplineFiles(filePath, idx, type);
        break;
    }
}

void LecturesManager::saveFileContent(const QString& text, const int& idx, FileType type)
{
    qDebug() << "saveFileContent for id: " << idx;

    QString path;

    switch (type)
    {
    case ThemeLectureFileType:
    {
        auto fileIter = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                     [&idx](ThemeLectureFile& file){ return file.themeId == idx; });

        if (mThemeLectureFiles.end() == fileIter)
        {
            qWarning() << "Cannot save file content: lecture file does not exist";
            return;
        }

        if (fileIter->path.isEmpty())
        {
            qWarning() << "Cannot save file content: file path is empty";
            return;
        }

        path = fileIter->path;
    }
        break;
    case SubthemeLectureFileType:
    {
        auto fileIter = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                                     [&idx](SubthemeLectureFile& file){ return file.subthemeId == idx; });

        if (mSubthemeLectureFiles.end() == fileIter)
        {
            qWarning() << "Cannot save file content: lecture file does not exist";
            return;
        }

        if (fileIter->path.isEmpty())
        {
            qWarning() << "Cannot save file content: file path is empty";
            return;
        }

        path = fileIter->path;
    }
        break;
    case LiteratureListFileType:
    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter)
        {
            qWarning() << "Cannot save file content: lecture file does not exist";
            return;
        }

        if (fileIter->literPath.isEmpty())
        {
            qWarning() << "Cannot save file content: file path is empty";
            return;
        }

        path = fileIter->literPath;
    }
        break;
    case EducationPlanFileType:
    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter)
        {
            qWarning() << "Cannot save file content: lecture file does not exist";
            return;
        }

        if (fileIter->educPlanPath.isEmpty())
        {
            qWarning() << "Cannot save file content: file path is empty";
            return;
        }

        path = fileIter->educPlanPath;
    }
        break;
    case EducationProgramFileType:
    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter)
        {
            qWarning() << "Cannot save file content: lecture file does not exist";
            return;
        }

        if (fileIter->educProgPath.isEmpty())
        {
            qWarning() << "Cannot save file content: file path is empty";
            return;
        }

        path = fileIter->educProgPath;
    }
        break;
    }

    QFile file(QGuiApplication::applicationDirPath() + QDir::separator() + path);

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

void LecturesManager::copyFile(QString path, const int& idx, FileType type)
{
    QString baseFolder {"Lectures\\"};

    if (!QDir(QGuiApplication::applicationDirPath() + QDir::separator() + baseFolder).exists())
        QDir().mkdir(QGuiApplication::applicationDirPath() + QDir::separator() + baseFolder);

    switch (type)
    {
    case ThemeLectureFileType:
        baseFolder.append("Themes");
        break;
    case SubthemeLectureFileType:
        baseFolder.append("Subthemes");
        break;
    case LiteratureListFileType:
        baseFolder.append("Literatures");
        break;
    case EducationPlanFileType:
        baseFolder.append("EducPlans");
        break;
    case EducationProgramFileType:
        baseFolder.append("EducProgram");
        break;
    }

    if (!QDir(QGuiApplication::applicationDirPath() + QDir::separator() + baseFolder).exists())
        QDir().mkdir(QGuiApplication::applicationDirPath() + QDir::separator() + baseFolder);

    QString prefix("file:///");
    path.remove(0, prefix.size());
    QString fileName(path.right(path.size() - path.lastIndexOf('/') - 1));
    QString newPath(baseFolder + '\\' + fileName);
    QFile::copy(QGuiApplication::applicationDirPath() + QDir::separator() + path, QGuiApplication::applicationDirPath() + QDir::separator() + newPath);

    switch (type)
    {
    case ThemeLectureFileType:
        saveThemeLectureFile(newPath, idx);
        break;
    case SubthemeLectureFileType:
        saveSubthemeLectureFile(newPath, idx);
        break;
    }
}

QString LecturesManager::teacherEmail() const
{
    return mTeacherEmail;
}

void LecturesManager::setTeacherEmail(const QString& email)
{
    qDebug() << "setTeacherEmail: " << email;

    if (!mSaveManager)
    {
        qWarning() << "Cannot remove item due to invalid save manager";
        return;
    }

    mTeacherEmail = email;
    mSaveManager->updTeacherMail(email);
    emit teacherEmailChanged();
}

void LecturesManager::saveStudentCourses(QString courseName, int groupId)
{
    if (!mSaveManager)
    {
        qWarning() << "Cannot remove item due to invalid save manager";
        return;
    }

    auto disc = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                             [&courseName](Discipline& d){ return !d.name.compare(courseName); });

    if (mDisciplines.end() == disc)
    {
        qDebug() << "Cannot find discipline with name " << courseName;
        return;
    }

    mSaveManager->updStudentsCourses(disc->id, groupId);
}

QStringList LecturesManager::getCourses()
{
    QStringList result;

    for(auto& it: mDisciplines)
        result << it.name;

    return result;
}

QList<QObject*> LecturesManager::lecturesTree()
{
    if (mLecturesTree.size() <= 0)
    {
        TreeItem * d_item =  new TreeItem("Додати дисципліну", -1, 0);
        mLecturesTree << d_item;
    }

    return mLecturesTree;
}

bool LecturesManager::isEmptyTree()
{
    return mLecturesTree.size() <= 0;
}

void LecturesManager::initLecturesTree()
{
    mLecturesTree.clear();

    for (auto disc : mDisciplines)
    {
        TreeItem * d_item = new TreeItem(disc.name, disc.id, 0);

        if (!d_item)
        {
            qWarning() << "Cannot create discipline tree item";
            continue;
        }

        for (auto chapt : mChapters)
        {
            if (chapt.disciplineId != disc.id)
                continue;

            TreeItem * c_item = new TreeItem(chapt.name, chapt.id, 1);

            if (!c_item)
            {
                qWarning() << "Cannot create chapter tree item";
                continue;
            }

            for (auto them : mThemes)
            {
                if (them.chapterId != chapt.id)
                    continue;

                TreeItem * t_item = new TreeItem(them.name, them.id, 2);

                if (!t_item)
                {
                    qWarning() << "Cannot create theme tree item";
                    continue;
                }

                for (auto sub : mSubtheme)
                {
                    if (sub.themeId != them.id)
                        continue;

                    TreeItem * s_item = new TreeItem(sub.name, sub.id, 3);

                    if (!s_item)
                    {
                        qWarning() << "Cannot create subtheme tree item";
                        continue;
                    }

                    t_item->addChild(s_item);
                }
                c_item->addChild(t_item);
            }
            d_item->addChild(c_item);
        }

        mLecturesTree << d_item;
    }

    emit lecturesTreeChanged();
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

void LecturesManager::saveDisciplineFiles(const QString& path, const int& idx, LecturesManager::FileType type)
{
    qDebug() << "saveDisciplineFiles";

    {
        auto fileIter = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                                     [&idx](Discipline& file){ return file.id == idx; });

        if (mDisciplines.end() == fileIter)
        {
            qWarning() << "Cannot save discipline file: discipline does not exist";
            return;
        }

        switch(type)
        {
        case EducationPlanFileType:
            fileIter->educPlanPath = path;
            break;
        case EducationProgramFileType:
            fileIter->educProgPath = path;
            break;
        case LiteratureListFileType:
            fileIter->literPath = path;
            break;
        }

        BaseItem * add = new Discipline(idx, fileIter->name, fileIter->literPath, fileIter->educPlanPath, fileIter->educProgPath);
        mSaveManager->editItem(add, SaveManager::TYPE_DISCIPLINE);

        if (add)
            delete add;
    }
}

void LecturesManager::removeSubthemeFiles(const int& idx)
{
    for (auto it: mSubtheme)
    {
        if (it.themeId != idx)
            continue;

        auto file = std::find_if(mSubthemeLectureFiles.begin(), mSubthemeLectureFiles.end(),
                     [it](SubthemeLectureFile& file) { return file.subthemeId == it.id; });

        if (mSubthemeLectureFiles.end() == file)
            continue;

        if (QFile::remove(file->path))
            qWarning() << "Cannot remove file on file system: " << file->path;
    }
}

void LecturesManager::removeThemeFiles(const int& idx)
{
    for (auto it: mThemes)
    {
        if (it.chapterId != idx)
            continue;

        auto file = std::find_if(mThemeLectureFiles.begin(), mThemeLectureFiles.end(),
                                 [it](ThemeLectureFile& file) { return file.themeId == it.id; });

        if (mThemeLectureFiles.end() == file)
            continue;

        if (QFile::remove(file->path))
            qWarning() << "Cannot remove file on file system: " << file->path;

        removeSubthemeFiles(it.id);
    }
}


