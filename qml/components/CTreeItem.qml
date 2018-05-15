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
import ".."

Row {
    Rectangle{
        width: 20
        height: list.height
        color: palette.button
        opacity: 0
    }

    Column {
        id: list
        CLabel {
            height: 18.5 * settings.pixelDensity
            width: 0.25 * settings.windowWidth

            text: modelData.idx + " " + modelData.text
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: modelData.isOpen = !modelData.isOpen;
            }
            CHorizontalSeparator {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
            Rectangle {
                anchors.fill: parent
                color: palette.button
                visible: mouseArea.pressed
                opacity: 0.5
            }
        }

        Loader{ source: modelData.isOpen ? "CChildTreeList.qml" : "CTreeEmptyItem.qml"; }
    }
}


