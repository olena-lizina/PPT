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
    id: addStudentScreen

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

            CInputField{
                labelText: qsTr("Full name")
                placeholder: qsTr("Enter name")
            }

            CInputField{
                labelText: qsTr("Phone")
                placeholder: qsTr("+380501111111")
            }

            CInputField{
                labelText: qsTr("Email")
                placeholder: qsTr("email@gmail.com")
            }

            CInputField{
                labelText: qsTr("Group")
                placeholder: qsTr("IS53")
            }

            CNavigationButton {
                id: saveBtn
                text: qsTr("Save")
                icon: "\u2611"
            }

            CNavigationButton {
                id: clearBtn
                text: qsTr("Clear")
                icon: "\u2613"
            }
        }
    }

    CScrollBar {
        flickableItem: menuFlickable
    }
}
