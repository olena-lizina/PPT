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
import LecturesManager 1.1
import SyntaxHighlighter 1.1

Item {
    id: cReadyCodeArea

    property alias text: textEdit.text

    CFlickable {
        id: flickable

        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            left: parent.left
        }

        interactive: false

        TextArea {
            id: textEdit

            readOnly: true

            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                left: parent.left
                bottomMargin: 10
            }

            font.family: settings.font
            font.pixelSize: settings.fontSize
            wrapMode: TextEdit.Wrap
            textFormat: TextEdit.RichText

            SyntaxHighlighter {
                id: syntaxHighlighter

                normalColor: palette.editorNormal
                commentColor: palette.editorComment
                numberColor: palette.editorNumber
                stringColor: palette.editorString
                operatorColor: palette.editorOperator
                keywordColor: palette.editorKeyword
                builtInColor: palette.editorBuiltIn
                markerColor: palette.editorMarker
                itemColor: palette.editorItem
                propertyColor: palette.editorProperty
            }

            Component.onCompleted: {
                syntaxHighlighter.setHighlighter(textEdit)
            }

            onLinkActivated: Qt.openUrlExternally(link)

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.NoButton // we don't want to eat clicks on the Text
                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
            }
        }
    }
}
