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
import ProjectManager 1.1
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
                text: qsTr("Add new student")
                icon: "\uf067"
                onClicked: stackView.push(Qt.resolvedUrl("admins/AddStudentScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Edit existing student")
                icon: "\u270e"
                onClicked: stackView.push(Qt.resolvedUrl("admins/EditStudentScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Delete existing student")
                icon: "\u2613"
                onClicked: stackView.push(Qt.resolvedUrl("admins/DeleteStudentScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Add group")
                icon: "\uf067"
                onClicked: stackView.push(Qt.resolvedUrl("admins/AddGroupScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Edit group")
                icon: "\u270e"
                onClicked: stackView.push(Qt.resolvedUrl("admins/EditGroupScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Delete group")
                icon: "\u2613"
                onClicked: stackView.push(Qt.resolvedUrl("admins/DeleteGroupScreen.qml"))
            }
        }
    }

    CScrollBar {
        flickableItem: menuFlickable
    }
}
