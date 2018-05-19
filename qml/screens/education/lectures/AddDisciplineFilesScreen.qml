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
import ScreenContextBuffer 1.1
import LecturesManager 1.1
import "../../../components"

Rectangle {
    id: root

    width: 0.75 * settings.windowWidth
    height: settings.windowHeight - toolBar.height

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#ffffb3" }
        GradientStop { position: 1.0; color: "#b3d9ff" }
    }

    property int btnClicked

    CToolBar {
        id: topBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 18.5 * settings.pixelDensity

        Row {
            anchors.fill: topBar
            anchors.leftMargin: 20
            spacing: 3

            Item {
                height: 18.5 * settings.pixelDensity
                width: 0.24 * settings.windowWidth

                CLabel {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    text: qsTr("Literature list")

                    MouseArea {
                        id: mouseArea1
                        anchors.fill: parent

                        onClicked: {
                            btnClicked = 0

                            var litParams = {
                                title: qsTr("Literature list"),
                                label: qsTr("What would you like to do?"),
                                leftBtnText: qsTr("Create literature list"),
                                rightBtnText: qsTr("Import literature list")
                            }

                            var litCallback = function(value)
                            {
                                console.log("Callback: " + value)

                                if (value === 1)
                                {
                                    getFileDialog.source = "../../../components/dialogs/SelectFileDialog.qml"
                                }
                                else
                                {
                                    ScreenContextBuffer.screenType = LecturesManager.LiteratureListFile;
                                    ScreenContextBuffer.edit = true;
                                    LecturesManager.createFile(ScreenContextBuffer.nesting, ScreenContextBuffer.selectedIdx)
                                    ScreenContextBuffer.loaderSource = "education/lectures/EditTextScreen.qml"
                                }
                            }
                            dialog.open(dialog.types.insert, litParams, litCallback)
                        }
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: palette.button
                    visible: mouseArea1.pressed
                    opacity: 0.5
                }

                CHorizontalSeparator {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
            }

            Item {
                height: 18.5 * settings.pixelDensity
                width: 0.24 * settings.windowWidth

                CLabel {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    text: qsTr("Education program")
                    wrapMode: Text.Wrap

                    MouseArea {
                        id: mouseArea2
                        anchors.fill: parent

                        onClicked: {
                            btnClicked = 1

                            var progParams = {
                                title: qsTr("Education program"),
                                label: qsTr("What would you like to do?"),
                                leftBtnText: qsTr("Create education program"),
                                rightBtnText: qsTr("Import education program")
                            }

                            var progCallback = function(value)
                            {
                                if (value === 1)
                                {
                                    getFileDialog.source = "../../../components/dialogs/SelectFileDialog.qml"
                                }
                                else
                                {
                                    ScreenContextBuffer.screenType = LecturesManager.EducationProgramFile;
                                    ScreenContextBuffer.edit = true;
                                    LecturesManager.createFile(ScreenContextBuffer.nesting, ScreenContextBuffer.selectedIdx)
                                    ScreenContextBuffer.loaderSource = "education/lectures/EditTextScreen.qml"
                                }

                                console.log("Callback: " + value)
                            }
                            dialog.open(dialog.types.insert, progParams, progCallback)
                        }
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: palette.button
                    visible: mouseArea2.pressed
                    opacity: 0.5
                }

                CHorizontalSeparator {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
            }

            Item {
                height: 18.5 * settings.pixelDensity
                width: 0.24 * settings.windowWidth

                CLabel {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    text: qsTr("Education plan")
                    wrapMode: Text.Wrap

                    MouseArea {
                        id: mouseArea3
                        anchors.fill: parent

                        onClicked: {
                            btnClicked = 2

                            var progParams = {
                                title: qsTr("Education plan"),
                                label: qsTr("What would you like to do?"),
                                leftBtnText: qsTr("Create education plan"),
                                rightBtnText: qsTr("Import education plan")
                            }

                            var progCallback = function(value)
                            {
                                console.log("Callback: " + value)

                                if (value === 1)
                                {
                                    getFileDialog.source = "../../../components/dialogs/SelectFileDialog.qml"
                                }
                                else
                                {
                                    ScreenContextBuffer.screenType = LecturesManager.EducationPlanFile;
                                    ScreenContextBuffer.edit = true;
                                    LecturesManager.createFile(ScreenContextBuffer.nesting, ScreenContextBuffer.selectedIdx)
                                    ScreenContextBuffer.loaderSource = "education/lectures/EditTextScreen.qml"
                                }
                            }
                            dialog.open(dialog.types.insert, progParams, progCallback)
                        }
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: palette.button
                    visible: mouseArea3.pressed
                    opacity: 0.5
                }

                CHorizontalSeparator {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
            }
        }
    }

    Loader {
        id: getFileDialog
        focus: true
    }

    Connections {
        target: getFileDialog.item
        onProcess: {
            getFileDialog.source = ""
            if (btnClicked === 0)
            {
                ScreenContextBuffer.screenType = LecturesManager.LiteratureListFile;
                LecturesManager.copyLiterListLectureFile(value, ScreenContextBuffer.selectedIdx);
                ScreenContextBuffer.edit = false;
                ScreenContextBuffer.loaderSource = "education/lectures/EditTextScreen.qml"
            }
            else if (btnClicked === 1)
            {
                ScreenContextBuffer.screenType = LecturesManager.EducationProgramFile;
                LecturesManager.copyEducPlanLectureFile(value, ScreenContextBuffer.selectedIdx);
                ScreenContextBuffer.edit = false;
                ScreenContextBuffer.loaderSource = "education/lectures/EditTextScreen.qml"
            }
            else if (btnClicked === 2)
            {
                ScreenContextBuffer.screenType = LecturesManager.EducationPlanFile;
                LecturesManager.copyEducProgLectureFile(value, ScreenContextBuffer.selectedIdx);
                ScreenContextBuffer.edit = false;
                ScreenContextBuffer.loaderSource = "education/lectures/EditTextScreen.qml"
            }
        }
        onClose: {
            getFileDialog.source = ""
        }
    }
}
