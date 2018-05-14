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
import "../components"
import TreeModel 1.1

BlankScreen {
    id: educationalMaterialsScreen

    readonly property int disciplineType: 0
    readonly property int chapterType: 1
    readonly property int themeType: 2
    readonly property int subthemeType: 3

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
                text: qsTr("Educational materials")
            }

            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("MainMenuScreen.qml"))
            }
        }
    }

    ListModel {
        id: disciplinesModel

        ListElement {
            type: 0
            name: "disciplines"
            number: "disciplines"
        }
        ListElement {
            type: 0
            name: "disciplines"
            number: "disciplines"
        }
        ListElement {
            type: 0
            name: "disciplines"
            number: "disciplines"
        }
        ListElement {
            type: 0
            name: "disciplines"
            number: "disciplines"
        }
        ListElement {
            type: 0
            name: "disciplines"
            number: "disciplines"
        }
        ListElement {
            type: 0
            name: "disciplines"
            number: "disciplines"
        }
    }

    ListView {
        id: myView
        model: TreeModel.simpleTree//disciplinesModel
        delegate: CTreeItem{}
            /*CNavigationButton {
            text: name + ": " + number

            anchors.leftMargin: type === disciplineType ? 0 : type === chapterType ? 20 : type === themeType ? 30 : 40

            onClicked: {
                console.debug(name + " idx " + index)
                switch(type)
                {
                case disciplineType:
                    disciplinesModel.insert(index + 1, {"type": 1, "name": "chapters", "number": "chapters"})
                    break;
                case chapterType:
                    disciplinesModel.insert(index + 1, {"type": 2, "name": "themes", "number": "themes"})
                    break;
                case themeType:
                    disciplinesModel.insert(index + 1, {"type": 3, "name": "subthemes", "number": "subthemes"})
                    break;
                case subthemeType:
                    console.log("show file");
                    break;
                }
            }

            iconWidth: 0
        }*/

        width: 0.25 * settings.windowWidth
        height: settings.windowHeight - toolBar.height

        anchors {
            left: parent.left
            top: toolBar.bottom
        }
    }

    Rectangle {
        id: rightRect
        color: "blue"

        width: 0.75 * settings.windowWidth
        height: settings.windowHeight - toolBar.height

        anchors {
            left: myView.right
            top: toolBar.bottom
        }
    }
}
