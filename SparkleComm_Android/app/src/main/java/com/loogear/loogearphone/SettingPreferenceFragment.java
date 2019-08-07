package com.loogear.loogearphone;

import android.content.Intent;
import android.content.SharedPreferences;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Bundle;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.preference.RingtonePreference;
import android.text.TextUtils;
import android.util.Log;

public class SettingPreferenceFragment extends PreferenceFragment implements Preference.OnPreferenceClickListener {
    private final static String TAG = "SettingPreference";
    private Preference m_PrefAbout;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        //从xml文件加载选项
        addPreferencesFromResource(R.xml.setting_preference);

        //通过key得到界面上的Preference
        m_PrefAbout = (Preference) findPreference("about");
        m_PrefAbout.setOnPreferenceClickListener(this);

        // 输入选择的值显示出来
        bindPreferenceSummaryToValue(findPreference("sip_user"));
        bindPreferenceSummaryToValue(findPreference("sip_pwd"));
        bindPreferenceSummaryToValue(findPreference("sip_addr"));
        bindPreferenceSummaryToValue(findPreference("sip_domain"));

        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this.getActivity()) ;
        Log.i(TAG, "sip_user="+prefs.getString("sip_user",""));
        Log.i(TAG, "sip_pwd="+prefs.getString("sip_pwd",""));
        Log.i(TAG, "sip_addr="+prefs.getString("sip_addr",""));
        Log.i(TAG, "sip_domain="+prefs.getString("sip_domain",""));
    }

    @Override
    public boolean onPreferenceClick(Preference preference) {
        Log.i(TAG, "onPreferenceClick:"+preference.getKey());

        /*Intent intent = new Intent(Intent.ACTION_SEND);
        intent.setType("text/plain");
        intent.putExtra(Intent.EXTRA_SUBJECT, "分享");
        intent.putExtra(Intent.EXTRA_TEXT, "share content");
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(Intent.createChooser(intent, getTitle()));*/

        return true;
    }

    /**
     * bindPreferenceSummaryToValue 拷贝至as自动生成的preferences的代码，用于绑定显示实时值
     */
    private static Preference.OnPreferenceChangeListener sBindPreferenceSummaryToValueListener = new Preference.OnPreferenceChangeListener() {
        @Override
        public boolean onPreferenceChange(Preference preference, Object value) {
            String stringValue = value.toString();
            if (preference instanceof ListPreference) {
                // For list preferences, look up the correct display value in
                // the preference's 'entries' list.
                ListPreference listPreference = (ListPreference) preference;
                int index = listPreference.findIndexOfValue(stringValue);

                // Set the summary to reflect the new value.
                preference.setSummary(index >= 0 ? listPreference.getEntries()[index] : null);

            } else if (preference instanceof RingtonePreference) {
                // For ringtone preferences, look up the correct display value
                // using RingtoneManager.
                if (TextUtils.isEmpty(stringValue)) {
                    // Empty values correspond to 'silent' (no ringtone).
                    // liubo preference.setSummary(R.string.pref_ringtone_silent);
                } else {
                    Ringtone ringtone = RingtoneManager.getRingtone( preference.getContext(), Uri.parse(stringValue));
                    if (ringtone == null) {
                        // Clear the summary if there was a lookup error.
                        preference.setSummary(null);
                    } else {
                        // Set the summary to reflect the new ringtone display
                        // name.
                        String name = ringtone.getTitle(preference.getContext());
                        preference.setSummary(name);
                    }
                }
            } else {
                // For all other preferences, set the summary to the value's
                // simple string representation.
                preference.setSummary(stringValue);
            }
            return true;
        }
    };

    private static void bindPreferenceSummaryToValue(Preference preference) {
        // Set the listener to watch for value changes.
        preference.setOnPreferenceChangeListener(sBindPreferenceSummaryToValueListener);
        // Trigger the listener immediately with the preference's
        // current value.
        sBindPreferenceSummaryToValueListener.onPreferenceChange(preference, PreferenceManager.getDefaultSharedPreferences(preference.getContext()).getString(preference.getKey(), ""));
    }

}
