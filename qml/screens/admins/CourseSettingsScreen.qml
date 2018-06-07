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
import LecturesManager 1.1
import QtQuick.Controls.Styles 1.4
import "../../components"
import "../"

BlankScreen {
    id: selectStudentScreen

    property string prevEmail: LecturesManager.teacherEmail

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
                text: qsTr("Course settings")
            }
            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("../MainMenuScreen.qml"))
            }
        }
    }

    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: toolBar.bottom
        anchors.topMargin: 10
        spacing: 2 * settings.pixelDensity

        CLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Teacher email address:")
        }

        CTextField {
            id: email
            text: LecturesManager.teacherEmail
            horizontalAlignment: TextInput.AlignHCenter
        }

        CDialogButton {
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Save email")
            onClicked: if (email.text !== "") LecturesManager.teacherEmail = email.text.toString()

            CHorizontalSeparator {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
        }

        CLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Course name:")
        }

        ComboBox {
            id: course
            implicitHeight: 14 * settings.pixelDensity
            anchors.left: parent.left
            anchors.right: parent.right

            model: LecturesManager.getCourses()

            style: ComboBoxStyle {
                id: style1
                dropDownButtonWidth: 4 * settings.pixelDensity
                background: Rectangle {
                    anchors.fill: parent
                    color: palette.textFieldBackground
                    border.width: Math.max(1, Math.round(0.3 * settings.pixelDensity))
                    border.color: palette.textFieldBorder
                }

                label: CLabel {
                    text: control.currentText
                }
            }
        }

        CLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Assign for group:")
        }

        ComboBox {
            id: group
            implicitHeight: 14 * settings.pixelDensity
            anchors.left: parent.left
            anchors.right: parent.right

            model: StudentManager.getGroups()

            style: ComboBoxStyle {
                id: style2
                dropDownButtonWidth: 4 * settings.pixelDensity
                background: Rectangle {
                    anchors.fill: parent
                    color: palette.textFieldBackground
                    border.width: Math.max(1, Math.round(0.3 * settings.pixelDensity))
                    border.color: palette.textFieldBorder
                }

                label: CLabel {
                    text: control.currentText
                }
            }
        }

        CDialogButton {
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Assign")
            onClicked: LecturesManager.saveStudentCourses(course.currentText, StudentManager.getGroupIdx(group.currentText))

            CHorizontalSeparator {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
        }
    }
}
