#include "Student.h"

Student::Student(const QString& name, const QString& phone, const QString& email, const QString& group)
    : mName(name)
    , mPhone(phone)
    , mEmail(email)
    , mGroup(group)
{
}

/*virtual*/ ~Student()
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
