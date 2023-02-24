import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Dialogs 1.0

Item {
    id:element
    property int boxWidth: 45
    property int boxHeight: 25
    property int numberRangeFrom: 0
    property int numberRangeTo: 100
    property bool hideAlpha: false
    property string curColor: "ffffffff"
    signal valueUpdated(var value)
    Button{
        id:button
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: boxWidth
        height: boxHeight
        onClicked: {
            colorDialog.open()
        }
        background: Rectangle{
            id: colorRectangle
            color: colorDialog.color
            anchors.fill: parent
            radius: 4
        }
    }

    TextField {
        id: colorString
        text: isColorHex(curColor) ? curColor.substring(0,7): '#000000'
        anchors.left: button.right
        anchors.leftMargin: 4
        anchors.rightMargin: 4
        height: boxHeight
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 11
        verticalAlignment: Text.AlignVCenter
        onEditingFinished: {
            if (!isColorHex(text)) {
                colorString.text = '#000000';
            }
            changeColorByInput()
        }
    }

    ColorDialog{
        id:colorDialog
        color: "#00000000"
        showAlphaChannel: true
        title: "选择一个颜色"
        onAccepted: {
            //数据是RGBA 但是colorDialog.color读取的是ARGB
            colorString.text = rgb2Hex(colorDialog.color.r, colorDialog.color.g, colorDialog.color.b);
            let newColor = rgba2Hex(colorDialog.color.r, colorDialog.color.g, colorDialog.color.b, colorDialog.color.a);

            curColor = newColor
            valueUpdated(curColor)
        }
    }

    Component.onCompleted: {
        // rootObject.onPropertyDefChanged.connect(updateDisplay)
        // rootObject.onValueChanged.connect(updateDisplay);
        updateDisplay()
    }

    function updateDisplay(){
        if(curColor && isColorHex(curColor)) {
            var rgbColor = curColor.substring(0,7)
            var alpha = curColor.substring(7,9)
            colorDialog.color = rgbColor
            colorDialog.color.a =(('0x' +alpha) &255) /255
        }
    }

    function getRgbaByHex(rgbaHex,startIndex,endIndex) {
        var colorStr = rgbaHex ? rgbaHex.substring(startIndex,endIndex) : "00";
        if(colorStr === null || colorStr === ""){
            colorStr = "00"
        }
        return (('0x' +colorStr) & 255)
    }

    function getDecimalByHex(rgbaHex,startIndex,endIndex) {
        var colorStr = rgbaHex ? rgbaHex.substring(startIndex,endIndex) : "00";
        if(colorStr === null || colorStr === ""){
            colorStr = "00"
        }
        return Math.round((('0x' +colorStr) & 255) / 255 * 100);
    }

    function rgba2Hex(r,g,b,a) {
        r = Math.round(r * 255).toString(16);
        g = Math.round(g * 255).toString(16);
        b = Math.round(b * 255).toString(16);
        a = Math.round(a * 255).toString(16);

        if (r.length === 1)
            r = "0" + r;
        if (g.length === 1)
            g = "0" + g;
        if (b.length === 1)
            b = "0" + b;
        if (a.length === 1)
            a = "0" + a;
        return "#" + r + g + b + a;
    }

    function rgb2Hex(r,g,b) {
        r = Math.round(r * 255).toString(16);
        g = Math.round(g * 255).toString(16);
        b = Math.round(b * 255).toString(16);

        if (r.length === 1)
            r = "0" + r;
        if (g.length === 1)
            g = "0" + g;
        if (b.length === 1)
            b = "0" + b;
        return "#" + r + g + b;
    }

    function hex2Rgba(hex) {
        let c;
        if(/^#([A-Fa-f0-9]{3}){1,2}$/.test(hex)){
            c= hex.substring(1).split('');
            if(c.length === 3){
                c= [c[0], c[0], c[1], c[1], c[2], c[2]];
            }
            c= '0x'+c.join('');
            return 'rgba('+[(c>>16)&255, (c>>8)&255, c&255].join(',')+',1)';
        }
        throw new Error('Bad Hex');
    }

    function changeColorByInput(){
        let rgb = colorString.text;
        let aValue = 100;
        let aNum = parseInt(aValue.getText(0,aValue.length)) / 100;
        aNum = Math.round(aNum * 255).toString(16);
        if (aNum.length === 1) {
            aNum = "0" + a;
        }
        let rgba = rgb + aNum;
        curColor = rgba;

        valueUpdated(curColor)
    }

    function isColorHex(text) {
        if (typeof text != 'string') {
            return false;
        }
        let regex =  /^#([0-9a-fA-F]{3}|[0-9a-fA-F]{6,8})$/;
        return text.match(regex);
    }
}
