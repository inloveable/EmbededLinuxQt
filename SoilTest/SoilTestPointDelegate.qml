import QtQuick 2.15
import QtQuick.Controls 2.15
Rectangle {

    id:root
    signal densityInputActive(bool active)
    signal waterRateInputActive(bool active)
    signal solidityInputActive(bool active)
    property alias densityFocus:densityEdit.focus
    property alias waterRateFocus:waterRateEdit.focus
    property alias solidtiyFocus:solidity.focus

    color:"transparent"

    function disableEdit(){
        waterRateEdit.focus=false
        densityEdit.focus=false
        solidity.focus=false;
    }


    property alias indexText:indexText.text
    property alias comboBoxModel:modelCombo.model
    property alias comboValue:modelCombo.currentIndex
    property alias densityText:densityEdit.text
    property alias waterRateText:waterRateEdit.text
    property alias phaseAngleText:phaseAngleText.text
    property alias ampText:ampText.text
    property alias solidityText:solidity.text


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
        Rectangle{
            width:getWidth(0)
            height:parent.height
            color:"transparent"
            Text{
                id:indexText

                anchors.fill: parent


                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                font.pixelSize: 20
                font.bold: true
                color:"white"
            }
        }


        Rectangle{
            width:getWidth(1)
            height:parent.height
            color:"transparent"
            ComboBox{
                id:modelCombo
                anchors.fill:parent
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
                        if(focus===false)
                        model.density=densityEdit.text
                    }
                    //elide:Text.ElideRight
                }
            }
        }
        Rectangle{
            //color:backgroundColor
            opacity:0.5
            height:parent.height
            width:getWidth(3)


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
        Rectangle{
            height:parent.height
            width:getWidth(4)
            border.width: 1
            border.color: "gray"
            color:"transparent"

            Text{
                id:phaseAngleText
                anchors.fill: parent
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                font.pixelSize: 20
                font.bold: true
                color:"white"
            }
        }
        Rectangle{
            height:parent.height
            width:getWidth(5)
            border.width: 1
            border.color: "gray"

            color:"transparent"

            Text{
                id:ampText
                anchors.fill: parent
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                font.pixelSize: 20
                font.bold: true
                color:"white"
            }
        }

        Rectangle{
            height:parent.height
            width:getWidth(5)

            border.width: 1
            border.color: "white"
            radius:5
            color:solidity.focus?"white":"transparent"
            opacity:0.5

            TextEdit{
                id:solidity
                anchors.fill: parent
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                font.pixelSize: 20
                font.bold: true
                color:"white"
                onFocusChanged: {
                    solidityInputActive(solidity.focus)
                    if(focus===false)
                    model.solidity=solidity.text
                }
            }
        }
    }
}
