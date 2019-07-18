using PhoneNet;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SoftPhoneCSharp
{
    public partial class frmInCall : Form
    {
        private PhoneSdk m_Phone = PhoneSdk.Instance;

        private string m_RemoteNumber = ""; // 对方的号码
        private int m_CallID = 0; // 当前通话ID
        private int m_AccountID = 0;
        private bool m_IsVideo = false;//是否视频通话
        private DateTime m_CallBegin;// 通话开始时间
        private DateTime m_RecordBegin;// 录音开始时间
        private bool m_IsCallRelease = false;// 通话是否已经结束
        private int m_RecordId = 0; // 录音ID

        public frmInCall(int callID, int accid, string number)
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
            InitializeComponent();

            m_AccountID = accid;
            m_CallID = callID;
            m_RemoteNumber = number;
            lbNumber.Text = number;
            lbDtmf.Text = "";

            m_Phone.OnCallReleaseEvent += M_Phone_OnCallReleaseEvent;
            m_Phone.OnCallConnectedEvent += M_Phone_OnCallConnectedEvent;
        }

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                if (!PhoneNet.utils.Utils.IsXp())
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

        private void M_Phone_OnCallConnectedEvent(string peerAddr, int callID, int accid)
        {
            if (m_CallID == callID)
            {
                lbState.Visible = false;
                lbTime.Visible = true;
                lbTime.Text = "00:00";

                m_CallBegin = DateTime.Now;
                CallTimer.Start();
            }
        }
        
        /// <summary>
        /// 拨号，在拨号界面拨号时调用
        /// </summary>
        /// <param name="number">号码</param>
        /// <param name="video">是否拨打视频电话</param>
        /// <param name="accountId"></param>
        public void MakeCall(string number, bool video, int accountId)
        {
            m_RemoteNumber = number;
            m_IsVideo = video;
            m_AccountID = accountId;

            lbNumber.Text = m_RemoteNumber;
            lbState.Visible = true;
            lbState.Text = "正在拨号";

            new Thread(new ThreadStart(delegate ()
            {// 在子线程里进行拨号，因为拨号可能会稍微花点时间，避免UI迟钝
                m_CallID = m_Phone.MakeVoiceCall(number);

                if (m_CallID == -1)
                {
                    new WindowsFormsSynchronizationContext().Send((obj) =>
                    {// 主线程执行
                        MessageBox.Show("拨号失败！请检查麦克风、摄像头等", "错误");
                        m_IsCallRelease = true;
                        this.Close();
                    }, null);
                }

            })).Start();
        }

        private void M_Phone_OnCallReleaseEvent(string peerAddr, int callID, int causeCode, string reasonText, int accid)
        {// 释放电话
            if (callID == m_CallID)
            {
                m_IsCallRelease = true;
                CallTimer.Stop();
                this.Close();
            }
        }

        private void plTop_Resize(object sender, EventArgs e)
        {
            //plKeypad.Location = new System.Drawing.Point((plFill.Width - plKeypad.Width) / 2, (plFill.Height - plKeypad.Height) / 2);
        }

        private void plFill_Paint(object sender, PaintEventArgs e)
        {

        }

        /// <summary>
        /// 更新控件位置
        /// </summary>
        private void UpdateLocation()
        {
            imgHead.Location = new Point((plFill.Width - imgHead.Width) / 2, 22);
            lbNumber.Location = new Point((plFill.Width - lbNumber.Width) / 2, 144);
            lbTime.Location = new Point((plFill.Width - lbTime.Width) / 2, 170);
            lbState.Location = new Point((plFill.Width - lbState.Width) / 2, 197);
            grpFunction.Location = new System.Drawing.Point((plFill.Width - grpFunction.Width) / 2, plFill.Height - grpFunction.Height - 20);
            plKeypad.Location = new System.Drawing.Point((plFill.Width - plKeypad.Width) / 2, plFill.Height - plKeypad.Height - 100);
        }

        private void plFill_Resize(object sender, EventArgs e)
        {
            UpdateLocation();
        }

        private void frmInCall_Load(object sender, EventArgs e)
        {
            UpdateLocation();
        }

        private void frmInCall_FormClosed(object sender, FormClosedEventArgs e)
        {
            // 关闭窗口时，移除监听事件
            m_Phone.OnCallReleaseEvent -= M_Phone_OnCallReleaseEvent;
        }

        private string GetTimeDiffString(DateTime begin, DateTime end)
        {
            string text = "";
            TimeSpan ts = end - begin;
            if (ts.Hours > 0) text = text = ts.Hours.ToString("00") + ":" + ts.Minutes.ToString("00") + ":" + ts.Seconds.ToString("00");
            else text = ts.Minutes.ToString("00") + ":" + ts.Seconds.ToString("00");
            return text;
        }

        private void CallTimer_Tick(object sender, EventArgs e)
        {
            lbTime.Text = GetTimeDiffString(m_CallBegin, DateTime.Now);
            if (m_RecordId > 0)
            {
                lbRecord.Text = GetTimeDiffString(m_RecordBegin, DateTime.Now);
            }
            else
            {
                lbRecord.Text = "录音";
            }
        }

        private void btnHangup_Click(object sender, EventArgs e)
        {
            m_Phone.ReleaseCall(m_AccountID, m_CallID);
        }

        private void frmInCall_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_IsCallRelease)
            {// 点击窗口的关闭按钮，自动挂机
                e.Cancel = true;
                m_Phone.ReleaseCall(m_AccountID, m_CallID);
            }
        }

        private void chkHold_CheckedChanged(object sender, EventArgs e)
        {
            if (chkHold.Checked)
            {
                m_Phone.CallHold(m_AccountID, m_CallID);
            }
            else
            {
                m_Phone.ReleaseHold(m_AccountID, m_CallID);
            }
        }

        private void chkMute_CheckedChanged(object sender, EventArgs e)
        {
            m_Phone.SetMicMute(chkMute.Checked);
        }

        private void btnTrans_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("呼叫转移1007号码?", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
            {
                m_Phone.CallTransfer("1007", m_AccountID, m_CallID);
            }                
        }

        private void chkRecord_CheckedChanged(object sender, EventArgs e)
        {
            if(!chkRecord.Checked)
            {// 停止录音
                m_Phone.StopRecord(m_RecordId, m_AccountID, m_CallID);
                m_RecordId = 0;
                lbRecord.Text = "录音";
            }
            else
            {// 开始录音
                m_RecordBegin = DateTime.Now;
                m_RecordId = m_Phone.StartRecord(Application.StartupPath + "/Record/" + m_RemoteNumber + "_" + m_RecordBegin.ToString("yyyyMMddHHmmss")+".wav", m_AccountID, m_CallID);
            }            
        }

        private void plDialer_Paint(object sender, PaintEventArgs e)
        {
            // 绘制拨号盘的分割线
            Pen myPen = new Pen(Color.FromArgb(76, 76, 76), 1);
            // 绘制横线
            e.Graphics.DrawLine(myPen, 1, 0, plDialer.ClientSize.Width - 2, 0);
            e.Graphics.DrawLine(myPen, 1, 41, plDialer.ClientSize.Width - 2, 41);
            e.Graphics.DrawLine(myPen, 1, 82, plDialer.ClientSize.Width - 2, 82);
            e.Graphics.DrawLine(myPen, 1, 123, plDialer.ClientSize.Width - 2, 123);
            e.Graphics.DrawLine(myPen, 1, 164, plDialer.ClientSize.Width - 2, 164);

            // 绘制竖线
            //e.Graphics.DrawLine(myPen, 0, 0, 0, plDialer.ClientSize.Height - 1);//
            e.Graphics.DrawLine(myPen, 91, 0, 91, plDialer.ClientSize.Height - 1);
            e.Graphics.DrawLine(myPen, 182, 0, 182, plDialer.ClientSize.Height - 1);
            //e.Graphics.DrawLine(myPen, plDialer.ClientSize.Width - 1, 0, plDialer.ClientSize.Width - 1, plDialer.ClientSize.Height - 1);//
        }

        /// <summary>
        /// 输入按键
        /// </summary>
        /// <param name="key"></param>
        private void KeyInput(char key)
        {
            m_Phone.SendDtmf(key.ToString());
            if(lbDtmf.Text.Length > 20)
            {
                lbDtmf.Text = lbDtmf.Text.Substring(lbDtmf.Text.Length - 20, 20);
            }
            lbDtmf.Text += key.ToString();
        }

        private void btnNumber1_Click(object sender, EventArgs e)
        {
            KeyInput('1');
        }

        private void btnNumber2_Click(object sender, EventArgs e)
        {
            KeyInput('2');
        }

        private void btnNumber3_Click(object sender, EventArgs e)
        {
            KeyInput('3');
        }

        private void btnNumber4_Click(object sender, EventArgs e)
        {
            KeyInput('4');
        }

        private void btnNumber5_Click(object sender, EventArgs e)
        {
            KeyInput('5');
        }

        private void btnNumber6_Click(object sender, EventArgs e)
        {
            KeyInput('6');
        }

        private void btnNumber7_Click(object sender, EventArgs e)
        {
            KeyInput('7');
        }

        private void btnNumber8_Click(object sender, EventArgs e)
        {
            KeyInput('8');
        }

        private void btnNumber9_Click(object sender, EventArgs e)
        {
            KeyInput('9');
        }

        private void btnNumberStar_Click(object sender, EventArgs e)
        {
            KeyInput('*');
        }

        private void btnNumber0_Click(object sender, EventArgs e)
        {
            KeyInput('0');
        }

        private void btnNumberPound_Click(object sender, EventArgs e)
        {
            KeyInput('#');
        }

        private void chkKeypad_CheckedChanged(object sender, EventArgs e)
        {
            plKeypad.Visible = chkKeypad.Checked;
        }
    }
}
