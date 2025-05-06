#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QMutex>
#include <QAtomicPointer>

class HttpClient : public QObject {
    Q_OBJECT

public:
    // 回调接口定义
    class HttpResponse {
    public:
        virtual void onResponse(int code, const QByteArray &data) = 0;
        virtual ~HttpResponse() = default;
    };

    class LambdaHttpResponse : public HttpResponse {
    public:
        // 定义函数指针类型
        using Callback = std::function<void(int, const QByteArray&)>;

        // 构造函数接收Lambda
        LambdaHttpResponse(Callback cb) : m_callback(cb) {}

        // 实现纯虚函数
        void onResponse(int code, const QByteArray &data) override {
            m_callback(code, data);
        }

    private:
        Callback m_callback;
    };

    // Profile 结构体定义
    struct ServerConfig {
        QString serverAddr;
        int port = 0;
    };

    struct Profile {
        QString profileName;
        QString appServerBaseUrl;
        ServerConfig ass;
        ServerConfig im;
        ServerConfig push;
        // 其他字段根据需求补充...
    };

    // 构造函数
    explicit HttpClient(const QString &host, int port, bool ssl, QObject *parent = nullptr);
    static HttpClient* getInstance();
    static HttpClient* __instance;

    // 初始化及登录方法
    void init(const QString &apiKey, const QString &apiSecret);
    void login(const QString &uid, const QString &pwd, const QString &renterId, const QString &isSip);
    void queryBindByPhone(const QString &accountID, const QString &renterId);
    void getSipServerInfo(QString &addr, int &port);

    // 获取配置和Token
    static Profile* getConfProfile();
    static QString token();

signals:
    void loginFinished(bool success, QByteArray data);

private:
    QNetworkAccessManager m_networkManager;
    QString m_baseUrl;
    static Profile m_profile;
    static QString m_token;
    static QMutex m_profileMutex;

    // 内部方法
    void getServerAddress();
    void fetch(const QString &url, const QUrlQuery &formData, const QJsonDocument &json, HttpResponse *res, bool async = true);
    Profile* parseProfile(const QJsonDocument &json);
    static bool isUiThread();

};

#endif // HTTPCLIENT_H
