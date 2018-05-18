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

