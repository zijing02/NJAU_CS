/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11

Dialog {
    id: alarmDialog
    title: "添加闹钟"
    modal: true
    standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
    background: Rectangle {anchors.fill: parent;color: "#1f1e58"}
    property AlarmModel alarmModel

    function formatNumber(number) {
        return number < 10 && number >= 0 ? "0" + number : number.toString()
    }

    onAccepted: {
        alarmModel.append({
                              "hour": hoursTumbler.currentIndex,
                              "minute": minutesTumbler.currentIndex,
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
    onRejected: alarmDialog.close()

    contentItem: RowLayout {
        RowLayout {
            id: rowTumbler
            spacing: 20

            Tumbler {
                id: hoursTumbler
                model: 24
                delegate: TumblerDelegate {
                    text: formatNumber(modelData)
                    font.pixelSize: 40
                    color: "white"
                    font.bold: true
                }
            }
            Text {
                id: hour
                text: qsTr("时")
                color: "white"
                font.bold: true
                font.pixelSize: 15
            }
            Tumbler {
                id: minutesTumbler
                model: 60
                delegate: TumblerDelegate {
                    text: formatNumber(modelData)
                    font.pixelSize: 40
                    color: "white"
                    font.bold: true
                }
            }
            Text {
                id: min
                text: qsTr("分")
                color: "white"
                font.bold: true
                font.pixelSize: 15
            }
        }
    }
}
