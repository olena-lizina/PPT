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
    id: cRightButton

    property alias icon: icon.text

    signal rightButtonCLicked()

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
        onClicked: cRightButton.rightButtonCLicked()
    }
}
