import QtQuick 2.15

Rectangle {

    id:root
    signal densityInputActive(bool active)
    signal waterRateInputActive(bool active)


    property alias index:indexText.text
    property alias densityText:densityEdit.text
    property alias waterRateText:waterRateEdit.text


    property var widthRate:[]

    function getWidth(index){
        let count=0;
        for(var i=0;i<widthRate.length;++i){
            count+=widthRate[i];
        }
        let totalWidth=root.width
        let slice=totalWidth/count
        return slice*widthRate[index];
    }
    Row{
        anchors.fill: parent
        Text{
            id:indexText
            width:getWidth(0)
            height:parent.height

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            font.pixelSize: 16
            font.bold: true
            color:"white"
        }
        Rectangle{
            //color:backgroundColor
            opacity:0.5
            height:parent.height
            width:getWidth(1)

            Rectangle{
                width:parent.width-10
                height:parent.height-10
                anchors.centerIn: parent
                border.width: 1
                border.color: "white"
                radius:5

                color:densityEdit.focus?"white":"transparent"
                TextEdit{
                    id:densityEdit
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    text:density
                    inputMethodHints: Qt.ImhDigitsOnly

                    onEditingFinished: {

                         model.density=densityEdit.text

                    }
                    onFocusChanged: {
                        console.log("density focus:"+densityEdit.focus)
                        densityInputActive(densityEdit.focus)
                    }
                    //elide:Text.ElideRight
                }
            }
        }
        Rectangle{
            //color:backgroundColor
            opacity:0.5
            height:parent.height
            width:getWidth(2)

            Rectangle{
                width:parent.width-10
                height:parent.height-10
                anchors.centerIn: parent
                border.width: 1
                border.color: "white"
                radius:5
                color:waterRateEdit.focus?"white":"transparent"
                TextEdit{
                    id:waterRateEdit
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    font.bold: true
                    text:waterRate

                    inputMethodHints: Qt.ImhDigitsOnly

                    onFocusChanged: {
                        waterRateInputActive(waterRateEdit.focus)
                        if(focus===false)
                        model.waterRate=waterRateEdit.text
                    }
                }
            }
        }

    }
}
