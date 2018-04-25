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
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.0

Item {
    id: cFilterItem

    anchors.left: parent.left
    anchors.right: parent.right
    implicitHeight: 18.5 * settings.pixelDensity

    signal rightClicked()

    property alias comboLabelText: comboLabel.text
    property alias textFieldLabelText: textLabel.text

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

            CLabel {
                id: comboLabel
                width: implicitWidth
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                    leftMargin: 5 * settings.pixelDensity
                }
            }

            ComboBox {
                id: groupComboBox
                width: 0.15 * Screen.width
                anchors {
                    left: comboLabel.right
                    top: parent.top
                    bottom: parent.bottom
                    leftMargin: 5 * settings.pixelDensity
                    topMargin: 3 * settings.pixelDensity
                    bottomMargin: 3 * settings.pixelDensity
                }
            }

            CLabel {
                id: textLabel
                width: implicitWidth
                anchors {
                    left: groupComboBox.right
                    top: parent.top
                    bottom: parent.bottom
                    leftMargin: 5 * settings.pixelDensity
                }
            }

            TextField {
                id: textFieldIt
                width: 0.15 * Screen.width
                anchors {
                    left: textLabel.right
                    top: parent.top
                    bottom: parent.bottom
                    leftMargin: 5 * settings.pixelDensity
                    topMargin: 3 * settings.pixelDensity
                    bottomMargin: 3 * settings.pixelDensity
                }
            }
        }

        Item {
            id: rightButton

            Layout.fillHeight: true
            Layout.minimumWidth: height

            Rectangle {
                anchors.fill: parent
                color: palette.button
                visible: rightButtonMouseArea.pressed
            }

            CIcon {
                id: icon
                anchors.centerIn: parent
                text: "\u2315"
            }

            MouseArea {
                id: rightButtonMouseArea
                anchors.fill: parent
                onClicked: cFilterItem.rightClicked()
            }
        }
    }
}
