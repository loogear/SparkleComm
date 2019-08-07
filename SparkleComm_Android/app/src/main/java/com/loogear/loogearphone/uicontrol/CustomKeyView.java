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

public class CustomKeyView extends LinearLayout implements View.OnTouchListener {

    public interface OnKeyDownListener {
        /**
         * 拨号键被按下事件
         * @param object
         */
        void OnKeyDown(CustomKeyView object);
    }

    private TextView m_Title;
    private TextView m_Title2;
    private OnKeyDownListener m_OnKeyDownListener;

    public CustomKeyView(Context context, AttributeSet attrs) {
        super(context, attrs);
        LayoutInflater.from(context).inflate(R.layout.custom_keyview, this, true);
        m_Title = (TextView) findViewById(R.id.txtNumber);
        m_Title2 = (TextView) findViewById(R.id.txtLetter);
        this.setOnTouchListener(this);

        this.setOrientation(LinearLayout.VERTICAL);//设置垂直排序
        //this.setGravity(Gravity.CENTER);//设置居中

        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics dm = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(dm);

        TypedArray attributes = context.obtainStyledAttributes(attrs, R.styleable.CustomControl);
        if (attributes != null) {
            // 设置标题
            String titleText = attributes.getString(R.styleable.CustomControl_title_text);
            m_Title.setText(titleText);
            String title2Text = attributes.getString(R.styleable.CustomControl_title2_text);
            m_Title2.setText(title2Text);
            attributes.recycle();
        }
    }

    public void setKeyDownListener(OnKeyDownListener listener) {
        this.m_OnKeyDownListener = listener;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN: {
                this.setBackgroundColor(Color.parseColor("#55CCCCCC"));
                if(m_OnKeyDownListener != null){
                    m_OnKeyDownListener.OnKeyDown(this);
                }
                break;
            }
            case MotionEvent.ACTION_UP: {
                this.setBackgroundColor(Color.parseColor("#FFFFFFFF"));
                break;
            }
            case MotionEvent.ACTION_MOVE:
                break;
        }
        return true;
    }

}
