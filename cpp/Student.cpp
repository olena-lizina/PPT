#include "Student.h"

Student::Student(const QString& name, const QString& phone, const QString& email, const QString& group)

    : mName(name)
    , mPhone(phone)
    , mEmail(email)
    , mGroup(group)
{
}

Student Student::operator=(const Student& rhs)
{
    if (this != &rhs)
    {
        mName = rhs.name();
        mPhone = rhs.phone();
        mEmail = rhs.email();
        mGroup = rhs.group();
    }
    return *this;
}

bool Student::operator==(const Student& rhs)
{
    return !name().compare(rhs.name()) && !group().compare(rhs.group());
}

/*virtual*/ Student::~Student()
{
}

QString Student::name() const
{
    return mName;
}

QString Student::phone() const
{
    return mPhone;
}

QString Student::email() const
{
    return mEmail;
}

QString Student::group() const
{
    return mGroup;
}
