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

/*static*/ QQmlApplicationEngine *LecturesManager::m_qmlEngine = nullptr;
/*static*/ SaveManager::Ptr LecturesManager::mSaveManager;

LecturesManager::LecturesManager(QObject* parent)
    : QObject(parent)
{
    loadAllLectures();
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
    mParts = mSaveManager->getLectureParts(SaveManager::Part);
    mChapters = mSaveManager->getLectureParts(SaveManager::Chapter);
    mThemes = mSaveManager->getLectureParts(SaveManager::Theme);
    mSubThemes = mSaveManager->getLectureParts(SaveManager::SubTheme);

    mParts.sort([](const LecturePart& lhs, const LecturePart& rhs)
                { return lhs.getId() < rhs.getId(); });

    mChapters.sort([](const LecturePart& lhs, const LecturePart& rhs)
                { return lhs.getId() < rhs.getId(); });

    mThemes.sort([](const LecturePart& lhs, const LecturePart& rhs)
                { return lhs.getId() < rhs.getId(); });

    mSubThemes.sort([](const LecturePart& lhs, const LecturePart& rhs)
                { return lhs.getId() < rhs.getId(); });
}

QStringList LecturesManager::getListModel(const LecturesManager::Type& type)
{
    QStringList result;

    switch(type)
    {
    case Parts:
        for (auto& it : mParts)
            result << it.getName();
        break;
    case Chapters:
        for (auto& it : mChapters)
            if (mSelectedChapter.id == it.getId())
                result << it.getName();
        break;
    case Themes:
        for (auto& it : mThemes)
            if (mSelectedTheme.id == it.getId())
                result << it.getName();
        break;
    case SubThemes:
        for (auto& it : mSubThemes)
            if (mSelectedSubTheme.id == it.getId())
                result << it.getName();
        break;
    }

    return result;
}

bool LecturesManager::itemExists(const QString& name, const LecturesManager::Type& type)
{
    switch(type)
    {
    case Parts: return (mParts.end() != std::find_if(mParts.begin(), mParts.end(),
                                              [&name](LecturePart& it){ return !it.getName().compare(name); }));
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
    case Parts:
    {
        LecturePart part(name, mParts.back().getId() + 1, 0);
        mParts.push_back(part);
        mSaveManager->saveLecturePart(part, SaveManager::Part);
    }
        break;
    case Chapters:
    {
        LecturePart chapt(name, mChapters.back().getId() + 1, mSelectedPart.id);
        mChapters.push_back(chapt);
        mSaveManager->saveLecturePart(chapt, SaveManager::Chapter);
    }
        break;
    case Themes:
    {
        LecturePart theme(name, mThemes.back().getId() + 1, mSelectedChapter.id);
        if (!fileName.isEmpty())
            theme.setFileName(fileName);
        mThemes.push_back(theme);
        mSaveManager->saveLecturePart(theme, SaveManager::Theme);
    }
        break;
    case SubThemes:
    {
        LecturePart subTheme(name, mSubThemes.back().getId() + 1, mSelectedSubTheme.id);
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
    case Parts:
    {
        LecturePart part(name, mSelectedPart.id, 0);
        int id = mSelectedPart.id;
        auto it = std::find_if(mParts.begin(), mParts.end(),
                               [&id](LecturePart& item){ return item.getId() == id; });
        it->setName(name);
        mSaveManager->updateLecturePart(*it, part, SaveManager::Part);
    }
        break;
    case Chapters:
    {
        LecturePart chapt(name, mSelectedChapter.id, mSelectedPart.id);
        int id = mSelectedChapter.id;
        auto it = std::find_if(mChapters.begin(), mChapters.end(),
                               [&id](LecturePart& item){ return id == item.getParentId(); });
        it->setName(name);
        mSaveManager->updateLecturePart(*it, chapt, SaveManager::Chapter);
    }
        break;
    case Themes:
    {
        LecturePart theme(name, mSelectedTheme.id, mSelectedChapter.id);
        int id = mSelectedTheme.id;
        auto it = std::find_if(mThemes.begin(), mThemes.end(),
                               [&id](LecturePart& item){ return id == item.getParentId(); });
        it->setName(name);
        if (!fileName.isEmpty())
            it->setFileName(fileName);
        mSaveManager->updateLecturePart(*it, theme, SaveManager::Theme);
    }
        break;
    case SubThemes:
    {
        LecturePart subTheme(name, mSelectedSubTheme.id, mSelectedTheme.id);
        int id = mSelectedSubTheme.id;
        auto it = std::find_if(mSubThemes.begin(), mSubThemes.end(),
                               [&id](LecturePart& item){ return id == item.getParentId(); });
        it->setName(name);
        if (!fileName.isEmpty())
            it->setFileName(fileName);
        mSaveManager->updateLecturePart(*it, subTheme, SaveManager::SubTheme);
    }
        break;
    }
}

void LecturesManager::deleteItem(const LecturesManager::Type& type)
{
    switch(type)
    {
    case Parts:
    {
        int id = mSelectedPart.id;
        auto it = std::find_if(mParts.begin(), mParts.end(),
                               [&id](LecturePart& item){ return item.getId() == id; });
        mSaveManager->deleteLecturePart(*it, SaveManager::Part);
        mParts.erase(it);
    }
        break;
    case Chapters:
    {
        int id = mSelectedChapter.id;
        auto it = std::find_if(mChapters.begin(), mChapters.end(),
                               [&id](LecturePart& item){ return id == item.getParentId(); });
        mSaveManager->deleteLecturePart(*it, SaveManager::Chapter);
        mChapters.erase(it);
    }
        break;
    case Themes:
    {
        int id = mSelectedTheme.id;
        auto it = std::find_if(mThemes.begin(), mThemes.end(),
                               [&id](LecturePart& item){ return id == item.getParentId(); });
        mSaveManager->deleteLecturePart(*it, SaveManager::Theme);
        mThemes.erase(it);
    }
        break;
    case SubThemes:
    {
        int id = mSelectedSubTheme.id;
        auto it = std::find_if(mSubThemes.begin(), mSubThemes.end(),
                               [&id](LecturePart& item){ return id == item.getParentId(); });
        mSaveManager->deleteLecturePart(*it, SaveManager::SubTheme);
        mSubThemes.erase(it);
    }
        break;
    }
}

bool LecturesManager::hasSubThemes(const QString& theme)
{
    auto themeIt = std::find_if(mThemes.begin(), mThemes.end(), [&theme](LecturePart& it)
    { return !it.getName().compare(theme); });

    int themeId = themeIt->getId();

    auto it = std::find_if(mSubThemes.begin(), mSubThemes.end(),
                           [&themeId](LecturePart& item){ return themeId == item.getParentId(); });
    return mSubThemes.end() != it;
}

void LecturesManager::selectedItem(const QString& name, const LecturesManager::Type& type)
{
    switch(type)
    {
    case Parts:
    {
        mSelectedPart.name = name;

        auto partIt = std::find_if(mParts.begin(), mParts.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });

        mSelectedPart.id = partIt->getId();
    }
        break;
    case Chapters:
    {
        mSelectedChapter.name = name;

        auto chaptIt = std::find_if(mChapters.begin(), mChapters.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });

        mSelectedChapter.id = chaptIt->getId();
    }
        break;
    case Themes:
    {
        mSelectedTheme.name = name;

        auto themeIt = std::find_if(mThemes.begin(), mThemes.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });

        mSelectedTheme.id = themeIt->getId();
    }
        break;
    case SubThemes:
    {
        mSelectedSubTheme.name = name;

        auto subThemeIt = std::find_if(mSubThemes.begin(), mSubThemes.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });

        mSelectedSubTheme.id = subThemeIt->getId();
    }
        break;
    }
}

QString LecturesManager::selectedItem(const LecturesManager::Type& type)
{
    switch(type)
    {
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
        auto themeIt = std::find_if(mThemes.begin(), mThemes.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });
        if (mThemes.end() != themeIt)
            return !themeIt->getFileName().isEmpty();
    }
    else if (LecturesManager::SubThemes == type)
    {
        auto subThemeIt = std::find_if(mSubThemes.begin(), mSubThemes.end(), [&name](LecturePart& it)
        { return !it.getName().compare(name); });
        if (mSubThemes.end() != subThemeIt)
            return !subThemeIt->getFileName().isEmpty();
    }
    return false;
}
