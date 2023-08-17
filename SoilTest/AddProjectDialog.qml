import QtQuick 2.15
import QtQuick.Controls 2.15
import "../keyboard/content/"
import"../"
Dialog {
    id:root

    width:1024
    height:600

    property string projectName
    property real   dryness

    Screen01{
        id:keyboard1
        z:99
        width:parent.width
        height:parent.height/2

        opacity:0.6

        screenHeight: parent.height

        anchors.left: parent.left

        onHostHidden: {
               container.disableEdit()
        }
    }

    contentItem:Rectangle{

        color:"#33141e"

        implicitHeight: parent.height
        implicitWidth:parent.width


        Connections{
            target:keyboard1
            function onKeyPressed(textR){



                if(container.drynessFoucs){
                    //all these effert are for the absence of virtualkeyboard
                    if(textR!=="."&&isNaN(Number(textR))){
                        return;
                    }
                    let text=drynessEdit.text
                    drynessEdit.text=text+textR

                }
                else if(container.projectNameFocus){

                    let text=projectNameEdit.text
                    projectNameEdit.text=text+textR

                }
               else{
                    return;
                }
            }
            function onDeleteSignal(){
               let active;
               if(container.drynessFoucs){
                   //all these effert are for the absence of virtualkeyboard
                   let text=drynessEdit.text
                   drynessEdit.text=
                           text.substring(0, text.length - 1)
               }
               else if(container.projectNameFocus){
                   let text=projectNameEdit.text
                   projectNameEdit.text=
                           text.substring(0, text.length - 1)
               }else{
                   return;
               }
            }
        }


        FocusScope{
            anchors.centerIn: parent
            focus:true

            width:parent.width/2
            height:200
            Column{
                id:container

                anchors.fill: parent

                spacing:10

                property alias projectNameFocus:projectNameEdit.focus
                property alias drynessFoucs:drynessEdit.focus

                function disableEdit(){
                    projectNameEdit.focus=false
                    drynessEdit.focus=false
                }



                Row{
                    width:parent.width
                    height:parent.height/2-spacing
                    Text{
                        text:qsTr("工程名称:")
                        height:parent.height
                        width:parent.width/2
                        font.pixelSize: 20
                        font.bold: true
                        color:"white"

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle{
                        height:parent.height
                        width:parent.width/2

                        radius:10

                        opacity:0.5
                        TextEdit{
                            id:projectNameEdit

                            anchors.fill: parent

                            font.pixelSize: 20
                            font.bold: true
                            color:"white"



                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            onFocusChanged:function (active) {

                                console.log("focus changed")

                                if(projectNameEdit.focus===true){
                                    keyboard1.state="invoked"
                                }

                                //listView.currentIndex=index
                            }
                        }
                    }


                }
                Row{
                    width:parent.width
                    height:parent.height/2-spacing
                    Text{
                        text:qsTr("干密度:")
                        height:parent.height
                        width:parent.width/2

                        font.pixelSize: 20
                        font.bold: true
                        color:"white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle{
                        radius:10
                        height:parent.height
                        width:parent.width/2
                        opacity:0.5
                        TextEdit{
                            id:drynessEdit

                            anchors.fill: parent

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            onFocusChanged:function (active) {

                                if(drynessEdit.focus===true){
                                    keyboard1.state="invoked"
                                }
                                //listView.currentIndex=index
                            }

                            font.pixelSize: 20
                            font.bold: true
                            color:"white"
                        }
                    }


                }

            }
        }



        Row{
            id:buttons
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin:10

            width:parent.width/3
            height:50
            MyButton{
                width:parent.width/2
                height:50
                text:qsTr("确定")

                onClicked: {
                    dryness=drynessEdit.text
                    projectName=projectNameEdit.text
                    root.accept();
                }
            }
            MyButton{
                width:parent.width/2
                height:50
                text:qsTr("取消")
                onClicked: {
                    root.reject()
                }
            }
        }
    }


}
