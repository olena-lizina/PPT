#ifndef GRADESMANAGER_H
#define GRADESMANAGER_H
#include "ManagerInterface.h"

class GradesManager: public ManagerInterface
{
public:
    explicit GradesManager(QObject* parent = nullptr);
    static QObject* managerProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
};

#endif // GRADESMANAGER_H
