using PhoneNet;
using PhoneNet.Utils;
using SoftPhoneCSharp.DataBase;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SoftPhoneCSharp
{
    public partial class frmDialer : Form
    {
        private frmMain m_MainForm;
        private PhoneSdk m_Phone = PhoneSdk.Instance;

        public frmDialer(frmMain form)
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
            InitializeComponent();
            m_MainForm = form;
            lbPhone.Text = "";
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

        private void plFill_Paint(object sender, PaintEventArgs e)
        {
            // 绘制分割线
            Pen myPen = new Pen(Color.FromArgb(204, 204, 204), 1);
            e.Graphics.DrawLine(myPen, 0, 0, 0, plLeft.ClientSize.Height - 1);
        }

        private void plDialer_Paint(object sender, PaintEventArgs e)
        {
            // 绘制拨号盘的分割线
            Pen myPen = new Pen(Color.FromArgb(204, 204, 204), 1);
            // 绘制横线
            e.Graphics.DrawLine(myPen, 1, 0, plDialer.ClientSize.Width - 2, 0);
            e.Graphics.DrawLine(myPen, 1, 83, plDialer.ClientSize.Width - 2, 83);
            e.Graphics.DrawLine(myPen, 1, 166, plDialer.ClientSize.Width - 2, 166);
            e.Graphics.DrawLine(myPen, 1, 249, plDialer.ClientSize.Width - 2, 249);
            e.Graphics.DrawLine(myPen, 1, 332, plDialer.ClientSize.Width - 2, 332);

            // 绘制竖线
            //e.Graphics.DrawLine(myPen, 0, 0, 0, plDialer.ClientSize.Height - 1);
            e.Graphics.DrawLine(myPen, 165, 0, 165, plDialer.ClientSize.Height - 1);
            e.Graphics.DrawLine(myPen, 330, 0, 330, plDialer.ClientSize.Height - 1);
            //e.Graphics.DrawLine(myPen, plDialer.ClientSize.Width - 1, 0, plDialer.ClientSize.Width - 1, plDialer.ClientSize.Height - 1);
        }

        private void plFill_Resize(object sender, EventArgs e)
        {
            // 拨号界面始终居中显示
            plKeypad.Location = new System.Drawing.Point((plFill.Width - plKeypad.Width) / 2, (plFill.Height - plKeypad.Height) / 2);
            // 标题位置
            lbTitle.Location = new System.Drawing.Point((plFill.Width - lbTitle.Width) / 2, 30);
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            if (lbPhone.Text.Length > 0)
            {
                lbPhone.Text = lbPhone.Text.Substring(0, lbPhone.Text.Length - 1);
            }
            if (lbPhone.Text.Length == 0) btnDelete.Visible = false;
        }

        /// <summary>
        /// 输入按键
        /// </summary>
        /// <param name="key"></param>
        private void KeyInput(char key)
        {
            lbPhone.Text += key.ToString();
            btnDelete.Visible = true;
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

        /// <summary>
        /// 拨号
        /// </summary>
        /// <param name="number">号码</param>
        /// <param name="video">是否视频电话</param>
        private void MakeCall(string number, bool video)
        {
            if (number.Length == 0) return;
            if (!m_MainForm.IsRegister)
            {
                MessageBox.Show("没有注册到服务器，不能拨打电话");
                return;
            }
            /*if (m_Phone.GetCallCount() > 0)
            {
                MessageBox.Show("超过最大呼叫数[1]");
                return;
            }*/

            frmInCall form = new frmInCall(0, -1, number);
            form.Show();
            form.MakeCall(number, video, 0);
        }

        private void btnVideo_Click(object sender, EventArgs e)
        {
            MakeCall(lbPhone.Text, true);
        }

        private void btnVoice_Click(object sender, EventArgs e)
        {
            MakeCall(lbPhone.Text, false);
        }

        /// <summary>
        /// 更新SIP账号
        /// </summary>
        /// <param name="sttAccount"></param>
        public void UpdateSipAccount(SipAccountStruct sttAccount)
        {
            lbAccount.Text = sttAccount.Name+"@"+ sttAccount.Server+":"+ sttAccount.Port;
        }
    }
}
