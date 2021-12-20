#include "user.h"

User::User()
{

}

QString User::getUserName() const
{
    return userName;
}

void User::setUserName(const QString &value)
{
    userName = value;
}

QString User::getEmail() const
{
    return email;
}

void User::setEmail(const QString &value)
{
    email = value;
}

QString User::getPassWord() const
{
    return passWord;
}

void User::setPassWord(const QString &value)
{
    passWord = value;
}

User &User::operator =(User &u)
{
    this->userName = u.userName;
    this->email = u.email;
    this->passWord = u.passWord;

    return *this;
}

QJsonArray User::getPurchase() const
{
    return purchase;
}

void User::setPurchase(const QJsonArray &value)
{
    purchase = value;
}

