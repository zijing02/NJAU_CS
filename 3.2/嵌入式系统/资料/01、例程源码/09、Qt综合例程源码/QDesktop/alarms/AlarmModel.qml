/*****************************************************
    * @projectName   alarms
    * @brief         摘要
    * @author        Deng Zhimao
    * @email         1252699831@qq.com
    * @date          2020-07-08
*****************************************************/

import QtQuick 2.11
import QtQuick.LocalStorage 2.0
// Populate the model with some sample data.
ListModel {
    id: alarmModel
    ListElement {
        hour: 6
        minute: 0
        day: 2
        month: 8
        year: 2018
        activated: true
        label: "起床"
        repeat: true
        daysToRepeat: [
            ListElement { dayOfWeek: 0; repeat: false },
            ListElement { dayOfWeek: 1; repeat: false },
            ListElement { dayOfWeek: 2; repeat: false },
            ListElement { dayOfWeek: 3; repeat: false },
            ListElement { dayOfWeek: 4; repeat: false },
            ListElement { dayOfWeek: 5; repeat: false },
            ListElement { dayOfWeek: 6; repeat: false }
        ]
    }
    Component.onCompleted: loadData()
    Component.onDestruction: {
        saveData()
        if (rebootFlag)
            myDesktop.systemReboot()
        if (poweroffFlag)
            myDesktop.systemPoweroff()
    }
    function loadData() {
        var db = LocalStorage.openDatabaseSync("MyDB", "1.0", "My model SQL", 50000);
        db.transaction(
                    function(tx) {
                        // Create the database if it doesn't already exist
                        tx.executeSql('CREATE TABLE IF NOT EXISTS alarmData(hour INTEGER, minute INTEGER,
                                        activated TINYINT, label TEXT, repeat TINYINT, myValue INTEGER)');

                        var rs = tx.executeSql('SELECT * FROM alarmData');
                        var index = 0
                        if (rs.rows.length > 0) {
                            alarmModel.remove(0)
                            index = 0
                            while (index < rs.rows.length) {
                                var myItem = rs.rows.item(index);
                                //console.log(myItem.myValue)
                                alarmModel.append( {
                                                      "hour": myItem.hour,
                                                      "minute": myItem.minute,
                                                      "activated": myItem.activated ? true : false,
                                                      "label": myItem.label,
                                                      "repeat": myItem.repeat ? true : false,
                                                      "daysToRepeat": getMyvalue(myItem.myValue)
                                                  })
                                index++
                            }
                        } else {
                            alarmModel.remove(0)
                            alarmModel.append({
                                                  "hour": 6,
                                                  "minute": 0,
                                                  "activated": true,
                                                  "label": "",
                                                  "repeat": false,
                                                  "daysToRepeat": [
                                                      { "dayOfWeek": 0, "repeat": false },
                                                      { "dayOfWeek": 1, "repeat": false },
                                                      { "dayOfWeek": 2, "repeat": false },
                                                      { "dayOfWeek": 3, "repeat": false },
                                                      { "dayOfWeek": 4, "repeat": false },
                                                      { "dayOfWeek": 5, "repeat": false },
                                                      { "dayOfWeek": 6, "repeat": false }
                                                  ],
                                              })

                        }
                    }
                    )
    }
    function saveData() {
        var db = LocalStorage.openDatabaseSync("MyDB", "1.0", "My model SQL", 50000);
        db.transaction(
                    function(tx) {
                        tx.executeSql('DROP TABLE alarmData');
                        tx.executeSql('CREATE TABLE IF NOT EXISTS alarmData(hour INTEGER, minute INTEGER,
                                        activated TINYINT, label TEXT, repeat TINYINT, myValue INTEGER)');
                        var index = 0
                        while (index < alarmModel.count) {
                            var myItem = alarmModel.get(index);
                            var daysItem = myItem.daysToRepeat
                            var myValue = 0
                            for(var i = 0; i < myItem.daysToRepeat.count; i++){
                                myValue += daysItem.get(i).repeat << i
                            }
                            //console.log(myValue)
                            tx.executeSql('INSERT INTO alarmData VALUES(?,?,?,?,?,?)', [myItem.hour, myItem.minute,
                                                                                        myItem.activated, myItem.label,
                                                                                        myItem.repeat, myValue]);
                            index++;
                        }
                    }
                    )
    }

    function getMyvalue(mvalue) {
        var data = [
                    { "dayOfWeek": 0, "repeat": mvalue & 1 ? true : false },
                    { "dayOfWeek": 1, "repeat": mvalue & 2 ? true : false },
                    { "dayOfWeek": 2, "repeat": mvalue & 4 ? true : false },
                    { "dayOfWeek": 3, "repeat": mvalue & 8 ? true : false },
                    { "dayOfWeek": 4, "repeat": mvalue & 16 ? true : false },
                    { "dayOfWeek": 5, "repeat": mvalue & 32 ? true : false },
                    { "dayOfWeek": 6, "repeat": mvalue & 64 ? true : false }
                ]
        return  data
    }


}
