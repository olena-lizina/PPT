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
#ifndef STUDENTMODEL_H
#define STUDENTMODEL_H
#include <QObject>
#include <QString>
#include "DataTypes.h"

class StudentModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString name  READ name  WRITE setName  NOTIFY nameChanged)
    Q_PROPERTY(QString phone READ phone WRITE setPhone NOTIFY phoneChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString photo READ photo WRITE setPhoto NOTIFY photoChanged)
    Q_PROPERTY(QString group READ group WRITE setGroup NOTIFY groupChanged)

public:
    StudentModel(const Student& studInfo, const QString& group)
        : QObject(nullptr)
        , mId(studInfo.id)
        , mName(studInfo.name)
        , mPhone(studInfo.phone)
        , mEmail(studInfo.email)
        , mPhoto(studInfo.photoPath)
        , mGroup(group)
    {}

public slots:    

    int id() const { return mId; }
    QString name() const { return mName; }
    QString phone() const { return mPhone; }
    QString email() const { return mEmail; }
    QString photo() const { return mPhoto; }
    QString group() const { return mGroup; }

    void setName(QString name) { mName = name; }
    void setPhone(QString phone) { mPhone = phone; }
    void setEmail(QString email) { mEmail = email; }
    void setPhoto(QString photo) { mPhoto = photo; }
    void setGroup(QString group) { mGroup = group; }

signals:

    void nameChanged();
    void phoneChanged();
    void emailChanged();
    void photoChanged();
    void groupChanged();

private:
    const int mId;
    QString mName;
    QString mPhone;
    QString mEmail;
    QString mPhoto;
    QString mGroup;
};
#endif
