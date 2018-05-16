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
import "../components"
import LecturesManager 1.1

BlankScreen {
    id: educationalMaterialsScreen

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
                text: qsTr("Educational materials")
            }

            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("MainMenuScreen.qml"))
            }
        }
    }

    ListView {
        id: myView

        model: LecturesManager.labsTree
        delegate: CTreeItem {}

        width: 0.25 * settings.windowWidth
        height: settings.windowHeight - toolBar.height

        anchors {
            left: parent.left
            top: toolBar.bottom
        }

        onFocusChanged: {
            visible = false
            visible = true
        }

        MouseArea {
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button == Qt.LeftButton)
                    modelData.isOpen = !modelData.isOpen;
                else
                    contextMenu.visible = true
            }
        }
    }

    Rectangle {
        id: rightRect

        width: 0.75 * settings.windowWidth
        height: settings.windowHeight - toolBar.height

        anchors {
            left: myView.right
            top: toolBar.bottom
        }
    }
}
