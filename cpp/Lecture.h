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
#ifndef LECTURE_H
#define LECTURE_H
#include <QString>
#include <QList>

class LecturePart
{
public:
    LecturePart(const QString& name, const int& id, const int& parentId):
        mName(name), mId(id), mParentId(parentId) {}

    bool operator==(const LecturePart& rhs)
    {
        return mId == rhs.getId()
                && mParentId == rhs.getParentId()
                && !mName.compare(rhs.getName());
    }

    QString getName() const { return mName; }
    QString getFileName() const { return mFileName; }
    int getId() const { return mId; }
    int getParentId() const { return mParentId; }
    void setName(const QString& name) { mName = name; }
    void setFileName(const QString& file) { mFileName = file; }

protected:
    QString mName;
    QString mFileName;
    int mId;
    int mParentId;
};

#endif // LECTURE_H
