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
import LabsManager 1.1
import ScreenContextBuffer 1.1
import MailServiceManager 1.1
import "../../../components"
import "../.."

Item {
    id: rootItem

    property string textItText
    Component.onCompleted: {
        textItText = LabsManager.reportFileContent(ScreenContextBuffer.filePath)
    }

    CToolBar {
        id: topBar
        anchors.left: rootItem.left
        anchors.right: rootItem.right
        anchors.top: rootItem.top
        height: 18.5 * settings.pixelDensity

        Row {
            anchors.fill: topBar
            anchors.leftMargin: 20
            spacing: 3

            CNavigationButton {
                text: "Back"//ScreenContextBuffer.filePath

                onClicked: {
                    ScreenContextBuffer.callBackToReport()
                }
            }
        }
    }

    CCodeArea {
        id: codeArea

        anchors.top: topBar.bottom
        anchors.bottom: rootItem.bottom
        anchors.left: rootItem.left
        anchors.right: rootItem.right

        text: textItText
    }
}


