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

            onFindBtnClicked: {
                var dummy = "------";
                StudentManager.selectedGroupIdx(findMenu.comboBox.currentIndex);
                var currText = findMenu.comboBox.textAt(findMenu.comboBox.currentIndex)
                if (currText !== dummy && textItem !== "")
                    listView.model = StudentManager.getStudentsByNameAndGroup(findMenu.textItem, currText);
                if (currText !== dummy && textItem === "")
                    listView.model = StudentManager.getStudentsByGroup(currText);
                if (currText === dummy && textItem !== "")
                    listView.model = StudentManager.getStudentsByName(findMenu.textItem);
                if (currText === dummy && textItem === "")
                    listView.model = StudentManager.getAllStudents();
            }
        }
    }

    function addStudentBtnClicked()
    {
        var parameters = {
            title: qsTr("Add student"),
            action: "add"
        }

        var callback = function()
        {
            updateView()
        }

        dialog.open(dialog.types.studentInfoDialog, parameters, callback)
    }

    CListView {
        id: listView

        anchors {
            left: parent.left
            right: parent.right
            top: findMenuItem.bottom
            bottom: parent.bottom
        }

        delegate: CEditOrRemoveButton {
            text: modelData

            anchors.left: parent.left
            anchors.right: parent.right

            onEditClicked: {
                StudentManager.selectedStudent(modelData)
                editStudent()
            }

            onRemoveClicked: {
                StudentManager.selectedStudent(modelData)

                var parameters = {
                    title: qsTr("Delete the student"),
                    text: qsTr("Are you sure you want to delete \"%1\"?").arg(modelData)
                }

                var callback = function(value)
                {
                    if (value)
                    {
                        StudentManager.deleteStudent(StudentManager.selectedStudentName(), StudentManager.selectedStudentPhone(),
                                                     StudentManager.selectedStudentEmail(), StudentManager.selectedStudentGroup())
                        updateView()
                    }
                }

                dialog.open(dialog.types.confirmation, parameters, callback)
            }
        }
    }

    CScrollBar {
        flickableItem: listView
    }

    function editStudent()
    {
        var parameters = {
            title: qsTr("Edit student"),
            action: "edit",
            nameTxt: StudentManager.selectedStudentName(),
            phoneTxt: StudentManager.selectedStudentPhone(),
            emailTxt: StudentManager.selectedStudentEmail(),
            groupTxt: StudentManager.selectedStudentGroup()
        }

        var callback = function()
        {
            updateView()
        }

        dialog.open(dialog.types.studentInfoDialog, parameters, callback)
    }

    Stack.onStatusChanged:
    {
        if (Stack.status === Stack.Activating)
        {
            findMenu.comboBox.model = StudentManager.getGroups();
            findMenu.comboBox.currentIndex = StudentManager.selectedGroupIdx();
        }
        listView.model = StudentManager.getAllStudents();
    }

    function updateView()
    {
        findMenu.comboBox.model = StudentManager.getGroups();
        findMenu.comboBox.currentIndex = StudentManager.selectedGroupIdx();

        if (StudentManager.selectedGroupIdx() !== 0)
            listView.model = StudentManager.getStudentsByGroup(findMenu.comboBox.textAt(StudentManager.selectedGroupIdx()));
        else
            listView.model = StudentManager.getAllStudents();
    }
}
