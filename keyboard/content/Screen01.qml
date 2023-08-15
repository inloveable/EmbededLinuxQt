import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
Item {
    id: keyboard
    width: 960
    height: 320
    signal keyPressed(string keyText)
    signal hostHidden();
    signal deleteSignal();


    required property int screenHeight


    state:"hidden"
    states:[
        State{
            name:"invoked"
            PropertyChanges{
                target:keyboard
                y:screenHeight-height
            }
        },
        State{
            name:"hidden"
            PropertyChanges{
                target:keyboard
                y:screenHeight
            }
        }
    ]
    transitions: Transition {
        from: "invoked"
        to: "hidden"
        reversible: true

        PropertyAnimation{
            target:keyboard
            property:"y"
            duration:500
            easing.type: Easing.InOutQuad
        }

    }
    visible:true


    Rectangle{
        id:rec

        anchors.fill: parent
        color:"#78ad6a"
        GridLayout {
            id: grid

            columns: 11
            rowSpacing: 10
            columnSpacing: 10
            anchors.fill: parent
            anchors.margins:10
            Repeater {
                model: valueListLower
                id:repeater
                property var valueListLower:['0', '1', '2', '3', '4', '5', '6', '7', '8', '9','.',
                    'q', 'w', 'e', 'r', 't', 'y', 'u','i', 'o', 'p','\'','a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'Delete', 'Case','z', 'x', 'c', 'v', 'b', 'n', 'm','Enter','space ',"\u2193"]
                property var valueListUpper:['0', '1', '2', '3', '4', '5', '6', '7', '8', '9','.',
                    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O','P', '\'','A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L','Delete', 'Case', 'Z', 'X', 'C', 'V', 'B', 'N', 'M','Enter','Space ',"\u2193"]
                property bool upperOrLower:false//true->Upper false->lower()

                Button {
                    text:modelData
                    id:btnBg

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    GridLayout.columnSpan: text===' '?11:text.length>=2?2:1
                    contentItem:Text{
                        text:modelData
                        color:"white"
                        font.pointSize: 16
                        anchors.centerIn: parent
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }

                    onPressedChanged: {
                        if (pressed) {
                            calight.start()
                        } else {
                            castop.start()
                        }
                    }
                    onClicked: {
                        if(text==='Case')
                        { repeater.upperOrLower=!repeater.upperOrLower;
                            repeater.model=(repeater.upperOrLower)?repeater.valueListUpper:repeater.valueListLower;
                            return;
                        }

                        if(text==="\u2193"){//down arrow
                            keyboard.state="hidden"
                            hostHidden()
                            return;
                        }

                        if(text==="Delete"){
                            deleteSignal()
                            return;
                        }

                        if(text==="space "||text==="Space "){
                            text=" "
                        }

                        keyboard.keyPressed(text)
                    }

                    background:Rectangle{
                        id:brec
                        color:'#14B045'
                        ColorAnimation on color {id:castop; to:'#14B045' ; running: false ; duration: 100 }
                        ColorAnimation on color { id:calight;to: "#0e7930";  running: false ;duration: 100 }
                        radius:4
                    }
                }
            }
        }}
}

