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
    id: contextMenu
    visible: false

    Column {
        id: menu

        width: remove.implicitWidth

        Rectangle {
            id: editBack

            color: palette.contextBackground

            width: menu.width
            height: edit.height

            CLabel {
                id: edit
                width: parent.width
                font.pixelSize: 4 * settings.pixelDensity
                text: qsTr("Edit")

                MouseArea {
                    anchors.fill: parent
                    onEntered: editBack.color = palette.contextButton
                    onExited: editBack.color = palette.contextBackground
                    onClicked: {
                        contextMenu.visible = false
                        console.log("Edit clicked")
                    }
                }

                CHorizontalSeparator {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
            }
        }

        Rectangle {
            id: removeBack

            color: palette.contextBackground

            width: menu.width
            height: edit.height

            CLabel {
                id: remove
                font.pixelSize: 4 * settings.pixelDensity
                text: qsTr("Remove")

                MouseArea {
                    anchors.fill: parent
                    onEntered: removeBack.color = palette.contextButton
                    onExited: removeBack.color = palette.contextBackground

                    onClicked: {
                        contextMenu.visible = false
                        console.log("Remove clicked")
                    }
                }
            }
        }
    }

    function popup()
    {
        visible = true
    }
}
