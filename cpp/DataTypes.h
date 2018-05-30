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

    BaseItem(int id_)
        : id(id_)
    {}

    virtual ~BaseItem() {}
    int id;
};

struct Chapter: public BaseItem {
    QString name;
    int disciplineId;

    Chapter(int id_ = 0, QString name_ = "", int discId_ = 0)
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
    QString name;
    QString literPath;
    QString educPlanPath;
    QString educProgPath;

    Discipline(int id_ = 0, QString name_ = "",
               QString literPath_ = "",
               QString educPlanPath_ = "",
               QString educProgPath_ = "")
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
    QString name;

    Group(int id_ = 0, QString name_ = "")
        : BaseItem(id_)
        , name(name_)
    {}

    bool operator==(const Group& rhs)
    {
        return id == rhs.id && name == rhs.name;
    }
};

struct LabWork: public BaseItem {
    int disciplineId;
    QString finishDate;
    QString name;
    QString path;

    LabWork(int id_ = 0, int disciplineId_ = 0, QString finishDate_ = "", QString name_ = "", QString path_ = "")
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
    int themeId;
    QString path;

    ThemeLectureFile(int id_ = 0, int themeId_ = 0, QString path_ = "")
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
    int subthemeId;
    QString path;

    SubthemeLectureFile(int id_ = 0, int subthemeId_ = 0, QString path_ = "")
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
    int labId;
    QString delivDate;
    QString mark;
    QString evalDate;
    int studId;

    Report(int id_ = 0, int labId_ = 0, QString delivDate_ = "",
           QString mark_ = "", QString evalDate_ = "", int studId_ = 0)
        : BaseItem(id_)
        , labId(labId_)
        , delivDate(delivDate_)
        , mark(mark_)
        , evalDate(evalDate_)
        , studId(studId_)
    {}

    bool operator==(const Report& rhs)
    {
        return id == rhs.id
                && labId == rhs.labId
                && delivDate.compare(rhs.delivDate)
                && mark.compare(rhs.mark)
                && evalDate.compare(rhs.evalDate)
                && studId == rhs.studId;
    }
};

struct ReportFile: public BaseItem {
    int reportId;
    QString path;

    ReportFile(int id_ = 0, int reportId_ = 0, QString path_ = "")
        : BaseItem(id_)
        , reportId(reportId_)
        , path(path_)
    {}

    bool operator==(const ReportFile& rhs)
    {
        return id == rhs.id
                && reportId == rhs.reportId
                && path.compare(rhs.path);
    }
};

struct Student: public BaseItem {
    QString name;
    QString phone;
    QString email;
    QString photoPath;
    int groupId;

    Student(int id_ = 0, QString name_ = "",
            QString phone_ = "", QString email_ = "",
            QString photoPath_ = "", int groupId_ = 0)
        : BaseItem(id_)
        , name(name_)
        , phone(phone_)
        , email(email_)
        , photoPath(photoPath_)
        , groupId(groupId_)
    {}

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
    QString name;
    int themeId;

    Subtheme(int id_ = 0, QString name_ = "", int themeId_ = 0)
        : BaseItem(id_)
        , name(name_)
        , themeId(themeId_)
    {}

    bool operator==(const Subtheme& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && themeId == rhs.themeId;
    }
};

struct Theme: public BaseItem {
    QString name;
    int chapterId;

    Theme(int id_ = 0, QString name_ = "", int chapterId_ = 0):
        BaseItem(id_)
      , name(name_)
      , chapterId(chapterId_)
    {}

    bool operator==(const Theme& rhs)
    {
        return id == rhs.id
                && name.compare(rhs.name)
                && chapterId == rhs.chapterId;
    }
};

#endif // DATATYPES_H
