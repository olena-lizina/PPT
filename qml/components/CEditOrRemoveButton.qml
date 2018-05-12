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

    implicitHeight: 18.5 * settings.pixelDensity

    property alias label_1: firstLable.text
    property alias label_2: secondLabel.text
    property alias color: btnRect.color

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
            id: btnBody
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                id: btnRect
                anchors.fill: parent
                color: palette.button
                visible: buttonMouseArea.pressed
            }

            MouseArea {
                id: buttonMouseArea
                anchors.fill: parent
                onClicked: cButton.clicked()
            }

            CLabel {
                id: firstLable
                anchors {                    
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                }
            }

            CLabel {
                id: secondLabel
                anchors {
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }
            }
        }

        Item {
            id: editBtn

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
            id: removeBtn
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
