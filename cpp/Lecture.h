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
    void setId(const int& id) { mId = id; }
    void incrementId() { ++mId; }
    void setParentId(const int& id) { mParentId = id; }
    void setFileName(const QString& file) { mFileName = file; }

protected:
    QString mName;
    QString mFileName;
    int mId;
    int mParentId;
};

struct Chapter {
    int id;
    QString name;
    int orderId;
    int disciplineId;
};

struct DisciplineStud {
    int id;
    QString name;
    QString literPath;
};

struct DisciplineTeach {
    int id;
    QString name;
    QString literPath;
    QString educPlanPath;
    QString educProgPath;
};

struct Group {
    int id;
    QString name;
};

struct LabWork {
    int id;
    int themeId;
    int finishDate;
    QString name;
    QString path;
};
struct relLabWork {
    int disciplineId;
    int chapterId;
    LabWork labWork;
};

struct ThemeLectureFile {
    int id;
    int themeId;
    QString path;
};
struct relThemeLectureFile {
    int disciplineId;
    int chapterId;
    ThemeLectureFile themeLectureFile;
};

struct SubthemeLectureFile {
    int id;
    int subthemeId;
    QString path;
};
struct relSubthemeLectureFile {
    int disciplineId;
    int chapterId;
    int themeId;
    SubthemeLectureFile subthemeLectureFile;
};

struct Report {
    int id;
    int labId;
    int delivDate;
    QString mark;
    int evalDate;
    int studId;
};
struct relReport {
    int disciplineId;
    int chapterId;
    int themeId;
    Report report;
};

struct ReportFile {
    int id;
    int reportId;
    QString path;
};
struct relReportFile {
    int disciplineId;
    int chapterId;
    int themeId;
    int labId;
    ReportFile reportFile;
};

struct Student {
    int id;
    QString name;
    QString phone;
    QString email;
    QString photoPath;
    int groupId;
};

struct Subtheme {
    int id;
    QString name;
    int orderId;
    int themeId;
};
struct relSubtheme {
    int disciplineId;
    int chapterId;
    Subtheme subtheme;
};

struct Theme {
    int id;
    QString name;
    int orderId;
    int chapterId;
};
struct relTheme {
    int disciplineId;
    Theme theme;
};

#endif // LECTURE_H
