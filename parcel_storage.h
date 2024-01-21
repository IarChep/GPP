#ifndef PARCEL_STORAGE_H
#define PARCEL_STORAGE_H
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include "parcel.h"
#include "search.h"
#include <nlohmann/json.hpp>
#include <iostream>

namespace GPP {
class parcel_storage : public QObject
    {
    Q_OBJECT
    public:
        parcel_storage() : parcels() {}

        //the one function that has a code inside header file. I did it bcs templates work only this way.
        template<typename... Parcels>
        void add_parcel(Parcel parcel, Parcels... tail) {
            parcels.push_back(parcel);

            if constexpr (sizeof...(tail) > 0) {
                add_parcel(tail...);
            } else {
                emit added_successfully();
            }
        };
        void delete_parsel(int index);
        QList<Parcel> get_parcels();

        void load_shipping_infos();

        void load_from_json(const QString &path);
        void load_from_json();

        void save_to_json(const QString &path);
        void save_to_json();

        void move_parcel_to_other_storage(int index, GPP::parcel_storage other_storage);


    signals:
        void added_successfully();
        void json_ready();
        void load_finished();
        void infos_load_finished();
        void move_finished();
    private:
        int index=0;
        GPP::search s;
        std::string token;
        QList<Parcel> parcels;
        nlohmann::json j_array = nlohmann::json::array();
        nlohmann::json j;
        std::vector<std::string> shipping_states;
        std::vector<std::string> p_info;
    };
}


#endif // PARCEL_STORAGE_H
