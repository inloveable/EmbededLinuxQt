import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.VirtualKeyboard 2.15
import QtQuick.VirtualKeyboard.Styles 2.15
import QtQuick.VirtualKeyboard.Settings 2.15
Item {

    id:root

    width:1024
    height:100


    function disableEdit(){
        waterRateEdit.focus=false
        densityEdit.focus=false
    }

    property int itemIndex:0;
    property real density:0.90;
    property real waterRate:0;
    property double  amplitude:8000
    property double  anglePhase;
    property real temperature:26.3
    property bool gps:false




    Component.onCompleted: {



         VirtualKeyboardSettings.locale="Numeric"
    }

    signal requireTest(int index);


    property var widthRate:[1,3,3,2,2,2,2,2]


    function testComplete(){
        isPairing=false;
        updateProperties()

    }


    function getWidth(index){
        let count=0;
        for(var i=0;i<widthRate.length;++i){
            count+=widthRate[i];
        }
        let totalWidth=root.width
        let slice=totalWidth/count
        return slice*widthRate[index];
    }

    //--------------------private
    property color backgroundColor:"gray"
    property bool isPairing:false
    function updateProperties(){
        console.log("updating data from backend")
    }

    Row{
        anchors.fill: parent

        Rectangle{
            color:backgroundColor
            height:parent.height
            width:getWidth(0)

            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 15
                font.bold: true
                text:itemIndex
            }
        }
        Rectangle{
            color:backgroundColor
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
                         focus=false
                    }
                }
            }
        }
        Rectangle{
            color:backgroundColor
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

                    onEditingFinished: {
                        model.waterRate=waterRateEdit.text
                        //model.setData(index,waterRateEdit.text,"waterRate")
                        focus=false
                        console.log(focus)
                    }
                }
            }
        }
        Rectangle{
            color:backgroundColor
            height:parent.height
            width:getWidth(3)

            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 15
                font.bold: true
                text:amplitude
            }
        }
        Rectangle{
            color:backgroundColor
            height:parent.height
            width:getWidth(4)

            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 15
                font.bold: true
                text:anglePhase
            }
        }
        Rectangle{
            color:backgroundColor
            height:parent.height
            width:getWidth(5)

            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 15
                font.bold: true
                text:temperature
            }
        }
        Rectangle{
            color:backgroundColor
            height:parent.height
            width:getWidth(6)

            Image{
                id:icon
                anchors.fill: parent

                source:gps?"qrc:/images/icons/check.png":"qrc:/images/icons/close.png"
                fillMode:Qt.KeepAspectRatio
            }
        }

        Button{
            id:button
            height:parent.height
            width:getWidth(7)
            text:"配对"
            font.pointSize: 15
            font.bold: true

            background:Rectangle{
                color:root.isPairing?"#EBAA61":"green"
                radius:5
                border.width: 1
                border.color: "white"
            }

            onClicked: {
                if(root.isPairing){
                    return;
                }
                root.isPairing=true;
                requireTest(itemIndex);
            }

        }

    }
}
