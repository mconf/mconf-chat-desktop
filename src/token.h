#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <QtCrypto>

class token
{
public:
    token();
    static QString calcToken();
};

#endif // TOKEN_H
