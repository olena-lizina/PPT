/****************************************************************************
**
** Copyright (C) 2017-2018 Olena Lizina
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/

import QtQuick 2.5

Item {
    id: cInformationItem

    anchors.left: parent.left
    anchors.right: parent.right
    implicitHeight: 18.5 * settings.pixelDensity

    property alias text1: label1.text
    property alias text2: label2.text
    property alias text3: label3.text
    property alias text4: label4.text

    CHorizontalSeparator {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Column {
        id: firstCol
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        spacing: 0.5 * settings.pixelDensity

        CLabel {
            id: label1
            anchors.left: parent.left
            anchors.right: parent.right
        }

        CLabel {
            id: label2
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 5 * settings.pixelDensity
            color: palette.description
        }
    }

    Column {
        anchors.left: firstCol.right
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        spacing: 0.5 * settings.pixelDensity

        CLabel {
            id: label3
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 5 * settings.pixelDensity
            color: palette.description
        }

        CLabel {
            id: label4
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 5 * settings.pixelDensity
            color: palette.description
        }
    }
}
