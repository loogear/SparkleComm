package com.loogear.loogearphone;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

/**
 * 系统参数配置类
 */
public class SysConfig {

    /**
     * 获取SIP账户用户名
     * @param context
     * @return
     */
    public static String getSipUser(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        String sip_user = prefs.getString("sip_user","");
        return sip_user;
    }

    /**
     * 获取SIP账户密码
     * @param context
     * @return
     */
    public static String getSipPwd(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        String sip_pwd = prefs.getString("sip_pwd","");
        return sip_pwd;
    }

    /**
     * 获取SIP代码服务器地址：不包括端口号
     * @param context
     * @return
     */
    public static String getSipAddr(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        String sip_addr = prefs.getString("sip_addr","");
        int index = sip_addr.indexOf(":");
        if(index > 0) sip_addr = sip_addr.substring(0, index);
        return sip_addr;
    }

    /**
     * 获取SIP代码服务器端口
     * @param context
     * @return
     */
    public static int getSipPort(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        String sip_addr = prefs.getString("sip_addr","");
        int index = sip_addr.indexOf(":");
        if(index > 0) {
            String tmp = sip_addr.substring(index + 1);
            try {
                int port = Integer.parseInt(tmp);
                return port;
            }
            catch (Exception ex){
                return 5060;
            }
        }
        else return 5060;
    }

    /**
     * 获取SIP代理服务器域名
     * @param context
     * @return
     */
    public static String getSipDomain(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        String sip_domain = prefs.getString("sip_domain","");
        return sip_domain;
    }
}
