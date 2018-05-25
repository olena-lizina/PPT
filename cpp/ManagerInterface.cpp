#include "ManagerInterface.h"

/*static*/ QQmlApplicationEngine* ManagerInterface::mQmlEngine = nullptr;
/*static*/ SaveManager::Ptr ManagerInterface::mSaveManager;

ManagerInterface::ManagerInterface(QObject* parent)
    : QObject(parent)
{

}

/*static*/ void ManagerInterface::setQmlEngine(QQmlApplicationEngine *engine)
{
    ManagerInterface::mQmlEngine = engine;
}

/*static*/ void ManagerInterface::setSaveManager(std::shared_ptr<SaveManager> saveMgr)
{
    ManagerInterface::mSaveManager = saveMgr;
}

void ManagerInterface::clearComponentCache()
{
    ManagerInterface::mQmlEngine->clearComponentCache();
}
