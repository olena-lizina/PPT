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

BlankScreen {
    id: administrationScreen

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
                text: qsTr("Administration")
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
                text: qsTr("Students")
                icon: "\uD83D\uDC65"
                onClicked: stackView.push(Qt.resolvedUrl("admins/StudentScreen.qml"))
            }

            CNavigationButton {
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Manage courses")
                icon: "\uf085"
                onClicked: stackView.push(Qt.resolvedUrl("admins/CourseSettingsScreen.qml"))
            }
        }
    }

    CScrollBar {
        flickableItem: menuFlickable
    }
}
