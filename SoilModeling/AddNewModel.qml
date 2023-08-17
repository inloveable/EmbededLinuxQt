import QtQuick 2.15
import QtQuick.Controls 2.7


import "../"
import "../keyboard/content/"
import CppCore 1.0
Item {
    SwipeView{
        id:view

        anchors.fill: parent
        currentIndex: 0

        Rectangle{
            id:root

            Screen01{
               id:keyboard
               z:99
               width:parent.width
               height:parent.height/2

               opacity:0.3

               screenHeight: parent.height
               anchors.left: parent.left
               onHostHidden: {
                      rectangle3.focus=false
                      rectangle4.focus=false
                      rectangle2.focus=false

               }
            }

            color:"orange"
            gradient: Gradient
            {
                GradientStop { position: 0.0; color: "#f46b45" }
                GradientStop { position: 1.0; color: "#eea849" }
                orientation: Qt.Horizontal
            }


            Rectangle {
                id: rectangle
                width: parent.width/5*4
                height: parent.height/5*3
                color: "transparent"
                radius: 5
                border.color: "#ffffff"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter



                FocusScope{
                    width: parent.width
                    height: parent.height-rectangle1.height*0.5-5
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    Row {
                        id: row
                        anchors.fill: parent
                        property bool isGroupInternalSwitch:false

                        function anyFocus(){
                           return rectangle2.ptextFocus||rectangle3.ptextFocus||rectangle4.ptextFocus;
                        }

                        Connections{
                            target:keyboard

                            function onKeyPressed(textR){
                                 let active;
                                if(rectangle2.ptextFocus){
                                    active=rectangle2
                                    Service.setModelName(active.inputText+textR)
                                }
                                else if(rectangle3.ptextFocus){
                                    active=rectangle3
                                }else if(rectangle4.ptextFocus){
                                    active=rectangle4
                                }else{
                                    console.log("not page 1 input,return")
                                    return;
                                }
                                active.inputText=active.inputText+textR
                            }
                            function onDeleteSignal(){
                               let active;
                               if(rectangle2.ptextFocus){
                                   active=rectangle2
                                   Service.setModelName(active.inputText.substring(0, active.inputText.length - 1))

                               }
                               else if(rectangle3.ptextFocus){
                                   active=rectangle3
                               }else if(rectangle4.ptextFocus){
                                   active=rectangle4
                               }else{
                                   console.log("not page 1 input,return")
                                   return;
                               }
                               let str=active.inputText
                               active.inputText=str = str.substring(0, str.length - 1)
                            }
                        }

                        InputSnippet{
                            id: rectangle2
                            width:parent.width/3
                            height:parent.height
                            tipText:qsTr("模型名称")
                            inputText: "ABC"

                            onTextFocusChanged:function(fo){

                                console.log("inputing model name")
                                if(fo===true){

                                    keyboard.state="invoked"

                                }else{
                                    if(row.anyFocus()){
                                        return;
                                    }
                                    Service.setModelName(rectangle2.inputText)

                                    keyboard.state="hidden"
                                }
                            }
                        }
                        InputSnippet{
                            id: rectangle3
                            width:parent.width/3
                            height:parent.height

                            tipText:qsTr("最大干密度(g/cm^3)")
                            inputText: "ABC"
                            onTextFocusChanged:function(fo){

                                if(fo===true){
                                    keyboard.state="invoked"
                                }else{
                                    if(row.anyFocus()){
                                        return;
                                    }
                                    keyboard.state="hidden"
                                }
                            }


                        }
                        InputSnippet{
                            id: rectangle4
                            width:parent.width/3
                            height:parent.height

                            tipText:qsTr("最佳含水率(%)")
                            inputText: "ABC"
                            onTextFocusChanged:function(fo){

                                if(fo===true){
                                    keyboard.state="invoked"
                                }else{
                                    if(row.anyFocus()){
                                        return;
                                    }
                                    keyboard.state="hidden"
                                }
                            }
                        }
                    }
                }

            }

            Rectangle {
                id: rectangle1


                width: rectangle.width/2
                height: 100
                color: "transparent"
                gradient: Gradient
                {
                    GradientStop { position: 0.0; color: "#f46b45" }

                    GradientStop { position: 1.0; color: "#eea849" }
                    orientation: Qt.Horizontal

                }
                anchors.horizontalCenter: rectangle.horizontalCenter
                radius: 5
                border.color: "#ffffff"
                anchors.top: rectangle.top
                anchors.topMargin:-rectangle1.height/2

                Text {
                    id: text1
                    text: qsTr("模型设置")
                    anchors.fill: parent
                    font.pixelSize: 25
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                }


            }
        }

        Rectangle{
            id:page2

            FocusScope{
                anchors.fill: parent
                focus:true

                Screen01{
                   id:keyboard1
                   z:99
                   width:parent.width
                   height:parent.height/2

                   opacity:0.3

                   screenHeight: parent.height

                   anchors.left: parent.left

                   onHostHidden: {

                          listView.currentItem.disableEdit()
                   }
                }




                Rectangle{
                    id:testArea

                    width:700
                    height:parent.height*0.3
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 50
                    anchors.horizontalCenter: parent.horizontalCenter

                    radius:5

                    color:"transparent"
                    border.width: 2
                    border.color: "lightgreen"

                    TestTwoPoint{
                        id:twopoints
                        anchors.fill: parent
                        color:"transparent"

                        onTestA:{
                            Service.requestPointTest()
                        }
                        onTestB:{
                            Service.requestPointTest();
                        }
                    }

                }

                Rectangle {
                    id: rectangle5


                    width: parent.width*0.9
                    height: parent.height*0.4
                    color: "transparent"


                    border.width: 1
                    anchors.bottom: testArea.top
                    anchors.bottomMargin: 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    //border.color: "white"
                    radius:5

                    Connections{
                        target:keyboard1
                        function onKeyPressed(textR){
                            if(textR!=="."&&isNaN(Number(textR))){
                                return;
                            }

                            if(listView.currentItem.densityFocus){
                                //all these effert are for the absence of virtualkeyboard
                                let text=listView.currentItem.densityText
                                listView.currentItem.densityText=text+textR

                            }
                            else if(listView.currentItem.waterRateFocus){
                                let text=listView.currentItem.waterRateText
                                listView.currentItem.waterRateText=text+textR

                            }else{
                                return;
                            }
                        }
                        function onDeleteSignal(){
                           let active;
                           if(listView.currentItem.densityFocus){
                               //all these effert are for the absence of virtualkeyboard
                               let text=listView.currentItem.densityText
                               listView.currentItem.densityText=
                                       text.substring(0, text.length - 1)
                           }
                           else if(listView.currentItem.waterRateFocus){
                               let text=listView.currentItem.waterRateText
                               listView.currentItem.waterRateText=
                                       text.substring(0, text.length - 1)
                           }else{
                               return;
                           }
                        }
                    }

                    ListView {
                        id: listView

                        anchors.fill: parent
                        anchors.topMargin: 0

                        Connections{
                            target:Service
                            function onRequestParingComplete(result,index){
                                let item=listView.itemAtIndex(index)
                                if(result===true){
                                    item.paringStatus="INIT"
                                }else{
                                    item.paringStatus="ERROR"
                                }
                            }
                            function onPointTestCompelete(i){
                                twopoints.resetTestStatus()

                            }
                        }

                        header:ViewHeader{
                               width:parent.width
                               height:50

                               widthRate:[1,3,3,2,2,2,2,2]

                               itemInfo:ListModel{

                                   ListElement{
                                       label:qsTr("测点")
                                   }
                                   ListElement{
                                       label:qsTr("湿密度")
                                   }
                                   ListElement{
                                       label:qsTr("含水率")
                                   }
                                   ListElement{
                                       label:qsTr("幅值")
                                   }
                                   ListElement{
                                       label:qsTr("相角")
                                   }
                                   ListElement{
                                       label:qsTr("温度")
                                   }
                                   ListElement{
                                       label:qsTr("GPS")
                                   }
                                   ListElement{
                                       label:""
                                   }
                               }
                        }

                        model: Service.getTestPointModel()

                        delegate: AddPointDelegate{
                            id:addPointDele
                            width:parent.width
                            height:50

                            itemIndex: model.index
                            density: model.density
                            waterRate: model.waterRate
                            amplitude:model.ampitude//error spelling
                            anglePhase: model.phaseAngle
                            temperature: model.temperature
                            gps:model.gps
                            isPairing:model.isPairing

                            onDensityInputActive:function (active) {

                                if(active===true){
                                    keyboard1.state="invoked"
                                }
                                listView.currentIndex=index
                            }
                            onWaterRateInputActive: function(active){

                                if(active===true){
                                    keyboard1.state="invoked"
                                }
                                listView.currentIndex=index
                            }
                            onRequireTest: function(index){
                                Service.requestPointInfoUpdate(index)
                                paringStatus="PAIRING"
                            }


                        }
                    }
                }

                property color buttonColor:"#6441a5"

                MyButton {
                    id: button
                    width:100
                    height:30
                    text: qsTr("增加项")
                    anchors.left: rectangle5.left
                    anchors.bottom: rectangle5.top
                    anchors.bottomMargin: 20
                    anchors.leftMargin: 0

                    originColor: "#8e0e00"
                    hoverColor: Qt.lighter(originColor,1.2)
                    fontSize:13

                    onClicked: {
                        Service.addNewModelTestPoint()
                    }
                }

                MyButton {
                    id: button1
                    text: qsTr("删除项")
                    width:100
                    height:30
                    anchors.left: button.right
                    anchors.top: button.top
                    anchors.topMargin: 0
                    anchors.leftMargin: 20
                    fontSize:13

                    originColor: "#8e0e00"
                    hoverColor: Qt.lighter(originColor,1.2)

                    onClicked: {
                        Service.deleteNewModelTestPoint(listView.count-1);

                    }


                }
                MyButton {
                    id: button3
                    text: qsTr("编辑完成")
                    width:130
                    height:30
                    anchors.left: button1.right
                    anchors.top: button1.top
                    anchors.topMargin: 0
                    anchors.leftMargin: 20
                    fontSize:13

                    originColor: "#8e0e00"
                    hoverColor: Qt.lighter(originColor,1.2)

                    onClicked:{
                        button3.focus=true
                    }
                }
            }



            gradient: Gradient {
                        GradientStop { position: 0.0; color: "#8e0e00" }

                        GradientStop { position: 1.0; color: "#1f1c18" }
                        //start: Qt.point(0, 0)
                        //end: Qt.point(width, height)
                        orientation: Qt.Horizontal

                    }

        }


        ModelOptimization{
            id:optimization
        }
    }

    PageIndicator{
        id:indicator
        height:50

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        count:view.count
        currentIndex:view.currentIndex
        delegate: Rectangle {
            implicitWidth: 16
            implicitHeight: 16

            radius: width / 2
            color: "#21be2b"

            opacity: index === indicator.currentIndex ? 0.95 : pressed ? 0.7 : 0.45

            Behavior on opacity {
                OpacityAnimator {
                    duration: 100
                }
            }
        }


    }


    Rectangle{
        width:300
        height:50

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom

        color:"transparent"
        Text{

            id:timeLabel
            anchors.fill: parent
            font.pointSize: 13
            color:"white"

        }

        Connections{
            target:Service

            function onSendTime(time,val){
                timeLabel.text=time;
            }
        }
    }
}

