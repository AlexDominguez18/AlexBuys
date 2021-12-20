#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonArray>
#include <QJsonObject>

class User
{
private:
    QString userName;
    QString email;
    QString passWord;
    QJsonArray purchase;
public:
    User();

    QString getUserName() const;
    void setUserName(const QString &value);

    QString getEmail() const;
    void setEmail(const QString &value);

    QString getPassWord() const;
    void setPassWord(const QString &value);

    QJsonArray getPurchase() const;
    void setPurchase(const QJsonArray &value);

    User& operator = (User& u);

};

#endif // USER_H
