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
//import ProjectManager 1.1
import "../components"

BlankScreen {
    id: progressScreen

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
                text: qsTr("Learning progress")
            }

            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("MainMenuScreen.qml"))
            }
        }
    }

    CFlickable {
        id: menuFlickable
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        contentHeight: column.height

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right

            CNavigationButton {
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Work done")
                icon: "\u2611"
//                onClicked: {
//                    ProjectManager.baseFolder = ProjectManager.Examples
//                    stackView.push(Qt.resolvedUrl("ExamplesScreen.qml")) // EducationalMaterials.qml
//                }
            }

            CNavigationButton {
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Work on execution")
                icon: "\u26A0"
//                onClicked: {
//                    ProjectManager.baseFolder = ProjectManager.Projects
//                    stackView.push(Qt.resolvedUrl("AdministrationScreen.qml")) //
//                }
            }

            CNavigationButton {
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Gradebook")
                icon: "\uD83D\uDCCA"
                //onClicked: stackView.push(Qt.resolvedUrl("SettingsScreen.qml"))
            }

            CNavigationButton {
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Reports")
                icon: "\u2709"
                //onClicked: stackView.push(Qt.resolvedUrl("ModulesScreen.qml"))
            }            
        }
    }

    CScrollBar {
        flickableItem: listView
    }
}
