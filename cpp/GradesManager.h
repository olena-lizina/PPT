#ifndef GRADESMANAGER_H
#define GRADESMANAGER_H
#include "ManagerInterface.h"

class GradesManager: public ManagerInterface
{
public:
    explicit GradesManager(QObject* parent = nullptr);
    static QObject* managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE QList<QObject*> studMarks();

signals:
    void gradesModelChanged();

private:

    void initModel();

    QList<Student> mStudents;
    QList<LabWork> mLabs;
    QList<Report> mReports;
    QList<QObject*> mModel;
};

#endif // GRADESMANAGER_H
