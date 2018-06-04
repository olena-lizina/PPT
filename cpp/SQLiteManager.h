#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QString>
#include <QVariant>
#include <QList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QEventLoop>
#include <QSharedPointer>
#include <QThread>

class SQLiteManager
        : public QObject
{
    Q_OBJECT
public:
    SQLiteManager(const QString& dbName);
    ~SQLiteManager();

    //!Can be executed from different thread
    QPair<bool, QList<QList<QVariant> > > execute(QString query, QList<QString> values = QList<QString>());

signals:
    void finished();
    void exec();
private slots:
    void onExec();

private:
    QSharedPointer<QSqlDatabase> mDbConnection;
    QSharedPointer<QSqlQuery> mQuery;
    QThread mThread;
    QList<QList<QVariant> > mResult;
    QString mRequest;
    QList<QString> mValues;
    bool mSuccess;
    QEventLoop mLoop;
};

#endif // DBMANAGER_H
