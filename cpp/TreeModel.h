#ifndef TREEMODEL_H
#define TREEMODEL_H
#include <QObject>
#include <QQmlApplicationEngine>
#include "TreeItem.h"

class TreeModel: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> simpleTree READ simpleTree NOTIFY simpleTreeChanged)
public:
    TreeModel(QObject *parent = 0);
    // singleton type provider function
    static QObject* treeModelProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    Q_INVOKABLE void clearComponentCache();

    QList<QObject*> simpleTree() const;

signals:
    void simpleTreeChanged();

private:
    QList<QObject*> m_simpleTree;
    static QQmlApplicationEngine *m_qmlEngine;
};

#endif // TREEMODEL_H
