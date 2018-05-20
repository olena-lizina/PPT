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

QtObject {
    id: basePalette

    property color background: "#e6f0ff"

    property color toolBarBackground: "#b3d9ff"
    property color toolBarStripe: "#0099e6"
    property color toolBarShadowBegin: "#30000000"
    property color toolBarShadowEnd: "#00000000"

    property color label: "#0033cc"
    property color description: "#001a66"
    property color icon: "#ffff00"
    property color warning: "#ff0000"
    property color link: "#990099"

    property color textFieldBackground: "#e6f2ff"
    property color textFieldBorder: "#ccccff"
    property color textFieldPlaceholder: "#9999ff"
    property color textFieldSelection: "#ccccff"

    property color button: "#ccccff"

    property color contextBackground: "#ccccff"
    property color contextButton: "#8080ff"

    property color separator: "#9999ff"

    property color scrollBar: "#30000000"

    property color dialogOverlay: "#e6e6ff"
    property color dialogBackground: "#ccccff"
    property color dialogShadow: "#00004d"

    property color sliderFilledStripe: "#5c5c8a";
    property color sliderEmptyStripe: "#b3b3cc";
    property color sliderHandle: "#ffffff"
    property color sliderHandleBorder: "#47476b"

    property color tooltipBackground: "#47476b"
    property color tooltipText: "#ffffff"

    property color switcherBackground: "#ffffff"
    property color switcherBorder: "#47476b"
    property color switcherHandle: "#ffffff"

    property color contextMenuButton: "#cc000000"
    property color contextMenuButtonPressed: "#ff000000"
    property color contextMenuButtonText: "#ffffff"

    property color lineNumbersBackground:"#d1d1e0"
    property color lineNumber: "#9494b8"

    property color editorSelection: "#9494b8"
    property color editorSelectedText: "#ffffff"

    property color editorSelectionHandle: "#777777"

    property color editorNormal: "#000000"
    property color editorComment: "#008000"
    property color editorNumber: "#000080"
    property color editorString: "#008000"
    property color editorOperator: "#000000"
    property color editorKeyword: "#808000"
    property color editorBuiltIn: "#0055af"
    property color editorMarker: "#ffff00"
    property color editorItem: "#800080"
    property color editorProperty: "#800000"
}
