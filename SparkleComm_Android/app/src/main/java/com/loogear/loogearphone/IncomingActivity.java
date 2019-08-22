package com.loogear.loogearphone;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.media.AudioManager;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Build;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import com.loogear.phone.PhoneApi;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;


public class IncomingActivity extends AppCompatActivity implements View.OnClickListener, View.OnTouchListener{
    private final static String TAG = "IncomingActivity";
    private TextView m_PhoneNumber;
    private TextView m_Info;
    private ImageView m_Answer;
    private ImageView m_Reject;
    private String m_Number = ""; // 来电号码
    private int m_CallId = 0;// 来电ID
    private int m_AccountId = 0;// 账户ID
    private Ringtone m_Ringtone;
    private static IncomingActivity m_IncomingActivity = null;
    private Map<Integer,IncomingInfo> IncomingMap = new HashMap<Integer, IncomingInfo>(); // 所有来电信息

    /**
     * 来电信息
     */
    private class IncomingInfo {
        /**
         * 来电号码
         */
        public String Number = "";

        /**
         * 来电ID
         */
        public int CallId = 0;

        /**
         * 账户ID
         */
        public int AccountId = 0;
    }

    /**
     * 来电中时，有挂断消息时
     */
    public static void onCallRelease(int callID, int accid){
        Log.i(IncomingActivity.TAG, "onCallRelease:callID="+callID+" accid="+accid);
        if(m_IncomingActivity != null)
        {
            Log.i(IncomingActivity.TAG, "onCallRelease:2");
            m_IncomingActivity.doCallReleaseEvent(callID, accid);
        }
    }

    /**
     * 处理来电挂断事件
     * @param callID
     * @param accid
     */
    private void doCallReleaseEvent(int callID, int accid){
        Log.i(IncomingActivity.TAG, "doCallReleaseEvent");
        IncomingMap.remove(Integer.valueOf(callID));
        if(IncomingMap.size() == 0) { // 全部来电都挂断了，关闭界面
            m_IncomingActivity.finish();
            m_IncomingActivity = null;
        }
        else {// 更新来电显示界面
            Set<Integer> keys = IncomingMap.keySet();
            Iterator<Integer> iterator = keys.iterator();
            while (iterator.hasNext()) {// 获取下一个来电信息，并显示
                Integer key = iterator.next();
                IncomingInfo info = IncomingMap.get(key);
                updateInfo(info);
                break;
            }
        }
    }

    /**
     * 更新来电界面显示信息
     * @param info
     */
    private void updateInfo(IncomingInfo info) {
        m_Number = info.Number;
        m_CallId = info.CallId;
        m_AccountId = info.AccountId;
        m_PhoneNumber.setText(m_Number);
        String text = this.getText(R.string.call_incoming).toString();
        if(IncomingMap.size() > 1) {
            text = text + "("+IncomingMap.size()+")";
        }
        m_Info.setText(text);
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

        setContentView(R.layout.activity_incoming);
        Log.i(TAG, "onCreate");
        m_IncomingActivity = this;
        m_PhoneNumber = (TextView) findViewById(R.id.txtPhoneNumber);
        m_Answer = (ImageView) findViewById(R.id.imgAnswer);
        m_Reject = (ImageView) findViewById(R.id.imgHangup);
        m_Info = (TextView) findViewById(R.id.txtInfo);
        m_Answer.setOnClickListener(this);
        m_Answer.setOnTouchListener(this);
        m_Reject.setOnClickListener(this);
        m_Reject.setOnTouchListener(this);

        processExtraData();
        playRing();
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);//must store the new intent unless getIntent() will return the old one
        processExtraData();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "onDestroy");
        m_IncomingActivity = null;
        stopRing();
    }

    /**
     * 处理来电，可以收到多个来电信息
     */
    private void processExtraData(){
        Intent intent = getIntent();

        // 保存来电信息
        IncomingInfo info = new IncomingInfo();
        info.Number = intent.getStringExtra("number");
        info.CallId = intent.getIntExtra("callid", 0);
        info.AccountId = intent.getIntExtra("accid", 0);
        IncomingMap.put(Integer.valueOf(info.CallId), info);
        updateInfo(info);
    }

    /**
     * 恢复音频模式
     */
    private void SetNormalAudioMode(){
        Log.i(TAG, "SetNormalAudioMode");
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

    /**
     * 铃声模式
     */
    private void SetRingAudioMode(){
        Log.i(TAG, "SetRingAudioMode");
        try {
            AudioManager audioManager = (AudioManager) getSystemService(AUDIO_SERVICE);
            audioManager.setMode(AudioManager.MODE_RINGTONE);
            if (!audioManager.isSpeakerphoneOn()) {
                audioManager.setSpeakerphoneOn(true);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 播放铃声
     */
    private void playRing() {
        SetRingAudioMode();
        Uri notification = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
        m_Ringtone = RingtoneManager.getRingtone(this, notification);
        m_Ringtone.play();
    }

    /**
     * 停止播放铃声
     */
    private void stopRing() {
        if(m_Ringtone.isPlaying()){
            m_Ringtone.stop();;
        }
    }

    @Override
    public void onClick(View view) {
        switch(view.getId()) {
            case R.id.imgAnswer: {// 接听
                m_IncomingActivity = null; // 先置为null, 避免处理onCallRelease消息

                Set<Integer> keys = IncomingMap.keySet();
                Iterator<Integer> iterator = keys.iterator();
                while (iterator.hasNext()) {// 把其他所有来电都拒绝，只接听一个
                    Integer key = iterator.next();
                    IncomingInfo info = IncomingMap.get(key);
                    if(info.CallId != m_CallId) {
                        PhoneApi.getInstance().releaseCall(info.CallId, info.AccountId);
                    }
                }

                Intent intent = new Intent();
                intent.setClass(this, InCallActivity.class);
                intent.putExtra("number", m_PhoneNumber.getText().toString());// 电话号码
                intent.putExtra("video", false); // 是否视频电话
                intent.putExtra("incoming", true); // true=来电; false=去电
                intent.putExtra("callid", m_CallId); // 来电callid
                intent.putExtra("accountid", m_AccountId); // 账户ID
                startActivity(intent);

                finish();
                break;
            }
            case R.id.imgHangup: { // 拒绝
                PhoneApi.getInstance().releaseCall(m_CallId, m_AccountId);
				SetNormalAudioMode();
                break;
            }
        }
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                if(v.getId() == R.id.imgAnswer || v.getId() == R.id.imgHangup) {
                    ImageView image = (ImageView)v;
                    image.setColorFilter(Color.GRAY, PorterDuff.Mode.MULTIPLY);
                }
                break;
            case MotionEvent.ACTION_UP:
                if(v.getId() == R.id.imgAnswer || v.getId() == R.id.imgHangup) {
                    ImageView image = (ImageView)v;
                    image.clearColorFilter();
                }
                break;
            case MotionEvent.ACTION_MOVE:
                break;
        }
        return false;
    }
}
