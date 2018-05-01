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
import QtGraphicalEffects 1.0
import StudentManager 1.1
import ".."

BaseDialog {
    id: studentDialog

    property alias title: titleLabel.text
    property alias nameTxt: nameTextField.text
    property alias phoneTxt: phoneTextField.text
    property alias emailTxt: emailTextField.text
    property alias groupTxt: groupTextField.text

    property string action: ""

    function initialize(parameters) {
        for (var attr in parameters) {
            studentDialog[attr] = parameters[attr]
        }
    }

    DropShadow {
        anchors.fill: contentBackground
        radius: 5 * settings.pixelDensity
        color: palette.dialogShadow
        transparentBorder: true
        fast: true
        source: contentBackground
    }

    Rectangle {
        id: contentBackground
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        color: palette.dialogBackground
    }

    Item {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent

        Rectangle {
            id: header

            height: 22 * settings.pixelDensity
            anchors.left: parent.left
            anchors.right: parent.right
            color: palette.toolBarBackground

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                height: Math.max(1, Math.round(0.8 * settings.pixelDensity))
                color: palette.toolBarStripe
            }

            CLabel {
                id: titleLabel
                anchors.fill: parent
                anchors.leftMargin: 5 * settings.pixelDensity
                font.pixelSize: 10 * settings.pixelDensity
            }
        }

        CFlickable {
            id: flickable

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: header.bottom
            anchors.bottom: footer.top
            boundsBehavior: Flickable.StopAtBounds
            clip: true

            property double margin: 3 * settings.pixelDensity

            leftMargin: margin
            rightMargin: margin
            topMargin: margin
            bottomMargin: margin

            contentWidth: width - margin * 2
            contentHeight: column.height

            Column {
                id: column
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 2 * settings.pixelDensity

                CLabel {
                    id: nameLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: qsTr("Student name :")
                }

                CTextField {
                    id: nameTextField

                    onTextChanged: {
                        if (nameWarningLabel.visible)
                            nameWarningLabel.visible = false
                    }
                }

                CLabel {
                    id: nameWarningLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    wrapMode: Text.WordWrap
                    color: palette.warning
                    visible: false
                    text: qsTr("Student name cannot be left blank")
                }

                CLabel {
                    id: phoneLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: qsTr("Phone :")
                }

                CTextField {
                    id: phoneTextField


                    onTextChanged: {
                        if (phoneWarningLabel.visible)
                            phoneWarningLabel.visible = false
                    }
                }

                CLabel {
                    id: phoneWarningLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    wrapMode: Text.WordWrap
                    color: palette.warning
                    visible: false
                    text: qsTr("Phone field cannot be left blank")
                }

                CLabel {
                    id: emailLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: qsTr("Email :")
                }

                CTextField {
                    id: emailTextField

                    onTextChanged: {
                        if (emailWarningLabel.visible)
                            emailWarningLabel.visible = false
                    }
                }

                CLabel {
                    id: emailWarningLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    wrapMode: Text.WordWrap
                    color: palette.warning
                    visible: false
                    text: qsTr("Email field cannot be left blank")
                }

                CLabel {
                    id: groupLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: qsTr("Group :")
                }

                CTextField {
                    id: groupTextField

                    onTextChanged: {
                        if (groupWarningLabel.visible)
                            groupWarningLabel.visible = false
                    }
                }

                CLabel {
                    id: groupWarningLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    wrapMode: Text.WordWrap
                    color: palette.warning
                    visible: false
                    text: qsTr("Group field cannot be left blank")
                }
            }
        }

        CScrollBar {
            flickableItem: flickable
        }

        CDialogButton {
            anchors.left: parent.left
            anchors.right: footer.left
            anchors.bottom: parent.bottom
            text: qsTr("Cancel")
            onClicked: studentDialog.close()
        }

        CVerticalSeparator {
            id: footer
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
        }

        CDialogButton {
            anchors.left: footer.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            text: qsTr("OK")

            onClicked: {
                var name = nameTextField.text.toString()
                var phone = phoneTextField.text.toString()
                var email = emailTextField.text.toString()
                var group = groupTextField.text.toString()

                if (name.length === 0)
                {
                    nameWarningLabel.visible = true
                }
                if (phone.length === 0)
                {
                    phoneWarningLabel.visible = true
                }
                if (email.length === 0)
                {
                    emailWarningLabel.visible = true
                }
                if (group.length === 0)
                {
                    groupWarningLabel.visible = true
                }

                if (name.length > 0 && phone.length > 0 && email.length > 0 && group.length > 0)
                {
                    if (action === "add")
                    {
                        if (StudentManager.existsStudent(name, group))
                        {
                            nameWarningLabel.text = "Student with such name already exists";
                            nameWarningLabel.visible = true
                        }
                        else
                        {
                            StudentManager.createStudent(name, phone, email, group)
                            studentDialog.process(name)
                        }
                    }
                    else if (action === "edit")
                    {
                        StudentManager.deleteStudent(StudentManager.selectedStudentName(), StudentManager.selectedStudentPhone(),
                                                     StudentManager.selectedStudentEmail(), StudentManager.selectedStudentGroup())
                        StudentManager.createStudent(name, phone, email, group)
                        studentDialog.process(name)
                    }
                }
            }
        }
    }
}
