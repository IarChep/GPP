/*/
 *
 * 1. Tracking a parcel
 *
 /*/
GPP::search s;
QObject::connect(&s, &GPP::search::search_finished, [=](QList<GPP::info_storage> result) {
    for (GPP::info_storage i : result){
        if (i.error.isEmpty()) {
            qDebug().noquote() << i.delivery_state.toStdString() << i.status.toStdString() << i.date.toStdString() << i.delivery_service.toStdString() << i.icon.toStdString();
        } else {
            qDebug().noquote() << i.error;
        }
    }
});
s.parcel_search("Your track num");

/*/
 *
 * 1.2 Tracking parcels
 *
 /*/
GPP::search s;
QStringList track_num_list = {"first", "second", "etc..."};
QObject::connect(&s, &GPP::search::search_finished_multiple, [=](QList<QList<GPP::info_storage>> result) {
    for (QList<GPP::info_storage> i : result){
        for(GPP::info_storage j : i) {
            if (j.error.isEmpty()) {
                qDebug().noquote() << j.delivery_state.toStdString() << j.status.toStdString() << j.date.toStdString() << j.delivery_service.toStdString() << j.icon.toStdString();
            } else {
                qDebug().noquote() << j.error;
            }
        }
        }
});
s.multiple_search(track_num_list);

/*/
 *
 * 1.3 Tracking a parcel. Bool value
 *
 /*/
GPP::search s;
QObject::connect(&s, &GPP::search::search_finished_bool, [=](bool result) {
    qDebug().noquote() << result;
});
s.parcel_search_bool("Your track num");

/*/
 *
 * 1.4 Tracking parcels. Bool value
 *
 /*/
GPP::search s;
QStringList track_num_list = {"first", "second", "etc..."};
QObject::connect(&s, &GPP::search::search_finished_multiple_bool, [=](QBitArray result) {
    for(int i = 0; i < result.size() ; i++) {
        qDebug().noquote() << result[i];
    }
});
s.multiple_search_bool(track_num_list);


/*/
 *
 * 2. Tracking a parcel using URL from GdePosylka
 *
 /*/
GPP::search s;
QObject::connect(&s, &GPP::search::search_finished, [=](QList<GPP::info_storage> result) {
    for (GPP::info_storage i : result){
        if (i.error.isEmpty()) {
            qDebug().noquote() << i.delivery_state.toStdString() << i.status.toStdString() << i.date.toStdString() << i.delivery_service.toStdString() << i.icon.toStdString();
        } else {
            qDebug().noquote() << i.error;
        }
    }
});
s.parcel_search_from_url(QUrl("GP's url"));

/*/
 *
 * 2.2 Tracking parcels using URL from GdePosylka
 *
 /*/
GPP::search s;
QList<QUrl> url_list = {QUrl("first"), QUrl("second"), QUrl("etc...")};
QObject::connect(&s, &GPP::search::search_finished_multiple, [=](QList<QList<GPP::info_storage>> result) {
    for (QList<GPP::info_storage> i : result){
        for(GPP::info_storage j : i) {
            if (j.error.isEmpty()) {
                qDebug().noquote() << j.delivery_state.toStdString() << j.status.toStdString() << j.date.toStdString() << j.delivery_service.toStdString() << j.icon.toStdString();
            } else {
                qDebug().noquote() << j.error;
            }
        }
    }
});
s.multiple_search_from_url(url_list);

/*/
 *
 * 2.3 Tracking a parcel using URL from GdePosylka. Bool value
 *
 /*/
GPP::search s;
QObject::connect(&s, &GPP::search::search_finished_bool, [=](bool result) {
    qDebug().noquote() << result;
});
s.parcel_search_bool_from_url(QUrl("url"));

/*/
 *
 * 2.4 Tracking parcels using URL from GdePosylka. Bool value
 *
 /*/
GPP::search s;
QList<QUrl> url_list = {QUrl("first"), QUrl("second"), QUrl("etc...")};
QObject::connect(&s, &GPP::search::search_finished_multiple_bool, [=](QBitArray result) {
    for(int i = 0; i < result.size() ; i++) {
        qDebug().noquote() << result[i];
    }
});
s.multiple_search_bool_from_url(url_list);



/*/
 *
 * 3. Working with parcel class. Creating a parcel
 *
 /*/
GPP::Parcel parcel("Only name");
GPP::Parcel parcel2("name", "track_num");



/*/
 *
 * 3.1 Working with parcel class. Tracking Parcels
 *
 /*/
GPP::Parcel parcel("name", "track_num");
GPP::search s;
QObject::connect(&s, &GPP::search::search_finished, [=](QList<GPP::info_storage> result) {
    for (GPP::info_storage i : result){
        if (i.error.isEmpty()) {
            qDebug().noquote() << i.delivery_state.toStdString() << i.status.toStdString() << i.date.toStdString() << i.delivery_service.toStdString() << i.icon.toStdString();
        } else {
            qDebug().noquote() << i.error;
        }
    }
});
s.parcel_search(parcel.track_num);

/*/
 *
 * 3.2 Working with parcel class. Tracking Parcels and synchronising data (info and GP's url)
 *
 /*/
GPP::Parcel parcel("name", "track_num");
GPP::search s;
QObject::connect(&s, &GPP::search::parcel_info_recieved, [&](QStringList info) {
    parcel.info = info;
});
QObject::connect(&s, &GPP::search::parcel_url_recieved, [&](QUrl gp_url) {
    parcel.gp_url = gp_url.toString();
});
QObject::connect(&s, &GPP::search::search_finished, [=](QList<GPP::info_storage> result) {
    for (QString j : parcel.info) {
        qDebug().noquote() << j;
    }
    qDebug().noquote() << parcel.gp_url;
    for (GPP::info_storage i : result){
        if (i.error.isEmpty()) {
            qDebug().noquote() << i.delivery_state.toStdString() << i.status.toStdString() << i.date.toStdString() << i.delivery_service.toStdString() << i.icon.toStdString();
        } else {
            qDebug().noquote() << i.error;
        }
    }
});
s.parcel_search(parcel.track_num);


/*/
 *
 * 4. Using parcel_storage
 *
 /*/
GPP::parcel_storage storage;
storage.add_parcels(GPP::Parcel("name", "track_num"), GPP::Parcel("name", "track_num")); //you can place here as many GPP::Parcel as you want
//print all parcels
for(GPP::Parcel i : storage.get_parcels()) {
    qDebug().noquote() << i.name << i.track_num;
}

/*/
 *
 * 4.1 Using parcel_storage. Loading information for GPP::Parcel's
 *
 /*/
GPP::parcel_storage storage;
QObject::connect(&storage, &GPP::parcel_storage::added_successfully, [&]{
    qDebug().noquote() << "Parcels added sucsessfully. Now loading information";
    storage.load_shipping_infos();
}); // you can skip this step bcs parcels can be added really quickly

QObject::connect(&storage, &GPP::parcel_storage::infos_load_finished, [&]{
    qDebug().noquote() << "Information loaded!";
    for (GPP::Parcel i : storage.get_parcels()) {
        for (QString j : i.info) {
            qDebug().noquote() << j;
        }
        qDebug().noquote() << i.gp_url;
        for (GPP::info_storage h : i.shipping_state){
            if (h.error.isEmpty()) {
                qDebug().noquote() << h.delivery_state.toStdString() << h.status.toStdString() << h.date.toStdString() << h.delivery_service.toStdString() << h.icon.toStdString();
            } else {
                qDebug().noquote() << h.error;
            }
        }
    }
});
storage.add_parcels(GPP::Parcel("name", "track_num"), GPP::Parcel("name", "track_num")); //you can place here as many GPP::Parcel as you want

/*/
 *
 * 4.2 Using parcel_storage. Saving GPP::parcel_storage to a json document
 *
 /*/
GPP::parcel_storage storage;
QObject::connect(&storage, &GPP::parcel_storage::added_successfully, [&]{
    qDebug().noquote() << "Parcels added sucsessfully. Now loading information";
    storage.load_shipping_infos();
}); // you can skip this step bcs parcels can be added really quickly

QObject::connect(&storage, &GPP::parcel_storage::infos_load_finished, [&]{
    qDebug().noquote() << "Information loaded!";
    for (GPP::Parcel i : storage.get_parcels()) {
        for (QString j : i.info) {
            qDebug().noquote() << j;
        }
        qDebug().noquote() << i.gp_url;
        for (GPP::info_storage h : i.shipping_state){
            if (h.error.isEmpty()) {
                qDebug().noquote() << h.delivery_state.toStdString() << h.status.toStdString() << h.date.toStdString() << h.delivery_service.toStdString() << h.icon.toStdString();
            } else {
                qDebug().noquote() << h.error;
            }
        }
    }
    storage.save_to_json(); // will save a json doc to ./gpp/parcels.json
});
QObject::connect(&storage, &GPP::parcel_storage::json_ready, [&]{
    qDebug().noquote() << "Json saved sucsessfully!";
});
storage.add_parcels(GPP::Parcel("name", "track_num"), GPP::Parcel("name", "track_num")); //you can place here as many GPP::Parcel as you want

/*/
 *
 * 4.3 Using parcel_storage. Loading GPP::parcel_storage from json document
 *
 /*/
GPP::parcel_storage storage;
QObject::connect(&storage, &GPP::parcel_storage::load_finished, [&]{
    qDebug().noquote() << "parcel_storage restored!";
    //now you can read info from it
}); // you can skip this step bcs parcels can be added really quickly
storage.load_from_json(); //will load from ./gpp/parcels.json
