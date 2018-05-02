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
    id: chapterScreen

    Stack.onStatusChanged: {
        listView.model = LecturesManager.getListModel(LecturesManager.Chapters);
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
                text: LecturesManager.selectedItem(LecturesManager.Parts)
            }

            CToolButton {
                Layout.fillHeight: true
                icon: "\uf067"
                tooltipText: qsTr("New chapter")
                onClicked: {
                    var parameters = {
                        title: qsTr("New chapter")
                    }

                    var callback = function(value)
                    {
                        LecturesManager.addItem(value, LecturesManager.Chapters)
                        listView.model = LecturesManager.getListModel(LecturesManager.Chapters)
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
                console.debug(modelData);
                LecturesManager.selectedItem(modelData, LecturesManager.Chapters);
                stackView.push(Qt.resolvedUrl("ThemesScreen.qml"))
            }

            onRightClicked: {
                LecturesManager.selectedChapter(modelData);
                var parameters = {
                    title: qsTr("Edit chapter"),
                    label: qsTr("Chapter name :"),
                    valueEdit: modelData
                }

                var callback = function(value)
                {
                    if (value === "delete")
                        LecturesManager.deleteItem(LecturesManager.Chapters)
                    else
                        LecturesManager.editItem(value, LecturesManager.Chapters)
                    listView.model = LecturesManager.getListModel(LecturesManager.Chapters)
                }

                dialog.open(dialog.types.editLecture, parameters, callback)
            }
        }
    }

    CScrollBar {
        flickableItem: listView
    }
}
