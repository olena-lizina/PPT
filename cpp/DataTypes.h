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

struct Chapter {
    int id;
    QString name;
    int orderId;
    int disciplineId;

    bool operator==(const Chapter& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && orderId == rhs.orderId
                && disciplineId == rhs.disciplineId;
    }
};

struct DisciplineStud {
    int id;
    QString name;
    QString literPath;

    bool operator==(const DisciplineStud& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && literPath.compare(rhs.literPath);
    }
};

struct DisciplineTeach {
    int id;
    QString name;
    QString literPath;
    QString educPlanPath;
    QString educProgPath;

    bool operator==(const DisciplineTeach& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && educPlanPath.compare(rhs.educPlanPath)
                && educProgPath.compare(rhs.educProgPath);
    }
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

struct ThemeLectureFile {
    int id;
    int themeId;
    QString path;
};

struct SubthemeLectureFile {
    int id;
    int subthemeId;
    QString path;
};

struct Report {
    int id;
    int labId;
    int delivDate;
    QString mark;
    int evalDate;
    int studId;
};

struct ReportFile {
    int id;
    int reportId;
    QString path;
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

    bool operator==(const Subtheme& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && orderId == rhs.orderId
                && themeId == rhs.themeId;
    }
};

struct Theme {
    int id;
    QString name;
    int orderId;
    int chapterId;

    bool operator==(const Theme& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && orderId == rhs.orderId
                && chapterId == rhs.chapterId;
    }
};

#endif // LECTURE_H
