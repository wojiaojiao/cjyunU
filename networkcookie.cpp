#include "networkcookie.h"

NetworkCookie::NetworkCookie(QObject *parent)
    : QNetworkCookieJar(parent)
{

}

NetworkCookie::~NetworkCookie()
{

}

QList<QNetworkCookie> NetworkCookie::getCookies()
{
    return allCookies();
}


