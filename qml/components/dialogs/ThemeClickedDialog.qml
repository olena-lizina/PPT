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
import LecturesManager 1.1
import ".."

BaseDialog {
    id: themeClickedDialog

    property alias title: titleLabel.text
    property alias label: colLabel.text

    signal rightClicked()

    function initialize(parameters) {
        for (var attr in parameters) {
            themeClickedDialog[attr] = parameters[attr]
        }
    }

    onRightClicked: {
        themeClickedDialog.close()
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
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                    right: rightButton.left
                }

                anchors.leftMargin: 5 * settings.pixelDensity
                font.pixelSize: 10 * settings.pixelDensity
            }

            Item {
                id: rightButton

                width: height

                anchors {
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }

                Rectangle {
                    anchors.fill: parent
                    color: palette.button
                    visible: rightButtonMouseArea.pressed
                }

                CIcon {
                    id: icon
                    anchors.centerIn: parent
                    text: "\u2716"
                }

                MouseArea {
                    id: rightButtonMouseArea
                    anchors.fill: parent
                    onClicked: themeClickedDialog.rightClicked()
                }
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
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                CLabel {
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                CLabel {
                    id: colLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 10 * settings.pixelDensity
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
            text: qsTr("Create sub theme")
            onClicked: themeClickedDialog.process(LecturesManager.SubThemes)
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
            text: qsTr("Create lecture")
            onClicked: themeClickedDialog.process(LecturesManager.Themes)
        }
    }
}
