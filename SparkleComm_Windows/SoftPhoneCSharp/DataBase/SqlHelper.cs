using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

namespace SoftPhoneCSharp.DataBase
{
    /// <summary>
    /// Sql数据流式读取
    /// </summary>
    public abstract class SqlDataReader
    {
        /// <summary>
        /// 读取下一行数据
        /// </summary>
        /// <returns>是否成功</returns>
        public virtual bool Read()
        {
            return false;
        }

        /// <summary>
        /// 关闭读取
        /// </summary>
        public virtual void Close()
        { 
        }

        public virtual object this[int i]
        {
            //实现索引器的get方法
            get
            {
                return null;
            }
        }

        public virtual object this[string name]
        {
            //实现索引器的get方法
            get
            {
                return null;
            }
        } 
    }

    /// <summary>
    /// 数据库操作接口
    /// 提供Mysql/Sqlite操作的基类
    /// </summary>
    public abstract class SqlHelper
    {
        /// <summary>
        /// 连接字符串
        /// </summary>
        public string SqlConnString
        {
            get;
            set;
        }

        /// <summary>
        /// 关闭数据库
        /// </summary>
        public virtual void Close()
        {
        }

        /// <summary>
        /// 执行查询语句
        /// </summary>
        /// <param name="cmdText">Sql查询语句</param>
        /// <param name="p">参数（没有时填null）</param>
        /// <returns>查询结果集</returns>
        public virtual DataSet ExecuteQuery(string cmdText, params object[] p)
        {
            return null;
        }

        /// <summary>
        /// 执行Insert、Update、Delete更新语句
        /// </summary>
        /// <param name="cmdText">命令文本</param>
        /// <param name="p">参数（没有时填null）</param>
        /// <returns>返回受影响的行数</returns>
        public virtual int ExecuteNonQuery(string cmdText, params object[] p)
        {
            return 0;
        }

        /// <summary>
        /// 执行Insert,并返回自增的ID
        /// </summary>
        /// <param name="cmdText"></param>
        /// <param name="p"></param>
        /// <returns></returns>
        public virtual int ExecuteInsert(string cmdText, params object[] p)
        {
            return 0;
        }

        /// <summary>
        /// 返回SqlDataReader对象
        /// </summary>
        /// <param name="cmdText">命令文本</param>
        /// <param name="p">参数</param>
        /// <returns>SqlDataReader对象</returns>
        public virtual SqlDataReader ExecuteReader(string cmdText, params object[] p)
        {
            return null;
        }

    }
}
