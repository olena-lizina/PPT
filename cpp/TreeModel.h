#ifndef TREEMODEL_H
#define TREEMODEL_H
#include <QObject>
#include "TreeItem.h"

class TreeModel: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> simpleTree READ simpleTree NOTIFY simpleTreeChanged)
public:
    TreeModel(QObject *parent = 0);
    QList<QObject*> simpleTree() const;

signals:
    void simpleTreeChanged();

private:
    QList<QObject*> m_simpleTree;
};

#endif // TREEMODEL_H
