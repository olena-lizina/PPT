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
//                onClicked: {
//                    ProjectManager.baseFolder = ProjectManager.Examples
//                    stackView.push(Qt.resolvedUrl("ExamplesScreen.qml")) // EducationalMaterials.qml
//                }
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
        flickableItem: listView
    }
}
