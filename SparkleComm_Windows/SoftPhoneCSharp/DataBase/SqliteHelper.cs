using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.SQLite;
using SparkleCommNet.Utils;

namespace SoftPhoneCSharp.DataBase
{
    /// <summary>
    /// Sql数据流式读取
    /// </summary>
    public class SqliteReader : SqlDataReader
    {
        private SQLiteDataReader mSQLiteDataReader;

        public SqliteReader(SQLiteCommand command)
        {
            mSQLiteDataReader = command.ExecuteReader();
        }

        /// <summary>
        /// 读取下一行数据
        /// </summary>
        /// <returns>是否成功</returns>
        public override bool Read()
        {
            if (mSQLiteDataReader == null)
            {
                return false;
            }

            return mSQLiteDataReader.Read();
        }

        /// <summary>
        /// 关闭读取
        /// </summary>
        public override void Close()
        {
            if (mSQLiteDataReader != null)
            {
                mSQLiteDataReader.Close();
            }            
        }

        public override object this[int i]
        {
            get
            {
                if (mSQLiteDataReader == null)
                {
                    return "";
                }

                return mSQLiteDataReader[i];
            }
        }

        public override object this[string name]
        {
            //实现索引器的get方法
            get
            {
                if (mSQLiteDataReader == null)
                {
                    return "";
                }

                return mSQLiteDataReader[name];
            }
        }
    }

    public class SqliteHelper:SqlHelper
    {
        private SQLiteConnection mSQLiteConnection;

        public SqliteHelper(string ConnectionString)
        {
            SqlConnString = ConnectionString;
            mSQLiteConnection = new SQLiteConnection(ConnectionString);
            mSQLiteConnection.Open();

            // 执行下面的优化语句，可以加快数据插入的速度
            ExecuteNonQuery("PRAGMA synchronous = OFF; ");
        }

        /// <summary>
        /// 关闭数据库
        /// </summary>
        public override void Close()
        {
            if (mSQLiteConnection != null)
            {
                mSQLiteConnection.Close();
                mSQLiteConnection = null;
            }
        }

        private void PrepareCommand(SQLiteCommand cmd, string cmdText, params object[] p)
        {
            if (mSQLiteConnection.State != ConnectionState.Open)
            {
                mSQLiteConnection.Open();
            }
            cmd.Parameters.Clear();
            cmd.Connection = mSQLiteConnection;
            cmd.CommandText = cmdText;
            cmd.CommandType = CommandType.Text;
            cmd.CommandTimeout = 30;
            if (p != null)
            {
                foreach (object parm in p)
                {
                    cmd.Parameters.AddWithValue(string.Empty, parm);
                }
            }
        }

        /// <summary>
        /// 执行查询语句
        /// </summary>
        /// <param name="cmdText">Sql查询语句</param>
        /// <param name="p">参数（没有时填null）</param>
        /// <returns>查询结果集</returns>
        public override DataSet ExecuteQuery(string cmdText, params object[] p)
        {
            if (mSQLiteConnection == null)
            {
                Log.WriteError(typeof(SqliteHelper), "数据库未连接");
                return null;
            }

            using (SQLiteCommand command = new SQLiteCommand())
            {
                DataSet ds = new DataSet();
                PrepareCommand(command, cmdText, p);
                SQLiteDataAdapter da = new SQLiteDataAdapter(command);
                da.Fill(ds);
                return ds;
            }
        }

        /// <summary>
        /// 执行Insert、Update、Delete更新语句
        /// </summary>
        /// <param name="cmdText">命令文本</param>
        /// <param name="p">参数（没有时填null）</param>
        /// <returns>返回受影响的行数</returns>
        public override int ExecuteNonQuery(string cmdText, params object[] p)
        {
            if (mSQLiteConnection == null)
            {
                Log.WriteError(typeof(SqliteHelper), "数据库未连接");
                return 0;
            }

            using (SQLiteCommand command = new SQLiteCommand())
            {
                PrepareCommand(command, cmdText, p);
                int n = 0;
                try
                {
                    n = command.ExecuteNonQuery();
                }
                catch (Exception ex)
                {
                    Log.WriteError(typeof(SqliteHelper), ex);
                }
                return n;
            }
        }

        /// <summary>
        /// 执行Insert,并返回自增的ID
        /// </summary>
        /// <param name="cmdText"></param>
        /// <param name="p"></param>
        /// <returns></returns>
        public override int ExecuteInsert(string cmdText, params object[] p)
        {
            if (mSQLiteConnection == null)
            {
                Log.WriteError(typeof(SqliteHelper), "数据库未连接");
                return 0;
            }

            cmdText += ";select last_insert_rowid();";
            using (SQLiteCommand command = new SQLiteCommand())
            {
                PrepareCommand(command, cmdText, p);
                object ret = command.ExecuteScalar();
                if (ret != null)
                {
                    int id = Convert.ToInt32(ret);
                    return id;
                }
                return 0;
            }
        }

        /// <summary>
        /// 返回SqlDataReader对象
        /// </summary>
        /// <param name="cmdText">命令文本</param>
        /// <param name="p">参数</param>
        /// <returns>SqlDataReader对象</returns>
        public override SqlDataReader ExecuteReader(string cmdText, params object[] p)
        {
            SQLiteCommand command = new SQLiteCommand();
            PrepareCommand(command, cmdText, p);
            return new SqliteReader(command);
        }
    }
}
