#include "TreeModel.h"

/*static*/ QQmlApplicationEngine *TreeModel::m_qmlEngine = nullptr;

TreeModel::TreeModel(QObject *parent)
    : QObject(parent)
{
    // Формирую отображаемое дерево.
    // Создам один элемент о добавлю в него вложенные.
//    TreeItem * item = new TreeItem(QObject::trUtf8("Элемент 1"), 0);
//    item->addChild(new TreeItem(QObject::trUtf8("Подэлемент 1"), 0));
//    item->addChild(new TreeItem(QObject::trUtf8("Подэлемент 2"), 1));
//    item->addChild(new TreeItem(QObject::trUtf8("Подэлемент 3"), 2));

//    m_simpleTree << item;
//    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 2"), 1);
//    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 3"), 2);



}

/*static*/ QObject* TreeModel::treeModelProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    TreeModel *treeModel = new TreeModel();
    return treeModel;
}

/*static*/ void TreeModel::setQmlEngine(QQmlApplicationEngine *engine)
{
    TreeModel::m_qmlEngine = engine;
}

void TreeModel::clearComponentCache()
{
    TreeModel::m_qmlEngine->clearComponentCache();
}

QList<QObject *> TreeModel::simpleTree() const
{
    return m_simpleTree;
}
