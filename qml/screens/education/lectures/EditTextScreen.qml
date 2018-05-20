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
import "../../../components"
import "../.."

Item {
    id: rootItem

    property string textItText: LecturesManager.getFileContent(ScreenContextBuffer.screenType, ScreenContextBuffer.selectedIdx, ScreenContextBuffer.nesting)

    Component.onCompleted: {
        codeArea.myNesting = ScreenContextBuffer.nesting
        codeArea.myIdx = ScreenContextBuffer.selectedIdx
        if (ScreenContextBuffer.edit)
        {
            btn1.visible = true
            btn2.visible = true
            btn3.visible = true
            btn5.visible = true
            btn6.visible = false
            btn4.visible = false
        }
        else
        {
            btn1.visible = false
            btn2.visible = false
            btn3.visible = false
            btn4.visible = true
            btn5.visible = false
            btn6.visible = true
        }
    }

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
                icon: "\uf034"
                tooltipText: qsTr("Select all")
                onClicked: codeArea.selectAll()
            }

            CToolButton {
                id: btn2
                visible: true
                Layout.fillHeight: true
                icon: "\uf0c5"
                tooltipText: qsTr("Copy")
                onClicked: codeArea.copy()
            }

            CToolButton {
                id: btn3
                visible: true
                Layout.fillHeight: true
                icon: "\uf0c4"
                tooltipText: qsTr("Cut")
                onClicked: codeArea.cut()
            }

            CToolButton {
                id: btn4
                Layout.fillHeight: true
                icon: "\u270e"
                tooltipText: qsTr("Edit")
                onClicked: {
                    btn1.visible = true
                    btn2.visible = true
                    btn3.visible = true
                    btn5.visible = true
                    btn6.visible = false
                    visible = false
                    ScreenContextBuffer.edit = true
                }
            }

            CToolButton {
                id: btn5
                visible: true
                Layout.fillHeight: true
                icon: "\u2714"
                tooltipText: qsTr("Save lecture")
                onClicked: {

                    if (ScreenContextBuffer.screenType === LecturesManager.LiteratureListFile)
                        LecturesManager.saveLiterListFileContent(codeArea.text, ScreenContextBuffer.selectedIdx)
                    else if (ScreenContextBuffer.screenType === LecturesManager.EducationProgramFile)
                        LecturesManager.saveEducProgFileContent(codeArea.text, ScreenContextBuffer.selectedIdx)
                    else if (ScreenContextBuffer.screenType === LecturesManager.EducationPlanFile)
                        LecturesManager.saveEducPlanFileContent(codeArea.text, ScreenContextBuffer.selectedIdx);
                    else
                        LecturesManager.saveFileContent(codeArea.text, ScreenContextBuffer.nesting, ScreenContextBuffer.selectedIdx)

                    LecturesManager.clearComponentCache()
                    Qt.inputMethod.hide()
                    btn1.visible = false
                    btn2.visible = false
                    btn3.visible = false
                    btn4.visible = true
                    btn6.visible = true
                    visible = false
                    ScreenContextBuffer.edit = false
                }
            }

            CToolButton {
                id: btn6
                Layout.fillHeight: true
                icon: "\u2716"
                tooltipText: qsTr("Delete")
                onClicked: {
                    var parameters = {
                        title: qsTr("Delete file"),
                        text: qsTr("Are you sure you want to delete this file?")
                    }

                    var callback = function(value)
                    {
                        if (value)
                        {
                            LecturesManager.removeFile(ScreenContextBuffer.screenType, ScreenContextBuffer.selectedIdx, ScreenContextBuffer.nesting);
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

        property int myNesting
        property int myIdx

        Component.onDestruction: {
            if (ScreenContextBuffer.screenType === LecturesManager.LiteratureListFile)
                LecturesManager.saveLiterListFileContent(codeArea.text, myIdx)
            else if (ScreenContextBuffer.screenType === LecturesManager.EducationProgramFile)
                LecturesManager.saveEducProgFileContent(codeArea.text, myIdx)
            else if (ScreenContextBuffer.screenType === LecturesManager.EducationPlanFile)
                LecturesManager.saveEducPlanFileContent(codeArea.text, myIdx);
            else
                LecturesManager.saveFileContent(codeArea.text, myNesting, myIdx)
        }

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
}


