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

    width: 0.75 * settings.windowWidth
    height: settings.windowHeight - toolBar.height

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#ffffb3" }
        GradientStop { position: 1.0; color: "#b3d9ff" }
    }

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
                width: 0.35 * settings.windowWidth

                CLabel {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Create lecture")

                    MouseArea {
                        id: mouseArea1
                        anchors.fill: parent

                        onClicked: {
                            LecturesManager.createFile(ScreenContextBuffer.nesting, ScreenContextBuffer.selectedIdx)
                            ScreenContextBuffer.edit = true;
                            ScreenContextBuffer.screenType = LecturesManager.LectureFile;
                            ScreenContextBuffer.loaderSource = "education/lectures/EditTextScreen.qml"
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
                width: 0.35 * settings.windowWidth

                CLabel {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    text: qsTr("Import lecture")
                    wrapMode: Text.Wrap

                    MouseArea {
                        id: mouseArea2
                        anchors.fill: parent
                        onClicked: {
                            getFileDialog.source = "../../../components/dialogs/SelectFileDialog.qml"
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
            ScreenContextBuffer.screenType = LecturesManager.LectureFile;
            LecturesManager.copyLectureFile(value, ScreenContextBuffer.nesting, ScreenContextBuffer.selectedIdx)
            ScreenContextBuffer.edit = false;
            ScreenContextBuffer.loaderSource = "education/lectures/EditTextScreen.qml"
        }
        onClose: {
            getFileDialog.source = ""
        }
    }
}

