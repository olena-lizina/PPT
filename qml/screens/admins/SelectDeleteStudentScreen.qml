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
import StudentManager 1.1
import "../../components"
import "../"

BlankScreen {
    id: selectStudentScreen

    property alias findMenuAlias: findMenu
    property alias listViewAlias: listView

    signal findByGroup()

    Stack.onStatusChanged: {
        if (Stack.status === Stack.Activating)
        {
            findMenu.comboBox.model = StudentManager.getGroups();
            findMenu.comboBox.currentIndex = StudentManager.selectedGroupIdx();
        }
        listView.model = StudentManager.getStudentsByGroup(findMenu.comboBox.textAt(findMenu.comboBox.currentIndex));
    }

    onFindByGroup: {
        listView.model = StudentManager.getStudentsByGroup(findMenu.comboBox.currentIndex);
    }

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
                text: qsTr("Delete existing student")
            }
        }
    }

    Column {
        id: column

        anchors {
            left: parent.left
            right: parent.right
            top: toolBar.bottom
        }

        CFilterItem {
            id: findMenu

            property string dummy: "------"

            comboLabelText: qsTr("Select group")
            textFieldLabelText: qsTr("Enter name")

            comboBox.onCurrentIndexChanged:
            {
                if (findMenu.comboBox.textAt(findMenu.comboBox.currentIndex) !== dummy)
                    StudentManager.selectedGroupIdx(findMenu.comboBox.currentIndex);
                listView.model = StudentManager.getStudentsByGroup(findMenu.comboBox.textAt(findMenu.comboBox.currentIndex));
            }

            onFindBtnClicked: {
                if (findMenu.comboBox.textAt(findMenu.comboBox.currentIndex) !== dummy && textItem !== "")
                {
                    StudentManager.selectedGroupIdx(findMenu.comboBox.currentIndex);
                    listView.model = StudentManager.getStudentsByNameAndGroup(findMenu.textItem, findMenu.comboBox.textAt(findMenu.comboBox.currentIndex));
                }
                if (findMenu.comboBox.textAt(findMenu.comboBox.currentIndex) !== dummy && textItem === "")
                {
                    StudentManager.selectedGroupIdx(findMenu.comboBox.currentIndex);
                    listView.model = StudentManager.getStudentsByGroup(findMenu.comboBox.textAt(findMenu.comboBox.currentIndex));
                }
                else if (findMenu.comboBox.textAt(findMenu.comboBox.currentIndex) === dummy && textItem !== "")
                {
                    StudentManager.selectedGroupIdx(0);
                    listView.model = StudentManager.getStudentsByName(findMenu.textItem);
                }
            }
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

        delegate: CFileButton {
            text: modelData
            rightButtonIcon: "\u2613"

            onRightClicked:
            {
                var parameters = {
                    title: qsTr("Delete the student"),
                    text: qsTr("Are you sure you want to delete student \n\"%1\"?").arg(modelData)
                }

                var callback = function(value)
                {
                    if (value)
                    {
                        var dummy = "------";
                        StudentManager.deleteStudent(modelData);
                        selectStudentScreen.findMenuAlias.comboBox.model = StudentManager.getGroups();
                        StudentManager.selectedGroupIdx(0);
                        selectStudentScreen.findMenuAlias.comboBox.currentIndex = StudentManager.selectedGroupIdx();
                    }
                }

                dialog.open(dialog.types.confirmation, parameters, callback)
            }
        }
    }

    CScrollBar {
        flickableItem: listView
    }
}
