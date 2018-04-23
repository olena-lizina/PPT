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
    id: cVerticalScrollBar
    implicitWidth: 12 * settings.pixelDensity

    property Item flickableItem

    Connections {
        target: flickableItem.visibleArea
        onHeightRatioChanged:
            cVerticalScrollBar.visible = (flickableItem.visibleArea.heightRatio < 1)
    }

    Rectangle {
        id: handler
        y: flickableItem.visibleArea.yPosition * flickableItem.height
        anchors.left: parent.left
        anchors.right: parent.right
        height: flickableItem.visibleArea.heightRatio * flickableItem.height
        color: palette.scrollBar
    }

    MouseArea {
        anchors.fill: parent
        preventStealing: true

        onMouseYChanged:
            if (mouseY - handler.height / 2 <= 0) {
                flickableItem.contentY = 0
            }
            else if ((mouseY - handler.height / 2) * flickableItem.contentHeight / cVerticalScrollBar.height >=
                     flickableItem.contentHeight - cVerticalScrollBar.height) {
                flickableItem.contentY = flickableItem.contentHeight - cVerticalScrollBar.height
            }
            else
                flickableItem.contentY = (mouseY - handler.height / 2) * flickableItem.contentHeight / cVerticalScrollBar.height
    }
}
