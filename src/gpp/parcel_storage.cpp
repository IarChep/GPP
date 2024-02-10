#include "parcel_storage.h"

void GPP::parcel_storage::delete_parsel(int index) {
    try {
        if (index >= 0 && index < this->parcels.size()) {
            this->parcels.removeAt(index);
        } else {
            throw std::out_of_range("Index out of range. This function takes an index to identify your parcel. Index - is a numeric place of the element in list? it is lower that real place for 1. So if your parcel is second, it's index will be 1");
        }
    } catch (const std::out_of_range& e) {
        qDebug() << "Caught an exception: " << e.what();
    }
}
void GPP::parcel_storage::save_to_json(const QString &path) {
    for (int i = 0; i < this->parcels.size(); i++) {
        QJsonObject parcel_obj;
        parcel_obj.insert("name",QJsonValue(this->parcels.at(i).name));
        parcel_obj.insert("track_num", QJsonValue(this->parcels.at(i).track_num));
        parcel_obj.insert("gp_url", QJsonValue(this->parcels.at(i).gp_url));
        for(QString is : this->parcels.at(i).info) {
            p_info.push_back(QJsonValue(is));
        }
        for(GPP::info_storage i : this->parcels.at(i).shipping_state) {
            QString data;
            if(i.error.isEmpty()) {
                data = i.delivery_state + "#*#" + i.status + "#*#" + i.date + "#*#" + i.delivery_service + "#*#" + i.icon;
            } else {
                shipping_states.push_back("Error!");
                data = i.error;
            }
            shipping_states.push_back(QJsonValue(data));
        }
        parcel_obj.insert("shipping_state", shipping_states);
        parcel_obj.insert("info", p_info);

        j_obj["Parcel"] = parcel_obj;

        main_array.push_back(j_obj);

        shipping_states = QJsonArray();
        p_info = QJsonArray();
        j_obj = QJsonObject();
    }
    QFile json_doc(path);
    QFileInfo jinfo(json_doc);
    if (!json_doc.exists()) {
        QDir dir;
        if (!dir.exists(jinfo.absolutePath())) {
            dir.mkpath(jinfo.absolutePath());
        }
    }
    if (json_doc.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)){
        QTextStream out(&json_doc);
        out << QJsonDocument(main_array).toJson(QJsonDocument::Indented);
        json_doc.close();
        emit json_ready();
    } else {
        qDebug() <<"Failed to open a file!";
    }
    main_array = QJsonArray();


}
void GPP::parcel_storage::save_to_json() {
    for (int i = 0; i < this->parcels.size(); i++) {
        QJsonObject parcel_obj;
        parcel_obj.insert("name",QJsonValue(this->parcels.at(i).name));
        parcel_obj.insert("track_num", QJsonValue(this->parcels.at(i).track_num));
        parcel_obj.insert("gp_url", QJsonValue(this->parcels.at(i).gp_url));
        for(QString is : this->parcels.at(i).info) {
            p_info.push_back(QJsonValue(is));
        }
        for(GPP::info_storage i : this->parcels.at(i).shipping_state) {
            QString data;
            if(i.error.isEmpty()) {
                data = i.delivery_state + "#*#" + i.status + "#*#" + i.date + "#*#" + i.delivery_service + "#*#" + i.icon;
            } else {
                shipping_states.push_back("Error!");
                data = i.error;
            }
            shipping_states.push_back(QJsonValue(data));
        }
        parcel_obj.insert("shipping_state", shipping_states);
        parcel_obj.insert("info", p_info);

        j_obj["Parcel"] = parcel_obj;

        main_array.push_back(j_obj);

        shipping_states = QJsonArray();
        p_info = QJsonArray();
        j_obj = QJsonObject();
    }
    QDir gpp_dir("./gpp");
    if (!gpp_dir.exists()) {
        gpp_dir.mkdir(".");
    }
    QFile json_doc("./gpp/parcels.json");
    if (json_doc.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        QTextStream out(&json_doc);
        out << QJsonDocument(main_array).toJson(QJsonDocument::Indented);
        json_doc.close();
        emit json_ready();
    }else {
        qDebug() <<"Failed to open a file!";
    }
    main_array = QJsonArray();

}
void GPP::parcel_storage::load_from_json(const QString &path) {
    GPP::Parcel holder("0");
    QFile json_doc(path);
    if (json_doc.open(QIODevice::ReadOnly | QIODevice::Text)) {
        main_array = QJsonDocument::fromJson(json_doc.readAll()).array();
        for (const QJsonValueRef& item : main_array) {
            if (!item.toObject()["name"].isNull() && !item.toObject()["track_num"].isNull()) {
                holder.set_name(item.toObject()["name"].toString());
                holder.set_track_num(item.toObject()["track_num"].toString());
            }
            if (item.toObject()["name"].isNull() && !item.toObject()["track_num"].isNull()) {
                holder.set_track_num(item.toObject()["track_num"].toString());
            }
            if (!item.toObject()["shipping_state"].isNull()) {
                for (QJsonValueRef i : item.toObject()["shipping_state"].toArray()) {
                    QList<GPP::info_storage> list;
                    GPP::info_storage is;
                    std::string delimiter = "#*#";
                    int counter=0;
                    size_t pos = 0;
                    std::string shipping_text = i.toString().toStdString();
                    if (i.toString() == "Error!") {
                        is.error = item.toObject()["shipping_state"].toArray()[1].toString();
                        counter = 10;
                    } else {
                        while ((pos = shipping_text.find(delimiter)) != std::string::npos) {
                            token = shipping_text.substr(0, pos);
                            switch (counter) {
                            case 0:
                                is.delivery_state = QString::fromStdString(token);
                                break;
                            case 1:
                                is.status = QString::fromStdString(token);
                                break;
                            case 2:
                                is.date = QString::fromStdString(token);
                                break;
                            case 3:
                                is.delivery_service = QString::fromStdString(token);
                                break;
                            case 4:
                                is.icon = QString::fromStdString(token);
                                break;
                            }
                            counter += 1;
                            shipping_text.erase(0, pos + delimiter.length());
                        };
                    }
                    holder.add_shipping_state(is);
                }
            }
            if (!item.toObject()["gp_url"].isNull()) {
                holder.gp_url = item.toObject()["gp_url"].toString();
            }
            if (!item.toObject()["info"].isNull()) {
                for(QJsonValueRef i : item.toObject()["info"].toArray()) {
                    holder.info.push_back(i.toString());
                }
            }
            this->add_parcels(holder);
            holder.clear_shipping_states();
            holder.info.clear();
        }
    } else {
        qDebug() << "Failed to open a file!";
        return;
    }
    main_array = QJsonArray();
    emit load_finished();
}
void GPP::parcel_storage::load_from_json() {
    GPP::Parcel holder("0");
    QFile json_doc("./gpp/parcels.json");
    if (json_doc.open(QIODevice::ReadOnly | QIODevice::Text)) {
        main_array = QJsonDocument::fromJson(json_doc.readAll()).array();
        for (const QJsonValueRef& item : main_array) {
            if (!item.toObject()["Parcel"].toObject()["name"].isNull() && !item.toObject()["Parcel"].toObject()["track_num"].isNull()) {
                holder.set_name(item.toObject()["Parcel"].toObject()["name"].toString());
                holder.set_track_num(item.toObject()["Parcel"].toObject()["track_num"].toString());
            }
            if (item.toObject()["Parcel"].toObject()["name"].isNull() && !item.toObject()["Parcel"].toObject()["track_num"].isNull()) {
                holder.set_track_num(item.toObject()["Parcel"].toObject()["track_num"].toString());
            }
            if (!item.toObject()["Parcel"].toObject()["shipping_state"].isNull()) {
                for (QJsonValueRef i : item.toObject()["Parcel"].toObject()["shipping_state"].toArray()) {
                    QList<GPP::info_storage> list;
                    GPP::info_storage is;
                    std::string delimiter = "#*#";
                    int counter=0;
                    size_t pos = 0;
                    std::string shipping_text = i.toString().toStdString();
                    if (i.toString() == "Error!") {
                        is.error = item.toObject()["Parcel"].toObject()["shipping_state"].toArray()[1].toString();
                        counter = 10;
                    } else {
                        while ((pos = shipping_text.find(delimiter)) != std::string::npos) {
                            token = shipping_text.substr(0, pos);
                            switch (counter) {
                            case 0:
                                is.delivery_state = QString::fromStdString(token);
                                break;
                            case 1:
                                is.status = QString::fromStdString(token);
                                break;
                            case 2:
                                is.date = QString::fromStdString(token);
                                break;
                            case 3:
                                is.delivery_service = QString::fromStdString(token);
                                break;
                            case 4:
                                is.icon = QString::fromStdString(token);
                                break;
                            }
                            counter += 1;
                            shipping_text.erase(0, pos + delimiter.length());
                        };
                    }
                    holder.add_shipping_state(is);
                }
            }
            if (!item.toObject()["Parcel"].toObject()["gp_url"].isNull()) {
                holder.gp_url = item.toObject()["Parcel"].toObject()["gp_url"].toString();
            }
            if (!item.toObject()["Parcel"].toObject()["info"].isNull()) {
                for(QJsonValueRef i : item.toObject()["Parcel"].toObject()["info"].toArray()) {
                    holder.info.push_back(i.toString());
                }
            }
            this->add_parcels(holder);
            holder.clear_shipping_states();
            holder.info.clear();
        }
    } else {
        qDebug() << "Failed to open a file!";
        return;
    }
    main_array = QJsonArray();
    emit load_finished();
}
void GPP::parcel_storage::load_shipping_infos() {
        index=0;
        QObject::connect(&s, &GPP::search::parcel_info_recieved, [&](QStringList info) {
            this->parcels[index].info = info;
        });
        QObject::connect(&s, &GPP::search::parcel_url_recieved, [&](QUrl p_url) {
            this->parcels[index].gp_url = p_url.toString();
        });
        QObject::connect(&s, &GPP::search::search_finished, [&](QList<GPP::info_storage> result){
            this->parcels[index].clear_shipping_states();
            this->parcels[index].shipping_state = result;
            index+=1;
            if (index < parcels.length()) {
                if(this->parcels[index].gp_url.isEmpty()) {
                    s.parcel_search(this->parcels[index].track_num);
                } else {
                    s.parcel_search_from_url(QUrl(this->parcels[index].gp_url));
                }
            } else {
                emit infos_load_finished();
            }
        });
        if (index < this->parcels.length()) {
            if(this->parcels[index].gp_url.isEmpty()) {
                s.parcel_search(this->parcels[index].track_num);
            } else {
                s.parcel_search_from_url(QUrl(this->parcels[index].gp_url));
            }

        } else {
            emit infos_load_finished();
        }
}
void GPP::parcel_storage::move_parcel_to_other_storage(int index, GPP::parcel_storage other_storage) {
    other_storage.add_parcels(parcels[index]);
    this->delete_parsel(index);
}
QList<GPP::Parcel> GPP::parcel_storage::get_parcels() {
        return this->parcels;
}
