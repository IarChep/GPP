# Documentation
  * [Namespace GPP](#gpp)
  * [search.h](#search)
  * [parcel.h](#parcel)
  * [parcel_storage.h](#parcel_storage)
  * [info_storage.h](#info_storage)
## <a id="gpp">Namespace GPP</a>
### <a id="search">search.h</a>
 Signals:  
    1. `
       void search_finished(QList<GPP::info_storage>);
       ` - emits when parcel_search or parcel_search_from_url finish. Returns the [info_storage](#info_storage) list.  !!ONLY THIS SIGNAL RETURNS CONNECTION ERRORS!!  
    2. `
       void search_finished_multiple(QList<QList<GPP::info_storage>>);
       ` - emits when multiple_search or multiple_search_from_url finish. Returns a list of [info_storage](#info_storage) list  
    3. `
       void search_finished_bool(bool);
       ` - emits when parcel_search_bool or parcel_search_bool_from_url finish. Returns a bool value(found = true, not found = false)  
    4. `
       void search_finished_multiple_bool(QBitArray);
       ` - emits when multiple_search_bool or multiple_search_bool_from_url finish. Returns a QBitArray (bool values list)  
    5. `
       void parcel_info_recieved(QStringList);
       ` - emits when program read parcel info sucsessfully. search_finished_* signal do not return parcel's info.  
    6. `
       void parcel_url_recieved(QUrl);
       ` - emits when program read parcel's info sucsessfully. search_finished_* signal do not return parcel's url.  
  
  1. ```cpp
     void set_service(); // prints a message with choises
     void set_service(int service_num); // sets a tracker service

     // usage
     #include <gpp/gpp.h>
     GPP::search s;
     s.set_service(1); // to select GdePosylka
     s.set_service(2); //to select PackageRadar
     ```
     This function allows you to select a parcel tracking service. Use 1 as a service_num value to select GdePosylka and 2 for PackageRadar.
  2. ```cpp
      void save_service(); // will save selected service

      // usage
      #include <gpp/gpp.h>
      GPP::search s;
      s.set_service(2); //to select PackageRadar
      s.save_service();
      ```
      This function saves current service to a txt file. If the service has been saved, it will be automatically restored when creating a search object.
  3. ```cpp
      void parcel_search(QString track_num);
      void parcel_search_from_url(QUrl url);
      emits void search_finished(QList<GPP::info_storage>) after tracking is finished
     
      // usage
      #include <gpp/gpp.h>
      GPP::search s;
      QObject::connect(&s, &GPP::search::search_finished, [=](QList<GPP::info_storage> result) {
        for (GPP::info_storage i : result){
            if (i.error.isEmpty()) {
                qDebug().noquote() << i.delivery_state.toStdString() << i.status.toStdString() << i.date.toStdString() << i.delivery_service.toStdString() << i.icon.toStdString();
             } else {
                 qDebug().noquote() << i.error;
             }
         }
      }); // connecting to search_finished(QList<GPP::info_storage>) signal
      s.parcel_search("Your track num"); //if you know only parcel's tracking num
      s.parcel_search_from_url(QUrl("if you know parcel's url on GdePosylka")); // faste, but only if you know parcel's url
      ```
      parcel_search makes search in GdePasylka/PackageRadar through https://gdeposylka.ru/form. Emits void search_finished(QList<GPP::info_storage>) after tracking is finished
      **!!Use parcel_search (and other search methods too) as little as possible. Otherwise, GdePosylka will ask you to pass a captcha, which is impossible from this library. Instead of it, use parcel_search_from_url (and other search_from_url methods) method!!**
      parcel_search_from_url runs parsing without searching, because it uses url you provided. For example, you can use this url: https://gdeposylka.ru/courier/cainiao/tracking/your_cainiao_tracking_number
  4. ```cpp
      void parcel_search_bool(QString track_num);
      void parcel_search_bool_from_url(QUrl url);
      emits void search_finished_bool(bool); after tracking is finished
     
      // usage
      #include <gpp/gpp.h>
      GPP::search s;
      QObject::connect(&s, &GPP::search::search_finished_bool, [=](bool result) {
        qDebug().noquote() << result;
     }); // connecting to void search_finished_bool(bool); signal
      s.parcel_search_bool("Your track num"); 
      s.parcel_search_bool_from_url(QUrl("if you know parcel's url on GdePosylka"));
      ```
      parcel_search_bool makes search in GdePasylka/PackageRadar through https://gdeposylka.ru/form. emits void search_finished_bool(bool); after tracking is finished
      **!!Use parcel_search_bool (and other search methods too) as little as possible. Otherwise, GdePosylka will ask you to pass a captcha, which is impossible from this library. Instead of it, use parcel_search_bool_from_url (and other search_from_url methods) method!!**
      parcel_search_bool_from_url runs parsing without searching, because it uses url you provided. For example, you can use this url: https://gdeposylka.ru/courier/cainiao/tracking/your_cainiao_tracking_number
  5. ```cpp
      void parcel_search(QString track_num);
      void parcel_search_from_url(QUrl url);
      emits void search_finished_multiple(QList<QList<GPP::info_storage>>) after tracking is finished
     
      // usage
      #include <gpp/gpp.h>
      GPP::search s;
      QStringList track_num_list = {"first", "second", "etc..."};
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
      }); // connecting to search_finished_multiple(QList<QList<GPP::info_storage>>) signal
      s.multiple_search(track_num_list);
      s.multiple_search_from_url(url_list);
      ```
      multiple_search makes search in GdePasylka/PackageRadar through https://gdeposylka.ru/form for every track number in the list.  
      **!!Use multiple_search (and other search methods too) as little as possible. Otherwise, GdePosylka will ask you to pass a captcha, which is impossible from this library. Instead of it, use multiple_search_from_url (and other search_from_url methods) method!!**
      multiple_search_from_url runs parsing without searching for every URL in the list, because it uses url you provided.
  6. ```cpp
      void multiple_search_bool(QStringList track_num_list);
      void multiple_search_bool_from_url(QList<QUrl> url_list);
      emits void search_finished_multiple_bool(QBitArray); after tracking is finished
     
      // usage
      #include <gpp/gpp.h>
      GPP::search s;
      QStringList track_num_list = {"first", "second", "etc..."};
      QList<QUrl> url_list = {QUrl("first"), QUrl("second"), QUrl("etc...")};
      QObject::connect(&s, &GPP::search::search_finished_multiple_bool, [=](QBitArray result) {
        for(int i = 0; i < result.size() ; i++) {
            qDebug().noquote() << result[i];
        }
      }); // connecting to void search_finished_multiple_bool(QBitArray); signal
      s.multiple_search_bool(track_num_list);
      s.multiple_search_bool_from_url(url_list);
      ```
      multiple_search makes search in GdePasylka/PackageRadar through https://gdeposylka.ru/form for every track number in the list. emits void search_finished_multiple_bool(QBitArray); after tracking is finished
      **!!Use multiple_search_bool (and other search methods too) as little as possible. Otherwise, GdePosylka will ask you to pass a captcha, which is impossible from this library. Instead of it, use multiple_search_bool_from_url (and other search_from_url methods) method!!**
      multiple_search_bool_from_url runs parsing without searching for every URL in the list, because it uses url you provided.
### <a id="parcel">parcel.h</a>
Contains variables:
  - QString name; - parcel's name
  - QString track_num; - parcel's track_num
  - QList<GPP::info_storage> shipping_state; - parcel's shipping states
  - QList<QString> info; - parcel's information
  - QString gp_url; - parcel's url in GP
  
Availible constructors:
  - ```cpp
    Parcel(QString track_num) : track_num(track_num) {}
    ```
  - ```cpp
    Parcel(QString name, QString track_num) : name(name), track_num(track_num) {}
    ```

Methods:
  1. ```cpp
     void set_name(const QString &n) {
            name = n;
        }
     ```
  2. ```cpp
     void set_track_num(const QString &tn) {
            track_num = tn;
        }
     ```
  3. ```cpp
     void set_shipping_states(const QList<GPP::info_storage> &states) {
            shipping_state = states;
        }
     ```
  4. ```cpp
     void add_shipping_state(const GPP::info_storage &state) {
            shipping_state.push_back(state);
        }
     ```
  5. ```cpp
     void clear_shipping_states() {
            shipping_state.clear();
        }
     ```

Usage:
  1. ```cpp
     //creating parcels
     GPP::Parcel parcel("Only name");
     GPP::Parcel parcel2("name", "track_num");
     ```
  2. ```cpp
     //searching for parcel in GP
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
     ```  
  Find more usage examples in examples!
## <a id="parcel_storage">parcel_storage.h</a>
Signals:
 1. `
    void added_successfully();
    ` - emits when parcels added sucsessfully with add_parcels
 2. `
    void json_ready();
    ` - emits when json file saved sucsessfully
 3. `
    void load_finished();
    ` - emits whe parcel_storage restored information from json file sucsessfully
 4. `
     void infos_load_finished();
     ` - emits when information for parcels in parcel_storage load sucsessfully with load_shipping_infos();
 5. `
    void move_finished();
    ` - emits when parcel sucsessfully moved from one parcel_storage to another
    
Methods + usage:
1. ```cpp
   void add_parcel(Parcel parcel, Parcels... tail)
   emits void added_successfully(); if parcels added sucsessfully
   //usage
   GPP::parcel_storage ps;
   storage.add_parcels(GPP::Parcel("name", "track_num"), GPP::Parcel("name", "track_num")); //you can place here as many GPP::Parcel as you want
   ```
2. ```cpp
   void delete_parsel(int index);

   //usage
   GPP::parcel_storage ps;
   ps.add_parcels(GPP::Parcel("name", "track_num"), GPP::Parcel("name", "track_num")); //you can place here as many GPP::Parcel as you want
   ps.delete_parcel(0) // deleted first parcel
   ```
3. ```cpp
   QList<Parcel> get_parcels(); //returns QList<Parcel> with GPP::Parcel inside of parcel_storage

   //usage
   GPP::parcel_storage ps;
   ps.add_parcels(GPP::Parcel("name", "track_num"), GPP::Parcel("name", "track_num")); //you can place here as many GPP::Parcel as you want
   for (GPP::Parcel i : ps.get_parcels()) {
       qDebug().noquote() << i.name << i.track_num;
   }
   ```
4. ```cpp
   void load_shipping_infos();
   emits void infos_load_finished(); is information loaded sucsessfully

   //usage
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
   ```
 5. ```cpp
    //saves all parcels to a json file
    void save_to_json(const QString &path); // this method will save json file using path you entered
    void save_to_json(); // this method wiil save json to ./gpp/parcels.json
    emits void json_ready(); when json sucsessfully saved

    //usage
    GPP::parcel_storage storage;
    QObject::connect(&storage, &GPP::parcel_storage::added_successfully, [&]{
        qDebug().noquote() << "Parcels added sucsessfully. Now saving";
        storage.save_to_json(); // will save a json doc to ./gpp/parcels.json
    }); // you can skip this step bcs parcels can be added really quickly

    QObject::connect(&storage, &GPP::parcel_storage::json_ready, [&]{
        qDebug().noquote() << "Json saved sucsessfully!";
    });
    storage.add_parcels(GPP::Parcel("name", "track_num"), GPP::Parcel("name", "track_num")); //you can place here as many GPP::Parcel as you want
    ```
 6. ```cpp
    //load information from json file from path you provided, or from ./gpp/parcels.json
    void load_from_json(const QString &path);
    void load_from_json();

    //usage
    GPP::parcel_storage storage;
    QObject::connect(&storage, &GPP::parcel_storage::load_finished, [&]{
        qDebug().noquote() << "parcel_storage restored!";
        //now you can read info from it
    }); // you can skip this step bcs parcels can be added really quickly
    storage.load_from_json(); //will load from ./gpp/parcels.json
    ```
 7. ```cpp
    //moves parcel to another parcel_storage
    void move_parcel_to_other_storage(int index, GPP::parcel_storage other_storage);

    //usage
    GPP::parcel_storage psf;
    GPP::parcel_storage pss;
    psf.add_parcels(GPP::Parcel("first", "track_num"), GPP::Parcel("second", "track_num"));
    pss.add_parcels(GPP::Parcel("third", "track_num"), GPP::Parcel("fourth", "track_num"));
    pss.move_parcel_to_other_storage(1, psf) // moved third parcel to the first parcel storage
    ```
### <a id="info_storage">info_storage.h</a>
Contains variables:
- QString delivery_state; - Info recieved, Delivering or Delivered
- QString status; - In transit - Arrived at the waypoint or smt
- QString date; - status date
- QString delivery_service; - parcel's delivery service
- QString icon; - contains a path to delivery service's icon in GP
- QString error; - contains an error, if happened
