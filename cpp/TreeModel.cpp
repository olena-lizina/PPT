#include "TreeModel.h"

TreeModel::TreeModel(QObject *parent)
    : QObject(parent)
{
    // Формирую отображаемое дерево.
    // Создам один элемент о добавлю в него вложенные.
    TreeItem * item = new TreeItem(QObject::trUtf8("Элемент 1"), 0);
    item->addChild(new TreeItem(QObject::trUtf8("Подпункт 1"), 0));
    item->addChild(new TreeItem(QObject::trUtf8("Подпункт 2"), 1));
    item->addChild(new TreeItem(QObject::trUtf8("Подпункт 3"), 2));
    item->setOpen(true);

    // Добавлю элемент в список верхнего уровня.
    m_simpleTree << item;

    // Добавлю элемент без вложенных элементов.
    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 2"), 1);

    // Создам ещё один элемент имеющий вложенные элементы.
    item = new TreeItem(QObject::trUtf8("Элемент 3"), 0);
    item->addChild(new TreeItem(QObject::trUtf8("Подпункт 1"), 1));

    // В один из вложенных элементов добалю вложения, тем самым сформировав третий
    // уровень вложенности.
    TreeItem * subitem = new TreeItem(QObject::trUtf8("Подпункт 2"), 3);
    subitem->addChild(new TreeItem(QObject::trUtf8("Подэлемент 1"), 4));
    item->addChild(subitem);
    item->addChild(new TreeItem(QObject::trUtf8("Подпункт 3"), 5));
    m_simpleTree << item;

    // Добавлю ещё несколько элементов без вложенных элементов.
    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 4"), 3);
    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 5"), 4);
    m_simpleTree << new TreeItem(QObject::trUtf8("Элемент 6"), 5);
}
QList<QObject *> TreeModel::simpleTree() const
{
    return m_simpleTree;
}
