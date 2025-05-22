#include "httpclient.h"
#include <QEventLoop>

// 静态成员初始化
HttpClient::Profile HttpClient::m_profile;
QString HttpClient::m_token;
QString HttpClient::m_renterId;
QMutex HttpClient::m_profileMutex;
HttpClient *HttpClient::__instance = nullptr;

HttpClient *HttpClient::getInstance()
{
    if (__instance == nullptr)
    {
        __instance = new HttpClient("183.230.190.196", 41833, false);
    }

    return __instance;
}

HttpClient::HttpClient(const QString &host, int port, bool ssl, QObject *parent)
    : QObject(parent)
{
    __instance = this;
    QString scheme = ssl ? "https://" : "http://";
    m_baseUrl = QString("%1%2:%3/SparkleComm").arg(scheme, host, QString::number(port));
    // 初始化获取服务器地址
    getServerAddress();
}

void HttpClient::getServerAddress()
{
    QString url = m_baseUrl + "/XMLReader/reader.do";
    QUrlQuery formData;

    qDebug() << url;
    fetch(url, formData, QJsonDocument(), new LambdaHttpResponse
          {
              [this](int code, const QByteArray & data)
              {
                  if (code >= 200 && code < 300)
                  {
                      QJsonDocument json = QJsonDocument::fromJson(data);

                      qDebug() << json;
                      if (json["success"].toBool())
                      {
                          QMutexLocker locker(&m_profileMutex);

                          // 存储新配置
                          Profile* newProfile = parseProfile(json);
                          m_profile = *newProfile;
                          qDebug() << "m_profile.sip:" << m_profile.ass.serverAddr << ":" << m_profile.ass.port;
                          m_baseUrl = newProfile->appServerBaseUrl;
                      }
                  }
              }
          }, false);
}

// 解析 Profile
HttpClient::Profile* HttpClient::parseProfile(const QJsonDocument &json)
{
    Profile* profile = new Profile();
    QJsonObject configList = json["configList"].toObject();
    profile->profileName = configList["a"].toString();
    profile->appServerBaseUrl = configList["c"].toString();

    // 解析 SIP 服务器配置
    QString sipAppIp = configList["f"].toString();
    if (!sipAppIp.isEmpty())
    {
        QStringList parts = sipAppIp.split(":");
        profile->ass.serverAddr = parts.value(0);
        profile->ass.port = parts.value(1).toInt();
        qDebug() << "sip:" << profile->ass.serverAddr << ":" << profile->ass.port;
    }

    // 解析 即时消息 服务器配置
    QString timelyIp = configList["d"].toString();
    if (!timelyIp.isEmpty())
    {
        QStringList parts = timelyIp.split(":");
        profile->im.serverAddr = parts.value(0);
        profile->im.port = parts.value(1).toInt();
    }

    // 解析 消息推送 服务器配置
    QString pushIp = configList["e"].toString();
    if (!pushIp.isEmpty())
    {
        QStringList parts = pushIp.split(":");
        profile->push.serverAddr = parts.value(0);
        profile->push.port = parts.value(1).toInt();
    }
    return profile;
}

// 发送 HTTP 请求
void HttpClient::fetch(const QString &url, const QUrlQuery &formData, const QJsonDocument &json,
                       HttpResponse *res, bool async)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");

    if (!m_token.isEmpty())
    {
        request.setRawHeader("Authorization", "Bearer " + m_token.toUtf8());
    }

    QNetworkReply *reply = nullptr;

    if (!json.isNull())
    {
        reply = m_networkManager.post(request, json.toJson());
    }
    else if (!formData.isEmpty())
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        reply = m_networkManager.post(request, formData.toString(QUrl::FullyEncoded).toUtf8());
    }
    else
    {
        reply = m_networkManager.post(request, "");
    }

    // 异步处理
    if (async)
    {
        QObject::connect(reply, &QNetworkReply::finished, [reply, res]()
        {
            if (reply->error() == QNetworkReply::NoError)
            {
                res->onResponse(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                                reply->readAll());
            }
            else
            {
                res->onResponse(reply->error(), reply->errorString().toUtf8());
            }

            reply->deleteLater();
        });
    }
    else
    {
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError)
        {
            res->onResponse(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                            reply->readAll());
        }
        else
        {
            res->onResponse(reply->error(), reply->errorString().toUtf8());
        }

        reply->deleteLater();
    }
}

void HttpClient::init(const QString &apiKey, const QString &apiSecret)
{
    QString url = m_baseUrl + "/api/get_token";
    // 创建JSON请求体
    QJsonObject jsonBody;
    jsonBody["apiKey"] = apiKey;
    jsonBody["apiSecret"] = apiSecret;
    QJsonDocument jsonDoc(jsonBody);
    // 创建空表单数据
    QUrlQuery formData;
    qDebug() << url << jsonBody;
    fetch(url, formData, jsonDoc, new LambdaHttpResponse
          {
              [](int code, const QByteArray & data)
              {
                  if (code >= 200 && code < 300)
                  {
                      QJsonDocument json = QJsonDocument::fromJson(data);

                      if (json["code"].toInt() == 0)
                      {
                          qDebug() << "init success" << json["msg"].toString();
                          QJsonObject jo = json["data"].toObject();
                          m_token = jo["token"].toString();
                          m_renterId = jo["renterid"].toString();
                          qDebug() << "renter:" << m_renterId << "token:" << m_token;
                      }
                      else
                      {
                          qDebug() << "init failed" << json["msg"].toString();
                      }
                  }
                  else
                  {
                      qDebug() << "init failed" << code;
                  }
              }
          });
}

// 登录方法
void HttpClient::login(const QString &uid, const QString &pwd, const QString &renterId,
                       const QString &isSip)
{
    QString url = m_baseUrl + "/mobile/login.do";
    QUrlQuery formData;
    formData.addQueryItem("phone", uid);
    formData.addQueryItem("pwd", pwd);
    formData.addQueryItem("renterId", m_renterId);
    formData.addQueryItem("isSip", isSip);
    qDebug() << "login:" << url << uid << pwd << m_renterId << isSip;
    fetch(url, formData, QJsonDocument(), new LambdaHttpResponse
    {
        [this, renterId](int code, const QByteArray & data)
        {
            if (code >= 200 && code < 300)
            {
                QJsonDocument json = QJsonDocument::fromJson(data);

                if (json["success"].toBool())
                {
                    qDebug() << "login success" << json;
                    m_token = json["token"].toString();
                    QString accountID = json["user"].toObject()["phone"].toString();
                    queryBindByPhone(accountID, renterId);
                }
                else
                {
                    qDebug() << "login failed" << json["error"].toString()
                             << json["errorStr"].toString();
                    emit loginFinished(false, data);
                }
            }
            else
            {
                qDebug() << "login failed:" << code;
                emit loginFinished(false, data);
            }
        }
    });
}

void HttpClient::queryBindByPhone(const QString &accountID, const QString &renterId)
{
    Q_UNUSED(renterId)
    QString url = m_baseUrl + "/mobile/queryBindByPhone";
    QUrlQuery formData;
    formData.addQueryItem("phone", accountID);
    formData.addQueryItem("renterId", m_renterId);
    qDebug() << url << accountID << m_renterId;
    fetch(url, formData, QJsonDocument(), new LambdaHttpResponse
          {
              [this](int code, const QByteArray & data)
              {
                  if (code >= 200 && code < 300)
                  {
                      QJsonDocument json = QJsonDocument::fromJson(data);

                      if (json["code"].toInt() == 0)
                      {
                          qDebug() << "queryBindByPhone success" << json["msg"].toString();
                          emit loginFinished(true, data);
                      }
                      else
                      {
                          qDebug() << "queryBindByPhone failed" << json["msg"].toString();
                          emit loginFinished(false, data);
                      }
                  }
                  else
                  {
                      qDebug() << "queryBindByPhone failed:" << code;
                      emit loginFinished(false, data);
                  }
              }
          });
}

void HttpClient::getSipServerInfo(QString &addr, int &port)
{
    addr = m_profile.ass.serverAddr;
    port = m_profile.ass.port;
}
