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
import QtQuick.Layouts 1.2
import LecturesManager 1.1

Item {
    id: cButton

    anchors.left: parent.left
    anchors.right: parent.right
    implicitHeight: 18.5 * settings.pixelDensity

    property alias text: buttonLabel.text

    signal clicked()
    signal editClicked()
    signal removeClicked()

    CHorizontalSeparator {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                anchors.fill: parent
                color: palette.button
                visible: buttonMouseArea.pressed
            }

            CLabel {
                id: buttonLabel
                anchors.fill: parent
                anchors.leftMargin: 5 * settings.pixelDensity
                anchors.rightMargin: 3 * settings.pixelDensity
            }

            MouseArea {
                id: buttonMouseArea
                anchors.fill: parent
                onClicked: cButton.clicked()
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.minimumWidth: height

            Rectangle {
                anchors.fill: parent
                color: palette.button
                visible: editMouseArea.pressed
            }

            CIcon {
                anchors.centerIn: parent
                text: "\u270e"
            }

            MouseArea {
                id: editMouseArea
                anchors.fill: parent
                onClicked: cButton.editClicked()
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.minimumWidth: height

            Rectangle {
                anchors.fill: parent
                color: palette.button
                visible: removeMouseArea.pressed
            }

            CIcon {
                id: icon
                anchors.centerIn: parent
                text: "\u2716"
            }

            MouseArea {
                id: removeMouseArea
                anchors.fill: parent
                onClicked: cButton.removeClicked()
            }
        }
    }
}
