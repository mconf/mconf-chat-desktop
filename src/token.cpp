#include "token.h"

token::token()
{
}

QString token::calcToken()
{
    int tokenLength = 8;
    QCA::SecureArray randomBytes(tokenLength);
    QString stringToken;

    randomBytes=QCA::Random::randomArray(tokenLength);
    stringToken = QCA::Hex().arrayToString(randomBytes).toAscii().data();
    return stringToken;
}
