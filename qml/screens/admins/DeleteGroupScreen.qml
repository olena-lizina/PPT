/****************************************************************************
**
** Copyright (C) 2017-2018 Olena Lizina
**
****************************************************************************/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import ProjectManager 1.1
import "../../components"
import "../"

BlankScreen {
    id: deleteGroupScreen

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
                text: qsTr("Delete group")
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

            CInformationItem {
                text: qsTr("ISz41")
                description: qsTr("Group ID")
                descriptionColor: "#778899"
            }

            CInformationItem {
                text: qsTr("IOT")
                description: qsTr("Faculty")
                descriptionColor: "#778899"
            }

            CInformationItem {
                text: qsTr("4")
                description: qsTr("Year")
                descriptionColor: "#778899"
            }

            CNavigationButton {
                id: deleteBtn
                text: qsTr("Delete")
                icon: "\u2611"
            }

            CNavigationButton {
                id: clearBtn
                text: qsTr("Cancel")
                icon: "\u2613"
            }
        }
    }

    CScrollBar {
        flickableItem: menuFlickable
    }
}
