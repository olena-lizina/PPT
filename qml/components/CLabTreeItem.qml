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
import LabsManager 1.1
import ScreenContextBuffer 1.1
import ".."

Row {
    id: root

    Rectangle{
        width: 20
        height: list.height
        opacity: 0
    }

    Column {
        id: list

        CLabel {
            height: 18.5 * settings.pixelDensity
            width: 0.25 * settings.windowWidth - 20

            text: modelData.text
            wrapMode: Text.Wrap

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                onClicked: {
                    if (modelData.nesting === -1)
                    {
                        stackView.push(Qt.resolvedUrl("../../BaseLecturesScreen.qml"))
                        return;
                    }

                    if (modelData.nesting === 0)
                        ScreenContextBuffer.labDisciplineId = modelData.idx
                    if (modelData.nesting === 1)
                        ScreenContextBuffer.labId = modelData.idx

                    ScreenContextBuffer.labsNesting = modelData.nesting

                    if (mouse.button == Qt.LeftButton)
                    {
                        modelData.isOpen = !modelData.isOpen;
                    }
                    else if (mouse.button == Qt.RightButton)
                    {
                        if (modelData.nesting !== 0)
                        {
                            contextMenu.visible = !contextMenu.visible;
                        }
                    }
                }

                onPressAndHold: tooltip.show(modelData.text)
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

            Item {
                id: contextMenu
                visible: false

                property int delegateWidth: parent.width
                property int delegateHeight: 9 * settings.pixelDensity

                Column {

                    CContextMenuButton {
                        text: qsTr("Edit")
                        width: contextMenu.delegateWidth
                        height: contextMenu.delegateHeight
                        visible: modelData.nesting !== 0
                        onClicked: {
                            root.contextMenuCallback(0)
                        }
                    }

                    CContextMenuButton {
                        width: contextMenu.delegateWidth
                        height: contextMenu.delegateHeight
                        text: qsTr("Remove")
                        visible: modelData.nesting !== 0
                        onClicked: {
                            root.contextMenuCallback(1)
                        }
                    }
                }
            }
        }

        Loader{ source: modelData.isOpen ? "CLabChildTreeList.qml" : "CTreeEmptyItem.qml"; }
    }

    function contextMenuCallback(idx)
    {
        contextMenu.visible = false;

        switch (idx)
        {
        case 0:
            var editParams = {
                title: qsTr("Edit laboratory work"),
                label: qsTr("Enter the name"),
                dateLabel: qsTr("Enter finish date"),
                text: modelData.text,
                date: Date.fromLocaleString(Qt.locale(), LabsManager.getLabFinishDate(modelData.idx),"dd.MM.yyyy")
            }

            var callback = function(value1, value2)
            {
                console.log("name: " + value1 + ", date: " + value2)
                LabsManager.editLab(value1, value2, ScreenContextBuffer.labId)
            }
            dialog.open(dialog.types.createLab, editParams, callback)
            break;
        case 1:
            var removeParameters = {
                title: qsTr("Remove laboratory work"),
                text: qsTr("Press OK to delete laboratory work with name: \"%1\"").arg(modelData.text)
            }

            var removeCallback = function(value)
            {
                console.log("Callback: " + value + ", idx: " + modelData.idx)

                if (!value)
                    return;

                LabsManager.removeLab(modelData.idx)
            }

            dialog.open(dialog.types.confirmation, removeParameters, removeCallback)
            break;
        }
    }
}
