#include "DBManager.h"
#include <mutex>

SQLiteManager::SQLiteManager(const QString& dbName)
{
    mDbConnection.reset(new QSqlDatabase());
    *mDbConnection = QSqlDatabase::addDatabase("QSQLITE");
    mDbConnection->setDatabaseName(dbName);

    if (!mDbConnection->open())
        return;

    mQuery.reset(new QSqlQuery(*mDbConnection));

    this->moveToThread(&mThread);
    connect(&mThread, &QThread::finished, this, &QObject::deleteLater);
    mThread.start();

    connect(this, &SQLiteManager::exec, this, &SQLiteManager::onExec);
    connect(this, &SQLiteManager::finished, &mLoop, &QEventLoop::quit);
}

SQLiteManager::~SQLiteManager()
{
    mQuery.reset();

    if (!mDbConnection)
        return;

    mDbConnection->close();
    mDbConnection->removeDatabase("QSQLITE");
    mDbConnection.reset();

    mThread.quit();
    mThread.wait();
}

QPair<bool, QList<QList<QVariant> > > SQLiteManager::execute(QString query, QList<QString> values)
{
    static std::recursive_mutex mutex;
    std::lock_guard<std::recursive_mutex > locker(mutex);
    mResult.clear();
    mSuccess = false;
    mRequest = query;
    mValues = values;

    emit exec();
    mLoop.exec();
    QPair<bool, QList<QList<QVariant> > > out;
    out.first = mSuccess;
    out.second = mResult;

    return out;
}

void SQLiteManager::onExec()
{
    mResult.clear();
    mSuccess = false;

    if (!mQuery)
        emit finished();

    const int countVals = mValues.size();
    QList<QString> placeholders;

    for (int i = 0; i < countVals; i++)
        placeholders << ":" + QString::number(i);

    QString prepareStr;

    switch (countVals)
    {
    case 0:
        prepareStr = mRequest;
        break;
    default:
        prepareStr = QString(mRequest).arg(placeholders.join(","));
        break;
    }

    if (!mQuery->prepare(prepareStr))
    {
        emit finished();
        return;
    }

    for (int i = 0; i < countVals; i++)
        mQuery->bindValue(placeholders.at(i), mValues.at(i));

    if (!mQuery->exec())
    {
        emit finished();
        return;
    }

    QList<QList<QVariant> > result;

    while (mQuery->next())
    {
        int index = 0;
        QList<QVariant> row;

        while (mQuery->value(index).isValid())
        {
            row << mQuery->value(index);
            ++index;
        }

        result << row;
    }

    mResult = result;
    mSuccess = true;

    emit finished();
}
