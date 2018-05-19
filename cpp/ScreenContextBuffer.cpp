#include "ScreenContextBuffer.h"
#include <QDebug>

/*static*/ QQmlApplicationEngine *ScreenContextBuffer::m_qmlEngine = nullptr;

ScreenContextBuffer::ScreenContextBuffer(QObject *parent)
    : QObject(parent)
    , mNesting(0)
    , mSelectedIdx(0)
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
    setNesting(nesting);
    setSelectedIdx(idx);
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

void ScreenContextBuffer::setNesting(const int& nesting)
{
    qDebug() << "setNesting: " << nesting;

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
}

void ScreenContextBuffer::setScreenType(const int& type)
{
    qDebug() << "setScreenType: " << type;

    if (mScreenType != type)
    {
        mScreenType = type;
        emit screenTypeChanged();
        qDebug() << "emit screenTypeChanged";
    }
}
