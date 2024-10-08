import QtQuick 2.0
import QtMultimedia 5.0
import Analyzer 1.0

Item {
    width: 640
    height: 480

    Camera {
        id: camera
        focus.focusMode: Camera.FocusAuto

        onCameraStatusChanged: {
            if (camera.cameraStatus === Camera.LoadedStatus)
                handler.camera = camera;
        }
    }

    CameraHandler {
        id: handler
        onFilenameChanged: paused.source = "file:/" + handler.filename
    }

    VideoOutput {
        source: camera
        anchors.fill: parent
    }
}
