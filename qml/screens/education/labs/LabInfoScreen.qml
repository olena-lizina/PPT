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
import LabsManager 1.1
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
                    text: qsTr("Open laboratory work")

                    MouseArea {
                        id: mouseArea1
                        anchors.fill: parent

                        onClicked: {
                            ScreenContextBuffer.edit = false
                            ScreenContextBuffer.openLabScreen()
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

                    text: qsTr("Should be done due: \"%1\"").arg(LabsManager.getLabFinishDate(ScreenContextBuffer.labId))
                    wrapMode: Text.Wrap
                }

                CHorizontalSeparator {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
            }
        }
    }

    CListView {
        id: listView

        anchors {
            left: parent.left
            right: parent.right
            top: topBar.bottom
            bottom: parent.bottom
        }

        model: LabsManager.getExecutors(ScreenContextBuffer.labId);

        delegate: CInformationItem {

            text1: modelData.studName
            text2: qsTr("Finished: ") + modelData.finished
            text3: qsTr("Mark: ") + modelData.mark
            text4: qsTr("Evaluated: ") + modelData.evalDate
            reportId: modelData.id

            onClicked: {
                ScreenContextBuffer.executorId = modelData.id
            }
        }
    }

    CScrollBar {
        flickableItem: listView
    }

}

