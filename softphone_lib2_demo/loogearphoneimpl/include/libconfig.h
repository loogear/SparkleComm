#ifndef LIBCONFIG_H
#define LIBCONFIG_H
#include <iostream>
#include <string>
#include "sdkpublic.h"

/*!
 * \brief SDK 配置类,用于配置SDK的细节参数.配置采用JSON格式存储。
 */
class SOFTPHONE_API LibConfig
{
public:
    LibConfig();
    /*!
     * \brief generateDefault 生成配置参数,此方法用户让SDK生成一份默认的参数配置.
     * 可用于修改后再调用\a setConfig 来设置配置参数。
     * \return 返回JSON格式字符串
     */
    std::string generateDefault();

    /*!
     * \brief setConfig 设置完整的配置(JSON格式)
     * \param configParas JSON格式的配置参数,配置模版请使用\a generateDefault 生成
     * \return 成功与否
     */
    bool  setConfig(std::string configParas);

    /*!
     * \brief getJsonStrings 获取配置完整的配置参数,不同于\a generateDefault ,此方法是获取已经生效的配置
     *  如果之前做过配置修改，则是修改后的完整配置。
     * \return 成功与否
     */
    std::string getJsonStrings();

    /*!
     * \brief getStringValue 获取单项配置的值(String类型)
     * \param key 配置项名称，如"Media.echoCancel.options"
     * \return 配置项的值
     */
    std::string  getStringValue(std::string key);
    /*!
     * \brief getIntValue 获取单项配置的值(Int类型)
     * \param key 配置项名称，如"Media.echoCancel.options"
     * \return 配置项的值
     */
    int  getIntValue(std::string key);

    /*!
     * \brief getFloatValue 获取单项配置的值(Float类型)
     * \param key 配置项名称，如"Media.echoCancel.options"
     * \return 配置项的值
     */
    float getFloatValue(std::string key);

    /*!
     * \brief getBoolValue 获取单项配置的值(Float类型)
     * \param key 配置项名称，如"Media.echoCancel.options"
     * \return 配置项的值
     */
    bool  getBoolValue(std::string key);

    /*!
     * \brief setValue 设置单项配置的值(String类型)
     * \param key 配置项名称，如"Media.echoCancel.options"
     * \param value 配置项的值
     * \return 成功与否
     */
    bool  setStringValue(std::string key,std::string value);

    /*!
     * \brief setValue 设置单项配置的值(int 类型)
     * \param key 配置项名称，如"Media.echoCancel.options"
     * \param value 配置项的值
     * \return 成功与否
     */
    bool  setIntValue(std::string key,int value);

    /*!
     * \brief setValue 设置单项配置的值(bool 类型)
     * \param key 配置项名称，如"Media.echoCancel.options"
     * \param value 配置项的值
     * \return 成功与否
     */
    bool  setBoolValue(std::string key,bool value);

    /*!
     * \brief setValue 设置单项配置的值(float 类型)
     * \param key 配置项名称，如"Media.echoCancel.options"
     * \param value 配置项的值
     * \return 成功与否
     */
    bool  setFloatValue(std::string key,float value);


};

#endif // LIBCONFIG_H
