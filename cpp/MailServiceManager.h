#ifndef MAILSERVICEMANAGER_H
#define MAILSERVICEMANAGER_H
#include "ManagerInterface.h"
#include "SmtpMime"
#include "SaveManager.h"
#include "DataTypes.h"

class MailServiceManager: public ManagerInterface
{
    Q_OBJECT

public:
    explicit MailServiceManager(QObject* parent = nullptr);
    static QObject* managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

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

    const QString host;
    const int port;
    const bool ssl;
    const bool auth;
    const QString user;
    const QString password;
    const QString sender;
};

#endif // MAILSERVICEMANAGER_H
