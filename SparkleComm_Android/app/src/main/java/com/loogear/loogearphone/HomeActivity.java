package com.loogear.loogearphone;

import android.Manifest;
import android.app.Fragment;
import android.app.FragmentManager;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.loogear.loogearphone.uicontrol.CustomCheckBox;
import com.loogear.loogearphone.uicontrol.CustomImageView;
import com.loogear.loogearphone.uicontrol.CustomRadio;
import com.loogear.phone.CallInfo;
import com.loogear.phone.ISipEvent;
import com.loogear.phone.PhoneApi;

import java.util.List;

public class HomeActivity extends AppCompatActivity implements View.OnClickListener, CustomCheckBox.OnCheckChangedListener, CustomRadio.OnRadioClickListener,
        DialerFragment.OnFragmentInteractionListener, ContactsFragment.OnFragmentInteractionListener, MessageFragment.OnFragmentInteractionListener, MeetingFragment.OnFragmentInteractionListener {

    private final static String TAG = "HomeActivity";

    public static final int PERMISSION_RECORD_AUDIO = 1;
    public static final int PERMISSION_ACCESS_COARSE_LOCATION = 2;
    public static final int PERMISSION_WRITE_EXTERNAL_STORAGE = 3;
    public static final int PERMISSION_CALL_PHONE = 4;
    public static final int PERMISSION_CAMERA = 5;
    public static final int PERMISSION_WRITE_CONTACTS = 6;
    public static final int PERMISSION_END = 7;

    private final static int TAB_DIALER = 1; // 拨号
    private final static int TAB_CONTACTS = 2; // 联系人
    private final static int TAB_MESSAGE = 3; // 消息
    private final static int TAB_MEETING = 4; // 会议

    private CustomRadio m_TabDialer;
    private CustomRadio m_TabContacts;
    private CustomRadio m_TabMessage;
    private CustomRadio m_TabMetting;
    private CustomImageView m_Setting;

    private DialerFragment m_DialerFragment;
    private ContactsFragment m_ContactsFragment;
    private MessageFragment m_MessageFragment;
    private MeetingFragment m_MeetingFragment;
    private FragmentManager m_FragmentManager;

    private int m_RequestCode = 0; // 系统权限请求代码
    public static boolean m_IsRegistered = false; // 是否已经注册成功
    private ImageView m_OnlineImage;
    private TextView m_OnlineState;
    private TextView m_SipUrl;

    private String m_SipUser = "";
    private String m_SipPwd = "";
    private String m_SipAddr = "";
    private int m_SipPort = 5060;
    private String m_SipDomain = "";
    private boolean m_IsBackground = false;// 是否在后台

    private static HomeActivity m_HomeActivity = null;
    public static HomeActivity getInstance(){
        return m_HomeActivity;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "onCreate:");
        m_HomeActivity = this;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            Window window = getWindow();
            window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
            window.setStatusBarColor(getResources().getColor(android.R.color.white));
            window.getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);
        }

        setContentView(R.layout.activity_home);

        m_FragmentManager = getFragmentManager();

        m_TabDialer = (CustomRadio) findViewById(R.id.tab_dialer);
        m_TabContacts = (CustomRadio) findViewById(R.id.tab_contacts);
        m_TabMessage = (CustomRadio) findViewById(R.id.tab_message);
        m_TabMetting = (CustomRadio) findViewById(R.id.tab_metting);

        m_Setting = (CustomImageView) findViewById(R.id.setting);
        m_Setting.setImageClickListener(this);

        m_TabDialer.setRadioClicListener(this);
        m_TabContacts.setRadioClicListener(this);
        m_TabMessage.setRadioClicListener(this);
        m_TabMetting.setRadioClicListener(this);

        m_OnlineImage = (ImageView) findViewById(R.id.OnlineImage);
        m_OnlineState = (TextView) findViewById(R.id.OnlineState);
        m_SipUrl = (TextView) findViewById(R.id.sip_url);

        InitView();
        getSipInfo();
        if(m_SipUser.length() > 0) {// 配置了SIP账户信息时，注册SIP服务器
            RegisterSip();
        }
        else {
            m_SipUrl.setText("未添加账户");
        }
    }

    private void updateSipInfo(){
        String user = SysConfig.getSipUser(this);
        String pwd = SysConfig.getSipPwd(this);
        String addr = SysConfig.getSipAddr(this);
        int port = SysConfig.getSipPort(this);
        String domain = SysConfig.getSipDomain(this);

        if( !user.equals(m_SipUser) ||
            !pwd.equals(m_SipPwd) ||
            !addr.equals(m_SipAddr) ||
            port != m_SipPort ||
            !domain.equals(m_SipDomain) ){ // 更新了SIP账户信息时，重新注册SIP服务器
            m_SipUser = user;
            m_SipPwd = pwd;
            m_SipAddr = addr;
            m_SipPort = port;
            m_SipDomain = domain;
            Log.i(TAG, "updateSipInfo:已经修改了SIP账户信息，重新注册");

            PhoneApi.getInstance().unRegister(0);
            m_SipUrl.setText("SIP账户已更新,注册中");
            m_SipUrl.setTextColor(Color.RED);
            new Handler().postDelayed(new Runnable(){
                public void run() {
                    RegisterSip();
                }
            }, 3000);
        }
    }

    /**
     * 进入后台
     */
    public void enterBackground(){
        Log.i(TAG,"enterBackground:进入后台，注销SDK");
        if(PhoneApi.getInstance().isInCalling()){
            Log.i(TAG,"enterBackground:通话中，不能注销");
            return;
        }
        PhoneApi.getInstance().unRegister(0);
        m_IsBackground = true;
    }

    /**
     * 退出后台
     */
    public void exitBackground(){
        if(m_IsBackground){// 重新注册SDK
            Log.i(TAG,"exitBackground:进入前台");
            m_IsBackground = false;
            getSipInfo();
            if(m_SipUser.length() > 0) {// 配置了SIP账户信息时，注册SIP服务器
                RegisterSip();
            }
            else {
                m_SipUrl.setText("未添加账户");
            }
        }
    }

    /**
     * 初始化界面
     */
    private void InitView(){
        Log.i(TAG, "InitView:");
        m_TabDialer.setCheckState(true);// 启动时，默认打开拨号界面
        SetFragment(TAB_DIALER);
    }

    @Override
    protected void onStart() {
        super.onStart();
        checkPermission(PERMISSION_RECORD_AUDIO);
        Log.i(TAG, "onStart:");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "onDestroy:");
        PhoneApi.getInstance().unRegister(0);
        m_HomeActivity = null;
        finish();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i(TAG, "onResume:");
        updateSipInfo();
    }

    private void SetFragment(int type) {
        //开启一个事务
        FragmentTransaction beginTransaction = getSupportFragmentManager().beginTransaction();

        if (m_DialerFragment != null) {
            beginTransaction.hide(m_DialerFragment);
        }
        if (m_ContactsFragment != null) {
            beginTransaction.hide(m_ContactsFragment);
        }
        if (m_MessageFragment != null) {
            beginTransaction.hide(m_MessageFragment);
        }
        if (m_MeetingFragment != null) {
            beginTransaction.hide(m_MeetingFragment);
        }

        switch (type) {
            case TAB_DIALER:
                if (m_DialerFragment == null) {
                    m_DialerFragment = new DialerFragment();
                    //加入事物
                    beginTransaction.add(R.id.content, m_DialerFragment);
                } else {
                    //否则就显示
                    beginTransaction.show(m_DialerFragment);
                }
                break;
            case TAB_CONTACTS:
                if (m_ContactsFragment == null) {
                    m_ContactsFragment = new ContactsFragment();
                    //加入事物
                    beginTransaction.add(R.id.content, m_ContactsFragment);
                } else {
                    //否则就显示
                    beginTransaction.show(m_ContactsFragment);
                }
                break;
            case TAB_MESSAGE:
                if (m_MessageFragment == null) {
                    m_MessageFragment = new MessageFragment();
                    //加入事物
                    beginTransaction.add(R.id.content, m_MessageFragment);
                } else {
                    //否则就显示
                    beginTransaction.show(m_MessageFragment);
                }
                break;
            case TAB_MEETING:
                if (m_MeetingFragment == null) {
                    m_MeetingFragment = new MeetingFragment();
                    //加入事物
                    beginTransaction.add(R.id.content, m_MeetingFragment);
                } else {
                    //否则就显示
                    beginTransaction.show(m_MeetingFragment);
                }
                break;
        }
        //执行
        beginTransaction.commit();
    }

    /**
     * 获取SIP服务器信息
     */
    private void getSipInfo(){
        m_SipUser = SysConfig.getSipUser(this);
        m_SipPwd = SysConfig.getSipPwd(this);
        m_SipAddr = SysConfig.getSipAddr(this);
        m_SipPort = SysConfig.getSipPort(this);
        m_SipDomain = SysConfig.getSipDomain(this);
    }

    @Override
    public void onClick(View v) {
        switch(v.getId()){
            case R.id.setting:{// 设置

                Intent intent = new Intent();
                intent.setClass(HomeActivity.this, SettingPreferenceActivity.class);
                startActivity(intent);

                break;
            }
        }
    }

    @Override
    public void OnCheckChanged(CustomCheckBox object, boolean check) {

    }

    @Override
    public void OnRadioClick(CustomRadio object, boolean click) {
        switch(object.getId()) {
            case R.id.tab_dialer: {
                Log.i(TAG, "OnRadioClick:Dialer");
                m_TabContacts.setCheckState(false);
                m_TabMessage.setCheckState(false);
                m_TabMetting.setCheckState(false);
                SetFragment(TAB_DIALER);
                break;
            }
            case R.id.tab_contacts: {
                Log.i(TAG, "OnRadioClick:Contacts");
                m_TabDialer.setCheckState(false);
                m_TabMessage.setCheckState(false);
                m_TabMetting.setCheckState(false);
                SetFragment(TAB_CONTACTS);
                break;
            }
            case R.id.tab_message: {
                Log.i(TAG, "OnRadioClick:Message");
                m_TabDialer.setCheckState(false);
                m_TabContacts.setCheckState(false);
                m_TabMetting.setCheckState(false);
                SetFragment(TAB_MESSAGE);
                break;
            }
            case R.id.tab_metting: {
                Log.i(TAG, "OnRadioClick:Metting");
                m_TabDialer.setCheckState(false);
                m_TabContacts.setCheckState(false);
                m_TabMessage.setCheckState(false);
                SetFragment(TAB_MEETING);
                break;
            }
        }
    }

    @Override
    public void onFragmentInteraction(Uri uri) {
        Log.i(TAG, "onFragmentInteraction:"+uri);
    }

    /**
     * 检查权限是否打开
     */
    private void checkPermission(int requestCode) {
        //检查系统是否开启了录音权限;
        if (requestCode <= PERMISSION_RECORD_AUDIO && ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
            m_RequestCode = PERMISSION_RECORD_AUDIO + 1;
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO}, requestCode);
            return;
        }
        //检查系统是否开启了地理位置权限;
        if (requestCode <= PERMISSION_ACCESS_COARSE_LOCATION && ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            m_RequestCode = PERMISSION_ACCESS_COARSE_LOCATION + 1;
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, requestCode);
            return;
        }
        //检查系统是否开启了外部存储权限;
        if (requestCode <= PERMISSION_WRITE_EXTERNAL_STORAGE && ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            m_RequestCode = PERMISSION_WRITE_EXTERNAL_STORAGE + 1;
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, requestCode);
            return;
        }
        //检查系统是否开启了电话权限;
        if (requestCode <= PERMISSION_CALL_PHONE && ContextCompat.checkSelfPermission(this, Manifest.permission.CALL_PHONE) != PackageManager.PERMISSION_GRANTED) {
            m_RequestCode = PERMISSION_CALL_PHONE + 1;
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CALL_PHONE}, requestCode);
            return;
        }
        //检查系统是否开启了相机权限;
        if (requestCode <= PERMISSION_CAMERA && ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            m_RequestCode = PERMISSION_CAMERA + 1;
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, requestCode);
            return;
        }
        //检查系统是否开启了通讯录权限;
        if (requestCode <= PERMISSION_WRITE_CONTACTS && ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_CONTACTS) != PackageManager.PERMISSION_GRANTED) {
            m_RequestCode = PERMISSION_WRITE_CONTACTS + 1;
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_CONTACTS}, requestCode);
            return;
        }
        m_RequestCode = PERMISSION_END;
        return;
    }

    /*
     * requestPermissions方法执行后的回调方法
     * requestCode:相当于一个标志，
     * permissions：需要传进的permission，不能为空
     * grantResults：用户进行操作之后，或同意或拒绝回调的传进的两个参数;
     * */
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        //这里实现用户操作，或同意或拒绝的逻辑
        Log.i(TAG, "requestCode="+requestCode+" grantResults="+grantResults.toString());
        /*if(grantResults.length > 0 && grantResults.length > 0) */{
            if (grantResults[0] == android.content.pm.PackageManager.PERMISSION_DENIED && permissions[0].equals(Manifest.permission.RECORD_AUDIO)) {
                Toast.makeText(this, R.string.permissions_error_record, Toast.LENGTH_LONG).show();
            } else if (grantResults[0] == android.content.pm.PackageManager.PERMISSION_DENIED && permissions[0].equals(Manifest.permission.ACCESS_COARSE_LOCATION)) {
                Toast.makeText(this, R.string.permissions_error_location, Toast.LENGTH_LONG).show();
            } else if (grantResults[0] == android.content.pm.PackageManager.PERMISSION_DENIED && permissions[0].equals(Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                Toast.makeText(this, R.string.permissions_error_storage, Toast.LENGTH_LONG).show();
            } else if (grantResults[0] == android.content.pm.PackageManager.PERMISSION_DENIED && permissions[0].equals(Manifest.permission.CALL_PHONE)) {
                Toast.makeText(this, R.string.permissions_error_phone, Toast.LENGTH_LONG).show();
            } else if (grantResults[0] == android.content.pm.PackageManager.PERMISSION_DENIED && permissions[0].equals(Manifest.permission.CAMERA)) {
                Toast.makeText(this, R.string.permissions_error_camera, Toast.LENGTH_LONG).show();
            } else if (grantResults[0] == android.content.pm.PackageManager.PERMISSION_DENIED && permissions[0].equals(Manifest.permission.WRITE_CONTACTS)) {
                Toast.makeText(this, R.string.permissions_error_contacts, Toast.LENGTH_LONG).show();
            }
        }

        if(m_RequestCode < PERMISSION_END) {
            checkPermission(m_RequestCode);
        }
    }

    /**
     * 注册SIP服务器
     */
    private void RegisterSip() {
        // 初始化SDK
        PhoneApi.getInstance().initSdk(this, new HomeActivity.MySipEvent());
        PhoneApi.getInstance().registerToSipServer(m_SipAddr, m_SipPort, m_SipUser, m_SipPwd,0,m_SipDomain,false,"",false,"");
        m_SipUrl.setText(m_SipUser+"@"+m_SipDomain+":"+m_SipPort);
        m_SipUrl.setTextColor(Color.parseColor("#76000000"));
        //m_SdkApi.registerToSipServer("as1.loogear.com","5960","1999999","11111","0","loogear.com","false","","false","");
        //m_SdkApi.registerToSipServer("192.168.0.247","5060","1004","123456","0","192.168.0.247","false","","false","");
        //PhoneApi.getInstance().registerToSipServer("192.168.0.67",5060,"99009860113","832D8DE3D5D34DA69B1CE48089D2F0C4",0,"192.168.0.67",false,"",false,"");
        //PhoneApi.getInstance().registerToSipServer("192.168.0.247",5060,"1019","123456",0,"192.168.0.247",false,"",false,"");
        //PhoneApi.getInstance().registerToSipServer("as1.loogear.com", 5960, "99009860022", "3B98F93B4DB52452BDFFF1422ADE755C", 0, "loogear.com", false, "", false,"X-SparkleComm-Call-DEVICE-ID:xxxxxxxxxxxxxxxx");
    }

    /**
     * SIP 消息回调，属于主线程
     */
    private class MySipEvent implements ISipEvent {
        @Override
        public void onSipConnected(int accid) { // 连接上SIP服务器
            Log.i(TAG, "onSipConnected:"+" Thread: "+android.os.Process.myTid()+" name "+Thread.currentThread().getName());
            // 更新状态
            m_OnlineImage.setImageResource(R.drawable.state_online);
            m_OnlineState.setText(R.string.online);
            m_IsRegistered = true;
        }

        @Override
        public void onSipDisconnected(int accid) {// SIP服务器连接断开
            Log.i(TAG, "onSipDisconnected:accid=" + accid);
            m_OnlineImage.setImageResource(R.drawable.state_offline);
            m_OnlineState.setText(R.string.offline);
            m_IsRegistered = false;
        }

        @Override
        public void onSignalReport(int Strength, int accid) {// 信号强度报告 信号强度值0~5,数值越低说明信号强度越差，0为断开
            Log.i(TAG, "onSignalReport:Strength=" + Strength+" accid="+accid);
        }

        @Override
        public void onCallConnectEarly(String peerAddr, int callID, int accid) { // 通话连接后，准备进行媒体协商
            Log.i(TAG, "onCallConnectEarly:peerAddr=" + peerAddr+" callID="+callID+" accid="+accid);
        }

        @Override
        public void onCallConnectMedia(String peerAddr, MediaType mediaType, int callID, int accid) { // 通话已经连接成功
            Log.i(TAG, "onCallConnectMedia:peerAddr=" + peerAddr+" callID="+callID+" accid="+accid+" mediaType="+mediaType);
            boolean enableVideo = false;
            if(mediaType == MediaType.ONLY_VIDEO || mediaType == MediaType.VIDEO_VOICE) enableVideo = true;
            if(InCallActivity.getInstance() != null) {
                InCallActivity.getInstance().InitVideo(enableVideo);// 初始化视频窗口
            }
        }

        @Override
        public void onCallConnected(String peerAddr, int callID, int accid) { // 已接通，通话连接后的媒体协商已经成功
            Log.i(TAG, "onCallConnected:peerAddr=" + peerAddr+" callID="+callID+" accid="+accid);
            if(InCallActivity.getInstance() != null) {
                InCallActivity.getInstance().onCallConnected(peerAddr, callID, accid);
            }
        }

        @Override
        public void onCallRelease(String peerAddr, int callID, int causeCode, String reasonText, int accid) { // 通话已经释放
            Log.i(TAG, "onCallRelease:peerAddr=" + peerAddr+" callID="+callID+" accid="+accid+" causeCode="+causeCode+" reasonText="+reasonText);
            IncomingActivity.onCallRelease(callID, accid);
            if(InCallActivity.getInstance() != null) {
                InCallActivity.getInstance().onCallRelease(callID, accid);
            }
        }

        @Override
        public void onCallIncoming(String peerAddr, int callID, int accid) {// 来电消息
            String message = peerAddr;
            String number = message;
            int index = message.indexOf("<");
            int index1 = message.indexOf("@");
            number = message.substring(index + 5, index1);

            Log.i(TAG, "onCallIncoming:peerAddr=" + peerAddr+" callID="+callID+" accid="+accid+" number="+number);

            // 打开来电界面
            Intent intent = new Intent();
            intent.setClass(HomeActivity.this, IncomingActivity.class);
            intent.putExtra("number", number);// 来电号码
            intent.putExtra("callid", callID);// 通话ID
            intent.putExtra("accid", accid);// 账户ID
            startActivity(intent);
        }

        @Override
        public void onUpdateCallInfo(List<CallInfo> callList) {
            int i = 0;
            /*for(CallInfo call: callList) {
                Log.i(TAG, "onUpdateCallInfo:i="+i+" remote addr="+call.getUrl()+" number="+call.getSipNo()+" AccountId="+call.getAccountId()+" CallId="+
                        call.getCallId()+" Role="+call.getRole().toString()+" State="+call.getState().toString());
                i++;
            }*/
            if(InCallActivity.getInstance() != null) {
                InCallActivity.getInstance().UpdateCallInfo(callList);
            }
        }
    }
}
