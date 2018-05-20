#ifndef SCREENCONTEXTBUFFER_H
#define SCREENCONTEXTBUFFER_H
#include <QObject>
#include <QQmlApplicationEngine>

class ScreenContextBuffer: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int nesting READ nesting WRITE setNesting NOTIFY nestingChanged)
    Q_PROPERTY(int selectedIdx READ selectedIdx WRITE setSelectedIdx NOTIFY selectedIdxChanged)
    Q_PROPERTY(QString loaderSource READ loaderSource WRITE setLoaderSource NOTIFY loaderSourceChanged)
    Q_PROPERTY(int screenType READ screenType WRITE setScreenType NOTIFY screenTypeChanged)
    Q_PROPERTY(bool edit READ edit WRITE setEdit NOTIFY editChanged)
    Q_PROPERTY(int courseId READ courseId WRITE setCourseId NOTIFY courseIdChanged)
    Q_PROPERTY(QString courseName READ courseName WRITE setCourseName NOTIFY courseNameChanged)

public:
    ScreenContextBuffer(QObject *parent = 0);
    // singleton type provider function
    static QObject* screenContextBufferProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    Q_INVOKABLE void clearComponentCache();

    Q_INVOKABLE void setNestingAndIndex(const int& nesting, const int& idx);

    int nesting() const;
    int selectedIdx() const;
    QString loaderSource() const;
    int screenType() const;
    bool edit() const;
    int courseId() const;
    QString courseName() const;

    void setNesting(const int& nesting);
    void setSelectedIdx(const int& idx);
    void setLoaderSource(const QString& path);
    void setScreenType(const int& type);
    void setEdit(bool edit);
    void setCourseId(const int& id);
    void setCourseName(const QString& name);

signals:
    void nestingChanged();
    void selectedIdxChanged();
    void loaderSourceChanged();
    void screenTypeChanged();
    void itemChanged();
    void editChanged();
    void courseIdChanged();
    void courseNameChanged();

private:
    static QQmlApplicationEngine *m_qmlEngine;
    int mNesting;
    int mSelectedIdx;
    int mScreenType;
    QString mLoaderSource;
    bool mEdit;
    int mCourseId;
    QString mCourseName;
};

#endif // SCREENCONTEXTBUFFER_H
