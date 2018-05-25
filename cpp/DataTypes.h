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
#ifndef DATATYPES_H
#define DATATYPES_H
#include <QString>
#include <QList>

struct BaseItem {
    virtual ~BaseItem() {}
    int id;
};

struct Chapter: public BaseItem {
    int id;
    QString name;
    int disciplineId;

    Chapter(int id_, QString name_, int discId_)
        : BaseItem(id_)
        , name(name_)
        , disciplineId(discId_)
    {
    }

    bool operator==(const Chapter& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && disciplineId == rhs.disciplineId;
    }
};

struct Discipline: public BaseItem {
    int id;
    QString name;
    QString literPath;
    QString educPlanPath;
    QString educProgPath;

    Discipline(int id_, QString name_, QString literPath_, QString educPlanPath_, QString educProgPath_)
        : BaseItem(id_)
        , name(name_)
        , literPath(literPath_)
        , educPlanPath(educPlanPath_)
        , educProgPath(educProgPath_)
    {}

    bool operator==(const Discipline& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && educPlanPath.compare(rhs.educPlanPath)
                && educProgPath.compare(rhs.educProgPath);
    }
};

struct Group: public BaseItem {
    int id;
    QString name;

    Group(int id_, QString name_)
        : BaseItem(id_)
        , name(name_)
    {}

    bool operator==(const Group& rhs)
    {
        return id == rhs.id && name == rhs.name;
    }
};

struct LabWork: public BaseItem {
    int id;
    int disciplineId;
    QString finishDate;
    QString name;
    QString path;

    LabWork(int id_, int disciplineId_, QString finishDate_, QString name_, QString path_)
        :BaseItem(id_)
        , disciplineId(disciplineId_)
        , finishDate(finishDate_)
        , name(name_)
        , path(path_)
    {}

    bool operator==(const LabWork& rhs)
    {
        return id == rhs.id
                && disciplineId == rhs.disciplineId
                && finishDate.compare(rhs.finishDate)
                && name.compare(rhs.name)
                && path.compare(rhs.path);
    }
};

struct ThemeLectureFile: public BaseItem {
    int id;
    int themeId;
    QString path;

    ThemeLectureFile(int id_, int themeId_, QString path_)
        : BaseItem(id_)
        , themeId(themeId_)
        , path(path_)
    {}

    bool operator==(const ThemeLectureFile& rhs)
    {
        return id == rhs.id
                && themeId == rhs.themeId
                && path.compare(rhs.path);
    }
};

struct SubthemeLectureFile: public BaseItem {
    int id;
    int subthemeId;
    QString path;

    SubthemeLectureFile(int id_, int subthemeId_, QString path_)
        : BaseItem(id_)
        , subthemeId(subthemeId_)
        , path(path_)
    {}

    bool operator==(const SubthemeLectureFile& rhs)
    {
        return id == rhs.id
                && subthemeId == rhs.subthemeId
                && path.compare(rhs.path);
    }
};

struct Report: public BaseItem {
    int id;
    int labId;
    int delivDate;
    QString mark;
    int evalDate;
    int studId;

    Report()
    {}

    bool operator==(const Report& rhs)
    {
        return id == rhs.id
                && labId == rhs.labId
                && delivDate == rhs.delivDate
                && mark.compare(rhs.mark)
                && evalDate == rhs.evalDate
                && studId == rhs.studId;
    }
};

struct ReportFile: public BaseItem {
    int id;
    int reportId;
    QString path;

    bool operator==(const ReportFile& rhs)
    {
        return id == rhs.id
                && reportId == rhs.reportId
                && path.compare(rhs.path);
    }
};

struct Student: public BaseItem {
    int id;
    QString name;
    QString phone;
    QString email;
    QString photoPath;
    int groupId;

    bool operator==(const Student& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && phone.compare(rhs.phone)
                && email.compare(rhs.email)
                && photoPath.compare(rhs.photoPath)
                && groupId == rhs.groupId;
    }
};

struct Subtheme: public BaseItem {
    int id;
    QString name;
    int themeId;

    bool operator==(const Subtheme& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && themeId == rhs.themeId;
    }
};

struct Theme: public BaseItem {
    int id;
    QString name;
    int chapterId;

    bool operator==(const Theme& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && chapterId == rhs.chapterId;
    }
};

#endif // DATATYPES_H
