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
import LecturesManager 1.1
import "../../../components"
import "../.."

BlankScreen {
    id: themesScreen

    Stack.onStatusChanged: {
        listView.model = {}
        listView.model = LecturesManager.getThemes();
    }

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
                text: LecturesManager.selectedChapter()
            }

            CToolButton {
                Layout.fillHeight: true
                icon: "\uf067"
                tooltipText: qsTr("New theme")
                onClicked: {
                    var parameters = {
                        title: qsTr("New theme")
                    }

                    var callback = function(value)
                    {
                        LecturesManager.addTheme(value)
                        listView.model = LecturesManager.getThemes()
                    }

                    dialog.open(dialog.types.newLecture, parameters, callback)
                }
            }
        }
    }

    CListView {
        id: listView

        anchors {
            left: parent.left
            right: parent.right
            top: toolBar.bottom
            bottom: parent.bottom
        }

        delegate: CFileButton {
            text: modelData
            rightButtonIcon: "\u270e"

            onClicked: {
                LecturesManager.selectedTheme(modelData);
//                if (LecturesManager.hasSubThemes(modelData))
//                    stackView.push(Qt.resolvedUrl("SubThemesScreen.qml"))
//                else
//                    console.debug("You have to show file here");
                if (LecturesManager.hasSubThemes(modelData))
                {
                    stackView.push(Qt.resolvedUrl("SubThemesScreen.qml"))
                }
                else if (LecturesManager.hasLecture(modelData))
                {
                    stackView.push(Qt.resolvedUrl("TemplateLectureScreen.qml"))
                }
            }

            onRightClicked: {
                LecturesManager.selectedTheme(modelData);
                var parameters = {
                    title: qsTr("Edit theme"),
                    label: qsTr("Theme name :"),
                    valueEdit: modelData
                }

                var callback = function(value)
                {
                    if (value === "delete")
                        LecturesManager.deleteTheme()
                    else
                        LecturesManager.editTheme(value)
                    listView.model = LecturesManager.getThemes()
                }

                dialog.open(dialog.types.editLecture, parameters, callback)
            }
        }
    }

    CScrollBar {
        flickableItem: listView
    }
}
