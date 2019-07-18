using SoftPhoneCSharp.SettingUI;
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
    public partial class frmSetting : Form
    {
        private frmAccountSetting m_AccountSetting;
        private frmVideoSetting m_VideoSetting;
        private frmVoiceSetting m_VoiceSetting;

        public frmSetting()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
            InitializeComponent();
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

        private void rdbAccount_CheckedChanged(object sender, EventArgs e)
        {
            if (m_AccountSetting == null) m_AccountSetting = new frmAccountSetting();
            if (rdbAccount.Checked)
            {
                OpenForm(m_AccountSetting);
                m_AccountSetting.UpdateCtrlValue();
            }
        }

        private void rdbVideoSet_CheckedChanged(object sender, EventArgs e)
        {
            if (m_VideoSetting == null) m_VideoSetting = new frmVideoSetting();
            if (rdbVideoSet.Checked)
            {
                OpenForm(m_VideoSetting);
            }
        }

        private void rdbVoiceSet_CheckedChanged(object sender, EventArgs e)
        {
            if (m_VoiceSetting == null) m_VoiceSetting = new frmVoiceSetting();
            if (rdbVoiceSet.Checked)
            {
                OpenForm(m_VoiceSetting);
            }
        }

        private void plLeft_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
