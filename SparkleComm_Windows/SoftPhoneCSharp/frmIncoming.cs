using SparkleCommNet;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SoftPhoneCSharp
{
    public partial class frmIncoming : Form
    {
        private PhoneSdk m_Phone = PhoneSdk.Instance;

        private string m_RemoteNumber = ""; // 对方的号码
        private int m_CallID = 0; // 当前通话ID
        private int m_AccountID = 0;
        public frmIncoming()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
            InitializeComponent();
            this.TopMost = true;
            m_Phone.OnCallReleaseEvent += M_Phone_OnCallReleaseEvent;
        }

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                if (!SparkleCommNet.utils.Utils.IsXp())
                {
                    if (!DesignMode)
                    {
                        cp.ExStyle |= (int)LightUI.WindowStyle.WS_CLIPCHILDREN;
                        //cp.ClassStyle |= (int)ClassStyle.CS_DropSHADOW;  //实现窗体边框阴影效果
                    }
                }
                return cp;
            }
        }

        private void M_Phone_OnCallReleaseEvent(string peerAddr, int callID, int causeCode, string reasonText, int accid)
        {// 释放电话
            if (callID == m_CallID)
            {
                this.Close();
            }
        }

        /// <summary>
        /// 设置来电信息
        /// </summary>
        /// <param name="remoteAddr"></param>
        /// <param name="callId"></param>
        public void SetIncomingInfo(string remoteAddr, int callId, int accid)
        {
            string message = remoteAddr;
            string number = message;
            int index = message.IndexOf("<");
            int index1 = message.IndexOf("@");
            if(index > 0 && index1 > (index+5))
            number = message.Substring(index + 5, index1 - index - 5);

            m_RemoteNumber = number;
            m_CallID = callId;
            m_AccountID = accid;
            lbAddr.Text = m_RemoteNumber;
        }

        private void btnHangup_Click(object sender, EventArgs e)
        {
            m_Phone.ReleaseCall(m_AccountID, m_CallID);
        }

        private void btnAnswer_Click(object sender, EventArgs e)
        {
            frmInCall form = new frmInCall(m_CallID, m_AccountID, m_RemoteNumber);
            form.Show();

            m_Phone.AnswerCall(0, m_CallID);
            this.Close();
        }

        private void frmIncoming_FormClosed(object sender, FormClosedEventArgs e)
        {
            // 关闭窗口时，移除监听事件
            m_Phone.OnCallReleaseEvent -= M_Phone_OnCallReleaseEvent;
        }
    }
}
