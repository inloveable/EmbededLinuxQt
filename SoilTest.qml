import QtQuick 2.15


import "./SoilModeling/"
import QtQuick.Controls 2.15

import CppCore 1.0
Rectangle {
    id: item1
    width:1024
    height:600

    color:"blue"

    signal returnSignal()

    StackView {
        id: swipeView
        anchors.fill: parent

        pushEnter: Transition {
               PropertyAnimation {
                   property: "opacity"
                   from: 0
                   to:1
                   duration: 200
                   easing.type:Easing.InOutQuad
               }
           }
           pushExit: Transition {
               PropertyAnimation {
                   property: "opacity"
                   from: 1
                   to:0
                   duration: 200
                   easing.type:Easing.InOutQuad
               }
           }
           popEnter: Transition {
               PropertyAnimation {
                   property: "opacity"
                   from: 0
                   to:1
                   duration: 200
                   easing.type:Easing.InOutQuad
               }
           }
           popExit: Transition {
               PropertyAnimation {
                   property: "opacity"
                   from: 1
                   to:0
                   duration: 200
                   easing.type:Easing.InOutQuad
               }

            }
        initialItem: projectList
        Rectangle{
            id:projectList

            color:"#2b333e"
            Text{
                width:parent.width/2
                height:100
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                font.bold:true
                font.pointSize:20

                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter



                text:"工程表单"
            }



            ListView {
                id:listView
                function removeCurrent(){
                    //TODO:replace these with  actual model actions
                    //listView.model.remove(currentIndex)
                }
                anchors.centerIn: parent
                height:parent.height*3/4
                width:parent.width*2/3

                header:ViewHeader{
                    width:parent.width
                    height:50


                    widthRate:[1,1,4]

                    itemInfo:ListModel{
                        ListElement{
                            label:"编号"
                        }
                        ListElement{
                            label:"名称"
                        }
                        ListElement{
                            label:"创建时间"
                        }
                    }

                }

                delegate: Rectangle {
                    width:parent.width
                    height:50




                    property string projectName:projectNameText.text
                    color:ListView.isCurrentItem?"#47484c":"#61649f"

                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                            listView.currentIndex=index
                        }
                    }

                    Row {
                        id: row1
                        anchors.fill: parent

                        property var widthRate:[1,1,4]

                        function getWidth(cindex){
                            let count=0;

                            for(var i=0;i<widthRate.length;++i){
                                count+=widthRate[i];
                            }
                            let totalWidth=row1.width
                            let slice=totalWidth/count
                            return slice*widthRate[cindex];
                        }

                        Text {
                            width: row1.getWidth(0)
                            height: parent.height
                            text:modelData.index

                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                        }

                        Text {
                            id:projectNameText
                            width: row1.getWidth(1)
                            height: parent.height
                            text:modelData.name

                            font.bold: true
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                        }
                        Text {
                            width: row1.getWidth(2)
                            height: parent.height
                            text:modelData.createTime
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            font.bold: true
                        }

                    }
                }

                Component.onCompleted: {
                    Service.requestProjectInfo()
                }

                //model: Service.requestProjectInfo()
            }

            Connections{
                target:Service

                function onSendProjectInfo(list){
                      listView.model=list
                }
                function onProjectInfoNeedsUpdate(){
                    Service.requestProjectInfo();
                }
            }

            ExpbandableNavigator {
                id: expbandableNavigator

                width:65
                height:350

                anchors.right: parent.right
                anchors.bottom: parent.bottom

                //anchors.bottomMargin:40
                anchors.rightMargin: 40

                onReturnSignal:{
                    item1.returnSignal()

                }

                onTypeInvoked: function(type){
                    if(type==="deleteTest"){
                        listView.removeCurrent()
                    }else if(type==="openTest"){
                        let item=swipeView.push(
                                "qrc:/SoilTest/SoilTestPoint.qml",
                                {
                                    projectName:listView.currentItem.projectName
                                }

                                )

                        item.returnSignal.connect(function(){
                            swipeView.pop()
                        }
                        )
                    }
                }

            }
        }
    }
}
