package com.loogear.loogearphone.uicontrol;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.loogear.loogearphone.R;

public class CustomKeyPad extends LinearLayout implements View.OnTouchListener, CustomKeyView.OnKeyDownListener, View.OnClickListener, View.OnLongClickListener {

    public interface OnCustomKeyPadListener {
        /**
         * 拨号键被按下事件
         * @param key
         */
        void OnKeyPadDown(String key);

        /**
         * 拨打电话
         * @param number
         * @param isVideo
         */
        void OnMakeCall(String number, boolean isVideo);
    }

    private TextView m_Phone;
    private CustomImageView m_Delete;
    private CustomImageView m_CallVideo;
    private CustomImageView m_CallVoice;
    private OnCustomKeyPadListener m_OnCustomKeyPadListener;
    private LinearLayout m_HeadLayout;

    public CustomKeyPad(Context context, AttributeSet attrs) {
        super(context, attrs);
        LayoutInflater.from(context).inflate(R.layout.custom_keypad, this, true);
        m_Phone = (TextView) findViewById(R.id.txtPhone);
        m_Delete = (CustomImageView) findViewById(R.id.delete);
        m_CallVideo = (CustomImageView) findViewById(R.id.call_video);
        m_CallVoice = (CustomImageView) findViewById(R.id.call_voice);
        m_HeadLayout = (LinearLayout) findViewById(R.id.headLayout);

        m_Phone.setText("");
        m_HeadLayout.setVisibility(View.GONE);

        m_Delete.setImageClickListener(this);
        m_Delete.setImageLongClickListener(this);
        m_CallVideo.setImageClickListener(this);
        m_CallVoice.setImageClickListener(this);

        ((CustomKeyView) findViewById(R.id.one)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.two)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.three)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.four)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.five)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.six)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.seven)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.eight)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.nine)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.zero)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.star)).setKeyDownListener(this);
        ((CustomKeyView) findViewById(R.id.pound)).setKeyDownListener(this);

        this.setOnTouchListener(this);

        this.setOrientation(LinearLayout.VERTICAL);//设置垂直排序

        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics dm = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(dm);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN: {

                break;
            }
            case MotionEvent.ACTION_UP: {

                break;
            }
            case MotionEvent.ACTION_MOVE:
                break;
        }
        return true;
    }

    @Override
    public void OnKeyDown(CustomKeyView object) {
        String key = "";
        switch (object.getId()){
            case R.id.one:{
                key = "1";
                break;
            }
            case R.id.two:{
                key = "2";
                break;
            }
            case R.id.three:{
                key = "3";
                break;
            }
            case R.id.four:{
                key = "4";
                break;
            }
            case R.id.five:{
                key = "5";
                break;
            }
            case R.id.six:{
                key = "6";
                break;
            }
            case R.id.seven:{
                key = "7";
                break;
            }
            case R.id.eight:{
                key = "8";
                break;
            }
            case R.id.nine:{
                key = "9";
                break;
            }
            case R.id.zero:{
                key = "0";
                break;
            }
            case R.id.star:{
                key = "*";
                break;
            }
            case R.id.pound:{
                key = "#";
                break;
            }
        }

        m_Phone.setText(m_Phone.getText() + key);
        if(getPhoneNumber().length() > 0) {
            updateHeadLayout();
        }
        if(getPhoneNumber().length() > 0 && m_OnCustomKeyPadListener != null) {
            // 处理最后一个按键回调
            m_OnCustomKeyPadListener.OnKeyPadDown(key);
        }
    }

    /**
     * 更新电话号码和删除按钮界面
     */
    private void updateHeadLayout(){
        if(getPhoneNumber().length() > 0 && m_HeadLayout.getVisibility() != View.VISIBLE) {
            m_HeadLayout.setVisibility(View.VISIBLE);
        }
        else if(getPhoneNumber().length() == 0 && m_HeadLayout.getVisibility() != View.GONE) {
            m_HeadLayout.setVisibility(View.GONE);
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.delete:{//删除按键
                String text = m_Phone.getText().toString();
                if(text.length() > 0) {
                    m_Phone.setText(text.substring(0, text.length()-1));
                    if(getPhoneNumber().length() == 0) {
                        updateHeadLayout();
                    }
                }
                break;
            }
            case R.id.call_video:{// 视频拨号
                if(getPhoneNumber().length() > 0 && m_OnCustomKeyPadListener != null) {
                    m_OnCustomKeyPadListener.OnMakeCall(getPhoneNumber(), true);
                }
                break;
            }
            case R.id.call_voice:{// 语音拨号
                if(getPhoneNumber().length() > 0 && m_OnCustomKeyPadListener != null) {
                    m_OnCustomKeyPadListener.OnMakeCall(getPhoneNumber(), false);
                }
                break;
            }
        }
    }

    /**
     * 获取号码
     * @return
     */
    public String getPhoneNumber(){
        return m_Phone.getText().toString();
    }

    @Override
    public boolean onLongClick(View v) {
        switch (v.getId()){
            case R.id.delete:{
                m_Phone.setText("");
                updateHeadLayout();
                break;
            }
        }
        return false;
    }

    public void setOnCustomKeyPadListener(OnCustomKeyPadListener listener){
        this.m_OnCustomKeyPadListener = listener;
    }
}
