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
import "../../components"
import "../"

BlankScreen {
    id: selectStudentScreen

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
                text: qsTr("Select student to delete")
            }
        }
    }

    signal findButtonCLicked()

    Column {
        id: column

        anchors {
            left: parent.left
            right: parent.right
            top: toolBar.bottom
        }

        CFilterItem {
            comboLabelText: qsTr("Select group")
            textFieldLabelText: qsTr("Enter name")
        }        
    }

    CListView {
        id: listView

        anchors {
            left: parent.left
            right: parent.right
            top: column.bottom
            bottom: parent.bottom
        }

        model: ListModel{
            ListElement {
                    name: "Lizina Olena Volodymyrivna"
                    group: "ISz41"
                }
            ListElement {
                    name: "Ivashchenko Galyna Ivanivna"
                    group: "ISz41"
                }
        }

        delegate: CFileButton {
            text: name + ',' + group
            rightButtonIcon: "\u2613"

            onRightClicked: stackView.push(Qt.resolvedUrl("DeleteStudentScreen.qml"))
        }
    }

    CScrollBar {
        flickableItem: listView
    }
}
