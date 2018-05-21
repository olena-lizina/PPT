#ifndef MAILSERVICEMANAGER_H
#define MAILSERVICEMANAGER_H
#include <QObject>
#include "SmtpMime"
#include "SaveManager.h"
#include "DataTypes.h"
#include <QQmlApplicationEngine>

class MailServiceManager: public QObject
{
    Q_OBJECT

public:
    explicit MailServiceManager(QObject* parent = nullptr);
    virtual ~MailServiceManager();

    // singleton type provider function
    static QObject* mailServiceManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    static void setSaveManager(std::shared_ptr<SaveManager> saveMgr);
    Q_INVOKABLE void clearComponentCache();

    // to student
    Q_INVOKABLE void sendEducationMaterials(const QString& courceName, const int& courseId);
    Q_INVOKABLE void sendLabWorks(const QString courseName, const int courseId, const QString labName);
    Q_INVOKABLE void sendMarks() {}

    // to teacher
    Q_INVOKABLE void sendHomeWorkMaterials() {}

private:
    void zipFile(QString fileName, QString zipName);
    bool copyPath(const QString &srcFilePath,
                  const QString &tgtFilePath);

private:
    static SaveManager::Ptr mSaveManager;
    static QQmlApplicationEngine *m_qmlEngine;

    const QString host;
    const int port;
    const bool ssl;
    const bool auth;
    const QString user;
    const QString password;
    const QString sender;
};

#endif // MAILSERVICEMANAGER_H
