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
import QtQuick.Layouts 1.2
import "../../../components/"
import "../.."
import LabsManager 1.1
import ScreenContextBuffer 1.1

BlankScreen {
    id: labsScreen

    CToolBar {
        id: toolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        RowLayout {
            anchors.fill: parent
            spacing: 0

            CBackButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Laboratory works")
            }

            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("../../MainMenuScreen.qml"))
            }
        }
    }

    ListView {
        id: leftMenu

        model: LabsManager.labsTree
        delegate: CLabTreeItem {}

        width: 0.25 * settings.windowWidth
        height: settings.windowHeight - toolBar.height

        anchors {
            left: parent.left
            top: toolBar.bottom
        }
    }

    Rectangle {
        id: separator
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: leftMenu.right
        anchors.leftMargin: 3
        width: Math.max(1, Math.round(0.8 * settings.pixelDensity))
        color: palette.toolBarStripe
        z: 2
    }

    Rectangle {
        id: rightRect

        width: 0.75 * settings.windowWidth
        height: settings.windowHeight - toolBar.height

        anchors {
            left: separator.right
            top: toolBar.bottom
            bottom: parent.bottom
        }

        Component.onCompleted: {
            rectLoader.source = "DummyScreen.qml"
        }
    }

    Connections {
        target: ScreenContextBuffer
        onLabsNestingChanged: handleItemChanged()
        onLabIdChanged: handleItemChanged()

        onExecutorIdChanged: rectLoader.source = "ReportInfoScreen.qml"

        onBackToReport: rectLoader.source = "ReportInfoScreen.qml"

        onFilePathChanged: {
            rectLoader.source = "ShowReportScreen.qml"
        }

        onOnOpenLabScreen: {
            LabsManager.selectedLab = ScreenContextBuffer.labId
            rectLoader.source = "EditLabScreen.qml"
        }
    }

    Loader {
        id: rectLoader
        anchors.fill: rightRect
        source: ""
    }

    function handleItemChanged()
    {
        if (ScreenContextBuffer.labsNesting === 0) // discipline
        {
            rectLoader.source = "AddLabScreen.qml"
        }
        else
        {
            if (!LabsManager.labExist(ScreenContextBuffer.labId))
                rectLoader.source = "AddLabScreen.qml"
            else
                rectLoader.source = "LabInfoScreen.qml"
        }
    }
}
