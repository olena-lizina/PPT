#ifndef LECTURESMANAGER_H
#define LECTURESMANAGER_H
#include <QObject>
#include <QQmlApplicationEngine>
#include <memory>
#include "SaveManager.h"

class LecturesManager: public QObject
{
    Q_OBJECT
public:
    explicit LecturesManager(QObject* parent = nullptr);

    // singleton type provider function
    static QObject* lecturesManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    static void setSaveManager(std::shared_ptr<SaveManager> saveMgr);
    Q_INVOKABLE void clearComponentCache();

private:
    static std::shared_ptr<SaveManager> mSaveManager;
    static QQmlApplicationEngine *m_qmlEngine;
};

#endif // LECTURESMANAGER_H
