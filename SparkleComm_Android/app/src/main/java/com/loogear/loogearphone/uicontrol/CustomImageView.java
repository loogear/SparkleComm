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

public class CustomImageView extends LinearLayout implements View.OnTouchListener {
    private int m_ImageId = -1;
    private int m_Image2Id = -1;
    private ImageView m_Image;
    private TextView m_Title;
    private int m_TitleTextColor;

    public CustomImageView(Context context, AttributeSet attrs) {
        super(context, attrs);
        LayoutInflater.from(context).inflate(R.layout.custom_imageview, this, true);
        m_Image = (ImageView) findViewById(R.id.imgHead);
        m_Title = (TextView) findViewById(R.id.txtTitle);
        m_Image.setId(this.getId());
        m_Image.setOnTouchListener(this);

        this.setOrientation(LinearLayout.VERTICAL);//设置垂直排序
        //this.setGravity(Gravity.CENTER);//设置居中

        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics dm = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(dm);

        TypedArray attributes = context.obtainStyledAttributes(attrs, R.styleable.CustomControl);
        if (attributes != null) {
            m_ImageId = attributes.getResourceId(R.styleable.CustomControl_image_drawable, -1); // 正常显示的图片
            m_Image2Id = attributes.getResourceId(R.styleable.CustomControl_image_drawable2, -1);//按下显示的图片
            if(m_ImageId != -1) {
                m_Image.setImageResource(m_ImageId);

                int image_width = attributes.getInt(R.styleable.CustomControl_image_width, 80);
                int image_height = attributes.getInt(R.styleable.CustomControl_image_height, 80);
                LinearLayout.LayoutParams params = new LinearLayout.LayoutParams((int)(image_width*dm.density), (int)(image_height*dm.density));
                m_Image.setLayoutParams(params);// 更新图片的显示尺寸
            }

            // 设置标题
            String titleText = attributes.getString(R.styleable.CustomControl_title_text);
            m_Title.setText(titleText);
            //标题显示颜色
            m_TitleTextColor = attributes.getColor(R.styleable.CustomControl_title_text_color, Color.WHITE);
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

    public void setImageClickListener(OnClickListener onClickListener) {
        if (onClickListener != null) {
            m_Image.setOnClickListener(onClickListener);
        }
    }

    public void setImageLongClickListener(OnLongClickListener onLongClickListener) {
        if (onLongClickListener != null) {
            m_Image.setOnLongClickListener(onLongClickListener);
        }
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN: {
                if (v.getId() == m_Image.getId()) {
                    if (m_Image2Id > 0) m_Image.setImageResource(m_Image2Id);
                    else m_Image.setColorFilter(Color.GRAY, PorterDuff.Mode.MULTIPLY);
                }
                break;
            }
            case MotionEvent.ACTION_UP: {
                if (v.getId() == m_Image.getId() && m_ImageId > 0) {
                    m_Image.setImageResource(m_ImageId);
                    m_Image.clearColorFilter();
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

    @Override
    public void setEnabled(boolean enabled) {
        super.setEnabled(enabled);

        if(enabled) {
            m_Image.clearColorFilter();
            m_Title.setTextColor(m_TitleTextColor);
        }
        else {
            m_Image.setColorFilter(Color.GRAY, PorterDuff.Mode.MULTIPLY);
            m_Title.setTextColor(Color.GRAY);
        }
    }
}
