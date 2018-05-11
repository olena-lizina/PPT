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

/*
    TODO: If you want to set order for items - need to add additional parameter with order id.
*/

#include "LecturesManager.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QClipboard>
#include <QMimeData>
#include <QApplication>
#include <QPixmap>
#include <future>

/*static*/ QQmlApplicationEngine *LecturesManager::m_qmlEngine = nullptr;
/*static*/ SaveManager::Ptr LecturesManager::mSaveManager;

LecturesManager::LecturesManager(QObject* parent)
    : QObject(parent)
{
    loadAllLectures();
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

void LecturesManager::loadAllLectures()
{
    mDisciplines = mSaveManager->getLectureParts(SaveManager::Discipline);
    mParts = mSaveManager->getLectureParts(SaveManager::Part);
    mChapters = mSaveManager->getLectureParts(SaveManager::ChapterType);
    mThemes = mSaveManager->getLectureParts(SaveManager::ThemeType);
    mSubThemes = mSaveManager->getLectureParts(SaveManager::SubTheme);

    mDisciplines.sort([](const LecturePart& lhs, const LecturePart& rhs)
                { return lhs.getId() < rhs.getId(); });

    mParts.sort([](const LecturePart& lhs, const LecturePart& rhs)
                { return lhs.getId() < rhs.getId(); });

    mChapters.sort([](const LecturePart& lhs, const LecturePart& rhs)
                { return lhs.getId() < rhs.getId(); });

    mThemes.sort([](const LecturePart& lhs, const LecturePart& rhs)
                { return lhs.getId() < rhs.getId(); });

    mSubThemes.sort([](const LecturePart& lhs, const LecturePart& rhs)
                { return lhs.getId() < rhs.getId(); });
}

void LecturesManager::saveCurrentIdxs(const QString& name, const int& id, const LecturesManager::Type& type)
{
    switch(type)
    {
    case Disciplines: mDisciplinesIds[name] = id; break;
    case Parts: mPartsIds[name] = id; break;
    case Chapters: mChaptersIds[name] = id; break;
    case Themes: mThemesIds[name] = id; break;
    case SubThemes: mSubThemesIds[name] = id; break;
    }
}

void LecturesManager::createFile()
{
    const QString fileExtension {"qppt"};
    const QString baseFolder {"Lectures"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString filePath(baseFolder + QDir::separator() + QString::number(mSelectedPart.id)
                  + "_" + QString::number(mSelectedChapter.id)
                  + "_" + QString::number(mSelectedTheme.id)
                  + (hasSubThemes(mSelectedTheme.name) ? QString::number(mSelectedSubTheme.id) : "")
                  + "." + fileExtension);

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to create file " << filePath << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream << filePath;
    mSelectedFile = filePath;

    if (!hasSubThemes(mSelectedTheme.name))
        editItem(mSelectedTheme.name, Themes, filePath);
    else
        editItem(mSelectedSubTheme.name, SubThemes, filePath);

    file.close();
}

void LecturesManager::selectFile()
{
    const QString fileExtension {"qppt"};
    const QString baseFolder {"Lectures"};

    if (!QDir(baseFolder).exists())
        QDir().mkdir(baseFolder);

    QString filePath(baseFolder + QDir::separator() + QString::number(mSelectedPart.id)
                  + "_" + QString::number(mSelectedChapter.id)
                  + "_" + QString::number(mSelectedTheme.id)
                  + (hasSubThemes(mSelectedTheme.name) ? QString::number(mSelectedSubTheme.id) : "")
                  + "." + fileExtension);

    mSelectedFile = filePath;
}

void LecturesManager::saveFileContent(QString content)
{
    QFile file(mSelectedFile);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << mSelectedFile << file.errorString();
        return;
    }

    QTextStream textStream(&file);
    textStream<<content;

    file.close();
}

QString LecturesManager::getFileContent()
{
    QFile file(mSelectedFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file " << mSelectedFile << file.errorString();
        return QString();
    }

    QTextStream textStream(&file);
    QString fileContent = textStream.readAll().trimmed();
    file.close();
    return fileContent;
}

QStringList LecturesManager::getListModel(const LecturesManager::Type& type)
{
    QStringList result;

    switch(type)
    {
    case Disciplines:
        for (auto& it : mDisciplines)
            result << it.getName();
        break;
    case Parts:
        for (auto& it : mParts)
            if (mSelectedDisciplines.id == it.getParentId())
                result << it.getName();
        break;
    case Chapters:
        for (auto& it : mChapters)
            if (mSelectedPart.id == it.getParentId())
                result << it.getName();
        break;
    case Themes:
        for (auto& it : mThemes)
            if (mSelectedChapter.id == it.getParentId())
                result << it.getName();
        break;
    case SubThemes:
        for (auto& it : mSubThemes)
            if (mSelectedTheme.id == it.getParentId())
                result << it.getName();
        break;
    }

    return result;
}

bool LecturesManager::itemExists(const QString& name, const LecturesManager::Type& type)
{
    switch(type)
    {
    case Disciplines:
        return (mDisciplines.end() != std::find_if(mDisciplines.begin(), mDisciplines.end(),
        [&name](LecturePart& it){ return !it.getName().compare(name); }));
    case Parts:
    {
        int discId = mSelectedDisciplines.id;
        return (mParts.end() != std::find_if(mParts.begin(), mParts.end(),
                                             [&name, &discId](LecturePart& it)
        { return !it.getName().compare(name) && discId == it.getParentId(); }));
    }
    case Chapters:
    {
        int partId = mSelectedPart.id;
        return (mChapters.end() != std::find_if(mChapters.begin(), mChapters.end(),
                                                 [&name, &partId](LecturePart& it)
        { return !it.getName().compare(name) && it.getParentId() == partId; }));
    }
    case Themes:
    {
        int chaptId = mSelectedChapter.id;
        return (mThemes.end() != std::find_if(mThemes.begin(), mThemes.end(),
                                                 [&name, &chaptId](LecturePart& it)
        { return !it.getName().compare(name) && it.getParentId() == chaptId; }));
    }
    case SubThemes:
    {
        int themeId = mSelectedTheme.id;
        return (mSubThemes.end() != std::find_if(mSubThemes.begin(), mSubThemes.end(),
                                                 [&name, &themeId](LecturePart& it)
        { return !it.getName().compare(name) && it.getParentId() == themeId; }));
    }
    }
    return false;
}

void LecturesManager::addItem(const QString& name, const LecturesManager::Type& type, const QString& fileName)
{
    switch(type)
    {
    case Disciplines:
    {
        int id = 0;
        if (!mDisciplines.empty())
            id = mDisciplines.back().getId() + 1;
        LecturePart discipline(name, id, 0);
        mDisciplines.push_back(discipline);
        mSaveManager->saveLecturePart(discipline, SaveManager::Discipline);
    }
    break;
    case Parts:
    {
        int id = 0;
        if (!mParts.empty())
            id = mParts.back().getId() + 1;
        LecturePart part(name, id, mSelectedDisciplines.id);
        mParts.push_back(part);
        mSaveManager->saveLecturePart(part, SaveManager::Part);
    }
    break;
    case Chapters:
    {
        int id = 0;
        if (!mChapters.empty())
            id = mChapters.back().getId() + 1;
        LecturePart chapt(name, id, mSelectedPart.id);
        mChapters.push_back(chapt);
        mSaveManager->saveLecturePart(chapt, SaveManager::ChapterType);
    }
    break;
    case Themes:
    {
        int id = 0;
        if (!mThemes.empty())
            id = mThemes.back().getId() + 1;
        LecturePart theme(name, id, mSelectedChapter.id);
        if (!fileName.isEmpty())
            theme.setFileName(fileName);
        mThemes.push_back(theme);
        mSaveManager->saveLecturePart(theme, SaveManager::ThemeType);
    }
        break;
    case SubThemes:
    {
        int id = 0;
        if (!mSubThemes.empty())
            id = mSubThemes.back().getId() + 1;
        LecturePart subTheme(name, id, mSelectedTheme.id);
        if (!fileName.isEmpty())
            subTheme.setFileName(fileName);
        mSubThemes.push_back(subTheme);
        mSaveManager->saveLecturePart(subTheme, SaveManager::SubTheme);
    }
        break;
    }
}

void LecturesManager::editItem(const QString& name, const LecturesManager::Type& type, const QString& fileName)
{
    switch(type)
    {
    case Disciplines:
    {
        LecturePart discipline(name, mSelectedDisciplines.id, 0);
        int id = mSelectedDisciplines.id;
        auto it = std::find_if(mDisciplines.begin(), mDisciplines.end(),
                               [&id](LecturePart& item){ return item.getId() == id; });

        if (mDisciplines.end() == it)
            break;

        mSaveManager->updateLecturePart(*it, discipline, SaveManager::Discipline);
        it->setName(name);
    }
    break;
    case Parts:
    {
        LecturePart part(name, mSelectedPart.id, 0);
        int id = mSelectedPart.id;
        int parentId = mSelectedDisciplines.id;
        auto it = std::find_if(mParts.begin(), mParts.end(),
                               [&id, &parentId](LecturePart& item)
        { return item.getId() == id && parentId == item.getParentId(); });

        if (mParts.end() == it)
            break;

        mSaveManager->updateLecturePart(*it, part, SaveManager::Part);
        it->setName(name);
    }
    break;
    case Chapters:
    {
        LecturePart chapt(name, mSelectedChapter.id, mSelectedPart.id);
        int id = mSelectedChapter.id;
        int parentId = mSelectedPart.id;
        auto it = std::find_if(mChapters.begin(), mChapters.end(),
                               [&id, &parentId](LecturePart& item)
        { return id == item.getId() && parentId == item.getParentId(); });

        if (mChapters.end() == it)
            break;

        mSaveManager->updateLecturePart(*it, chapt, SaveManager::ChapterType);
        it->setName(name);
    }
    break;
    case Themes:
    {
        LecturePart theme(name, mSelectedTheme.id, mSelectedChapter.id);

        if (!fileName.isEmpty())
            theme.setFileName(fileName);

        int id = mSelectedTheme.id;
        int parentId = mSelectedChapter.id;
        auto it = std::find_if(mThemes.begin(), mThemes.end(),
                               [&id, &parentId](LecturePart& item)
        { return id == item.getId() && parentId == item.getParentId(); });

        if (mThemes.end() == it)
            break;

        mSaveManager->updateLecturePart(*it, theme, SaveManager::ThemeType);
        it->setName(name);

        if (!fileName.isEmpty())
            it->setFileName(fileName);
    }
    break;
    case SubThemes:
    {
        LecturePart subTheme(name, mSelectedSubTheme.id, mSelectedTheme.id);

        if (!fileName.isEmpty())
            subTheme.setFileName(fileName);

        int id = mSelectedSubTheme.id;
        int parentId = mSelectedTheme.id;
        auto it = std::find_if(mSubThemes.begin(), mSubThemes.end(),
                               [&id, &parentId](LecturePart& item)
        { return id == item.getId() && parentId == item.getParentId(); });

        if (mSubThemes.end() == it)
            break;

        mSaveManager->updateLecturePart(*it, subTheme, SaveManager::SubTheme);
        it->setName(name);

        if (!fileName.isEmpty())
            it->setFileName(fileName);
    }
    break;
    }
}

void LecturesManager::deleteItem(const LecturesManager::Type& type)
{
    switch(type)
    {
    case Disciplines:
    {
        int id = mSelectedDisciplines.id;
        mSaveManager->deleteLecturePart(id, 0, SaveManager::Discipline);
        mDisciplines.remove_if([&id](LecturePart it){ return it.getId() == id; });
        removeRelatedParts(id);
    }
    break;
    case Parts:
    {
        int id = mSelectedPart.id;
        int parentId = mSelectedDisciplines.id;
        mSaveManager->deleteLecturePart(id, parentId, SaveManager::Part);
        mParts.remove_if([&id, &parentId](LecturePart it){ return it.getId() == id && it.getParentId() == parentId; });
        removeRelatedChapters(id);
    }
    break;
    case Chapters:
    {
        int id = mSelectedChapter.id;
        int parentId = mSelectedPart.id;
        mSaveManager->deleteLecturePart(id, parentId, SaveManager::ChapterType);
        mChapters.remove_if([&id, &parentId](LecturePart it){ return it.getId() == id && it.getParentId() == parentId; });
        removeRelatedThemes(id);
    }
    break;
    case Themes:
    {
        int id = mSelectedTheme.id;
        int parentId = mSelectedChapter.id;
        mSaveManager->deleteLecturePart(id, parentId, SaveManager::ThemeType);
        mThemes.remove_if([&id, &parentId](LecturePart it){ return it.getId() == id && it.getParentId() == parentId; });
        removeRelatedSubThemes(id);
    }
    break;
    case SubThemes:
    {
        int id = mSelectedSubTheme.id;
        int parentId = mSelectedTheme.id;
        mSaveManager->deleteLecturePart(id, parentId, SaveManager::SubTheme);
        mSubThemes.remove_if([&id, &parentId](LecturePart it){ return it.getId() == id && it.getParentId() == parentId; });
    }
    break;
    }
}

void LecturesManager::removeRelatedSubThemes(const int& themeId)
{
    for (auto it : mSubThemes)
        if (it.getParentId() == themeId)
            mSaveManager->deleteLecturePartByParentId(themeId, SaveManager::SubTheme);

    mSubThemes.remove_if([&themeId](LecturePart item)
    { return item.getParentId() == themeId; });
}

void LecturesManager::removeRelatedThemes(const int& chapterId)
{
    for (auto it : mThemes)
    {
        if (it.getParentId() == chapterId)
        {
            removeRelatedSubThemes(it.getId());
            mSaveManager->deleteLecturePartByParentId(it.getId(), SaveManager::ThemeType);
        }
    }
    mThemes.remove_if([&chapterId](LecturePart item)
    { return item.getParentId() == chapterId; });
}

void LecturesManager::removeRelatedChapters(const int& partId)
{
    for (auto it : mChapters)
    {
        if (it.getParentId() == partId)
        {
            removeRelatedThemes(it.getId());
            mSaveManager->deleteLecturePartByParentId(it.getId(), SaveManager::ChapterType);
        }
    }
    mChapters.remove_if([&partId](LecturePart item)
    { return item.getParentId() == partId; });
}

void LecturesManager::removeRelatedParts(const int& disciplineId)
{
    for (auto it : mParts)
    {
        if (it.getParentId() == disciplineId)
        {
            removeRelatedChapters(it.getId());
            mSaveManager->deleteLecturePartByParentId(it.getId(), SaveManager::Part);
        }
    }
    mParts.remove_if([&disciplineId](LecturePart item)
    { return item.getParentId() == disciplineId; });
}

void LecturesManager::updateIndexes(const LecturesManager::Type& type)
{
    switch(type)
    {
    case Disciplines:
    {
        if (mDisciplinesIds.size() <= 0)
            break;

        std::list<LecturePart> toInsert;
        std::list<LecturePart> toInsertSub;

        auto disc = mDisciplines.begin();
        while (disc != mDisciplines.end())
        {
            if (mDisciplinesIds.contains(disc->getName()))
            {
                toInsert.push_back(LecturePart(disc->getName(), mDisciplinesIds[disc->getName()], 0));

                auto part = mParts.begin();
                while (part != mParts.end())
                {
                    if (part->getParentId() == disc->getId())
                    {
                        toInsertSub.push_back(LecturePart(part->getName(), part->getId(), mDisciplinesIds[disc->getName()]));
                        std::async(std::launch::async, &SaveManager::deleteLecturePart, mSaveManager, part->getId(), part->getParentId(), SaveManager::Part);
                        //mSaveManager->deleteLecturePart(part->getId(), part->getParentId(), SaveManager::Part);
                        mParts.erase(part++);
                        continue;
                    }
                    ++part;
                }

                std::async(std::launch::async, &SaveManager::deleteLecturePart, mSaveManager, disc->getId(), disc->getParentId(), SaveManager::Discipline);
                mDisciplines.erase(disc++);
                continue;
            }
            ++disc;
        }

        for (auto & them : toInsert)
        {
            std::async(std::launch::async, &SaveManager::saveLecturePart, mSaveManager, them, SaveManager::Discipline);
            mDisciplines.push_back(them);
        }

        for (auto & sub : toInsertSub)
        {
            std::async(std::launch::async, &SaveManager::saveLecturePart, mSaveManager, sub, SaveManager::Part);
            mParts.push_back(sub);
        }

        mDisciplines.sort([](const LecturePart& lhs, const LecturePart& rhs)
                    { return lhs.getId() < rhs.getId(); });
        mParts.sort([](const LecturePart& lhs, const LecturePart& rhs)
                    { return lhs.getId() < rhs.getId(); });
    }
    break;
    case Parts:
    {
        if (mPartsIds.size() <= 0)
            break;

//        int discId = mSelectedDisciplines.id;
//        std::list<LecturePart> selectedParts;
//        std::copy_if(mParts.begin(), mParts.end(), std::back_inserter(selectedParts),
//                     [&discId](LecturePart part){ return part.getParentId() == discId; });

//        std::list<LecturePart> neededParts;
//        std::copy_if(selectedParts.begin(), selectedParts.end(), std::back_inserter(neededParts),
//            [this](LecturePart part) {
//                return mPartsIds.end() != std::find_if(mPartsIds.begin(), mPartsIds.end(),
//                    [&part](QPair<QString, int> elem){
//                        return elem.first.compare(part.getName()); });
//        });








        std::list<LecturePart> toInsert;
        std::list<LecturePart> toInsertSub;

        auto part = mParts.begin();
        while (part != mParts.end())
        {
            if (mPartsIds.contains(part->getName()))
            {
                toInsert.push_back(LecturePart(part->getName(), mPartsIds[part->getName()], mSelectedDisciplines.id));

                auto chapt = mChapters.begin();
                while (chapt != mChapters.end())
                {
                    if (chapt->getParentId() == part->getId())
                    {
                        toInsertSub.push_back(LecturePart(chapt->getName(), chapt->getId(), mPartsIds[part->getName()]));
                        std::async(std::launch::async, &SaveManager::deleteLecturePart, mSaveManager, chapt->getId(), chapt->getParentId(), SaveManager::ChapterType);
                        mChapters.erase(chapt++);
                        continue;
                    }
                    ++chapt;
                }

                std::async(std::launch::async, &SaveManager::deleteLecturePart, mSaveManager, part->getId(), part->getParentId(), SaveManager::Part);
                mParts.erase(part++);
                continue;
            }
            ++part;
        }

        for (auto & them : toInsert)
        {
            std::async(std::launch::async, &SaveManager::saveLecturePart, mSaveManager, them, SaveManager::Part);
            mParts.push_back(them);
        }
        for (auto & sub : toInsertSub)
        {
            std::async(std::launch::async, &SaveManager::saveLecturePart, mSaveManager, sub, SaveManager::ChapterType);
            mChapters.push_back(sub);
        }

        mParts.sort([](const LecturePart& lhs, const LecturePart& rhs)
                    { return lhs.getId() < rhs.getId(); });
        mChapters.sort([](const LecturePart& lhs, const LecturePart& rhs)
                    { return lhs.getId() < rhs.getId(); });
    }
    break;
    case Chapters:
    {
        if (mChaptersIds.size() <= 0)
            break;

        std::list<LecturePart> toInsert;
        std::list<LecturePart> toInsertSub;

        auto chapt = mChapters.begin();
        while (chapt != mChapters.end())
        {
            if (mChaptersIds.contains(chapt->getName()))
            {
                toInsert.push_back(LecturePart(chapt->getName(), mChaptersIds[chapt->getName()], mSelectedPart.id));

                auto them = mThemes.begin();
                while (them != mThemes.end())
                {
                    if (them->getParentId() == chapt->getId())
                    {
                        toInsertSub.push_back(LecturePart(them->getName(), them->getId(), mChaptersIds[chapt->getName()]));
                        std::async(std::launch::async, &SaveManager::deleteLecturePart, mSaveManager, them->getId(), them->getParentId(), SaveManager::ThemeType);
                        mThemes.erase(them++);
                        continue;
                    }
                    ++them;
                }

                std::async(std::launch::async, &SaveManager::deleteLecturePart, mSaveManager, chapt->getId(), chapt->getParentId(), SaveManager::ChapterType);
                mChapters.erase(chapt++);
                continue;
            }
            ++chapt;
        }

        for (auto & them : toInsert)
        {
            std::async(std::launch::async, &SaveManager::saveLecturePart, mSaveManager, them, SaveManager::ChapterType);
            mChapters.push_back(them);
        }
        for (auto & sub : toInsertSub)
        {
            std::async(std::launch::async, &SaveManager::saveLecturePart, mSaveManager, sub, SaveManager::ThemeType);
            mThemes.push_back(sub);
        }

        mChapters.sort([](const LecturePart& lhs, const LecturePart& rhs)
                    { return lhs.getId() < rhs.getId(); });
        mThemes.sort([](const LecturePart& lhs, const LecturePart& rhs)
                    { return lhs.getId() < rhs.getId(); });
    }
    break;
    case Themes:
    {
        if (mThemesIds.size() <= 0)
            break;

        std::list<LecturePart> toInsert;
        std::list<LecturePart> toInsertSub;

        auto them = mThemes.begin();
        while (them != mThemes.end())
        {
            if (mThemesIds.contains(them->getName()))
            {
                LecturePart lect(them->getName(), mThemesIds[them->getName()], mSelectedChapter.id);
                lect.setFileName(them->getFileName());
                toInsert.push_back(lect);

                auto sub = mSubThemes.begin();
                while (sub != mSubThemes.end())
                {
                    if (sub->getParentId() == them->getId())
                    {
                        LecturePart subLect(sub->getName(), sub->getId(), mThemesIds[them->getName()]);
                        subLect.setFileName(sub->getFileName());
                        toInsertSub.push_back(subLect);
                        std::async(std::launch::async, &SaveManager::deleteLecturePart, mSaveManager, sub->getId(), sub->getParentId(), SaveManager::SubTheme);
                        mSubThemes.erase(sub++);
                        continue;
                    }
                    ++sub;
                }

                std::async(std::launch::async, &SaveManager::deleteLecturePart, mSaveManager, them->getId(), them->getParentId(), SaveManager::ThemeType);
                mThemes.erase(them++);
                continue;
            }
            ++them;
        }

        for (auto & them : toInsert)
        {
            std::async(std::launch::async, &SaveManager::saveLecturePart, mSaveManager, them, SaveManager::ThemeType);
            mThemes.push_back(them);
        }
        for (auto & sub : toInsertSub)
        {
            std::async(std::launch::async, &SaveManager::saveLecturePart, mSaveManager, sub, SaveManager::SubTheme);
            mSubThemes.push_back(sub);
        }

        mThemes.sort([](const LecturePart& lhs, const LecturePart& rhs)
                    { return lhs.getId() < rhs.getId(); });
        mSubThemes.sort([](const LecturePart& lhs, const LecturePart& rhs)
                    { return lhs.getId() < rhs.getId(); });
    }
    break;
    case SubThemes:
    {
        std::list<LecturePart> toInsert;

        if (mSubThemesIds.size() <= 0)
            break;

        auto sub = mSubThemes.begin();
        while (sub != mSubThemes.end())
        {
            if (mSubThemesIds.contains(sub->getName()))
            {
                LecturePart lect(sub->getName(), mSubThemesIds[sub->getName()], mSelectedTheme.id);
                lect.setFileName(sub->getFileName());
                toInsert.push_back(lect);
                std::async(std::launch::async, &SaveManager::deleteLecturePart, mSaveManager, sub->getId(), sub->getParentId(), SaveManager::SubTheme);
                mSubThemes.erase(sub++);
                continue;
            }
            ++sub;
        }

        for (auto & sub : toInsert)
        {
            std::async(std::launch::async, &SaveManager::saveLecturePart, mSaveManager, sub, SaveManager::SubTheme);
            mSubThemes.push_back(sub);
        }

        mSubThemes.sort([](const LecturePart& lhs, const LecturePart& rhs)
                    { return lhs.getId() < rhs.getId(); });
    }
    break;
    }

    mDisciplinesIds.clear();
    mPartsIds.clear();
    mChaptersIds.clear();
    mThemesIds.clear();
    mSubThemesIds.clear();
}

bool LecturesManager::hasSubThemes(const QString& theme)
{
    auto themeIt = std::find_if(mThemes.begin(), mThemes.end(), [&theme](LecturePart& it)
    { return !it.getName().compare(theme); });

    if (mThemes.end() == themeIt)
        return false;

    int themeId = themeIt->getId();

    auto it = std::find_if(mSubThemes.begin(), mSubThemes.end(),
                           [&themeId](LecturePart& item){ return themeId == item.getParentId(); });

    if (mSubThemes.end() == it)
        return false;

    return mSubThemes.end() != it;
}

void LecturesManager::selectedItem(const QString& name, const LecturesManager::Type& type)
{
    switch(type)
    {
    case Disciplines:
    {
        mSelectedDisciplines.name = name;

        auto discIt = std::find_if(mDisciplines.begin(), mDisciplines.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });

        if (mDisciplines.end() == discIt)
            break;

        mSelectedDisciplines.id = discIt->getId();
    }
        break;
    case Parts:
    {
        mSelectedPart.name = name;

        auto partIt = std::find_if(mParts.begin(), mParts.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });

        if (mParts.end() == partIt)
            break;

        mSelectedPart.id = partIt->getId();
    }
        break;
    case Chapters:
    {
        mSelectedChapter.name = name;

        auto chaptIt = std::find_if(mChapters.begin(), mChapters.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });

        if (mChapters.end() == chaptIt)
            break;

        mSelectedChapter.id = chaptIt->getId();
    }
        break;
    case Themes:
    {
        mSelectedTheme.name = name;

        auto themeIt = std::find_if(mThemes.begin(), mThemes.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });

        if (mThemes.end() == themeIt)
            break;

        mSelectedTheme.id = themeIt->getId();
    }
        break;
    case SubThemes:
    {
        mSelectedSubTheme.name = name;

        auto subThemeIt = std::find_if(mSubThemes.begin(), mSubThemes.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });

        if (mSubThemes.end() == subThemeIt)
            break;

        mSelectedSubTheme.id = subThemeIt->getId();
    }
        break;
    }
}

QString LecturesManager::selectedItem(const LecturesManager::Type& type)
{
    switch(type)
    {
    case Disciplines: return mSelectedDisciplines.name;
    case Parts: return mSelectedPart.name;
    case Chapters: return mSelectedChapter.name;
    case Themes: return mSelectedTheme.name;
    case SubThemes: return mSelectedSubTheme.name;
    default: return "";
    }
}

bool LecturesManager::itemHasFile(const QString& name, const LecturesManager::Type& type)
{
    if (LecturesManager::Themes == type)
    {
        int chapterId = mSelectedChapter.id;

        auto themeIt = std::find_if(mThemes.begin(), mThemes.end(), [&name, &chapterId](LecturePart& it)
        { return chapterId == it.getParentId() && !it.getName().compare(name); });

        if (mThemes.end() != themeIt)
            return !themeIt->getFileName().isEmpty();
    }
    else if (LecturesManager::SubThemes == type)
    {
        int themeId = mSelectedTheme.id;

        auto subThemeIt = std::find_if(mSubThemes.begin(), mSubThemes.end(), [&name, &themeId](LecturePart& it)
        { return themeId == it.getParentId() && !it.getName().compare(name); });

        if (mSubThemes.end() != subThemeIt)
            return !subThemeIt->getFileName().isEmpty();
    }
    return false;
}
