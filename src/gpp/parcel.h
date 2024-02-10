#ifndef PARCEL_H
#define PARCEL_H
#include <QString>
#include "info_storage.h"
namespace GPP {
    class Parcel {

    public:
        QString name;
        QString track_num;
        QList<GPP::info_storage> shipping_state;
        QList<QString> info;
        QString gp_url;

        //constructors
        Parcel(QString track_num) : track_num(track_num) {}
        Parcel(QString name, QString track_num) : name(name), track_num(track_num) {}

        void set_name(const QString &n) {
            name = n;
        }

        void set_track_num(const QString &tn) {
            track_num = tn;
        }
        void set_shipping_states(const QList<GPP::info_storage> &states) {
            shipping_state = states;
        }
        void add_shipping_state(const GPP::info_storage &state) {
            shipping_state.push_back(state);
        }
        void clear_shipping_states() {
            shipping_state.clear();
        }

    };
}
#endif // PARCEL_CLASS_H
