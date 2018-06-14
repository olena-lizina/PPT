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
import QtQuick.Controls 1.4
import LecturesManager 1.1
import ScreenContextBuffer 1.1
import MailServiceManager 1.1
import "../../../components"
import "../.."

Item {
    id: rootItem

    property string textItText: LecturesManager.getFileContent(ScreenContextBuffer.screenType, ScreenContextBuffer.selectedIdx)

    CToolBar {
        id: toolBar
        anchors.left: rootItem.left
        anchors.right: rootItem.right
        anchors.top: rootItem.top
        height: 18.5 * settings.pixelDensity

        RowLayout {
            anchors.fill: parent
            spacing: 0

            CToolButton {
                id: btn1
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                icon: "\uf034"
                tooltipText: qsTr("Select all")
                onClicked: codeArea.selectAll()
                visible: ScreenContextBuffer.edit
            }

            CToolButton {
                id: btn2
                visible: ScreenContextBuffer.edit
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                icon: "\uf0c5"
                tooltipText: qsTr("Copy")
                onClicked: codeArea.copy()
            }

            CToolButton {
                id: btn3
                visible: ScreenContextBuffer.edit
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                icon: "\uf0c4"
                tooltipText: qsTr("Cut")
                onClicked: codeArea.cut()
            }

            CToolButton {
                id: btn4
                Layout.fillHeight: true
                Layout.fillWidth: ScreenContextBuffer.edit
                icon: "\u270e"
                tooltipText: qsTr("Edit")
                visible: !ScreenContextBuffer.edit
                onClicked: ScreenContextBuffer.edit = true
            }

            CToolButton {
                id: btn5
                visible: ScreenContextBuffer.edit
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                icon: "\u2714"
                tooltipText: qsTr("Save lecture")
                onClicked: saveContent()
            }

            CToolButton {
                id: btn6
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                icon: "\u2716"
                tooltipText: qsTr("Delete")
                visible: !ScreenContextBuffer.edit
                onClicked: {
                    var parameters = {
                        title: qsTr("Delete file"),
                        text: qsTr("Are you sure you want to delete this file?")
                    }

                    var callback = function(value)
                    {
                        if (value)
                        {
                            LecturesManager.removeFile(ScreenContextBuffer.screenType, ScreenContextBuffer.selectedIdx);
                            ScreenContextBuffer.loaderSource = ""
                        }
                    }

                    dialog.open(dialog.types.confirmation, parameters, callback)
                }
            }
        }
    }

    TextArea {
        id: codeArea

        Component.onDestruction: saveContent()

        anchors.top: toolBar.bottom
        anchors.bottom: rootItem.bottom
        anchors.left: rootItem.left
        anchors.right: rootItem.right

        text: textItText

        readOnly: !ScreenContextBuffer.edit

        onActiveFocusChanged: {
            if (activeFocus)
                forceActiveFocus()
        }

        selectByMouse: true
        wrapMode: TextEdit.Wrap
        textFormat: TextEdit.RichText
        inputMethodHints: Qt.ImhNoPredictiveText
        onLinkActivated: Qt.openUrlExternally(link)
    }

    function saveContent()
    {
        if (textItText !== codeArea.text)
        {
            LecturesManager.saveFileContent(codeArea.text, ScreenContextBuffer.selectedIdx, ScreenContextBuffer.screenType)
            if (ScreenContextBuffer.screenType === LecturesManager.ThemeLectureFileType || ScreenContextBuffer.screenType === LecturesManager.SubthemeLectureFileType)
                MailServiceManager.sendEducationMaterials(ScreenContextBuffer.courseName, ScreenContextBuffer.courseId)
        }

        LecturesManager.clearComponentCache()
        Qt.inputMethod.hide()
        ScreenContextBuffer.edit = false
    }
}


