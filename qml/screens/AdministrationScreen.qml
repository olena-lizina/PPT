/****************************************************************************
**
** Copyright (C) 2017-2018 Olena Lizina
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
//                onClicked: {
//                    ProjectManager.baseFolder = ProjectManager.Projects
//                    stackView.push(Qt.resolvedUrl("AdministrationScreen.qml")) //
//                }
            }

            CNavigationButton {
                text: qsTr("Edit existing student")
                icon: "\u270e"
//                onClicked: {
//                    ProjectManager.baseFolder = ProjectManager.Examples
//                    stackView.push(Qt.resolvedUrl("ExamplesScreen.qml")) // EducationalMaterials.qml
//                }
            }

            CNavigationButton {
                text: qsTr("Delete existing student")
                icon: "\u2613"
                //onClicked: stackView.push(Qt.resolvedUrl("SettingsScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Add group")
                icon: "\uf067"
                //onClicked: stackView.push(Qt.resolvedUrl("ModulesScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Edit group")
                icon: "\u270e"
                //onClicked: stackView.push(Qt.resolvedUrl("AboutScreen.qml"))
            }

            CNavigationButton {
                text: qsTr("Delete group")
                icon: "\u2613"
                //onClicked: stackView.push(Qt.resolvedUrl("AboutScreen.qml"))
            }
        }
    }

    CScrollBar {
        flickableItem: listView
    }
}
