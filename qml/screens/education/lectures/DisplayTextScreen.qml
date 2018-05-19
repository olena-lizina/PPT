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
    id: editorScreen

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
                icon: "\u270e"
                tooltipText: qsTr("Edit")
                onClicked: ScreenContextBuffer.loaderSource = "education/lectures/EditTextScreen.qml"
            }
        }
    }

    CReadyCodeArea {
        id: codeArea

        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        text: LecturesManager.getFileContent(ScreenContextBuffer.screenType, ScreenContextBuffer.selectedIdx, ScreenContextBuffer.nesting)
    }
}
