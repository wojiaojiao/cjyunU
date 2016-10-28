#ifndef NETWORKCOOKIE_H
#define NETWORKCOOKIE_H

#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

class NetworkCookie : public QNetworkCookieJar
{
    Q_OBJECT

public:
    NetworkCookie(QObject *parent);
    ~NetworkCookie();

    QList<QNetworkCookie> getCookies();


private:

};

#endif // NETWORKCOOKIE_H
