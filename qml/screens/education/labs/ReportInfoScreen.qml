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
import QtQuick.Controls 1.4
import ScreenContextBuffer 1.1
import LabsManager 1.1
import "../../../components"

Rectangle {
    id: body

    width: 0.75 * settings.windowWidth
    height: settings.windowHeight - toolBar.height

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#ffffb3" }
        GradientStop { position: 1.0; color: "#b3d9ff" }
    }

    CToolBar {
        id: topBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 18.5 * settings.pixelDensity

        Row {
            anchors.fill: topBar
            anchors.leftMargin: 20
            spacing: 3

            CInformationItem {
                text1: LabsManager.getExecutor(ScreenContextBuffer.executorId).studName
                text2: qsTr("Finished: ") + LabsManager.getExecutor(ScreenContextBuffer.executorId).finished
                text3: qsTr("Mark: ") + LabsManager.getExecutor(ScreenContextBuffer.executorId).mark
                text4: qsTr("Evaluated: ") + LabsManager.getExecutor(ScreenContextBuffer.executorId).evalDate
            }
        }
    }

    CListView {
        id: list
        anchors {
            left: body.left
            right: body.right
            top: topBar.bottom
            bottom: body.bottom
        }

        model: LabsManager.getReportFiles(ScreenContextBuffer.executorId);

        delegate: CTextLink {
            text: modelData

            onClicked: {
                ScreenContextBuffer.filePath = modelData
            }
        }
    }
}

