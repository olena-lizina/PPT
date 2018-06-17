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

    Q_PROPERTY(int labsNesting READ labsNesting WRITE setLabsNesting NOTIFY labsNestingChanged)
    Q_PROPERTY(int labDisciplineId READ labDisciplineId WRITE setLabDisciplineId NOTIFY labDisciplineIdChanged)
    Q_PROPERTY(int labId READ labId WRITE setLabId NOTIFY labIdChanged)

    Q_PROPERTY(int executorId READ executorId WRITE setExecutorId NOTIFY executorIdChanged)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)

public:
    ScreenContextBuffer(QObject *parent = 0);
    // singleton type provider function
    static QObject* screenContextBufferProvider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static void setQmlEngine(QQmlApplicationEngine *engine);
    Q_INVOKABLE void clearComponentCache();
    Q_INVOKABLE void openLabScreen() { emit onOpenLabScreen(); }
    Q_INVOKABLE void setNestingAndIndex(const int& nesting, const int& idx);
    Q_INVOKABLE void callBackToReport() { emit backToReport(); }

    int nesting() const;
    int selectedIdx() const;
    QString loaderSource() const;
    int screenType() const;
    bool edit() const;
    int courseId() const;
    QString courseName() const;
    int labsNesting() const;
    int labDisciplineId() const;
    int labId() const;
    int executorId() const;
    QString filePath() const;

    void setNesting(const int& nesting);
    void setSelectedIdx(const int& idx);
    void setLoaderSource(const QString& path);
    void setScreenType(const int& type);
    void setEdit(bool edit);
    void setCourseId(const int& id);
    void setCourseName(const QString& name);
    void setLabsNesting(const int& nesting);
    void setLabDisciplineId(int id);
    void setLabId(int id);
    void setExecutorId(int id);
    void setFilePath(QString path);

signals:
    void nestingChanged();
    void selectedIdxChanged();
    void loaderSourceChanged();
    void screenTypeChanged();
    void itemChanged();
    void editChanged();
    void courseIdChanged();
    void courseNameChanged();
    void labsNestingChanged();
    void labDisciplineIdChanged();
    void labIdChanged();
    void onOpenLabScreen();
    void executorIdChanged();
    void filePathChanged();
    void backToReport();

private:
    static QQmlApplicationEngine *m_qmlEngine;
    int mNesting;
    int mSelectedIdx;
    int mScreenType;
    QString mLoaderSource;
    bool mEdit;
    int mCourseId;
    QString mCourseName;
    int mLabsNesting;
    int mLabDisId;
    int mLabId;
    int mExecutorId;
    QString mFilePath;
};

#endif // SCREENCONTEXTBUFFER_H
