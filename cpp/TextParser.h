#ifndef TEXTPARSER_H
#define TEXTPARSER_H
#include <QObject>

class TextParser: public QObject
{
    Q_OBJECT
public:
    explicit TextParser(QObject* parent = nullptr);
    virtual ~TextParser();
};

#endif // TEXTPARSER_H
