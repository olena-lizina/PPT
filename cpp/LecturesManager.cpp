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

/*static*/ QQmlApplicationEngine *LecturesManager::m_qmlEngine = nullptr;
/*static*/ SaveManager::Ptr LecturesManager::mSaveManager;

LecturesManager::LecturesManager(QObject* parent)
    : QObject(parent)
{}

/*static*/ QObject* LecturesManager::lecturesManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    LecturesManager *studentManager = new LecturesManager();
    return studentManager;
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

QStringList LecturesManager::getParts()
{
    return mSaveManager->getParts();
}

QStringList LecturesManager::getChapters()
{
    return mSaveManager->getChapters(mSelectedPart);
}

QStringList LecturesManager::getThemes()
{
    return mSaveManager->getThemes(mSelectedPart, mSelectedChapter);
}

QStringList LecturesManager::getSubThemes()
{
    return mSaveManager->getSubThemes(mSelectedPart, mSelectedChapter, mSelectedTheme);
}

bool LecturesManager::partExists(const QString& part)
{
    auto allParts = mSaveManager->getParts();
    if (allParts.count(part))
        return true;
    return false;
}

void LecturesManager::addPart(const QString& part)
{
    mSaveManager->savePart(part);
}

void LecturesManager::editPart(const QString& part)
{
    mSaveManager->updatePart(mSelectedPart, part);
}

void LecturesManager::deletePart()
{
    mSaveManager->deletePart(mSelectedPart);
}

bool LecturesManager::chapterExists(const QString& chapter)
{
    auto allChapters = mSaveManager->getChapters(mSelectedPart);
    if (allChapters.count(chapter))
        return true;
    return false;
}

void LecturesManager::addChapter(const QString& chapter)
{
    mSaveManager->saveChapter(mSelectedPart, chapter);
}

void LecturesManager::editChapter(const QString& chapter)
{
    mSaveManager->updateChapter(mSelectedChapter, chapter, mSelectedPart);
}

void LecturesManager::deleteChapter()
{
    mSaveManager->deleteChapter(mSelectedChapter, mSelectedPart);
}

bool LecturesManager::themesExists(const QString& theme)
{
    auto allThemes = mSaveManager->getThemes(mSelectedPart, mSelectedChapter);
    if (allThemes.count(theme))
        return true;
    return false;
}

void LecturesManager::addTheme(const QString& theme)
{
    mSaveManager->saveTheme(mSelectedPart, mSelectedChapter, theme);
}

void LecturesManager::editTheme(const QString& theme)
{
    mSaveManager->updateTheme(mSelectedTheme, theme, mSelectedPart, mSelectedChapter);
}

void LecturesManager::deleteTheme()
{
    mSaveManager->deleteTheme(mSelectedPart, mSelectedChapter, mSelectedTheme);
}

bool LecturesManager::hasSubThemes(const QString& theme)
{
    return mSaveManager->getSubThemes(mSelectedPart, mSelectedChapter, theme).size() > 0;
}

bool LecturesManager::subThemesExists(const QString& subTheme)
{
    auto allSubThemes = mSaveManager->getSubThemes(mSelectedPart, mSelectedChapter, mSelectedTheme);
    if (allSubThemes.count(subTheme))
        return true;
    return false;
}

void LecturesManager::addSubTheme(const QString& subTheme)
{
    mSaveManager->saveSubTheme(mSelectedPart, mSelectedChapter, mSelectedTheme, subTheme);
    emit signalUpdateThemes();
}

void LecturesManager::editSubTheme(const QString& subTheme)
{
    mSaveManager->updateSubTheme(mSelectedSubTheme, subTheme, mSelectedPart, mSelectedChapter, mSelectedTheme);
}

void LecturesManager::deleteSubTheme()
{
    mSaveManager->deleteSubTheme(mSelectedPart, mSelectedChapter, mSelectedTheme, mSelectedSubTheme);
}

void LecturesManager::selectedPart(const QString& part)
{
    mSelectedPart = part;
}

QString LecturesManager::selectedPart()
{
    return mSelectedPart;
}

void LecturesManager::selectedChapter(const QString& chapter)
{
    mSelectedChapter = chapter;
}

QString LecturesManager::selectedChapter()
{
    return mSelectedChapter;
}

void LecturesManager::selectedTheme(const QString& theme)
{
    mSelectedTheme = theme;
}

QString LecturesManager::selectedTheme()
{
    return mSelectedTheme;
}

void LecturesManager::selectedSubTheme(const QString& subTheme)
{
    mSelectedSubTheme = subTheme;
}

QString LecturesManager::selectedSubTheme()
{
    return mSelectedSubTheme;
}
