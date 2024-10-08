import QtQuick 2.0
import QtMultimedia 5.5
import com.loogear.softphone 1.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.2
Window {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    //    PhoneCall{
    //        id:_phoneCall
    //    }
    VideoSurfaceHandle{
        id:surfaceHandle_out
        windowRole:VideoSurfaceHandle.OUTGOING
    }
    VideoSurfaceHandle{
        id:surfaceHandle_in
        windowRole:VideoSurfaceHandle.INCOMING
    }
    Component.onCompleted: {


        // camera.stop()

    }

    Item {
        id:media
        anchors {
            bottom: parent.bottom
            right:parent.right
            topMargin: 0
        }
        visible: true
        width: parent.width/3
        height: parent.height/3
        //        Camera {
        //            id: camera
        //            focus.focusMode: Camera.FocusAuto
        //            //position :Camera.FrontFace
        //            onCameraStatusChanged: {
        //                 if (camera.cameraStatus === Camera.LoadedStatus){
        //                handler.camera = camera;
        //                console.log("handler.camera :"+handler.filename);
        //                console.log("camera.metaData: "+ camera.metaData.videoFrameRate);
        //                 }
        //            }
        //        }

        //        CameraHandler {
        //            id: handler
        //            onFilenameChanged: paused.source = "file:/" + handler.filename
        //            paused:true
        //        }

        //        VideoOutput {
        //            id:_videoOutput
        //            source:camera
        //            anchors.fill: parent
        //        }

    }
    MediaPlayer {
        id: player
        source:"file:///Volumes/data2/1.mp4"
        autoPlay: false
    }
    VideoOutput {
        anchors {
            top: parent.top
            topMargin: 0
            left:parent.left
        }

        id:_videoOutputIcoming
        source:surfaceHandle_in
        width:800
        height:  parent.height*2/3
        visible: true
    }

    VideoOutput {
        anchors {
            bottom: parent.bottom
            bottomMargin: 0
            right:parent.right
        }
        id:_videoOutput1
        source:surfaceHandle_out
        width:400
        height:  parent.height*2/3
        visible: true
    }

    //    Rectangle{
    //        width: parent.width/5;
    //        height: parent.height/5;
    //        Text{
    //        //    anchors.left:parnet.left;
    //            id:myNum_text;
    //            text:"本机号码:"
    //            font.pixelSize: 90
    //        }
    //        TextInput { id:myNum;
    //            anchors.left:myNum_text.right;
    //            text: "888212";
    //            font.pixelSize: 90
    //            font.weight: Font.DemiBold }

    //        Text{id:peerNum_text;
    //            anchors.left:myNum.right;
    //            text:"对方号码:"
    //            font.pixelSize: 90
    //        }
    //        TextInput { id:peerNum;
    //            anchors.left:peerNum_text.right;
    //            text: "888213";
    //            font.pixelSize: 90
    //            font.weight: Font.DemiBold }
    //      //  anchors.left:parnet.left;
    //        anchors.bottom: buttonInit.top;
    //        anchors.bottomMargin: 0
    //    }
    Button {
        id: buttonSwap
        width: parent.width/10
        height: parent.height/10
        text: qsTr("Swap")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        onClicked:{
            var  var1=myNum.text;
            var var2=peerNum.text;
            myNum.text=var2;
            peerNum.text=var1;

        }
    }
    Button {
        id: buttonInit
        width: parent.width/10
        height: parent.height/10
        text: qsTr("初始化")
        anchors.bottom: parent.bottom
        anchors.left:buttonSwap.right
        anchors.bottomMargin: 0
        onClicked:{
            var  retBool= _phoneCall.initVideos2(myNum.text);


        }
    }
    Button {
        id: buttonReg
        width: parent.width/10
        height: parent.height/10
        text: qsTr("登录")
        anchors.bottom: parent.bottom
        anchors.left:buttonInit.right
        anchors.bottomMargin: 0
        onClicked:{
            var  retBool= bridge.registerToServer("123","1123");
            console.log("registerToServer ret="+retBool)

        }
    }

    Button {
        id: buttonCall
        width: parent.width/10
        height: parent.height/10
        text: qsTr("呼叫")
        anchors.bottom: parent.bottom
        anchors.left:buttonReg.right
        anchors.bottomMargin: 0
        onClicked:{
            var  retBool= bridge.callto("888212");
            console.log("callto ret="+retBool)

        }
    }
    Button {
        id: buttonVideoCall
        width: parent.width/10
        height: parent.height/10
        text: qsTr("视频上传")
        anchors.bottom: parent.bottom
        anchors.left:buttonCall.right
        anchors.bottomMargin: 0
        onClicked:{
            var  retBool= _phoneCall.startVieoToPeer2(peerNum.text);
            console.log("callVideoto ret="+retBool)

        }
    }
    Button {
        id: buttonStopVideo
        width: parent.width/10
        height: parent.height/10
        text: qsTr("视频停止")
        anchors.bottom: parent.bottom
        anchors.left:buttonVideoCall.right
        anchors.bottomMargin: 0
        onClicked:{
            camera.stop()
        }
    }
    Button {
        id: buttonSartVideo
        width: parent.width/10
        height: parent.height/10
        text: qsTr("视频启动")
        anchors.bottom: parent.bottom
        anchors.left:buttonStopVideo.right
        anchors.bottomMargin: 0
        onClicked:{
            camera.start()
        }
    }
}

