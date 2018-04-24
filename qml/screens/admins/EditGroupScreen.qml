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
    id: editGroupScreen

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
                text: qsTr("Edit group")
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

            CInputField{
                labelText: qsTr("Group ID")
                tText: qsTr("Enter group id")
            }

            CInputField{
                labelText: qsTr("Faculty")
                tText: qsTr("Enter faculty")
            }

            CInputField{
                labelText: qsTr("Year")
                tText: qsTr("Enter year")
            }

            CNavigationButton {
                id: saveBtn
                text: qsTr("Save")
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
