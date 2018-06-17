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
import LabsManager 1.1

Item {
    id: cInformationItem

    signal clicked()

    anchors.left: parent.left
    anchors.right: parent.right
    implicitHeight: 18.5 * settings.pixelDensity

    property alias text1: label1.text
    property alias text2: label2.text
    property alias text3: label3.text
    property alias text4: label4.text
    property int reportId

    CHorizontalSeparator {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Column {
        id: firstCol
        anchors.left: cInformationItem.left
        anchors.right: secondCol.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        spacing: 0.5 * settings.pixelDensity
        width: 0.45 * settings.windowWidth

        CLabel {
            id: label1
            anchors.left: parent.left
            anchors.right: parent.right
        }

        CLabel {
            id: label2
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 5 * settings.pixelDensity
            color: palette.description
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: firstCol
        onClicked: cInformationItem.clicked()
    }

    Rectangle {
        id: btnRect
        anchors.fill: firstCol
        color: palette.button
        visible: mouseArea2.pressed
    }

    Column {
        id: secondCol
        anchors.left: firstCol.right
        anchors.right: evaluate.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        spacing: 0.5 * settings.pixelDensity
        width: 0.20 * settings.windowWidth

        CLabel {
            id: label3
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 5 * settings.pixelDensity
            color: palette.description
        }

        CLabel {
            id: label4
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 5 * settings.pixelDensity
            color: palette.description
        }
    }

    MouseArea {
        id: mouseArea2
        anchors.fill: secondCol
        onClicked: cInformationItem.clicked()
    }

    Rectangle {
        id: btnRect2
        anchors.fill: secondCol
        color: palette.button
        visible: mouseArea2.pressed
    }

    CNavigationButton {
        id:evaluate
        anchors.right: cInformationItem.right
        anchors.left: secondCol.rigth
        width: 0.10 * settings.windowWidth
        text: qsTr("Evaluate")

        onClicked: {
            var params = {
                title: qsTr("Evaluation"),
                text: qsTr("Mark:")
            }

            var callback = function(value)
            {
                var evaluationDate = new Date();
                LabsManager.saveEvaluation(reportId, value, evaluationDate.toLocaleDateString())
                text3 = qsTr("Mark: ") + value
                text4 = qsTr("Evaluated: ") + evaluationDate.toLocaleDateString()
            }
            dialog.open(dialog.types.evaluate, params, callback)
        }
    }
}
