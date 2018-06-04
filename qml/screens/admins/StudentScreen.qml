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

    signal findByGroup()
    signal showExistMsg()

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
                text: qsTr("Students")
            }

            CToolButton {
                Layout.fillHeight: true
                icon: "\uf067"
                tooltipText: qsTr("Add student")
                onClicked: addStudentBtnClicked()
            }

            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("../MainMenuScreen.qml"))
            }
        }        
    }

    Column {
        id: findMenuItem

        anchors {
            left: parent.left
            right: parent.right
            top: toolBar.bottom
        }

        CFilterItem {
            id: findMenu

            comboLabelText: qsTr("Select group")
            textFieldLabelText: qsTr("Enter name")

            comboBox.model: StudentManager.getGroups()

            onFindBtnClicked: {
                var currText = findMenu.comboBox.textAt(findMenu.comboBox.currentIndex)
                if (findMenu.textItem !== "")
                    listView.model = StudentManager.getStudentsByName(findMenu.textItem);
                else
                {
                    if (currText !== qsTr("All groups"))
                        listView.model = StudentManager.getStudentsByGroup(currText);
                    else
                        listView.model = StudentManager.getAllStudents();
                }
            }
        }
    }

    function addStudentBtnClicked()
    {
        var parameters = {
            title: qsTr("Add student"),
            addNew: true
        }

        var callback = function()
        {
            updateScreenContent()
        }

        dialog.open(dialog.types.editStudentDialog, parameters, callback)
    }

    CListView {
        id: listView

        anchors {
            left: parent.left
            right: parent.right
            top: findMenuItem.bottom
            bottom: parent.bottom
        }

        model: StudentManager.getAllStudents();

        delegate: CEditOrRemoveButton {

            label_1: modelData.name
            label_2: modelData.group

            anchors.left: parent.left
            anchors.right: parent.right

            onClicked: {
                var parameters = {
                    title: qsTr("Information about student"),
                    name: modelData.name,
                    phone: modelData.phone,
                    group: modelData.group,
                    email: modelData.email,
                    imagePath: modelData.photo === "" ? "/resources/images/dummy.png" : ("file:///" + applicationDirPath + "/photos/" + modelData.photo),
                    studentId: modelData.id,
                    readOnly: true
                }

                var callback = function() {}

                dialog.open(dialog.types.editStudentDialog, parameters, callback)
            }

            onEditClicked: {
                var parameters = {
                    title: qsTr("Edit student"),
                    name: modelData.name,
                    phone: modelData.phone,
                    group: modelData.group,
                    email: modelData.email,
                    imagePath: modelData.photo === "" ? "/resources/images/dummy.png" : ("file:///" + applicationDirPath + "/photos/" + modelData.photo),
                    studentId: modelData.id
                }

                var callback = function()
                {
                    updateScreenContent()
                }

                dialog.open(dialog.types.editStudentDialog, parameters, callback)
            }

            onRemoveClicked: {
                var parameters = {
                    title: qsTr("Delete the student"),
                    text: qsTr("Are you sure you want to delete \"%1\"?").arg(modelData.name)
                }

                var callback = function(value)
                {
                    if (value)
                    {
                        StudentManager.deleteStudent(modelData.id)
                        updateScreenContent()
                    }
                }

                dialog.open(dialog.types.confirmation, parameters, callback)
            }
        }
    }

    CScrollBar {
        flickableItem: listView
    }

    function updateScreenContent()
    {
        listView.model = StudentManager.getAllStudents()
        findMenu.comboBox.model = StudentManager.getGroups();
    }
}
