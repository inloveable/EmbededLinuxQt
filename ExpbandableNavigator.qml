import QtQuick 2.15
import QtQuick.Controls 2.15
Item {
    id:root

    property int orientation:Qt.Vertical


    signal returnSignal()
    signal typeInvoked(string type)

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
        delegate:RoundButton {
            id:rect
            width: root.width
            height: root.width
            radius: width

            icon.source:model.icon

            //opacity:0.5
            background:Rectangle{
                radius:rect.radius
                width:rect.width
                height:rect.height
                color:model.color
            }

            state:"stayPut"



            onClicked:{
                if(rect.state=="stayPut"){
                    buttons.allToState("expanded")

                }else if(rect.state=="expanded"){
                    buttons.allToState("stayPut")

                    console.log("invoked signal"+model.type)
                    if(type==="return"){
                        root.returnSignal()//tricky
                        return
                    }
                    typeInvoked(type)
                }

            }



            x:root.width/2-width/2
            y:root.height/2-height/2
            states: [
                State{
                    name:"stayPut"
                    PropertyChanges {
                        target: rect
                        y:root.height/2-height/2
                        rotation:0
                    }
                },
                State{
                    name:"expanded"
                    PropertyChanges {
                        target: rect
                        y:model.expandXoffset+root.height/2-height/2
                        rotation:360
                    }

                }
            ]

            Behavior on y {
                PropertyAnimation{
                    id: propertyAnimation
                    target:rect
                    property:"y"
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


        }

        model: buttonModel
    }

    ListModel {
        id: buttonModel


        ListElement {

            color:"#287362"
            expandXoffset:-75
            icon:"qrc:/images/icons/soiltesticons/open.png"
            borderColor: "#288B62"

            type:"openTest"
        }

        ListElement {

            color:"#287362"
            borderColor: "#288B62"
            icon:"qrc:/images/icons/soiltesticons/delete.png"
            expandXoffset:-150

            type:"deleteTest"
        }

        ListElement {

            color:"#287362"
            borderColor: "#288B62"
            icon:"qrc:/images/icons/soiltesticons/return.png"
            expandXoffset:-225


            type:"return"
        }

        ListElement {

            color:"#287362"
            expandXoffset:-300
            borderColor: "#288B62"
            icon:"qrc:/images/icons/soiltesticons/add.png"

            type:"addTest"

        }

        ListElement {

            color:"#287362"

            borderColor: "#288B62"
            icon:"qrc:/images/icons/soiltesticons/moreActions.png"
            expandXoffset:0

            type:"expand"

        }//at last to be on top


    }
}
