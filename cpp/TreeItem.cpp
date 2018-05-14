#include "TreeItem.h"
#include <QDebug>

TreeItem::TreeItem(const QString & text, const int& idx, QObject *parent)
    : QObject(parent)
    , m_text(text)
    , m_index(idx)
    , m_isOpen(false)
{}

QString TreeItem::text() const
{
    return m_text;
}

void TreeItem::setText(const QString & text)
{
    m_text = text;
    emit textChanged();
}

int TreeItem::idx() const
{
    return m_index;
}

void TreeItem::setIdx(const int& idx)
{
    m_index = idx;
    emit idxChanged();
}

QList<QObject*> TreeItem::childItems() const
{
    return m_childItems;
}

bool TreeItem::hasChild() const
{
    return m_childItems.size() > 0;
}

bool TreeItem::isOpen() const
{
    return hasChild() ? m_isOpen : false;
}

void TreeItem::addChild(TreeItem * child)
{
    if (child->parent() != this)
        child->setParent(this);

    m_childItems << child;

    if (m_childItems.size() == 1)
        emit hasChildChanged();
}

void TreeItem::setOpen(bool open)
{
    if (m_isOpen != open)
    {
        m_isOpen = open;
        emit isOpenChanged();
    }
}
