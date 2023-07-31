

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/



import QtQuick 2.7
import QtQuick.Controls 2.15


Rectangle {
    id:root
    width: 1920
    height: 1080

    Image {
        id: background
        anchors.fill: parent
        source: "qrc:/images/pietro-jeng-n6B49lTx7NM-unsplash.jpg"

    }

    signal invokeMainUi(string type)

    Repeater {
        z:1
        id: buttons

        function allToState(st){
            let i;
            for(i=0;i<buttons.count;++i){
                let rect=itemAt(i);
                rect.state=st;
            }
        }

        Rectangle {
            id:rect
            width: 120
            height: 120
            radius: width

            //opacity:0.5
            color:model.color
            border.color: model.borderColor

            state:"stayPut"

            MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        rectAnimation.to = 5;
                        rectAnimation.start();
                    }

                    onExited: {
                        rectAnimation.to = 0;
                        rectAnimation.start();
                    }

                    onClicked:{
                        if(rect.state=="stayPut"){
                              buttons.allToState("expanded")

                        }else if(rect.state=="expanded"){
                            buttons.allToState("stayPut")
                            invokeMainUi(text1.text);
                            console.log("invoked signal")
                        }


                    }
                }

                PropertyAnimation {
                    id: rectAnimation
                    target: rect
                    property: "border.width"
                    duration: 200
                    //easing.type: Easing.InOutQuad
                }
            x:root.width/2-width/2
            y:root.height/2-height/2
            states: [
                State{
                    name:"stayPut"
                    PropertyChanges {
                        target: rect
                        x:root.width/2-width/2
                        rotation:0
                    }
                },
                State{
                    name:"expanded"
                    PropertyChanges {
                        target: rect
                        x:model.expandXoffset+root.width/2-width/2
                        rotation:360
                    }
                    PropertyChanges {
                        target: text1
                        text:model.expandText?model.expandText:model.name
                    }

                }
            ]

            Behavior on x {
                PropertyAnimation{
                    id: propertyAnimation
                    target:rect
                    property:"x"
                    duration:500
                    easing.type:Easing.InOutQuad
                }

            }
            Behavior on rotation{
                RotationAnimation {
                     target:rect
                     duration: 500;
                     easing.type:Easing.InOutQuad
                }
            }

            Text{
                id:text1
                text:name
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pointSize: 16
                width:parent.width
                height:parent.height
                color: "#5d4707"
                anchors.centerIn: parent
            }
        }

        model: buttonModel
    }

    ListModel {
        id: buttonModel


        ListElement {
            name: "系统设置"
            color:"#287362"
            expandXoffset:-200
            borderColor: "#288B62"
        }

        ListElement {
            name: "数据传输"
            color:"#287362"
            borderColor: "#288B62"
            expandXoffset:-400
        }

        ListElement {
            name: "土壤建模"
            color:"#287362"
            borderColor: "#288B62"
            expandXoffset:200
        }

        ListElement {
            name: "工程测试"
            color:"#287362"
            expandXoffset:400
            borderColor: "#288B62"
        }

        ListElement {
            name: "开始"
            color:"#287362"
            expandText:"返回"
            borderColor: "#288B62"

            expandXoffset:0

        }
    }

}
