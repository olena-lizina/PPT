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
import "../components"
import LecturesManager 1.1

BlankScreen {
    id: mainMenuScreen

    Component.onCompleted: LecturesManager.loadAllLectures()

    CToolBar {
        id: toolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        CLabel {
            anchors.fill: parent
            anchors.leftMargin: 5 * settings.pixelDensity
            text: qsTr("PPT")
            font.pixelSize: 10 * settings.pixelDensity
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
                text: qsTr("Administration")
                icon: "\uD83D\uDC71"
                onClicked: stackView.push(Qt.resolvedUrl("AdministrationScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Educational materials")
                icon: "\uf115"
                onClicked: stackView.push(Qt.resolvedUrl("EducationalMaterialsScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Learning progress")
                icon: "\uf080"
                onClicked: stackView.push(Qt.resolvedUrl("ProgressScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Manual")
                icon: "\uf1c9"
                //onClicked: stackView.push(Qt.resolvedUrl("ModulesScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("About")
                icon: "\uf0e5"
                onClicked: stackView.push(Qt.resolvedUrl("AboutScreen.qml"))
            }
        }
    }

    CScrollBar {
        flickableItem: menuFlickable
    }
}

