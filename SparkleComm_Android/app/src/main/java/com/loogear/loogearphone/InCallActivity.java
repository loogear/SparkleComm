package com.loogear.loogearphone;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.media.AudioManager;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.loogear.loogearphone.uicontrol.CustomCheckBox;
import com.loogear.loogearphone.uicontrol.CustomImageView;
import com.loogear.phone.CallInfo;
import com.loogear.phone.PhoneApi;

import java.util.Calendar;
import java.util.Date;
import java.util.List;


public class InCallActivity extends AppCompatActivity implements View.OnClickListener, CustomCheckBox.OnCheckChangedListener {
    private final static String TAG = "InCallActivity";
    private static InCallActivity m_InCallActivity = null;
    private String m_PhoneNumber = "";
    private boolean m_IsVideo = false;
    private boolean m_IsPauseVideo = false; // 进入后台时，暂停视频
    private boolean m_IsIncoming = false; // 是否为来电接听
    private SurfaceView m_SurfacePreview;
    private SurfaceView m_SurfaceInVideo;
    private TextView m_HeadInfo;
    private TextView m_CallInfo;
    private TextView m_TimeInfo;
    private CustomImageView m_Hangup;
    private CustomImageView m_Trans;
    private CustomImageView m_MultiParty;
    private CustomImageView m_CameraSwitch;
    private CustomImageView m_CameraRotate;
    private CustomCheckBox m_CameraHide;
    private CustomCheckBox m_Mute;
    private CustomCheckBox m_Speaker;
    private CustomCheckBox m_Record;
    private CustomCheckBox m_Video;
    private LinearLayout m_KeyPadLayout;
    private LinearLayout m_GroupButton1Layout;
    private LinearLayout m_GroupButton2Layout;
    private LinearLayout m_CameraLayout;
    private LinearLayout m_CallInfoLayout;
    private LinearLayout m_FunctionLayout;
    private CustomCheckBox m_KeyPad;
    private CustomCheckBox m_Hold;
    private TextView m_DTMF;
    private int m_CurrVolume;
    private int m_CallId = -1;// 当前通话的CALLID
    private int m_AccountId = 0;// 当前账户ID
    private int m_RecordId = -1; // 当前录音ID
    private Calendar m_CallBegin; // 通话开始时间
    private Calendar m_RecordBegin; // 录音开始时间
    private int m_CameraOrient = 90;// 摄像头旋转角度
    private float m_PreviewWindowsX = 0;// 预览窗口的X位置
    private boolean m_IsFrontCamera = true; //当前为前置摄像头
    private Handler m_Handler = new Handler();

    private Runnable m_TimeTask = new Runnable() {
        @Override
        public void run() {
            if(m_InCallActivity != null) {
                Calendar now = Calendar.getInstance();
                String text = GetTimeDiffString(m_CallBegin.getTime(), now.getTime());
                m_TimeInfo.setText(text);// 更新通话时间显示
                if (m_Record.getCheckState()) {
                    text = GetTimeDiffString(m_RecordBegin.getTime(), now.getTime());
                    m_Record.setText(text);// 更新录音时间显示
                }
                PhoneApi.getInstance().getAllCallInfo();// 刷新通话状态
                m_Handler.postDelayed(m_TimeTask, 1000);
            }
        }
    };

    private Runnable m_UiHideTask = new Runnable() {
        @Override
        public void run() {
            if(m_IsVideo) { // 视频通话时，界面长时间未操作时，隐藏界面控件显示
                HideAllUiControl();
            }
            else if(m_FunctionLayout.getVisibility() != View.VISIBLE) { // 否则显示所有界面
                ShowAllUiControl();
            }

            m_Handler.postDelayed(m_UiHideTask, 3000);
        }
    };

    private void ResetUiHideTask() {
        m_Handler.removeCallbacks(m_UiHideTask);
        m_Handler.postDelayed(m_UiHideTask, 3000);
    }

    /**
     * 获取时间差的字符串：格式为：00:00或者01:00:00
     * @param begin
     * @param end
     * @return
     */
    private String GetTimeDiffString(Date begin, Date end){
        long diff = end.getTime() - begin.getTime(); // 差值是微秒级别
        long days = diff / (1000 * 60 * 60 * 24);
        long hours = (diff-days*(1000 * 60 * 60 * 24))/(1000* 60 * 60);
        long minutes = (diff-days*(1000 * 60 * 60 * 24)-hours*(1000* 60 * 60))/(1000* 60);
        long seconds = (diff-days*(1000 * 60 * 60 * 24)-hours*(1000* 60 * 60)-minutes*(1000*60))/1000;

        String text = "";
        if(days > 0) text = String.format("%02d:%02d:%02d:%02d", days, hours, minutes, seconds);
        else if(hours > 0) text = String.format("%02d:%02d:%02d", hours, minutes, seconds);
        else text = String.format("%02d:%02d", minutes, seconds);

        return text;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // 处理状态栏和导航栏透明显示背景图
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            // Android 5.0 以上 全透明
            Window window = getWindow();
            window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS
                    | WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
            window.getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
            window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
            window.setStatusBarColor(Color.TRANSPARENT);
            // 虚拟导航键
            window.setNavigationBarColor(Color.TRANSPARENT);
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            // Android 4.4 以上 半透明
            Window window = getWindow();
            // 状态栏
            window.addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            // 虚拟导航键
            window.addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
        }

        setContentView(R.layout.activity_incall);
        m_InCallActivity = this;
        Log.i(TAG, "onCreate");
        processExtraData();

        m_SurfacePreview = (SurfaceView)findViewById(R.id.surfacePreviewCapture);
        m_SurfacePreview.setBackgroundColor(0x00FF0000);
        m_SurfacePreview.getHolder().setFormat(PixelFormat.RGB_888);
        m_SurfacePreview.setZOrderOnTop(true);
        m_SurfacePreview.getHolder().addCallback(m_PreviewVideoCallback);

        m_SurfaceInVideo = (SurfaceView)findViewById(R.id.surfaceIncomingVideo);
        m_SurfaceInVideo.setBackgroundColor(0xFFFFFF);
        m_SurfaceInVideo.getHolder().setFormat(PixelFormat.RGB_888);
        m_SurfaceInVideo.getHolder().addCallback(m_RemoteVideoCallback);
        m_SurfaceInVideo.setOnClickListener(this);

		m_HeadInfo = (TextView) findViewById(R.id.txtPhoneNumber);
        m_TimeInfo = (TextView) findViewById(R.id.txtTime);
        m_CallInfo = (TextView) findViewById(R.id.txtInfo);
        m_Hangup = (CustomImageView) findViewById(R.id.call_hangup);
        m_CameraSwitch = (CustomImageView) findViewById(R.id.camera_switch);
        m_Mute = (CustomCheckBox) findViewById(R.id.call_mute);
        m_Speaker = (CustomCheckBox) findViewById(R.id.call_speaker);
        m_KeyPadLayout = (LinearLayout) findViewById(R.id.keypad);
        m_GroupButton1Layout = (LinearLayout) findViewById(R.id.group_button1);
        m_GroupButton2Layout = (LinearLayout) findViewById(R.id.group_button2);

        m_CameraLayout = (LinearLayout) findViewById(R.id.CameraLayout);
        m_CallInfoLayout = (LinearLayout) findViewById(R.id.CallInfoLayout);
        m_FunctionLayout = (LinearLayout) findViewById(R.id.FunctionLayout);

        m_KeyPad = (CustomCheckBox) findViewById(R.id.call_keypad);
        m_DTMF = (TextView) findViewById(R.id.dtmf);
        m_Hold = (CustomCheckBox) findViewById(R.id.call_hold);
        m_Record = (CustomCheckBox) findViewById(R.id.call_record);
        m_Trans = (CustomImageView) findViewById(R.id.call_trans);
        m_Video = (CustomCheckBox) findViewById(R.id.call_video);
        m_CameraRotate = (CustomImageView) findViewById(R.id.camera_rotate);
        m_CameraHide = (CustomCheckBox) findViewById(R.id.camera_hidden);
        m_MultiParty = (CustomImageView) findViewById(R.id.call_multiparty);

        m_Hangup.setImageClickListener(this);
        m_Mute.setCheckChangedListener(this);
        m_KeyPad.setCheckChangedListener(this);
        m_Speaker.setCheckChangedListener(this);
        m_Hold.setCheckChangedListener(this);
        m_Record.setCheckChangedListener(this);
        m_Trans.setImageClickListener(this);
        m_Video.setCheckChangedListener(this);
        m_CameraSwitch.setImageClickListener(this);
        m_CameraRotate.setImageClickListener(this);
        m_CameraHide.setCheckChangedListener(this);
        m_MultiParty.setImageClickListener(this);

        // 注册按键点击事件
        ((TextView) findViewById(R.id.one)).setOnClickListener(this);
        ((TextView) findViewById(R.id.two)).setOnClickListener(this);
        ((TextView) findViewById(R.id.three)).setOnClickListener(this);
        ((TextView) findViewById(R.id.four)).setOnClickListener(this);
        ((TextView) findViewById(R.id.five)).setOnClickListener(this);
        ((TextView) findViewById(R.id.six)).setOnClickListener(this);
        ((TextView) findViewById(R.id.seven)).setOnClickListener(this);
        ((TextView) findViewById(R.id.eight)).setOnClickListener(this);
        ((TextView) findViewById(R.id.nine)).setOnClickListener(this);
        ((TextView) findViewById(R.id.zero)).setOnClickListener(this);
        ((TextView) findViewById(R.id.star)).setOnClickListener(this);
        ((TextView) findViewById(R.id.pound)).setOnClickListener(this);

        m_HeadInfo.setText(m_PhoneNumber);
        HideVideoWindow();
        EnableControl(false);
        CloseSpeaker();// 默认听筒接听
        if(m_IsIncoming) { // 来电时
            AnswerPhone();
        }
        else { // 去电时
            CallPhone();
        }
    }

    /**
     * 获取摄像头旋转方向
     * @param isFrontCamera
     * @return
     */
    private int GetPreviewRotateDegree(boolean isFrontCamera){
        int phoneDegree = 0;
        int result = 0;
        //获得手机方向
        int phoneRotate =getWindowManager().getDefaultDisplay().getOrientation();
        //得到手机的角度
        switch (phoneRotate) {
            case Surface.ROTATION_0: phoneDegree = 0; break;        //0
            case Surface.ROTATION_90: phoneDegree = 90; break;      //90
            case Surface.ROTATION_180: phoneDegree = 180; break;    //180
            case Surface.ROTATION_270: phoneDegree = 270; break;    //270
        }

        //分别计算前后置摄像头需要旋转的角度
        Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
        if(isFrontCamera){
            Camera.getCameraInfo(Camera.CameraInfo.CAMERA_FACING_FRONT, cameraInfo);
            result = (cameraInfo.orientation + phoneDegree) % 360;
            result = (360 - result) % 360;
            Log.i(TAG, "FrontCamera:"+cameraInfo.orientation+" result="+result);
        }
        else {
            Camera.getCameraInfo(Camera.CameraInfo.CAMERA_FACING_BACK, cameraInfo);
            result = (cameraInfo.orientation - phoneDegree +360) % 360;
            Log.i(TAG, "BackCamera:"+cameraInfo.orientation+" result="+result);
        }

        // 角度可能需要进行如下公式计算，需要更多的设备验证，目前这个自动计算角度的问题，可能并不完善
        return result = (360-result)%360;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "onDestroy");
        RestoreAudioMode();
        m_InCallActivity = null;
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.i(TAG, "onStart");
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.i(TAG, "onPause");
        if(m_IsVideo) {
            m_IsPauseVideo = true;
            PhoneApi.getInstance().closeVideo();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i(TAG, "onResume");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        Log.i(TAG, "onRestart");
        if(m_IsPauseVideo) {
            m_IsPauseVideo = false;
            PhoneApi.getInstance().openVideo();
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch(keyCode){
             case KeyEvent.KEYCODE_BACK:return true;// 禁止按返回键，或者按返回键进行挂机操作
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        Log.i(TAG, "onNewIntent");
        setIntent(intent);//must store the new intent unless getIntent() will return the old one
        processExtraData();
        /* 暂时无用
        if(!m_IsIncoming) CallPhone();
        m_HeadInfo.setText(m_PhoneNumber);*/
    }

    private void processExtraData(){
        Intent intent = getIntent();
        m_PhoneNumber = intent.getStringExtra("number");
        m_IsVideo = intent.getBooleanExtra("video", false);
        m_IsIncoming = intent.getBooleanExtra("incoming", false);
        m_CallId = intent.getIntExtra("callid",-1);
        m_AccountId = intent.getIntExtra("accountid",0);

        Log.i(TAG, "processExtraData:number="+m_PhoneNumber+" callId="+m_CallId);
    }

    /**
     * 设置控件是否可以点击
     * @param enable
     */
    private void EnableControl(boolean enable){
        m_Mute.setEnabled(enable);
        m_MultiParty.setEnabled(enable);
        m_Video.setEnabled(enable);
        m_Hold.setEnabled(enable);
        m_Trans.setEnabled(enable);
        m_Record.setEnabled(enable);
    }

    /**
     * 视频通话时，点击视频后显示所有界面控件
     */
    private void ShowAllUiControl() {
        m_CameraLayout.setVisibility(View.VISIBLE);
        m_CallInfoLayout.setVisibility(View.VISIBLE);
        m_FunctionLayout.setVisibility(View.VISIBLE);
    }

    /**
     * 视频通话时，界面没有任何操作后，隐藏所有界面控件；
     */
    private void HideAllUiControl() {
        m_CameraLayout.setVisibility(View.INVISIBLE);
        m_CallInfoLayout.setVisibility(View.INVISIBLE);
        m_FunctionLayout.setVisibility(View.INVISIBLE);
    }

    /**
     * 隐藏视频界面
     */
    private void HideVideoWindow() {
        Log.i(TAG, "HideVideoWindow");
        FrameLayout preview = (FrameLayout)findViewById(R.id.surfacePreviewCapture_frame);
        FrameLayout incoming = (FrameLayout)findViewById(R.id.surfaceIn_frame);
        preview.setVisibility(View.INVISIBLE);
        m_PreviewWindowsX = preview.getX();
        preview.setX(-10000); // 修改位置的方式隐藏surface
        incoming.setVisibility(View.INVISIBLE);
        m_CameraSwitch.setVisibility(View.INVISIBLE);
        m_CameraHide.setVisibility(View.INVISIBLE);
        m_CameraRotate.setVisibility(View.INVISIBLE);
    }

    private void ShowVideoWindow() {
        Log.i(TAG, "ShowVideoWindow");
        FrameLayout preview = (FrameLayout)findViewById(R.id.surfacePreviewCapture_frame);
        FrameLayout incoming = (FrameLayout)findViewById(R.id.surfaceIn_frame);
        preview.setVisibility(View.VISIBLE);
        preview.setX(m_PreviewWindowsX); // 还原位置
        incoming.setVisibility(View.VISIBLE);
        m_CameraSwitch.setVisibility(View.VISIBLE);
        m_CameraHide.setVisibility(View.VISIBLE);
        m_CameraRotate.setVisibility(View.VISIBLE);
    }

    /**
     * 拨打电话
     */
    private void CallPhone(){
        m_CallInfo.setText(R.string.calling);
        PhoneApi.getInstance().enableVideo(m_IsVideo);// 是否视频通话
        // 在子线程里进行拨号，因为拨号可能会稍微花点时间，避免UI迟钝
        new Thread(new Runnable() {
            @Override
            public void run() {
                m_CallId = PhoneApi.getInstance().makeVoiceCall(m_PhoneNumber, "X-DEVICE-ID:", 0);
            }
        }).start();
    }

    /**
     * 接听电话
     */
    private void AnswerPhone(){
        m_CallInfo.setText(R.string.ready_answer);
        PhoneApi.getInstance().answerCall(m_CallId, m_AccountId);
    }

    /**
     * 处理按键消息
     * @param key
     */
    private void ProcessKeyMsg(char key)
    {
        Log.i(TAG, "ProcessKeyMsg:"+key);
        m_DTMF.setText(m_DTMF.getText()+String.valueOf(key));
        PhoneApi.getInstance().sendDtmf(String.valueOf(key));
    }

    @Override
    public void onClick(View view) {
        ResetUiHideTask();
        switch(view.getId()) {
            case R.id.call_hangup:{
                PhoneApi.getInstance().releaseCall(m_CallId, m_AccountId);
                break;
            }
            case R.id.call_trans:{// 呼叫转移
                String addr = "1007";
                Toast.makeText(this, "呼叫转移测试号码："+addr, Toast.LENGTH_LONG).show();
                PhoneApi.getInstance().callTransfer(addr, m_CallId, m_AccountId);
                break;
            }
            case R.id.camera_switch:{// 切换摄像头
                m_IsFrontCamera = !m_IsFrontCamera;
                m_CameraOrient = GetPreviewRotateDegree(m_IsFrontCamera);
                PhoneApi.getInstance().setVideoCaptureOrient(m_CameraOrient);
                PhoneApi.getInstance().cameraSwitch(m_IsFrontCamera);
                break;
            }
            case R.id.camera_rotate:{ // 旋转摄像头方向
                m_CameraOrient += 90;
                m_CameraOrient %= 360;
                PhoneApi.getInstance().setVideoCaptureOrient(m_CameraOrient);
                //Toast.makeText(this, "旋转摄像头", Toast.LENGTH_SHORT).show();
                break;
            }
            case R.id.surfaceIncomingVideo:{
                Log.i(TAG, "click video");
                if(m_FunctionLayout.getVisibility() != View.VISIBLE) {
                    ShowAllUiControl();
                }
                break;
            }
            case R.id.one:{
                ProcessKeyMsg('1');
                break;
            }
            case R.id.two:{
                ProcessKeyMsg('2');
                break;
            }
            case R.id.three:
            {
                ProcessKeyMsg('3');
                break;
            }
            case R.id.four: {
                ProcessKeyMsg('4');
                break;
            }
            case R.id.five: {
                ProcessKeyMsg('5');
                break;
            }
            case R.id.six:{
                ProcessKeyMsg('6');
                break;
            }
            case R.id.seven:{
                ProcessKeyMsg('7');
                break;
            }
            case R.id.eight:{
                ProcessKeyMsg('8');
                break;
            }
            case R.id.nine:{
                ProcessKeyMsg('9');
                break;
            }
            case R.id.zero:{
                ProcessKeyMsg('0');
                break;
            }
            case R.id.star:{
                ProcessKeyMsg('*');
                break;
            }
            case R.id.pound: {
                ProcessKeyMsg('#');
                break;
            }
        }
    }

    public static InCallActivity getInstance(){
        return m_InCallActivity;
    }

    /**
     * 更新通话状态
     * @param callList
     */
    public void UpdateCallInfo(List<CallInfo> callList) {
        //Log.i(TAG, "updateCallInfo:");
    }

    /**
     * 已经接通
     * @param peerAddr
     * @param callID
     * @param accid
     */
    public void onCallConnected(String peerAddr, int callID, int accid) {
        Log.i(TAG, "onCallConnected:peerAddr=" + peerAddr+" callID="+callID+" accid="+accid);
        // 在通话接通后调用
        m_CallBegin = Calendar.getInstance();
        m_Handler.postDelayed(m_TimeTask, 10);
        m_CallInfo.setText("");
        m_CallInfo.setVisibility(View.GONE);
        EnableControl(true);
        m_Handler.postDelayed(m_UiHideTask, 3000);
    }

    /**
     * 通话结束
     */
    public void onCallRelease(int callID, int accid){
        Log.i(TAG, "onCallRelease:callID="+callID+" accid="+accid);
        if(callID != m_CallId || accid != m_AccountId) {
            Log.i(TAG, "onCallRelease:不是当前通话的挂断，不关闭界面");
            return;
        }
        new Handler().postDelayed(new Runnable(){
            public void run() {
                finish();
                m_InCallActivity = null;
            }
        }, 1000);
        m_Handler.removeCallbacks(m_TimeTask);
        m_CallInfo.setText(R.string.callend);
        m_CallInfo.setVisibility(View.VISIBLE);
    }

    public void InitVideo(boolean enableVideo) {
        m_IsVideo = enableVideo;
        Log.i(TAG, "InitVideo:m_IsVideo="+m_IsVideo);
        if(enableVideo) {
            ShowVideoWindow();
            m_Video.setCheckState(true);
            m_CameraHide.setCheckState(true);
            m_CameraOrient = GetPreviewRotateDegree(true);
            PhoneApi.getInstance().setVideoResolution(PhoneApi.VIDEO_RES.VIDEO_QVGA);// 设置视频分比率大小
            PhoneApi.getInstance().setVideoCaptureOrient(m_CameraOrient);// 摄像头旋转90度，根据实际情况设置
            PhoneApi.getInstance().setVideoDisplays(m_SurfacePreview.getHolder().getSurface(), m_SurfaceInVideo.getHolder().getSurface()); // 设置视频显示窗口
        }
    }

    /**
     * 本地预览视频
     */
    SurfaceHolder.Callback m_PreviewVideoCallback = new SurfaceHolder.Callback() {
        @Override
        public void surfaceCreated(SurfaceHolder surfaceHolder) {
            Log.i(TAG, "surfaceCreated:priview video");
        }

        @Override
        public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int w, int h) {
            Log.i(TAG, "surfaceChanged:priview video format="+format+" w="+w+" h="+h);
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
            Log.i(TAG, "surfaceDestroyed:priview video");
        }
    };

    /**
     * 对方视频
     */
    SurfaceHolder.Callback m_RemoteVideoCallback = new SurfaceHolder.Callback() {
        @Override
        public void surfaceCreated(SurfaceHolder surfaceHolder) {
            Log.i(TAG, "surfaceCreated:remote video");
        }

        @Override
        public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int w, int h) {
            Log.i(TAG, "surfaceChanged:remote video format="+format+" w="+w+" h="+h);
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
            Log.i(TAG, "surfaceDestroyed:remote video");
        }
    };

    @Override
    public void OnCheckChanged(CustomCheckBox object, boolean check) {
        ResetUiHideTask();
        switch(object.getId()) {
            case R.id.call_mute: {
                Log.i(TAG, "OnCheckChanged:call_mute="+check);
                PhoneApi.getInstance().setMicMute(check);
                break;
            }
            case R.id.call_keypad:{
                m_GroupButton1Layout.setVisibility(check?View.GONE:View.VISIBLE);
                m_GroupButton2Layout.setVisibility(check?View.GONE:View.VISIBLE);
                m_KeyPadLayout.setVisibility(check?View.VISIBLE:View.GONE);
                break;
            }
            case R.id.call_speaker:{
                if(check) OpenSpeaker();
                else CloseSpeaker();
                break;
            }
            case R.id.call_hold:{
                if(check) PhoneApi.getInstance().callHold(m_AccountId, m_CallId);
                else PhoneApi.getInstance().releaseHold(m_AccountId, m_CallId);
                break;
            }
            case R.id.call_record:{
                if(check){// 开始录音
                    String path = Environment.getExternalStorageDirectory().getPath() + "/loogear/record/";
                    String fileName = m_PhoneNumber+ "_" + System.currentTimeMillis() + ".wav";
                    m_RecordId = PhoneApi.getInstance().startRecord(path, fileName, m_AccountId, m_CallId);
                    m_RecordBegin = Calendar.getInstance();
                }
                else { // 停止录音
                    m_Record.setText(this.getString(R.string.call_record));
                    PhoneApi.getInstance().stopRecord(m_RecordId, m_AccountId, m_CallId);
                    m_RecordId = -1;
                }
                break;
            }
            case R.id.call_video:{// 点击视频按钮
                if(m_IsVideo) {
                    PhoneApi.getInstance().closeVideo();
                    HideVideoWindow();
                }
                else {
                    PhoneApi.getInstance().openVideo();
                }
                m_IsVideo = !m_IsVideo;
                break;
            }case R.id.camera_hidden:{// 显示/隐藏前置摄像头
                FrameLayout frame = (FrameLayout)findViewById(R.id.surfacePreviewCapture_frame);
                if(check) {
                    frame.setX(0);
                    PhoneApi.getInstance().showVideoPrevieWindow();
                }
                else {
                    frame.setX(-10000);
                    PhoneApi.getInstance().hideVideoPrevieWindow();
                }
                break;
            }
        }
    }

    /**
     * 恢复音频模式
     */
    private void RestoreAudioMode(){
        Log.i(TAG, "RestoreAudioMode");
        try {
            AudioManager audioManager = (AudioManager) getSystemService(AUDIO_SERVICE);
            audioManager.setMode(AudioManager.MODE_NORMAL);
            if (!audioManager.isSpeakerphoneOn()) {
                audioManager.setSpeakerphoneOn(true);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //打开扬声器
    private void OpenSpeaker() {
        try {
            AudioManager audioManager = (AudioManager) getSystemService(AUDIO_SERVICE);
            audioManager.setMode(AudioManager.MODE_IN_COMMUNICATION);
            m_CurrVolume = audioManager.getStreamVolume(AudioManager.STREAM_VOICE_CALL);
            if (!audioManager.isSpeakerphoneOn()) {
                audioManager.setSpeakerphoneOn(true);
                audioManager.setStreamVolume(AudioManager.STREAM_VOICE_CALL, audioManager.getStreamMaxVolume(AudioManager.STREAM_VOICE_CALL), AudioManager.STREAM_VOICE_CALL);
                Log.i(TAG, "OpenSpeaker");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //关闭扬声器
    private void CloseSpeaker() {
        try {
            AudioManager audioManager = (AudioManager) getSystemService(AUDIO_SERVICE);
            audioManager.setMode(AudioManager.MODE_IN_COMMUNICATION);
            if (audioManager != null) {
                if (audioManager.isSpeakerphoneOn()) {
                    audioManager.setSpeakerphoneOn(false);
                    Log.i(TAG, "CloseSpeaker");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
