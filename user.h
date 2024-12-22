#ifndef USER_H
#define USER_H
#pragma once
#include <QString>
class User
{
public:
    QString username;
    QString email;
    QString phonenumber;
    int age;
    int id;
    QString sex;
    QString avatarUrl;
    User() {}
};

#endif // USER_H
