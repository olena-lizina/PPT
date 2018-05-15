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

QList<QObject*> LecturesManager::labsTree() const
{
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
}
