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

class SubTheme;
class Theme;
class Chapter;
class Part;

class Lecture
{
public:
    Lecture();
    Lecture operator=(const Lecture& rhs);
    bool operator==(const Lecture& rhs);
    virtual ~Lecture();

    QList<Part> getParts() const;
    QStringList parts() const;
    QStringList chapters(const QString& part);
    QStringList themes(const QString& part, const QString& chapter);
    QStringList subThemes(const QString& part, const QString& chapter, const QString& theme);

    void addPart(const QString& part);
    void addChapter(const QString& part, const QString& chapter);
    void addTheme(const QString& part, const QString& chapter, const QString& theme);
    void addSubTheme(const QString& part, const QString& chapter, const QString& theme, const QString& subTheme);

protected:
    QList<Part> mParts;
};

class SubTheme
{
public:
    SubTheme(const QString& subTheme);
    SubTheme operator=(const SubTheme& rhs);
    bool operator==(const SubTheme& rhs);

    QString subTheme() const;

protected:
    QString mSubTheme;
};

class Theme
{
public:
    Theme(const QString& theme);
    Theme operator=(const Theme& rhs);
    bool operator==(const Theme& rhs);

    QString theme() const;

    void subTheme(const QString& subTheme);
    QStringList subThemes() const;
    QList<SubTheme> getSubThemes() const;

protected:
    QString mTheme;
    QList<SubTheme> mSubThemes;
};

class Chapter
{
public:
    Chapter(const QString& chapter);
    Chapter operator=(const Chapter& rhs);
    bool operator==(const Chapter& rhs);

    QString chapter() const;

    void theme(const QString& theme);
    QStringList themes();
    QList<Theme> getThemes() const;

    void subTheme(const QString& theme, const QString& subTheme);
    QStringList subThemes(const QString& theme);

protected:
    QString mChapter;
    QList<Theme> mThemes;
};

class Part
{
public:
    Part(const QString& part);
    Part operator=(const Part& rhs);
    bool operator==(const Part& rhs);

    QString part() const;

    void chapter(const QString& chapter);
    QStringList chapters();
    QList<Chapter> getChapters() const;

    void theme(const QString& chapter, const QString& theme);
    QStringList themes(const QString& chapter);

    void subTheme(const QString& chapter, const QString& theme, const QString& subTheme);
    QStringList subThemes(const QString& chapter, const QString& theme);

protected:
    QString mPart;
    QList<Chapter> mChapters;
};

#endif // LECTURE_H
