#include "TreeModel.h"

/*static*/ QQmlApplicationEngine *TreeModel::m_qmlEngine = nullptr;

TreeModel::TreeModel(QObject *parent)
    : QObject(parent)
{
    // Формирую отображаемое дерево.
    // Создам один элемент о добавлю в него вложенные.
    TreeItem * item = new TreeItem(QObject::trUtf8("Элемент 1"), 0);
    item->addChild(new TreeItem(QObject::trUtf8("Подэлемент 1"), 0));
    item->addChild(new TreeItem(QObject::trUtf8("Подэлемент 2"), 1));
    item->addChild(new TreeItem(QObject::trUtf8("Подэлемент 3"), 2));

    m_simpleTree << item;
    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 2"), 1);
    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 3"), 2);


//    // Добавлю элемент в список верхнего уровня.
//    m_simpleTree << item;

//    // Добавлю элемент без вложенных элементов.
//    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 2"), 1);

//    // Создам ещё один элемент имеющий вложенные элементы.
//    item = new TreeItem(QObject::trUtf8("Элемент 3"), 0);
//    item->addChild(new TreeItem(QObject::trUtf8("Подпункт 1"), 1));

//    // В один из вложенных элементов добалю вложения, тем самым сформировав третий
//    // уровень вложенности.
//    TreeItem * subitem = new TreeItem(QObject::trUtf8("Подпункт 2"), 3);
//    subitem->addChild(new TreeItem(QObject::trUtf8("Подэлемент 1"), 4));
//    item->addChild(subitem);
//    item->addChild(new TreeItem(QObject::trUtf8("Подпункт 3"), 5));
//    m_simpleTree << item;

//    // Добавлю ещё несколько элементов без вложенных элементов.
//    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 4"), 3);
//    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 5"), 4);
//    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 6"), 5);

//    for (auto it : m_simpleTree)
//    {
//        if (it->)
//        auto ch = it->childItems();
//        for ()
//    }
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
