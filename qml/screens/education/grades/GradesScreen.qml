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
import "../../../components/"
import "../.."
import GradesManager 1.1
import ScreenContextBuffer 1.1

BlankScreen {
    id: labsScreen

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
                text: qsTr("Gradebook")
            }

            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("../../MainMenuScreen.qml"))
            }
        }
    }

    ListModel {
        id: dataModel

        ListElement {
            studName: "Блюс Дмитро Юрійович"
            labName: "Використання STL"
            mark: "A(90)"
        }
        ListElement {
            studName: "Васильков Влас Володимирович"
            labName: "Використання STL"
            mark: "A(92)"
        }
        ListElement {
            studName: "Гуреєв Олександр Олександрович"
            labName: "Використання STL"
            mark: "A(90)"
        }
        ListElement {
            studName: "Дірявка Андрій Олександрович"
            labName: "Використання STL"
            mark: "C(78)"
        }
    }

    TableView {

        anchors {
            top: toolBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        TableViewColumn {
            role: "studName"
            title: qsTr("Student Name")
            width: 0.2 * settings.windowWidth
        }

        TableViewColumn {
            role: "labName"
            title: qsTr("Laboratory work name")
            width: 0.2 * settings.windowWidth
        }

        TableViewColumn {
            role: "mark"
            title: qsTr("Mark")
            width: 0.1 * settings.windowWidth
        }

        model: dataModel

        rowDelegate: Component {
            Text {
                font.pixelSize: 6 * settings.pixelDensity
            }
        }
    }
}
