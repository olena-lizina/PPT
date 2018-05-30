#ifndef MANAGERINTERFACE_H
#define MANAGERINTERFACE_H
#include <QQmlApplicationEngine>
#include <QObject>
#include "SaveManager.h"

class ManagerInterface: public QObject
{
    Q_OBJECT
public:
    ManagerInterface(QObject* parent = nullptr);
    virtual ~ManagerInterface() {}

    static void setQmlEngine(QQmlApplicationEngine *engine);
    static void setSaveManager(std::shared_ptr<SaveManager> saveMgr);
    Q_INVOKABLE void clearComponentCache();

protected:
    static QQmlApplicationEngine *mQmlEngine;
    static SaveManager::Ptr mSaveManager;
};

#endif // MANAGERINTERFACE_H
