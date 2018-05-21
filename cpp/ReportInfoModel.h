/****************************************************************************
**
** Copyright (C) 2017-2018 Olena Lizina
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/
#ifndef REPORTINFOMODEL_H
#define REPORTINFOMODEL_H
#include <QObject>
#include <QString>
#include "DataTypes.h"

class ReportInfoModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString studName READ studName WRITE setStudName NOTIFY studNameChanged)
    Q_PROPERTY(QString finished READ finished WRITE setFinished NOTIFY finishedChanged)
    Q_PROPERTY(QString mark     READ mark     WRITE setMark     NOTIFY markChanged    )
    Q_PROPERTY(QString evalDate READ evalDate WRITE setEvalDate NOTIFY evalDateChanged)
    Q_PROPERTY(QString path     READ path     WRITE setPath     NOTIFY pathChanged    )

public:
    ReportInfoModel(const QString studName, const Report rep, const QString reportPath)
        : QObject(nullptr)
        , mId(rep.id)
        , mStudName(studName)
        , mFinishDate(rep.delivDate)
        , mMark(rep.mark)
        , mEvalDate(rep.evalDate)
        , mPath(reportPath)
    {}

public slots:    

    int id() const { return mId; }
    QString studName() const { return mStudName; }
    QString finished() const { return mFinishDate; }
    QString mark() const { return mMark; }
    QString evalDate() const { return mEvalDate; }
    QString path() const { return mPath; }

    void setStudName(QString name) { mStudName = name; }
    void setFinished(QString finish) { mFinishDate = finish; }
    void setMark(QString mark) { mMark = mark; }
    void setEvalDate(QString eval) { mEvalDate = eval; }
    void setPath(QString path) { mPath = path; }

signals:

    void studNameChanged();
    void finishedChanged();
    void markChanged    ();
    void evalDateChanged();
    void pathChanged    ();

private:
    const int mId;
    QString mStudName;
    QString mFinishDate;
    QString mMark;
    QString mEvalDate;
    QString mPath;
};
#endif // REPORTINFOMODEL_H
