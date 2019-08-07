package com.loogear.loogearphone;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.loogear.loogearphone.uicontrol.CustomKeyPad;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link DialerFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link DialerFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class DialerFragment extends Fragment implements CustomKeyPad.OnCustomKeyPadListener {
    private final static String TAG = "DialerFragment";
    private OnFragmentInteractionListener mListener;
    private ListView m_CallRecordList = null;
    private CustomKeyPad m_CustomKeyPad;

    private static class ListEntry {
        private TextView name; // 姓名
        private TextView phone;// 电话号码
        private TextView time; // 通话时间
        private ImageView head; // 头像
        private ImageView state; // 通话状态
        private String callInfo;// 通话信息
    }

    public DialerFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment DialerFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static DialerFragment newInstance(String param1, String param2) {
        DialerFragment fragment = new DialerFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        LoadCallRecord();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View root = inflater.inflate(R.layout.fragment_dialer, container, false);
        this.m_CallRecordList = (ListView) root.findViewById(R.id.CallRecordList);
        this.m_CustomKeyPad = (CustomKeyPad) root.findViewById(R.id.keypad);
        this.m_CustomKeyPad.setOnCustomKeyPadListener(this);
        return root;
    }

    // TODO: Rename method, update argument and hook method into UI event
    public void onButtonPressed(Uri uri) {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }

    /**
     * 加载通话记录
     */
    private void LoadCallRecord() {
        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                Log.i(TAG, "LoadCallRecord");
                //m_appList = AppManager.GetAllAppInfo(MainActivity.this);
                return null;
            }
            @Override
            protected void onPostExecute(Void result) {
                ShowCallRecord();
            }
        }.execute();
    }

    /**
     * 显示通话记录
     */
    private void ShowCallRecord() { // 全部是测试数据，没有实现
        Log.i(TAG, "ShowCallRecord");
        final String callLists[] = {"中国联通","中国移动","中国电信","10086","15922960000","15922960001","15922960002","15922960003","15923457891","15923457893","15923457895","15923457896","15983457891"};
        final LayoutInflater inflater = getLayoutInflater();
        final ListAdapter adapter = new ArrayAdapter<String>(this.getContext(), R.layout.call_record_listitem, callLists) {
            @Override
            public View getView(final int position, View convertView, ViewGroup parent) {
                ListEntry entry;
                if (convertView == null) {
                    // Inflate a new view
                    convertView = inflater.inflate(R.layout.call_record_listitem, parent, false);
                    //Log.d("MainActivity", ">> inflate("+convertView+")");
                    entry = new ListEntry();
                    entry.name = (TextView) convertView.findViewById(R.id.itemname);
                    entry.phone = (TextView) convertView.findViewById(R.id.itemphone);
                    entry.time = (TextView) convertView.findViewById(R.id.itemtime);
                    entry.head = (ImageView) convertView.findViewById(R.id.itemicon);
                    entry.state = (ImageView) convertView.findViewById(R.id.itemstate);
                    //entry.box_wifi.setOnCheckedChangeListener(MainActivity.this);
                    //entry.box_3g.setOnCheckedChangeListener(MainActivity.this);
                    convertView.setTag(entry);
                } else {
                    // Convert an existing view
                    entry = (ListEntry) convertView.getTag();
                }
                final String item = callLists[position];
                entry.callInfo = item;
                entry.name.setText(item);
                entry.head.setImageResource(R.drawable.gray_head);
                if(position > 3) {
                    entry.state.setImageResource(R.drawable.call_missed);
                }

                return convertView;
            }
        };
        this.m_CallRecordList.setAdapter(adapter);
    }

    @Override
    public void OnKeyPadDown(String key) {
        Log.i(TAG, "OnKeyPadDown:"+key);
    }

    @Override
    public void OnMakeCall(String number, boolean isVideo) {
        Log.i(TAG, "OnMakeCall:"+number+" video="+isVideo);
        if(HomeActivity.m_IsRegistered) {
            Intent intent = new Intent();
            intent.setClass(this.getContext(), InCallActivity.class);
            intent.putExtra("number", number);// 电话号码
            intent.putExtra("video", isVideo); // 是否视频电话
            intent.putExtra("incoming", false); // true=来电; false=去电
            intent.putExtra("accountid", 0); // 账户ID
            startActivity(intent);
        }else {
            Toast.makeText(this.getContext(), R.string.error_call, Toast.LENGTH_LONG).show();
        }
    }
}
