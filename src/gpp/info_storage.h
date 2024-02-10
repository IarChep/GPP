#ifndef INFO_STORAGE_H
#define INFO_STORAGE_H

#include <QObject>
#include <QString>

namespace GPP {
    class info_storage
    {
    public:
        QString delivery_state;
        QString status;
        QString date;
        QString delivery_service;
        QString icon;
        QString error;

    };
}


#endif // INFO_STORAGE_H
