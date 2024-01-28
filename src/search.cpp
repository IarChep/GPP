#include "search.h"

QString GPP::search::service_url;
QString GPP::search::user_agent;

GPP::search::search(QObject *parent)
    : QObject{parent}
{
    QFile mdata("./gpp/service.txt");
    if (mdata.open(QIODevice::ReadWrite | QIODevice::Text)) {
        if(mdata.size() != 0) {
            service_url = mdata.readAll();
        }
    }
    if(this->service_url.isEmpty()) {
        this->service_url = "https://gdeposylka.ru/";
    }
    user_agent = request.header(QNetworkRequest::UserAgentHeader).toString();
    if(user_agent.contains("Mobile")) {
        user_agent.replace("Mobile", "Desktop");
    }
}
void GPP::search::save_service() {
    QFile mdata("./gpp/service.txt");
    QFileInfo minfo(mdata);
    if (!mdata.exists()) {
        QDir dir;
        if (!dir.exists(minfo.absolutePath())) {
            dir.mkpath(minfo.absolutePath());
        }
    }
    if (mdata.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&mdata);
        out << service_url;
        mdata.close();
    }
}
void GPP::search::set_service() {
    qDebug() << "Select a tracking service.\n 1 - for GdePosylka(Russian version)\n2 - for PackageRadar(English version)";
}
void GPP::search::set_service(int service_num) {
    if (service_num == 1) {
        service_url = "https://gdeposylka.ru/";
    } else if (service_num == 2) {
        service_url = "https://packageradar.com/";
    }
    GPP::search::save_service();
}
void GPP::search::parcel_search(QString track_num) {
    request.setUrl(QUrl(service_url+"form"));
    request.setRawHeader("User-Agent", user_agent.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    cookieJar->setCookiesFromUrl(cookieList, QUrl("https://gdeposylka.ru/form"));
    manager.setCookieJar(cookieJar);
    postData.clear();
#if QT_VERSION_MAJOR >= 6
    postData.append("tracking_form[tracking_number]="+track_num.toStdString());
#else
    postData.append("tracking_form[tracking_number]="+track_num);
#endif
    reply = manager.post(request, postData);

    QObject::connect(reply, &QNetworkReply::finished, [&] {
       if (reply->error() == QNetworkReply::NoError) {
            QByteArray html_cba = reply->readAll();
            std::string html_code(html_cba.constData(), html_cba.length());
            emit search_finished(GPP::search::parse_html(html_code, reply->url()));
        } else {
            GPP::info_storage is;
            is.error = "Connection error!";
            emit search_finished(QList<GPP::info_storage>() << is);
       }
    });
}
void GPP::search::parcel_search_from_url(QUrl url) {
    request.setUrl(url);
    request.setRawHeader("User-Agent", user_agent.toUtf8());
    cookieJar->setCookiesFromUrl(cookieList, url);
    manager.setCookieJar(cookieJar);
    reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [&] {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray html_cba = reply->readAll();
            std::string html_code(html_cba.constData(), html_cba.length());
            emit search_finished(GPP::search::parse_html(html_code, reply->url()));
        } else {
            GPP::info_storage is;
            is.error = "Connection error!";
            emit search_finished(QList<GPP::info_storage>() << is);
        }
    });
}
void GPP::search::parcel_search_bool(QString track_num) {
    request.setUrl(QUrl(service_url+"form"));
    request.setRawHeader("User-Agent", user_agent.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    cookieJar->setCookiesFromUrl(cookieList, QUrl(service_url+"form"));
    manager.setCookieJar(cookieJar);
    postData.clear();
#if QT_VERSION_MAJOR >= 6
    postData.append("tracking_form[tracking_number]="+track_num.toStdString());
#else
    postData.append("tracking_form[tracking_number]="+track_num);
#endif
    reply = manager.post(request, postData);

    QObject::connect(reply, &QNetworkReply::finished, [&] {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray html_cba = reply->readAll();
            std::string html_code(html_cba.constData(), html_cba.length());

            html::node_ptr node = p.parse(html_code);
            std::vector<html::node*> selected = node->select("small.form-text.text-danger");
            if (selected.empty()) {
                emit search_finished_bool(true);
            } else {
                emit search_finished_bool(false);
            }
        } else {
            GPP::info_storage is;
            is.error = "Connection error!";
            emit search_finished(QList<GPP::info_storage>() << is);
        }
    });
}
void GPP::search::parcel_search_bool_from_url(QUrl url) {
    request.setUrl(url);
    request.setRawHeader("User-Agent", user_agent.toUtf8());
    cookieJar->setCookiesFromUrl(cookieList, url);
    manager.setCookieJar(cookieJar);
    reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [&] {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray html_cba = reply->readAll();
            std::string html_code(html_cba.constData(), html_cba.length());

            html::node_ptr node = p.parse(html_code);
            std::vector<html::node*> selected = node->select("small.form-text.text-danger");
            if (selected.empty()) {
                emit search_finished_bool(true);
            } else {
                emit search_finished_bool(false);
            }
        } else {
            GPP::info_storage is;
            is.error = "Connection error!";
            emit search_finished(QList<GPP::info_storage>() << is);
        }
    });
}

void GPP::search::multiple_search(QStringList track_num_list) {
    request.setUrl(QUrl(service_url+"form"));
    request.setRawHeader("User-Agent", user_agent.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    cookieJar->setCookiesFromUrl(cookieList, QUrl(service_url+"form"));
    QList<QList<GPP::info_storage>> f_result;
    manager.setCookieJar(cookieJar);
    for(QString i : track_num_list) {
        postData.clear();
#if QT_VERSION_MAJOR >= 6
        postData.append("tracking_form[tracking_number]="+i.toStdString());
#else
        postData.append("tracking_form[tracking_number]="+i);
#endif
        reply = manager.post(request, postData);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, [&] {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray html_cba = reply->readAll();
                std::string html_code(html_cba.constData(), html_cba.length());
                f_result.push_back(GPP::search::parse_html(html_code, reply->url()));
            } else {
                GPP::info_storage is;
                is.error = "Connection error!";
                emit search_finished(QList<GPP::info_storage>() << is);
                loop.quit();
                return;
            }
            loop.quit();
        });
        loop.exec();
    }
    emit search_finished_multiple(f_result);
}
void GPP::search::multiple_search_from_url(QList<QUrl> url_list) {
    QList<QList<GPP::info_storage>> f_result;
    request.setRawHeader("User-Agent", user_agent.toUtf8());
    for(QUrl url : url_list) {
        request.setUrl(url);
        cookieJar->setCookiesFromUrl(cookieList, url);
        manager.setCookieJar(cookieJar);
        reply = manager.get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, [&] {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray html_cba = reply->readAll();
                std::string html_code(html_cba.constData(), html_cba.length());
                f_result.push_back(GPP::search::parse_html(html_code, reply->url()));
            } else {
                GPP::info_storage is;
                is.error = "Connection error!";
                emit search_finished(QList<GPP::info_storage>() << is);
                loop.quit();
                return;
            }
            loop.quit();
        });
        loop.exec();
    }
    emit search_finished_multiple(f_result);
}
void GPP::search::multiple_search_bool(QStringList track_num_list) {
    request.setUrl(QUrl(service_url+"form"));
    request.setRawHeader("User-Agent", user_agent.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    cookieJar->setCookiesFromUrl(cookieList, QUrl(service_url+"form"));
    QBitArray f_result(track_num_list.length());
    manager.setCookieJar(cookieJar);
    for(int i = 0; i < track_num_list.length(); i++) {
        postData.clear();
#if QT_VERSION_MAJOR >= 6
        postData.append("tracking_form[tracking_number]="+track_num_list[i].toStdString());
#else
        postData.append("tracking_form[tracking_number]="+track_num_list[i]);
#endif
        reply = manager.post(request, postData);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, [&] {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray html_cba = reply->readAll();
                std::string html_code(html_cba.constData(), html_cba.length());

                html::node_ptr node = p.parse(html_code);
                std::vector<html::node*> selected = node->select("small.form-text.text-danger");
                if (selected.empty()) {
                    f_result[i]=true;
                } else {
                    f_result[i]=false;
                }
            } else {
                GPP::info_storage is;
                is.error = "Connection error!";
                emit search_finished(QList<GPP::info_storage>() << is);
                loop.quit();
                return;
            }
            loop.quit();
        });
        loop.exec();
    }
    emit search_finished_multiple_bool(f_result);
}
void GPP::search::multiple_search_bool_from_url(QList<QUrl> url_list) {
    QBitArray f_result(url_list.length());
    request.setRawHeader("User-Agent", user_agent.toUtf8());
    for(int i = 0; i < url_list.length(); i++) {
        request.setUrl(url_list.at(i));
        cookieJar->setCookiesFromUrl(cookieList, url_list.at(i));
        manager.setCookieJar(cookieJar);
        reply = manager.get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, [&] {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray html_cba = reply->readAll();
                std::string html_code(html_cba.constData(), html_cba.length());

                html::node_ptr node = p.parse(html_code);
                std::vector<html::node*> selected = node->select("small.form-text.text-danger");
                if (selected.empty()) {
                    f_result[i]=true;
                } else {
                    f_result[i]=false;
                }
            } else {
                GPP::info_storage is;
                is.error = "Connection error!";
                emit search_finished(QList<GPP::info_storage>() << is);
                loop.quit();
                return;
            }
            loop.quit();
        });
        loop.exec();
    }
    emit search_finished_multiple_bool(f_result);
}
QList<GPP::info_storage> GPP::search::parse_html(std::string html, QUrl site_url) {
    html::node_ptr node = p.parse(html);
    std::vector<html::node*> selected = node->select("small.form-text.text-danger");
    QList<GPP::info_storage> results;
    GPP::info_storage result;
    QStringList info_list;
    if (selected.empty()) {
        emit parcel_url_recieved(site_url);
        std::vector<html::node*> uls = node->select("ul.checkpoints");
        std::vector<html::node*> cards = node->select(".card-body dt,dd,p");
        std::vector<html::node*> pcards = node->select(".card-body p");
        for(html::node* card : cards) {
            std::string text = card->to_text();
            info_list.push_back(GPP::search::cleanString(text));
        }
        if(!pcards.empty()) {
            info_list.erase(info_list.begin() + 1);
        }
        emit parcel_info_recieved(info_list);
        for(html::node* ul : uls) {
            std::vector<html::node*> lis = ul->select("li");
            for(html::node* li : lis) {
                std::string deliveryStatus;
                if(!li->select("span.td.status.down.d-print-none").empty()) {
                    deliveryStatus = "Delivered";
                } else if(!li->select("span.td.status.up.d-print-none").empty()) {
                    deliveryStatus = "Info recieved";
                } else if(!li->select("span.td.status.d-print-none").empty()) {
                    deliveryStatus = "Delivering";
                }

                std::string dateAndTime = li->select("time.datetime2")[0]->to_text();
                std::string status = li->select("strong.checkpoint-status")[0]->to_text();
                std::string deliveryService = li->select("em.text-muted")[0]->to_text();
                std::string icon_raw = li->select("span.td.info.status-iconed")[0]->get_attr("style");
                std::string icon_final;

                std::size_t start = icon_raw.find("'");
                if (start != std::string::npos) {
                    std::size_t end = icon_raw.find("'", start + 1);
                    if (end != std::string::npos) {
                        icon_final = icon_raw.substr(start + 1, end - start - 1);
                    }
                }
                result.delivery_state=QString::fromStdString(deliveryStatus);
                result.status=GPP::search::cleanString(status);
                result.date = GPP::search::cleanString(dateAndTime);
                result.delivery_service = GPP::search::cleanString(deliveryService);
                result.icon = GPP::search::cleanString(icon_final);
                results.emplace_back(result);
            }
        }
    } else {
        for(html::node* i : selected) {
            result.error = QString::fromStdString(i->to_text());
            results.push_back(result);
        }
    }
    return results;
}
QString GPP::search::cleanString(std::string& str) {
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(0, str.find_first_not_of(' '));
    str.erase(std::find_if(str.rbegin(), str.rend(), [&](char ch) { return !std::isspace(ch); }).base(), str.end());
    str.erase(std::unique(str.begin(), str.end(),[&](char a, char b)  { return std::isspace(a) && std::isspace(b); }), str.end());
    return QString::fromStdString(str);
}
