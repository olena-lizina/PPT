#include "LecturesManager.h"

/*static*/ QQmlApplicationEngine *LecturesManager::m_qmlEngine = nullptr;
/*static*/ std::shared_ptr<SaveManager> LecturesManager::mSaveManager = nullptr;

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
