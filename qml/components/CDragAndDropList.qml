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
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import Qt.labs.settings 1.0
import QtQuick.Layouts 1.2
import QtQml.Models 2.1
import Qt.labs.settings 1.0
import LecturesManager 1.1

Item {

    id: root

    property real selectedItem
    property alias model: visualModel.model
    signal clicked()
    signal editClicked()
    signal removeClicked()

    Component {
        id: dragDelegate

        MouseArea {
            id: dragArea

            property bool held: false

            anchors { left: parent.left; right: parent.right }

            implicitHeight: 18.5 * settings.pixelDensity

            drag.target: held ? content : undefined
            drag.axis: Drag.YAxis

            onPressAndHold: held = true

            onReleased: {
                held = false
                console.log("onReleased: " + modelData + ", new idx: " + dragArea.DelegateModel.itemsIndex)
                LecturesManager.updateIndex(dragArea.DelegateModel.itemsIndex, modelData, selectedItem);
            }

            onClicked: {
                LecturesManager.selectedItem(modelData, selectedItem)
                root.clicked()
            }            

            Rectangle {
                id: content

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }

                width: dragArea.width;
                height: parent.implicitHeight

                color: palette.background
                Behavior on color { ColorAnimation { duration: 100 } }

                Drag.active: dragArea.held
                Drag.source: dragArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2

                CHorizontalSeparator {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }

                states: State {
                    when: dragArea.held

                    ParentChange { target: content; parent: root }
                    AnchorChanges {
                        target: content
                        anchors { horizontalCenter: undefined; verticalCenter: undefined }
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Rectangle {
                            id: btnRect
                            anchors.fill: parent
                            color: palette.button
                            visible: dragArea.pressed
                        }

                        CLabel {
                            id: buttonLabel
                            anchors.fill: parent
                            anchors.leftMargin: 5 * settings.pixelDensity
                            anchors.rightMargin: 3 * settings.pixelDensity
                            text: modelData
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.minimumWidth: height

                        Rectangle {
                            anchors.fill: parent
                            color: palette.button
                            visible: editMouseArea.pressed
                        }

                        CIcon {
                            anchors.centerIn: parent
                            text: "\u270e"
                        }

                        MouseArea {
                            id: editMouseArea
                            anchors.fill: parent
                            onClicked: {
                                LecturesManager.selectedItem(modelData, selectedItem)
                                root.editClicked()
                            }
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.minimumWidth: height

                        Rectangle {
                            anchors.fill: parent
                            color: palette.button
                            visible: removeMouseArea.pressed
                        }

                        CIcon {
                            id: icon
                            anchors.centerIn: parent
                            text: "\u2716"
                        }

                        MouseArea {
                            id: removeMouseArea
                            anchors.fill: parent
                            onClicked: {
                                LecturesManager.selectedItem(modelData, selectedItem)
                                root.removeClicked()
                            }
                        }
                    }
                }
            }

            DropArea {
                anchors { fill: parent; margins: 10 }

                onEntered: {
                    visualModel.items.move(
                                drag.source.DelegateModel.itemsIndex,
                                dragArea.DelegateModel.itemsIndex)
                }
            }
        }
    }

    DelegateModel {
        id: visualModel

        delegate: dragDelegate
        model: LecturesManager.getListModel(selectedItem)
    }

    ListView {
        id: view

        anchors.fill: parent

        model: visualModel

        cacheBuffer: 50

    }
}

