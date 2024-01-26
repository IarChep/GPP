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
            j["Parcel"]["name"] = this->parcels.at(i).name.toStdString();
            j["Parcel"]["track_num"] = this->parcels.at(i).track_num.toStdString();
            j["Parcel"]["gp_url"] = this->parcels.at(i).gp_url.toStdString();
            for(QString is : this->parcels.at(i).info) {
                p_info.push_back(is.toStdString());
            }
            for(GPP::info_storage i : this->parcels.at(i).shipping_state) {
                std::string data;
            if(i.error.isEmpty()) {
                    data = i.delivery_state.toStdString() + "#*#" + i.status.toStdString() + "#*#" + i.date.toStdString() + "#*#" + i.delivery_service.toStdString() + "#*#" + i.icon.toStdString();
                } else {
                    shipping_states.push_back("Error!");
                    data = i.error.toStdString();
                }
                shipping_states.push_back(data);
            }
            j["Parcel"]["shipping_state"] = shipping_states;
            j["Parcel"]["info"] = p_info;
            qDebug().noquote() << shipping_states;
            j_array.push_back(j);
            j.clear();
            shipping_states.clear();
            p_info.clear();
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
            out << QString::fromStdString(j_array.dump(4));
            json_doc.close();
            emit json_ready();
        } else {
            qDebug() <<"Failed to open a file!";
        }
        j_array.clear();

}
void GPP::parcel_storage::save_to_json() {
        for (int i = 0; i < this->parcels.size(); i++) {
            j["Parcel"]["name"] = this->parcels.at(i).name.toStdString();
            j["Parcel"]["track_num"] = this->parcels.at(i).track_num.toStdString();
            j["Parcel"]["gp_url"] = this->parcels.at(i).gp_url.toStdString();
            for(QString is : this->parcels.at(i).info) {
                p_info.push_back(is.toStdString());
            }
            for(GPP::info_storage i : this->parcels.at(i).shipping_state) {
                std::string data;
                if(i.error.isEmpty()) {
                    data = i.delivery_state.toStdString() + "#*#" + i.status.toStdString() + "#*#" + i.date.toStdString() + "#*#" + i.delivery_service.toStdString() + "#*#" + i.icon.toStdString();
                } else {
                    shipping_states.push_back("Error!");
                    data = i.error.toStdString();
                }
                shipping_states.push_back(data);
            }
            j["Parcel"]["shipping_state"] = shipping_states;
            j["Parcel"]["info"] = p_info;
            j_array.push_back(j);
            j.clear();
            shipping_states.clear();
        }
        QDir gpp_dir("./gpp");
        if (!gpp_dir.exists()) {
            gpp_dir.mkdir(".");
        }
        QFile json_doc("./gpp/parcels.json");
        if (json_doc.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
            QTextStream out(&json_doc);
            out << QString::fromStdString(j_array.dump(4));
            json_doc.close();
            emit json_ready();
        }else {
            qDebug() <<"Failed to open a file!";
        }
        j_array.clear();

}
void GPP::parcel_storage::load_from_json(const QString &path) {
        GPP::Parcel holder("0");
        QFile json_doc(path);
        if (json_doc.open(QIODevice::ReadOnly | QIODevice::Text)) {
            j = nlohmann::json::parse(json_doc.readAll().toStdString());
            for (const auto& item : j) {
                if (!item["Parcel"]["name"].empty() && !item["Parcel"]["track_num"].empty()) {
                    holder.set_name(QString::fromStdString(item["Parcel"]["name"]));
                    holder.set_track_num(QString::fromStdString(item["Parcel"]["track_num"]));
                }
                if (item["Parcel"]["name"].empty() && !item["Parcel"]["track_num"].empty()) {
                    holder.set_track_num(QString::fromStdString(item["Parcel"]["track_num"]));
                }
                if (!item["Parcel"]["shipping_state"].empty()) {
                    for (std::string i : item["Parcel"]["shipping_state"]) {
                        QList<GPP::info_storage> list;
                        GPP::info_storage is;
                        std::string delimiter = "#*#";
                        int counter=0;
                        size_t pos = 0;
                        if (i == "Error!") {
                            is.error = QString::fromStdString(item["Parcel"]["shipping_state"][1]);
                            counter = 10;
                        } else {
                            while ((pos = i.find(delimiter)) != std::string::npos) {
                                token = i.substr(0, pos);
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
                                i.erase(0, pos + delimiter.length());
                            };
                        }
                        holder.add_shipping_state(is);
                    }
                }
                if (!item["Parcel"]["gp_url"].empty()) {
                    holder.gp_url = QString::fromStdString(item["Parcel"]["gp_url"]);
                }
                if (!item["Parcel"]["info"].empty()) {
                    for(std::string i : item["Parcel"]["info"]) {
                        holder.info.push_back(QString::fromStdString(i));
                    }
                }
                this->add_parcels(holder);
                holder.clear_shipping_states();
            }
        }
        emit load_finished();
}
void GPP::parcel_storage::load_from_json() {
        GPP::Parcel holder("0");
        QFile json_doc("./gpp/parcels.json");
        if (json_doc.open(QIODevice::ReadOnly | QIODevice::Text)) {
            j = nlohmann::json::parse(json_doc.readAll().toStdString());
            for (const auto& item : j) {
                if (!item["Parcel"]["name"].empty() && !item["Parcel"]["track_num"].empty()) {
                    holder.set_name(QString::fromStdString(item["Parcel"]["name"]));
                    holder.set_track_num(QString::fromStdString(item["Parcel"]["track_num"]));
                }
                if (item["Parcel"]["name"].empty() && !item["Parcel"]["track_num"].empty()) {
                    holder.set_track_num(QString::fromStdString(item["Parcel"]["track_num"]));
                }
                if (!item["Parcel"]["shipping_state"].empty()) {
                    for (std::string i : item["Parcel"]["shipping_state"]) {
                        QList<GPP::info_storage> list;
                        GPP::info_storage is;
                        std::string delimiter = "#*#";
                        int counter=0;
                        size_t pos = 0;
                        if (i == "Error!") {
                            is.error = QString::fromStdString(item["Parcel"]["shipping_state"][1]);
                            counter = 10;
                        } else {
                            while ((pos = i.find(delimiter)) != std::string::npos) {
                                token = i.substr(0, pos);
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
                                i.erase(0, pos + delimiter.length());
                            };
                        }
                        holder.add_shipping_state(is);
                    }
                }
                if (!item["Parcel"]["gp_url"].empty()) {
                    holder.gp_url = QString::fromStdString(item["Parcel"]["gp_url"]);
                }
                if (!item["Parcel"]["info"].empty()) {
                    for(std::string i : item["Parcel"]["info"]) {
                        holder.info.push_back(QString::fromStdString(i));
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
