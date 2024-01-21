#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QUrl>
#include "html.hpp"
#include <QBitArray>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include "info_storage.h"
#include <nlohmann/json.hpp>

namespace GPP {
class search : public QObject
{
    Q_OBJECT
public:
    explicit search(QObject *parent = nullptr);


    void set_service();
    void set_service(int service_num);
    void save_service();

    void parcel_search(QString track_num);
    void parcel_search_from_url(QUrl url);

    void parcel_search_bool(QString track_num);
    void parcel_search_bool_from_url(QUrl url);

    void multiple_search(QStringList track_num_list);
    void multiple_search_from_url(QList<QUrl> url_list);

    void multiple_search_bool(QStringList track_num_list);
    void multiple_search_bool_from_url(QList<QUrl> url_list);

signals:
    void search_finished(QList<GPP::info_storage>);
    void search_finished_multiple(QList<QList<GPP::info_storage>>);
    void search_finished_bool(bool);
    void search_finished_multiple_bool(QBitArray);
    void parcel_info_recieved(QStringList);
    void parcel_url_recieved(QUrl);
private:
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply* reply;
    static QString user_agent;
    static QString service_url;
    html::parser p;
    QNetworkCookieJar* cookieJar = new QNetworkCookieJar(&manager);
    QList<QNetworkCookie> cookieList;
    QByteArray postData;
    QString cleanString(std::string& str);
    QList<GPP::info_storage> parse_html(std::string html, QUrl site_url);
    nlohmann::json j_array = nlohmann::json::array();
    nlohmann::json j;
};


}
#endif // SEARCH_H


