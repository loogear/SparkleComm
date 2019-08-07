package com.loogear.loogearphone;


import android.app.Activity;
import android.app.Application;
import android.os.Bundle;
import android.util.Log;

import com.loogear.phone.PhoneApi;

public class LoogearApplication extends Application {
    private final String TAG = "LoogearApplication";
    private static LoogearApplication m_LoogearApplication;
    private int m_ActivityCount = 0;
    private boolean m_IsBackground = false;
    @Override
    public void onCreate() {
        super.onCreate();

        m_LoogearApplication = new LoogearApplication();

        registerActivityLifecycleCallbacks(new ActivityLifecycleCallbacks() {
            @Override
            public void onActivityCreated(Activity activity, Bundle savedInstanceState) {
                Log.d(TAG,"onActivityCreated");
            }

            @Override
            public void onActivityStarted(Activity activity) {
                m_ActivityCount++;
                Log.d(TAG,"onActivityStarted:count="+m_ActivityCount);
                if(m_IsBackground){
                    m_IsBackground = false;
                    Log.d(TAG,"onActivityStarted:恢复到前台");
                    if(HomeActivity.getInstance() != null) {
                        HomeActivity.getInstance().exitBackground();
                    }
                }
            }

            @Override
            public void onActivityResumed(Activity activity) {
                Log.d(TAG,"onActivityResumed");
            }

            @Override
            public void onActivityPaused(Activity activity) {
                Log.d(TAG,"onActivityPaused");
            }

            @Override
            public void onActivityStopped(Activity activity) {
                m_ActivityCount--;
                Log.d(TAG,"onActivityStopped:count="+m_ActivityCount);
                if(m_ActivityCount == 0) { // APP 进入到后台，注销SDK
                    Log.d(TAG,"onActivityStopped:进入到后台");
                    m_IsBackground = true;
                    if(HomeActivity.getInstance() != null){
                        HomeActivity.getInstance().enterBackground();
                    }
                }
            }

            @Override
            public void onActivitySaveInstanceState(Activity activity, Bundle outState) {
                Log.d(TAG,"onActivitySaveInstanceState");
            }

            @Override
            public void onActivityDestroyed(Activity activity) {
                Log.d(TAG,"onActivityDestroyed");
            }
        });
    }

    public static LoogearApplication getInstance( ) {
        if (null == m_LoogearApplication) {
            m_LoogearApplication = new LoogearApplication();
        }
        return m_LoogearApplication;
    }

    public int getActivityCount( ) {
        return m_ActivityCount;
    }
}
