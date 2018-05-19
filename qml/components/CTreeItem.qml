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
import LecturesManager 1.1
import ScreenContextBuffer 1.1
import ".."

Row {
    id: root

    signal itemChanged()

    Rectangle{
        width: 20
        height: list.height
        opacity: 0
    }

    Column {
        id: list
        CLabel {
            height: 18.5 * settings.pixelDensity
            width: 0.25 * settings.windowWidth - modelData.nesting * 20 - 20

            text: modelData.text
            wrapMode: Text.Wrap

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if (mouse.button == Qt.LeftButton)
                    {
                        if (modelData.idx === -1)
                        {
                            var insertParameters = {
                                title: qsTr("Insert new discipline"),
                                nameLabel: qsTr("Enter discipline name:")
                            }

                            var insertCallback = function(value)
                            {
                                console.log("Callback: " + value + ", idx: " + modelData.idx)
                                LecturesManager.insertDiscipline(value, modelData.idx)
                            }
                            dialog.open(dialog.types.lecture, insertParameters, insertCallback)
                        }
                        else
                        {
                            modelData.isOpen = !modelData.isOpen;
                            ScreenContextBuffer.setNestingAndIndex(modelData.nesting, modelData.idx);
                            root.itemChanged()
                        }
                    }
                    else if (modelData.idx !== -1)
                        contextMenu.visible = !contextMenu.visible;
                }

                onPressAndHold: tooltip.show(modelData.text)
            }
            CHorizontalSeparator {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
            Rectangle {
                anchors.fill: parent
                color: palette.button
                visible: mouseArea.pressed
                opacity: 0.5
            }

            Item {
                id: contextMenu
                visible: false

                property int delegateWidth: parent.width
                property int delegateHeight: 6 * settings.pixelDensity

                Column {
                    CContextMenuButton {
                        width: contextMenu.delegateWidth
                        height: contextMenu.delegateHeight
                        text: qsTr("Insert")
                        onClicked: {
                            root.contextMenuCallback(0)
                        }
                    }

                    CContextMenuButton {
                        text: qsTr("Edit")
                        width: contextMenu.delegateWidth
                        height: contextMenu.delegateHeight
                        onClicked: {
                            root.contextMenuCallback(1)
                        }
                    }

                    CContextMenuButton {
                        width: contextMenu.delegateWidth
                        height: contextMenu.delegateHeight
                        text: qsTr("Remove")
                        onClicked: {
                            root.contextMenuCallback(2)
                        }
                    }
                }
            }
        }

        Loader{ source: modelData.isOpen ? "CChildTreeList.qml" : "CTreeEmptyItem.qml"; }
    }

    function contextMenuCallback(idx)
    {
        contextMenu.visible = false;

        switch(idx)
        {
        case 0:
            var leftBtn;
            var rightBtn;
            switch(modelData.nesting)
            {
            case 0:
                leftBtn = qsTr("Discipline");
                rightBtn = qsTr("Chapter");
                break;
            case 1:
                leftBtn = qsTr("Chapter");
                rightBtn = qsTr("Theme");
                break;
            case 2:
                leftBtn = qsTr("Theme");
                rightBtn = qsTr("Subtheme");
                break;
            }

            var insertParameters = {
                title: qsTr("Insert new item"),
                label: qsTr("What would you like to insert?"),
                leftBtnText: leftBtn,
                rightBtnText: rightBtn
            }

            var insertCallback = function(value)
            {
                console.log("Callback: " + value + ", idx: " + modelData.idx)

                var insTitle;
                var insLabel;
                switch(modelData.nesting)
                {
                case 0:
                    if (value === 0)
                    {
                        insTitle = qsTr("Insert new discipline");
                        insLabel = qsTr("Enter the name of the new discipline:");
                    }
                    else
                    {
                        insTitle = qsTr("Insert new chapter");
                        insLabel = qsTr("Enter the name of the new chapter:");
                    }
                    break;
                case 1:
                    if (value === 0)
                    {
                        insTitle = qsTr("Insert new chapter");
                        insLabel = qsTr("Enter the name of the new chapter:");
                    }
                    else
                    {
                        insTitle = qsTr("Insert new theme");
                        insLabel = qsTr("Enter the name of the new theme:");
                    }
                    break;
                case 2:
                    if (value === 0)
                    {
                        insTitle = qsTr("Insert new theme");
                        insLabel = qsTr("Enter the name of the new theme:");
                    }
                    else
                    {
                        insTitle = qsTr("Insert new subtheme");
                        insLabel = qsTr("Enter the name of the new subtheme:");
                    }
                    break;
                case 3:
                    if (value === 0)
                    {
                        insTitle = qsTr("Insert new subtheme");
                        insLabel = qsTr("Enter the name of the new subtheme:");
                    }
                    break;
                }

                var ins = {
                    title: insTitle,
                    nameLabel: insLabel
                }

                var insCallback = function(val)
                {
                    console.log("Callback: " + val + ", idx: " + modelData.idx)

                    switch(modelData.nesting)
                    {
                    case 0:
                        if (value === 0)
                            LecturesManager.insertDiscipline(val, modelData.idx)
                        else
                            LecturesManager.appendChapter(val, modelData.idx)
                        break;
                    case 1:
                        if (value === 0)
                            LecturesManager.insertChapter(val, modelData.idx)
                        else
                            LecturesManager.appendTheme(val, modelData.idx)
                        break;
                    case 2:
                        if (value === 0)
                            LecturesManager.insertTheme(val, modelData.idx)
                        else
                            LecturesManager.appendSubtheme(val, modelData.idx)
                        break;
                    case 3:
                        LecturesManager.insertSubtheme(val, modelData.idx)
                        break;
                    }
                }

                dialog.open(dialog.types.lecture, ins, insCallback)
            }

            if (modelData.nesting === 3)
                insertCallback(0);
            else
                dialog.open(dialog.types.insert, insertParameters, insertCallback)
            break;
        case 1:
            var editTitle;
            var editLabel;
            switch(modelData.nesting)
            {
            case 0:
                editTitle = qsTr("Edit discipline");
                editLabel = qsTr("Enter the name of the discipline:");
                break;
            case 1:
                editTitle = qsTr("Edit chapter");
                editLabel = qsTr("Enter the name of the chapter:");
                break;
            case 2:
                editTitle = qsTr("Edit theme");
                editLabel = qsTr("Enter the name of the theme:");
                break;
            case 3:
                editTitle = qsTr("Edit subtheme");
                editLabel = qsTr("Enter the name of the subtheme:");
                break;
            }
            var editParameters = {
                title: editTitle,
                nameLabel: editLabel,
                name: modelData.text
            }

            var editCallback = function(value)
            {
                console.log("Callback: " + value + ", idx: " + modelData.idx)

                switch(modelData.nesting)
                {
                case 0:
                    LecturesManager.updateDiscipline(value, modelData.idx)
                    break;
                case 1:
                    LecturesManager.updateChapter(value, modelData.idx)
                    break;
                case 2:
                    LecturesManager.updateTheme(value, modelData.idx)
                    break;
                case 3:
                    LecturesManager.updateSubtheme(value, modelData.idx)
                    break;
                }
            }

            dialog.open(dialog.types.lecture, editParameters, editCallback)
            break;
        case 2:
            var removeTitle;
            var removeLabel;
            switch(modelData.nesting)
            {
            case 0:
                removeTitle = qsTr("Remove discipline");
                removeLabel = qsTr("Press OK to delete discipline with name: \"%1\"");
                break;
            case 1:
                removeTitle = qsTr("Remove chapter");
                removeLabel = qsTr("Press OK to delete chapter with name: \"%1\"");
                break;
            case 2:
                removeTitle = qsTr("Remove theme");
                removeLabel = qsTr("Press OK to delete theme with name: \"%1\"");
                break;
            case 3:
                removeTitle = qsTr("Remove subtheme");
                removeLabel = qsTr("Press OK to delete subtheme with name: \"%1\"");
                break;
            }

            var removeParameters = {
                title: removeTitle,
                text: removeLabel.arg(modelData.text)
            }

            var removeCallback = function(value)
            {
                console.log("Callback: " + value + ", idx: " + modelData.idx)

                if (!value)
                    return;

                switch(modelData.nesting)
                {
                case 0:
                    LecturesManager.removeDiscipline(modelData.idx)
                    break;
                case 1:
                    LecturesManager.removeChapter(modelData.idx)
                    break;
                case 2:
                    LecturesManager.removeTheme(modelData.idx)
                    break;
                case 3:
                    LecturesManager.removeSubtheme(modelData.idx)
                    break;
                }
            }

            dialog.open(dialog.types.confirmation, removeParameters, removeCallback)
            break;
        }
    }
}
