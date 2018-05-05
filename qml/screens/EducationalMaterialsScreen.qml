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
                text: qsTr("Lectures")
                icon: "\uf0f6"
                onClicked: stackView.push(Qt.resolvedUrl("education/lectures/SelectLectureScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Laboratory works")
                icon: "\u26A0"
//                onClicked: {
//                    ProjectManager.baseFolder = ProjectManager.Projects
//                    stackView.push(Qt.resolvedUrl("AdministrationScreen.qml")) //
//                }
            }

            CNavigationButton {
                text: qsTr("Individual works")
                icon: "\u26A1"
                //onClicked: stackView.push(Qt.resolvedUrl("SettingsScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Tests")
                icon: "\u261D"
                //onClicked: stackView.push(Qt.resolvedUrl("ModulesScreen.qml"))
            }            
        }
    }

    CScrollBar {
        flickableItem: menuFlickable
    }
}
