#ifndef TREEITEM_H
#define TREEITEM_H
#include <QObject>

class TreeItem: public QObject
{
    Q_OBJECT

    /** Text element*/
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    /** Element index*/
    Q_PROPERTY(int idx READ idx WRITE setIdx NOTIFY idxChanged)
    /** Nesting*/
    Q_PROPERTY(int nesting READ nesting NOTIFY nestingChanged)
    /** Child list*/
    Q_PROPERTY(QList<QObject*> childItems READ childItems NOTIFY childItemsChanged)
    /** Has child*/
    Q_PROPERTY(bool hasChild READ hasChild NOTIFY hasChildChanged)
    /** Node is open*/
    Q_PROPERTY(bool isOpen READ isOpen WRITE setOpen NOTIFY isOpenChanged)
public:
    explicit TreeItem(const QString & text, const int& idx, const int& nesting, QObject *parent = 0);

    QString text() const;
    void setText(const QString & text);
    int idx() const;
    void setIdx(const int& idx);
    QList<QObject*> childItems() const;
    bool hasChild() const;
    bool isOpen() const;
    void addChild(TreeItem * child);
    void setOpen(bool open);
    int nesting() const;

signals:
    void textChanged();
    void idxChanged();
    void childItemsChanged();
    void hasChildChanged();
    void isOpenChanged();
    void nestingChanged();

private:
    /** Text element*/
    QString m_text;
    /** Element index*/
    int m_index;
    /** Child list*/
    QList<QObject*> m_childItems;
    /** Node is open*/
    bool m_isOpen;
    int mNesting;
};

#endif // TREEITEM_H
