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
import ScreenContextBuffer 1.1
import "../../../components"
import "../.."

Item {

    anchors.fill: parent

    CToolBar {
        id: toolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        RowLayout {
            anchors.fill: parent
            spacing: 0

            CToolButton {
                Layout.fillHeight: true
                icon: "\uf034"
                tooltipText: qsTr("Select all")
                onClicked: codeArea.selectAll()
            }

            CToolButton {
                visible: true
                Layout.fillHeight: true
                icon: "\uf0c5"
                tooltipText: qsTr("Copy")
                onClicked: codeArea.copy()
            }

            CToolButton {
                visible: true
                Layout.fillHeight: true
                icon: "\uf0c4"
                tooltipText: qsTr("Cut")
                onClicked: codeArea.cut()
            }

            CToolButton {
                visible: true
                Layout.fillHeight: true
                icon: "\u2714"
                tooltipText: qsTr("Save lecture")
                onClicked: {
                    LecturesManager.saveFileContent(codeArea.text, ScreenContextBuffer.nesting, ScreenContextBuffer.selectedIdx)
                    LecturesManager.clearComponentCache()
                    Qt.inputMethod.hide()
                    ScreenContextBuffer.loaderSource = "education/lectures/DisplayTextScreen.qml"
                }
            }
        }
    }

    CCodeArea {
        id: codeArea

        Component.onDestruction: {
            LecturesManager.saveFileContent(text, ScreenContextBuffer.nesting, ScreenContextBuffer.selectedIdx)
        }

        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        indentSize: settings.indentSize

        text: LecturesManager.getFileContent(ScreenContextBuffer.screenType, ScreenContextBuffer.selectedIdx, ScreenContextBuffer.nesting)

        onTextChanged: {
            LecturesManager
        }
    }
}


