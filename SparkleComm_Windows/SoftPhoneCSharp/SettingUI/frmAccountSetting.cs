using SoftPhoneCSharp.DataBase;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SoftPhoneCSharp.SettingUI
{
    public partial class frmAccountSetting : Form
    {
        private SipAccountStruct m_sttSipAccount;

        public frmAccountSetting()
        {
            InitializeComponent();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            SipAccount MySipAccount = new SipAccount(frmMain.m_SqlHelper);
            string strError = "";
            m_sttSipAccount.Name = txtUserName.Text.Trim();
            m_sttSipAccount.Pwd = txtUserPwd.Text.Trim();
            m_sttSipAccount.Domain = txtDomain.Text.Trim();
            m_sttSipAccount.Default = true;
            int index = txtServer.Text.IndexOf(":");
            bool success = false;
            if (index > 0)
            {
                m_sttSipAccount.Server = txtServer.Text.Substring(0, index);
                m_sttSipAccount.Port = Int32.Parse(txtServer.Text.Substring(index+1));
            }
            else
            {
                m_sttSipAccount.Server = txtServer.Text.Trim();
                m_sttSipAccount.Port = 5060;
            }

            if (m_sttSipAccount.ID == 0)
            {
                int ID = MySipAccount.Insert(ref m_sttSipAccount, ref strError);
                if (ID <= 0)
                {
                    lbStatus.Text = strError;
                    lbStatus.ForeColor = Color.Red;
                    //MessageBox.Show(strError, "错误");
                }
                else
                {
                    success = true;
                }
            }
            else
            {
                success = MySipAccount.Update(ref m_sttSipAccount, ref strError);
                if (!success)
                {
                    lbStatus.Text = strError;
                    lbStatus.ForeColor = Color.Red;
                    //MessageBox.Show(strError, "错误");
                }
            }
            if (success)
            {
                lbStatus.Text = "保存成功";
                lbStatus.ForeColor = Color.Green;
                frmMain.HandleMessage(frmMain.MessageType.MT_SIP_ACCOUNT_MODIFY);
            }
        }

        /// <summary>
        /// 更新控件值
        /// </summary>
        public void UpdateCtrlValue()
        {
            lbStatus.Text = "";
            SipAccount MySipAccount = new SipAccount(frmMain.m_SqlHelper);
            string strError = "";
            m_sttSipAccount = MySipAccount.GetDefaultSipAccount(ref strError);
            if (strError.Trim().Length > 0)
            {
                lbStatus.Text = strError;
                lbStatus.ForeColor = Color.Red;
                //MessageBox.Show(strError, "错误");
                return;
            }
            if (m_sttSipAccount.ID > 0)
            {
                txtUserName.Text = m_sttSipAccount.Name;
                txtUserPwd.Text = m_sttSipAccount.Pwd;
                txtServer.Text = m_sttSipAccount.Server + ":" + m_sttSipAccount.Port.ToString();
                txtDomain.Text = m_sttSipAccount.Domain;
            }
        }

        private void frmAccountSetting_Load(object sender, EventArgs e)
        {
            
        }
    }
}
