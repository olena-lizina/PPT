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
                text: LecturesManager.selectedItem(LecturesManager.Chapters)
            }

            CToolButton {
                Layout.fillHeight: true
                icon: "\uf067"
                tooltipText: qsTr("New theme")
                onClicked: {
                    var parameters = {
                        title: qsTr("New theme"),
                        label: qsTr("Enter theme name :"),
                        itemType: LecturesManager.Themes
                    }

                    var callback = function(value)
                    {
                        LecturesManager.addItem(value, LecturesManager.Themes)
                        listModel.model = LecturesManager.getListModel(LecturesManager.Themes)
                    }

                    dialog.open(dialog.types.newLecture, parameters, callback)
                }
            }

            CHomeButton {
                onClicked: stackView.push(Qt.resolvedUrl("../../MainMenuScreen.qml"))
            }
        }
    }

    CDragAndDropList {

        id: listModel

        selectedItem: LecturesManager.Themes

        anchors {
            top: toolBar.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom
        }

        onClicked: {
            if (LecturesManager.hasSubThemes(LecturesManager.selectedItem(LecturesManager.Themes)))
            {
                stackView.push(Qt.resolvedUrl("SubThemesScreen.qml"))
            }
            else if (LecturesManager.itemHasFile(LecturesManager.selectedItem(LecturesManager.Themes), LecturesManager.Themes))
            {
                LecturesManager.selectFile()
                stackView.push(Qt.resolvedUrl("DisplayTextScreen.qml"))
            }
            else
            {
                var parameters = {
                    title: qsTr(LecturesManager.selectedItem(LecturesManager.Themes)),
                    label: qsTr("Would you like to create subtheme or lecture?")
                }

                var callback = function(value)
                {
                    if (value === LecturesManager.SubThemes)
                    {
                        stackView.push(Qt.resolvedUrl("SubThemesScreen.qml"))
                    }
                    else
                    {
                        LecturesManager.createFile()
                        stackView.push(Qt.resolvedUrl("EditTextScreen.qml"))
                    }
                }

                dialog.open(dialog.types.createLectureOrSubTheme, parameters, callback)
            }
        }

        onEditClicked: {
            var parameters = {
                title: qsTr("Edit theme"),
                label: qsTr("Theme name :"),
                valueEdit: LecturesManager.selectedItem(LecturesManager.Themes),
                itemType: LecturesManager.Themes
            }

            var callback = function(value)
            {
                LecturesManager.editItem(value, LecturesManager.Themes)
                model = LecturesManager.getListModel(LecturesManager.Themes)
            }

            dialog.open(dialog.types.editLecture, parameters, callback)
        }

        onRemoveClicked: {
            var parameters = {
                title: qsTr("Delete the theme"),
                text: qsTr("Are you sure you want to delete \"%1\"?").arg(LecturesManager.selectedItem(LecturesManager.Themes))
            }

            var callback = function(value)
            {
                if (value)
                {
                    LecturesManager.deleteItem(LecturesManager.Themes)
                    model = LecturesManager.getListModel(LecturesManager.Themes)
                }
            }

            dialog.open(dialog.types.confirmation, parameters, callback)
        }
    }
}
