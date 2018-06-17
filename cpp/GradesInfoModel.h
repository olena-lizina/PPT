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
#ifndef GRADESINFOMODEL_H
#define GRADESINFOMODEL_H
#include <QObject>
#include <QString>
#include "DataTypes.h"

class GradesInfoModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int studId       READ studId   WRITE setStudId   NOTIFY studIdChanged  )
    Q_PROPERTY(int labId        READ labId    WRITE setLabId    NOTIFY labIdChanged   )
    Q_PROPERTY(QString studName READ studName WRITE setStudName NOTIFY studNameChanged)
    Q_PROPERTY(QString labName  READ labName  WRITE setLabName  NOTIFY labNameChanged )
    Q_PROPERTY(QString mark     READ mark     WRITE setMark     NOTIFY markChanged    )

public:
    GradesInfoModel(const int studId, const QString studName, const int labId, const QString labName, const QString mark)
        : QObject(nullptr)
        , mStudName(studName)
        , mMark(mark)
        , mLabName(labName)
        , mStudId(studId)
        , mLabId(labId)
    {}

public slots:    

    QString studName() const { return mStudName; }
    QString mark() const { return mMark; }
    QString labName() const { return mLabName; }
    int studId() const { return mStudId; }
    int labId() const { return mLabId; }

    void setStudName(QString name) { mStudName = name; }
    void setMark(QString mark) { mMark = mark; }
    void setLabName(QString labName) { mLabName = labName; }
    void setStudId(int id) { mStudId = id; }
    void setLabId(int id) { mLabId = id; }

signals:

    void studNameChanged();
    void markChanged    ();
    void studIdChanged  ();
    void labIdChanged   ();
    void labNameChanged ();

private:
    QString mStudName;
    QString mMark;
    QString mLabName;
    int mStudId;
    int mLabId;
};
#endif // GRADESINFOMODEL_H
