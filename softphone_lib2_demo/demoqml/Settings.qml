import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Item {
    property string shaderMatrix: "mat4( 1.0,  0.0, 0.0, 0.0 ,
                                         0.0,  1.0, 0.0, 0.0 ,
                                         0.0,  0.0, 1.0, 0.0 ,
                                         0.0,  0.0, 0.0, 1.0 );"

    id: root
    MouseArea {
        anchors.fill: parent
        onClicked: root.visible = false
    }

    Rectangle {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
        width: layout.width + 40
        height: layout.height + 40
        color: "white"
        opacity: 0.9
        radius: 10

        ExclusiveGroup {
            id: colorBlindeness
        }

        ColumnLayout {
            id: layout
            anchors.centerIn: parent
            spacing: 10

            Label {
                text: "Color Blindness Simulation"
                color: "black"
            }

            Repeater {
                model: ListModel {
                    ListElement { name: "Off"; isDefault: true;
                        matrix: "mat4( 1.0, 0.0, 0.0, 0.0 ,
                                       0.0, 1.0, 0.0, 0.0 ,
                                       0.0, 0.0, 1.0, 0.0 ,
                                       0.0, 0.0, 0.0, 1.0 );"
                    }
                    ListElement { name: "Deuteranopia (Green greatly reduced, 1% men)"
                        matrix: "mat4( 0.43 ,  0.72 , -0.15 ,  0.0 ,
                                                                                 0.34 ,  0.57 ,  0.09 ,  0.0 ,
                                                                                -0.02 ,  0.03 ,  1.00 ,  0.0 ,
                                                                                 0.0  ,  0.0  ,  0.0  ,  1.0 );"
                    }
                    //            'Deuteranopia':[0.625,0.375,0,0,0, 0.7,0.3,0,0,0, 0,0.3,0.7,0,0, 0,0,0,1,0, 0,0,0,0,1],
                    ListElement { name: "Deuteranomaly (Green weak, 5% men, 0.4% women)"
                        matrix: "mat4(0.8, 0.2, 0.0, 0.0,
                                      0.258, 0.742, 0.0, 0.0,
                                      0.0, 0.142, 0.858, 0.0,
                                      0.0, 0.0, 0.0, 1.0);"
                    }
                    ListElement { name: "Protanopia (Red greatly reduced, 1% men)"
                        matrix: "mat4(0.567, 0.433, 0.0 ,0.0,
                                      0.558, 0.442, 0.0, 0.0,
                                      0.0, 0.242, 0.758, 0.0,
                                      0.0, 0.0, 0.0, 1.0);"
                    //            'Protanopia':[0.567,0.433,0,0,0, 0.558,0.442,0,0,0, 0,0.242,0.758,0,0, 0,0,0,1,0, 0,0,0,0,1],
                    }
                    ListElement { name: "Protanomaly (Red weak, 1% men)"
                        matrix: "mat4(0.817, 0.183, 0.0, 0.0,
                                      0.333, 0.667, 0.0, 0.0,
                                      0.0, 0.125, 0.875, 0.0,
                                      0.0, 0.0, 0.0, 1.0 );"
                    //            'Protanomaly':[0.817,0.183,0,0,0, 0.333,0.667,0,0,0, 0,0.125,0.875,0,0, 0,0,0,1,0, 0,0,0,0,1],

                    }
                    ListElement { name: "Tritanopia (Blue greatly reduced, 0.003% of population)"
                        matrix: "mat4(0.95, 0.05, 0.0, 0.0,
                                      0.0, 0.433, 0.567, 0.0,
                                      0.0, 0.475, 0.525, 0.0,
                                      0.0, 0.0, 0.0, 1.0 );"
                    //            'Tritanopia':[0.95,0.05,0,0,0, 0,0.433,0.567,0,0, 0,0.475,0.525,0,0, 0,0,0,1,0, 0,0,0,0,1],
                    }
                    ListElement { name: "Tritanomaly (Blue weak, very seldom)"

                        matrix: "mat4(0.967, 0.033, 0.0, 0.0,
                                      0.0, 0.733, 0.267, 0.0,
                                      0.0, 0.183, 0.817, 0.0,
                                        0.0, 0.0, 0.0, 1.0 );"
                    //            'Tritanomaly':[0.967,0.033,0,0,0, 0,0.733,0.267,0,0, 0,0.183,0.817,0,0, 0,0,0,1,0, 0,0,0,0,1],
                    }
                    ListElement { name: "Achromatopsia (Almost no color vision, very seldom)"
                        matrix: "mat4(0.299, 0.587, 0.114, 0.0,
                                      0.299, 0.587, 0.114, 0.0,
                                      0.299, 0.587, 0.114, 0.0,
                                        0.0, 0.0, 0.0, 1.0 );"
                    //            'Achromatopsia':[0.299,0.587,0.114,0,0, 0.299,0.587,0.114,0,0, 0.299,0.587,0.114,0,0, 0,0,0,1,0, 0,0,0,0,1],
                    }
                    ListElement { name: "Achromatomaly (No color vision, very seldom)"
                        matrix: "mat4(0.618, 0.320, 0.062, 0.0,
                                      0.163, 0.775, 0.062, 0.0,
                                      0.163, 0.320, 0.516, 0.0,
                                      0.0, 0.0, 0.0, 1.0 );"
                    //            'Achromatomaly':[0.618,0.320,0.062,0,0, 0.163,0.775,0.062,0,0, 0.163,0.320,0.516,0,0,0,0,0,1,0,0,0,0,0]}[v]);
                    }
                }
                RadioButton {
                    text: name
                    exclusiveGroup: colorBlindeness
                    checked: isDefault
                    onCheckedChanged: if (checked) shaderMatrix = matrix
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
