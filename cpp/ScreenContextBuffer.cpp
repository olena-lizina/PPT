#include "ScreenContextBuffer.h"
#include <QDebug>

/*static*/ QQmlApplicationEngine *ScreenContextBuffer::m_qmlEngine = nullptr;

ScreenContextBuffer::ScreenContextBuffer(QObject *parent)
    : QObject(parent)
    , mNesting(0)
    , mSelectedIdx(1)
{
}

/*static*/ QObject* ScreenContextBuffer::screenContextBufferProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    ScreenContextBuffer *screenContextBuffer = new ScreenContextBuffer();
    return screenContextBuffer;
}

/*static*/ void ScreenContextBuffer::setQmlEngine(QQmlApplicationEngine *engine)
{
    ScreenContextBuffer::m_qmlEngine = engine;
}

void ScreenContextBuffer::clearComponentCache()
{
    ScreenContextBuffer::m_qmlEngine->clearComponentCache();
}

void ScreenContextBuffer::setNestingAndIndex(const int& nesting, const int& idx)
{
    bool isChanged = false;

    if (mNesting != nesting)
    {
        mNesting = nesting;
        isChanged = true;
    }
    if (mSelectedIdx != idx)
    {
        mSelectedIdx = idx;
        isChanged = true;
    }

    if (isChanged)
        emit itemChanged();
}

int ScreenContextBuffer::nesting() const
{
    return mNesting;
}

int ScreenContextBuffer::selectedIdx() const
{
    return mSelectedIdx;
}

QString ScreenContextBuffer::loaderSource() const
{
    return mLoaderSource;
}

int ScreenContextBuffer::screenType() const
{
    return mScreenType;
}

bool ScreenContextBuffer::edit() const
{
    return mEdit;
}

int ScreenContextBuffer::courseId() const
{
    return mCourseId;
}

QString ScreenContextBuffer::courseName() const
{
    return mCourseName;
}

int ScreenContextBuffer::labsNesting() const
{
    return mLabsNesting;
}

int ScreenContextBuffer::labDisciplineId() const
{
    return mLabDisId;
}

int ScreenContextBuffer::labId() const
{
    return mLabId;
}

void ScreenContextBuffer::setNesting(const int& nesting)
{
    //qDebug() << "setNesting: " << nesting;

    if (mNesting != nesting)
    {
        mNesting = nesting;
        emit nestingChanged();
        qDebug() << "emit nestingChanged";
    }
}

void ScreenContextBuffer::setSelectedIdx(const int& idx)
{
//    qDebug() << "setSelectedIdx: " << idx;
    if (mSelectedIdx != idx)
    {
        mSelectedIdx = idx;
        emit selectedIdxChanged();
//        qDebug() << "emit selectedIdxChanged";
    }
}

void ScreenContextBuffer::setLoaderSource(const QString& path)
{
    //qDebug() << "setLoaderSource: " << path;

    if (mLoaderSource.compare(path))
    {
        mLoaderSource = path;
        emit loaderSourceChanged();
        //qDebug() << "emit loaderSourceChanged";
    }
    emit loaderSourceChanged();
}

void ScreenContextBuffer::setScreenType(const int& type)
{
//    qDebug() << "setScreenType: " << type;

    if (mScreenType != type)
    {
        mScreenType = type;
        emit screenTypeChanged();
//        qDebug() << "emit screenTypeChanged";
    }
}

void ScreenContextBuffer::setEdit(bool edit)
{
    //qDebug() << "setEdit: " << edit;

    if (mEdit != edit)
    {
        mEdit = edit;
        emit editChanged();
        //qDebug() << "emit editChanged";
    }
}

void ScreenContextBuffer::setCourseId(const int& id)
{
    mCourseId = id;
}

void ScreenContextBuffer::setCourseName(const QString& name)
{
    mCourseName = name;
}

void ScreenContextBuffer::setLabsNesting(const int& nesting)
{
    //qDebug() << "setLabsNesting: " << nesting;

    if (mLabsNesting != nesting)
    {
        mLabsNesting = nesting;
        emit labsNestingChanged();
        qDebug() << "emit labsNestingChanged";
    }
}

void ScreenContextBuffer::setLabDisciplineId(int id)
{
    //qDebug() << "setLabDisciplineId: " << nesting;

    if (mLabDisId != id)
    {
        mLabDisId = id;
        emit labDisciplineIdChanged();
        qDebug() << "emit labDisciplineIdChanged";
    }
}

void ScreenContextBuffer::setLabId(int id)
{
    //qDebug() << "setLabId: " << nesting;

    if (mLabId != id)
    {
        mLabId = id;
        emit labIdChanged();
        qDebug() << "emit labIdChanged";
    }
}
