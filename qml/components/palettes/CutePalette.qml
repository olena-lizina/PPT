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

BasePalette {
    id: cutePalette

    background: "#e6f0ff"

    toolBarBackground: "#b3d9ff"
    toolBarStripe: "#0099e6"
    toolBarShadowBegin: "#30000000"
    toolBarShadowEnd: "#00000000"

    label: "#0033cc"
    description: "#001a66"
    icon: "#000099"
    warning: "#ff0000"
    link: "#990099"

    textFieldBackground: "#e6f2ff"
    textFieldBorder: "#ccccff"
    textFieldPlaceholder: "#9999ff"
    textFieldSelection: "#ccccff"

    button: "#99b3e6"

    contextBackground: "#99b3e6"
    contextButton: "#8080ff"

    separator: "#9999ff"

    scrollBar: "#30000000"

    dialogOverlay: "#e6e6ff"
    dialogBackground: "#f2f2f2"
    dialogShadow: "#00004d"

    sliderFilledStripe: "#5c5c8a";
    sliderEmptyStripe: "#b3b3cc";
    sliderHandle: "#ffffff"
    sliderHandleBorder: "#47476b"

    tooltipBackground: "#47476b"
    tooltipText: "#ffffff"

    switcherBackground: "#ffffff"
    switcherBorder: "#47476b"
    switcherHandle: "#ffffff"

    contextMenuButton: "#cc000000"
    contextMenuButtonPressed: "#ff000000"
    contextMenuButtonText: "#ffffff"

    lineNumbersBackground:"#d1d1e0"
    lineNumber: "#9494b8"

    editorSelection: "#9494b8"
    editorSelectedText: "#ffffff"

    editorSelectionHandle: "#777777"

    editorNormal: "#000000"
    editorComment: "#008000"
    editorNumber: "#000080"
    editorString: "#008000"
    editorOperator: "#000000"
    editorKeyword: "#808000"
    editorBuiltIn: "#0055af"
    editorMarker: "#ffff00"
    editorItem: "#800080"
    editorProperty: "#800000"
}
