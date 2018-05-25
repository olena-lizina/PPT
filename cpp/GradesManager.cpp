#include "GradesManager.h"

GradesManager::GradesManager(QObject* parent)
    : ManagerInterface(parent)
{

}

/*static*/ QObject* GradesManager::managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    GradesManager *manager = new GradesManager();
    return manager;
}
