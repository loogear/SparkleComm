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

public class CustomRadio extends LinearLayout implements View.OnTouchListener, View.OnClickListener {
    private int m_ImageCheckedId = -1;
    private int m_ImageUnCheckedId = -1;
    private ImageView m_Image;
    private TextView m_Title;
    private boolean m_Checked = false;
    private int m_TitleTextColor;
    private int m_TitleTextCheckedColor;
    private OnRadioClickListener m_OnRadioClickListener = null;

    public interface OnRadioClickListener {
        /**
         * 选中状态改变
         * @param object
         * @param click
         */
        void OnRadioClick(CustomRadio object, boolean click);
    }

    public CustomRadio(Context context, AttributeSet attrs) {
        super(context, attrs);
        LayoutInflater.from(context).inflate(R.layout.custom_radio, this, true);
        m_Image = (ImageView) findViewById(R.id.imgHead);
        m_Title = (TextView) findViewById(R.id.txtTitle);
        m_Image.setId(this.getId());
        this.setOnTouchListener(this);
        this.setOnClickListener(this);

        this.setOrientation(LinearLayout.VERTICAL);//设置垂直排序
        //this.setGravity(Gravity.CENTER);//设置居中

        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics dm = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(dm);

        TypedArray attributes = context.obtainStyledAttributes(attrs, R.styleable.CustomControl);
        if (attributes != null) {
            m_Checked = attributes.getBoolean(R.styleable.CustomControl_checked, false);

            m_ImageCheckedId = attributes.getResourceId(R.styleable.CustomControl_checked_image, -1); // 被选中显示的图片
            m_ImageUnCheckedId = attributes.getResourceId(R.styleable.CustomControl_unchecked_image, -1);//未选中显示的图片
            if(m_Checked && m_ImageCheckedId != -1) {
                m_Image.setImageResource(m_ImageCheckedId);
            }
            else if(!m_Checked && m_ImageUnCheckedId != -1) {
                m_Image.setImageResource(m_ImageUnCheckedId);
            }

            int image_width = attributes.getInt(R.styleable.CustomControl_image_width, 80);
            int image_height = attributes.getInt(R.styleable.CustomControl_image_height, 80);
            LinearLayout.LayoutParams params = new LinearLayout.LayoutParams((int)(image_width*dm.density), (int)(image_height*dm.density));
            m_Image.setLayoutParams(params);// 更新图片的显示尺寸

            // 设置标题
            String titleText = attributes.getString(R.styleable.CustomControl_title_text);
            m_Title.setText(titleText);
            //标题显示颜色
            m_TitleTextColor = attributes.getColor(R.styleable.CustomControl_title_text_color, Color.WHITE);
            m_TitleTextCheckedColor = attributes.getColor(R.styleable.CustomControl_checked_text_color, m_TitleTextColor);
            m_Title.setTextColor(m_TitleTextColor);

            //
            int top = attributes.getInt(R.styleable.CustomControl_title_text_margin_top, 0);
            LinearLayout.LayoutParams layoutParam = new LinearLayout.LayoutParams( m_Title.getLayoutParams());
            layoutParam.setMargins(0, (int)(dm.density*top), 0, 0);
            m_Title.setLayoutParams(layoutParam);

            // 设置字体大小
            int titleTextSize = attributes.getColor(R.styleable.CustomControl_title_text_fontsize, 17);
            m_Title.setTextSize(TypedValue.COMPLEX_UNIT_SP, titleTextSize);

            attributes.recycle();
        }
    }

    /**
     * 设置监听器
     * @param listener
     */
    public void setRadioClicListener(OnRadioClickListener listener) {
        m_OnRadioClickListener = listener;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN: {
                m_Image.setColorFilter(Color.GRAY, PorterDuff.Mode.MULTIPLY);
                m_Title.setTextColor(m_TitleTextCheckedColor);
                break;
            }
            case MotionEvent.ACTION_UP: {
                m_Image.clearColorFilter();
                if(m_Checked) {
                    m_Title.setTextColor(m_TitleTextCheckedColor);
                }
                else {
                    m_Title.setTextColor(m_TitleTextColor);
                }
                break;
            }
            case MotionEvent.ACTION_MOVE:
                break;
        }
        return false;
    }

    public ImageView getImageView() {
        return m_Image;
    }

    public TextView getTitleBarTitle() {
        return m_Title;
    }

    public void setText(String text) {
        m_Title.setText(text);
    }

    @Override
    public void onClick(View v) {
        if(!m_Checked) { // 未选中时，才产生点击事件
            m_Checked = !m_Checked;
            if (m_Checked) {
                m_Title.setTextColor(m_TitleTextCheckedColor);
                m_Image.setImageResource(m_ImageCheckedId);
            } else {
                m_Title.setTextColor(m_TitleTextColor);
                m_Image.setImageResource(m_ImageUnCheckedId);
            }
            if (m_OnRadioClickListener != null) {
                m_OnRadioClickListener.OnRadioClick(this, m_Checked);
            }
        }
    }

    /**
     * 获取选中状态
     * @return
     */
    public boolean getCheckState() {
        return m_Checked;
    }

    /**
     * 设置是否选中状态
     * @param checked
     */
    public void setCheckState(boolean checked){
        m_Checked = checked;
        if(m_Checked) {
            m_Title.setTextColor(m_TitleTextCheckedColor);
            m_Image.setImageResource(m_ImageCheckedId);
        }
        else {
            m_Title.setTextColor(m_TitleTextColor);
            m_Image.setImageResource(m_ImageUnCheckedId);
        }
    }

    @Override
    public void setEnabled(boolean enabled) {
        super.setEnabled(enabled);

        if(enabled) {
            m_Image.clearColorFilter();

            if(m_Checked) {
                m_Title.setTextColor(m_TitleTextCheckedColor);
            }
            else {
                m_Title.setTextColor(m_TitleTextColor);
            }
        }
        else {
            m_Image.setColorFilter(Color.GRAY, PorterDuff.Mode.MULTIPLY);
            m_Title.setTextColor(Color.GRAY);
        }
    }
}
