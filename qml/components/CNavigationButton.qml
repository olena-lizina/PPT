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

Item {
    id: cNavigationButton

    implicitHeight: 18.5 * settings.pixelDensity
    property alias color: btnRect.color
    property alias icon: buttonIcon.text
    property alias iconVisibility: buttonIcon.visible
    property alias text: buttonLabel.text

    signal clicked()
    signal iconClicked()

    CHorizontalSeparator {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Rectangle {
        id: btnRect

        anchors.fill: parent
        color: palette.button
        visible: mouseArea.pressed
    }

    RowLayout {
        id: body
        anchors.fill: parent
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        spacing: 3 * settings.pixelDensity

        CIcon {
            id: buttonIcon
            Layout.fillHeight: true
            visible: text !== ""
            MouseArea{
                anchors.fill: buttonIcon
                onClicked: cNavigationButton.iconClicked()
            }
        }
        CLabel {
            id: buttonLabel
            Layout.fillWidth: true
            MouseArea {
                id: mouseArea
                anchors.fill: buttonLabel
                onClicked: cNavigationButton.clicked()
            }
        }        
    }
}
