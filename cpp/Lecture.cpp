#include "Lecture.h"

Lecture::Lecture()
{
}

Lecture Lecture::operator=(const Lecture& rhs)
{
    if (this != &rhs)
    {
        mParts = rhs.getParts();
    }
    return *this;
}

bool Lecture::operator==(const Lecture& rhs)
{
    return mParts == rhs.getParts();
}

/*virtual*/ Lecture::~Lecture()
{}

QList<Part> Lecture::getParts() const
{
    return mParts;
}

QStringList Lecture::parts() const
{
    QStringList res;
    for (auto& part: mParts)
        res << part.part();
    return res;
}

QStringList Lecture::chapters(const QString& part)
{
    auto partIt = std::find_if(mParts.begin(), mParts.end(),
                             [&part](Part& p){ return !p.part().compare(part); });
    if (mParts.end() != partIt)
        return partIt->chapters();
    return QStringList();
}

QStringList Lecture::themes(const QString& part, const QString& chapter)
{
    auto partIt = std::find_if(mParts.begin(), mParts.end(),
                             [&part](Part& p){ return !p.part().compare(part); });
    if (mParts.end() == partIt)
        return QStringList();
    auto chapt = std::find_if(partIt->getChapters().begin(), partIt->getChapters().end(),
                             [&chapter](Chapter& ch){ return !ch.chapter().compare(chapter); });
    if (partIt->getChapters().end() == chapt)
        return QStringList();
    return chapt->themes();
}

QStringList Lecture::subThemes(const QString& part, const QString& chapter, const QString& theme)
{
    auto partIt = std::find_if(mParts.begin(), mParts.end(),
                             [&part](Part& p){ return !p.part().compare(part); });
    if (mParts.end() == partIt)
        return QStringList();
    auto chapt = std::find_if(partIt->getChapters().begin(), partIt->getChapters().end(),
                             [&chapter](Chapter& ch){ return !ch.chapter().compare(chapter); });
    if (partIt->getChapters().end() == chapt)
        return QStringList();
    auto themeIt = std::find_if(chapt->getThemes().begin(), chapt->getThemes().end(),
                             [&theme](Theme& th){ return !th.theme().compare(theme); });
    if (chapt->getThemes().end() == themeIt)
        return QStringList();
    return themeIt->subThemes();
}

void Lecture::addPart(const QString& part)
{
    mParts << Part(part);
}

void Lecture::addChapter(const QString& part, const QString& chapter)
{
    auto partIt = std::find_if(mParts.begin(), mParts.end(),
                             [&part](Part& p){ return !p.part().compare(part); });
    if (mParts.end() == partIt)
        return;
    partIt->chapter(chapter);
}

void Lecture::addTheme(const QString& part, const QString& chapter, const QString& theme)
{
    auto partIt = std::find_if(mParts.begin(), mParts.end(),
                             [&part](Part& p){ return !p.part().compare(part); });
    if (mParts.end() == partIt)
        return;
    auto chapt = std::find_if(partIt->getChapters().begin(), partIt->getChapters().end(),
                             [&chapter](Chapter& ch){ return !ch.chapter().compare(chapter); });
    if (partIt->getChapters().end() == chapt)
        return;
    chapt->theme(theme);
}

void Lecture::addSubTheme(const QString& part, const QString& chapter, const QString& theme, const QString& subTheme)
{
    auto partIt = std::find_if(mParts.begin(), mParts.end(),
                             [&part](Part& p){ return !p.part().compare(part); });
    if (mParts.end() == partIt)
        return;
    auto chapt = std::find_if(partIt->getChapters().begin(), partIt->getChapters().end(),
                             [&chapter](Chapter& ch){ return !ch.chapter().compare(chapter); });
    if (partIt->getChapters().end() == chapt)
        return;
    auto themeIt = std::find_if(chapt->getThemes().begin(), chapt->getThemes().end(),
                             [&theme](Theme& th){ return !th.theme().compare(theme); });
    if (chapt->getThemes().end() == themeIt)
        return;
    themeIt->subTheme(subTheme);
}

SubTheme::SubTheme(const QString& subTheme)
    : mSubTheme(subTheme)
{
}

bool SubTheme::operator==(const SubTheme& rhs)
{
    return !mSubTheme.compare(rhs.subTheme());
}

SubTheme SubTheme::operator=(const SubTheme& rhs)
{
    if (this == &rhs)
        return *this;

    mSubTheme = rhs.subTheme();
    return *this;
}

QString SubTheme::subTheme() const
{
    return mSubTheme;
}

Theme::Theme(const QString& theme)
    : mTheme(theme)
{
}

bool Theme::operator==(const Theme& rhs)
{
    return !mTheme.compare(rhs.theme()) && mSubThemes == rhs.getSubThemes();
}

Theme Theme::operator=(const Theme& rhs)
{
    if (this == &rhs)
        return *this;

    mTheme = rhs.theme();
    mSubThemes = rhs.getSubThemes();
    return *this;
}

QString Theme::theme() const
{
    return mTheme;
}

void Theme::subTheme(const QString& subTheme)
{
    mSubThemes << SubTheme(subTheme);
}

QStringList Theme::subThemes() const
{
    QStringList result;
    for (auto & subTheme : mSubThemes)
        result << subTheme.subTheme();
    return result;
}

QList<SubTheme> Theme::getSubThemes() const
{
    return mSubThemes;
}

Chapter::Chapter(const QString& chapter)
    : mChapter(chapter)
{
}

bool Chapter::operator==(const Chapter& rhs)
{
    return mChapter.compare(rhs.chapter()) && mThemes == rhs.getThemes();
}

Chapter Chapter::operator=(const Chapter& rhs)
{
    if (this == &rhs)
        return *this;

    mChapter = rhs.chapter();
    mThemes = rhs.getThemes();
    return *this;
}

QString Chapter::chapter() const
{
    return mChapter;
}

void Chapter::subTheme(const QString& theme, const QString& subTheme)
{
    auto res = std::find_if(mThemes.begin(), mThemes.end(),
                              [&theme](Theme& th){ return !th.theme().compare(theme); });
    if (mThemes.end() != res)
        res->subTheme(subTheme);
}

QStringList Chapter::subThemes(const QString& theme)
{
    auto res = std::find_if(mThemes.begin(), mThemes.end(),
                              [&theme](Theme& th){ return !th.theme().compare(theme); });
    if (mThemes.end() != res)
        return res->subThemes();
    return QStringList();
}

void Chapter::theme(const QString& theme)
{
    mThemes << Theme(theme);
}

QStringList Chapter::themes()
{
    QStringList themes;
    for (auto& theme : mThemes)
        themes << theme.theme();
    return themes;
}

QList<Theme> Chapter::getThemes() const
{
    return mThemes;
}

Part::Part(const QString& part)
    : mPart(part)
{
}

Part Part::operator=(const Part& rhs)
{
    if (this == &rhs)
        return *this;

    mPart = rhs.part();
    mChapters = rhs.getChapters();
    return *this;
}

bool Part::operator==(const Part& rhs)
{
    return mPart.compare(rhs.part()) && mChapters == rhs.getChapters();
}

QString Part::part() const
{
    return mPart;
}

void Part::chapter(const QString& chapter)
{
    mChapters << Chapter(chapter);
}

QStringList Part::chapters()
{
    QStringList chapters;
    for (auto& chapter : mChapters)
        chapters << chapter.chapter();
    return chapters;
}

QList<Chapter> Part::getChapters() const
{
    return mChapters;
}

void Part::theme(const QString& chapter, const QString& theme)
{
    auto res = std::find_if(mChapters.begin(), mChapters.end(),
                              [&chapter](Chapter& ch){ return !ch.chapter().compare(chapter); });
    if (mChapters.end() != res)
        res->theme(theme);
}

QStringList Part::themes(const QString& chapter)
{
    auto res = std::find_if(mChapters.begin(), mChapters.end(),
                              [&chapter](Chapter& ch){ return !ch.chapter().compare(chapter); });
    if (mChapters.end() != res)
        return res->themes();
    return QStringList();
}

void Part::subTheme(const QString& chapter, const QString& theme, const QString& subTheme)
{
    auto chapt = std::find_if(mChapters.begin(), mChapters.end(),
                              [&chapter](Chapter& ch){ return !ch.chapter().compare(chapter); });
    if (mChapters.end() == chapt)
        return;
    auto th = std::find_if(chapt->getThemes().begin(), chapt->getThemes().end(),
                           [&theme](Theme& t){ return !theme.compare(t.theme()); });
    if (chapt->getThemes().end() == th)
        return;
    th->subTheme(subTheme);
}

QStringList Part::subThemes(const QString& chapter, const QString& theme)
{
    auto chapt = std::find_if(mChapters.begin(), mChapters.end(),
                              [&chapter](Chapter& ch){ return !ch.chapter().compare(chapter); });
    if (mChapters.end() == chapt)
        return QStringList();
    auto th = std::find_if(chapt->getThemes().begin(), chapt->getThemes().end(),
                           [&theme](Theme& t){ return !theme.compare(t.theme()); });
    if (chapt->getThemes().end() == th)
        return QStringList();
    return th->subThemes();
}


