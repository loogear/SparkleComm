using SparkleCommNet.Utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SoftPhoneCSharp.DataBase
{
    public struct SipAccountStruct
    {
        /// <summary>
        /// 主键ID
        /// </summary>
        public int ID;

        /// <summary>
        /// 用户名
        /// </summary>
        public string Name;

        /// <summary>
        /// 密码
        /// </summary>
        public string Pwd;

        /// <summary>
        /// 服务器
        /// </summary>
        public string Server;

        /// <summary>
        /// 端口
        /// </summary>
        public int Port;

        /// <summary>
        /// 域名
        /// </summary>
        public string Domain;

        /// <summary>
        /// 是否默认账号
        /// </summary>
        public bool Default;
    }

    /// <summary>
    /// SIP账户类
    /// </summary>
    public class SipAccount
    {
        private SqlHelper mSqlHelper;

        public SipAccount(SqlHelper sqlHelper)
        {
            mSqlHelper = sqlHelper;
        }

        /// <summary>
        /// 添加账号
        /// </summary>
        /// <param name="sttSipAccount"></param>
        /// <param name="strError"></param>
        /// <returns></returns>
        public int Insert(ref SipAccountStruct sttSipAccount, ref string strError)
        {
            int ID = 0;

            try
            {
                string cmd = string.Format(@"insert into account(name, pwd, server, port, domain, is_default) values('{0}', '{1}', '{2}', {3}, '{4}', {5});",
                    sttSipAccount.Name, sttSipAccount.Pwd, sttSipAccount.Server, sttSipAccount.Port, sttSipAccount.Domain, sttSipAccount.Default?1:0);
                ID = mSqlHelper.ExecuteInsert(cmd);
                sttSipAccount.ID = ID;
            }
            catch (Exception ex)
            {
                strError = "添加数据失败";
                Log.WriteError(typeof(SipAccount), ex);
            }

            return ID;
        }

        /// <summary>
        /// 更新账号
        /// </summary>
        /// <param name="sttSipAccount"></param>
        /// <param name="strError"></param>
        /// <returns></returns>
        public bool Update(ref SipAccountStruct sttSipAccount, ref string strError)
        {
            bool ret = false;

            try
            {
                string cmd = string.Format(@"
update account 
set name='{0}',
    pwd='{1}',
    server='{2}',
    port={3},
    domain='{4}',
    is_default={5}
where id={6};",
                    sttSipAccount.Name,
                    sttSipAccount.Pwd,
                    sttSipAccount.Server,
                    sttSipAccount.Port,
                    sttSipAccount.Domain,
                    sttSipAccount.Default ? 1 : 0,
                    sttSipAccount.ID);

                int n = mSqlHelper.ExecuteNonQuery(cmd);
                ret = n > 0;
            }
            catch (Exception ex)
            {
                strError = "更新数据失败";
                Log.WriteError(typeof(SipAccount), ex);
            }

            return ret;
        }

        private bool GetSipAccount(string strQuery, ref SipAccountStruct sttSipAccount, ref string strError)
        {
            bool ret = false;

            try
            {
                SqlDataReader SqlReader = mSqlHelper.ExecuteReader(strQuery);
                while (SqlReader.Read())
                {
                    sttSipAccount.ID = Int32.Parse(SqlReader["id"].ToString());
                    sttSipAccount.Name = SqlReader["name"].ToString();
                    sttSipAccount.Pwd = SqlReader["pwd"].ToString();
                    sttSipAccount.Server = SqlReader["server"].ToString();
                    sttSipAccount.Port = Int32.Parse(SqlReader["port"].ToString());
                    sttSipAccount.Domain = SqlReader["domain"].ToString();
                    sttSipAccount.Default = Int32.Parse(SqlReader["is_default"].ToString()) == 1 ? true : false;
                }
                SqlReader.Close();
                ret = true;
            }
            catch (Exception ex)
            {
                strError = "查询数据失败";
                Log.WriteError(typeof(SipAccount), ex);
            }

            return ret;
        }

        /// <summary>
        /// 根据ID获取SIP账号信息
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public SipAccountStruct GetSipAccount(int id, ref string strError)
        {
            string cmd = string.Format("select * from account where id={0}", id);
            SipAccountStruct sttSipAccount = new SipAccountStruct();
            GetSipAccount(cmd, ref sttSipAccount, ref strError);

            return sttSipAccount;
        }

        /// <summary>
        /// 获取默认账号
        /// </summary>
        /// <returns></returns>
        public SipAccountStruct GetDefaultSipAccount(ref string strError)
        {
            string cmd = string.Format("select * from account where is_default={0} limit 1", 1);
            SipAccountStruct sttSipAccount = new SipAccountStruct();
            GetSipAccount(cmd, ref sttSipAccount, ref strError);
            if(sttSipAccount.ID <= 0)
            {// 不存在默认账号，获取第一条数据
                cmd = string.Format("select * from account order by id asc limit 1 ;");
                GetSipAccount(cmd, ref sttSipAccount, ref strError);
            }
            return sttSipAccount;
        }
    }
}
