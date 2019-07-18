using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using PhoneNet.Utils;
using System.Runtime.InteropServices;
using PhoneNet;
using System.Reflection;
using SoftPhoneCSharp.DataBase;

namespace SoftPhoneCSharp
{
    public partial class frmMain : Form
    {
        /// <summary>
        /// 消息类型
        /// </summary>
        public enum MessageType
        {
            /// <summary>
            /// 更新了SIP账号信息
            /// </summary>
            MT_SIP_ACCOUNT_MODIFY,
        }

        /// <summary>
        /// voip sdk类
        /// </summary>
        private PhoneSdk m_Phone = PhoneSdk.Instance;
        private frmDialer m_Dialer;
        private frmContacts m_Contacts;
        private frmMessage m_Message;
        private frmMetting m_Metting;
        private frmPtt m_Ptt;
        private frmSetting m_Setting;
        private bool m_IsRegister = false;// 是否已经注册

        public static SqlHelper m_SqlHelper;
        private SipAccountStruct m_SipAccount; // 当前SIP账号
        private static frmMain m_Instance = null;

        /// <summary>
        /// 是否已经注册
        /// </summary>
        public bool IsRegister
        {
            get { return m_IsRegister; }
        }

        public frmMain()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
            InitializeComponent();

            m_Instance = this;
            // 初始化SDK
            m_Phone.InitSdk(new WindowsFormsSynchronizationContext());
            m_Phone.OnSipConnectedEvent += M_Phone_OnSipConnectedEvent;
            m_Phone.OnSipDisconnectedEvent += M_Phone_OnSipDisconnectedEvent;
            m_Phone.OnCallConnectedEvent += M_Phone_OnCallConnectedEvent;
            m_Phone.OnCallIncomingEvent += M_Phone_OnCallIncomingEvent;

            //获取SDK版本
            Log.WriteInfo(typeof(frmMain), "sdk version=" + m_Phone.GetSdkVersion());

            m_Dialer = new frmDialer(this);
            m_Contacts = new frmContacts();
            m_Message = new frmMessage();
            m_Metting = new frmMetting();
            m_Ptt = new frmPtt();
            m_Setting = new frmSetting();

            this.Text = "劳格VOIP V" + AssemblyFileVersion();

            // 数据库
            m_SqlHelper = new SqliteHelper(@"Data Source=sip_setting.db;");
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

        private string AssemblyFileVersion()
        {
            object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyFileVersionAttribute), false);
            if (attributes.Length == 0)
            {
                return "";
            }
            else
            {
                return ((AssemblyFileVersionAttribute)attributes[0]).Version;
            }
        }

        /// <summary>
        /// 注册SIP服务器
        /// </summary>
        private void RegisterSip()
        {
            SipAccount MySipAccount = new SipAccount(frmMain.m_SqlHelper);
            string strError = "";
            m_SipAccount = MySipAccount.GetDefaultSipAccount(ref strError);
            if (m_SipAccount.ID > 0)
            {
                m_Phone.RegisterSipServer(m_SipAccount.Server, m_SipAccount.Port, m_SipAccount.Name, m_SipAccount.Pwd, 0, m_SipAccount.Domain, false, "", false, "");
            }
            m_Dialer.UpdateSipAccount(m_SipAccount);
            //m_Phone.RegisterSipServer("192.168.0.247", 5060, "1019", "123456", 0, "192.168.0.247", false, "", false, "");
        }

        private void M_Phone_OnCallIncomingEvent(string peerAddr, int callID, int accid)
        {
            frmIncoming Incoming = new frmIncoming();
            Incoming.SetIncomingInfo(peerAddr, callID, accid);
            Incoming.Show();
        }

        private void M_Phone_OnCallConnectedEvent(string peerAddr, int callID, int accid)
        {
        }

        private void M_Phone_OnSipDisconnectedEvent(int accid)
        {// 断开SIP服务器的连接
            Log.WriteInfo(typeof(frmMain), "M_Phone_OnSipDisconnectedEvent");
            m_IsRegister = false;
            imgHead.BackgroundNormal.Image = global::SoftPhoneCSharp.Properties.Resources.state_offline2;
            imgHead.BackgroundHover.Image = global::SoftPhoneCSharp.Properties.Resources.state_offline2;
            imgHead.BackgroundPushed.Image = global::SoftPhoneCSharp.Properties.Resources.state_offline2;
            lbOnlieState.Text = "离线";
            lbOnlieState.ForeColor = Color.Red;
            imgHead.Refresh();
        }

        private void M_Phone_OnSipConnectedEvent(int accid)
        {// 连接上SIP服务器
            Log.WriteInfo(typeof(frmMain), "M_Phone_OnSipConnectedEvent");

            m_IsRegister = true;
            imgHead.BackgroundNormal.Image = global::SoftPhoneCSharp.Properties.Resources.state_online2;
            imgHead.BackgroundHover.Image = global::SoftPhoneCSharp.Properties.Resources.state_online2;
            imgHead.BackgroundPushed.Image = global::SoftPhoneCSharp.Properties.Resources.state_online2;
            lbOnlieState.Text = "在线";
            lbOnlieState.ForeColor = Color.Green;
            imgHead.Refresh();
        }

        private void plLeft_Paint(object sender, PaintEventArgs e)
        {// 绘制分割线
            Pen myPen = new Pen(Color.FromArgb(204, 204, 204), 1);
            e.Graphics.DrawLine(myPen, plLeft.ClientSize.Width - 1, 0, plLeft.ClientSize.Width - 1, plLeft.ClientSize.Height - 1);
        }

        /// <summary>
        /// 打开窗口
        /// </summary>
        /// <param name="form"></param>
        private void OpenForm(Form form)
        {
            if (plFill.Controls.Count > 0 && plFill.Controls[0].Name == form.Name)
            {// 已经添加
                return;
            }

            plFill.Controls.Clear();
            form.TopLevel = false;  //设置为非顶级窗体
            form.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;//设置窗体为非边框样式
            form.Dock = System.Windows.Forms.DockStyle.Fill;//设置样式是否填充整个panel
            plFill.Controls.Add(form);//添加窗体
            form.Show();//窗体运行
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            rdbDialer.Checked = true;
            RegisterSip();
        }

        private void rdbDialer_CheckedChanged(object sender, EventArgs e)
        {
            if (rdbDialer.Checked) OpenForm(m_Dialer);
        }

        private void rdbMessage_CheckedChanged(object sender, EventArgs e)
        {
            if(rdbMessage.Checked) OpenForm(m_Message);
        }

        private void rdbContacts_CheckedChanged(object sender, EventArgs e)
        {
            if (rdbContacts.Checked) OpenForm(m_Contacts);
        }

        private void rdbMetting_CheckedChanged(object sender, EventArgs e)
        {
            if (rdbMetting.Checked) OpenForm(m_Metting);
        }

        private void rdbPtt_CheckedChanged(object sender, EventArgs e)
        {
            if (rdbPtt.Checked) OpenForm(m_Ptt);
        }

        private void rdbSetting_CheckedChanged(object sender, EventArgs e)
        {
            if (rdbSetting.Checked) OpenForm(m_Setting);
        }

        private void frmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (IsRegister)
            {
                m_Phone.UnRegister();
            }
        }

        private void ProcessMessage(MessageType mt)
        {
            switch(mt)
            {
                case MessageType.MT_SIP_ACCOUNT_MODIFY:// 重新注册
                    m_Phone.UnRegister();
                    RegisterSip();
                    break;
            }
        }

        public static void HandleMessage(MessageType mt)
        {
            m_Instance.ProcessMessage(mt);
        }
    }
}
