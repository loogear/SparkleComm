===================================
softphonesdk使用说明
===================================

概述
===================================
::

  本文档是SparkleComm Android SDK库的使用教程，请先把SparkleCommSDK-release.aar库集成到Android Studio开发环境里。
  Demo说明：
  HomeActivity.java 是主界面，包含了拨号功能；singleInstance模式。
  IncomingActivity.java 是来电界面；singleInstance模式。
  InCallActivity.java 是通话界面；singleInstance模式。
  LoogearApplication.java 实现前后台切换时，注册与注销的方法。

接口函数
===================================

PhoneApi是SDK的主要接口函数类；接口的具体使用流程可以参考DEMO APP的调用；

初始化SDK
------------------------
::

  /**
   * 初始化SDK，必须第一个被调用：每次进行注册时都先调用一次
   * @param context
   * @param sipEvent SIP消息接收接口，会在主线程回调
   */
  public void initSdk(Context context, ISipEvent sipEvent);
  

注册SIP服务器
------------------------
::

  /**
   * 注册到SIP服务器：initSdk调用后进行调用，注册SIP，注册消息会通过ISipEvent接口回调（initSdk函数里传递的）
   * @param serverAddr 服务器地址IP地址或者域名
   * @param port SIP服务器端口号
   * @param userID 用户ID如10011\@loogear.com或者10011
   * @param userPWD 注册密码
   * @param accountID 账号编号此处传入0
   * @param domain 域名如loogear.com，参考服务器的分配域名可能与serverAddr相同，也可能不同
   * @param authWithDomain 是否带域名认证，默认为false。参考服务器的要求。
   * @param idUri 参考服务器的注册要求，默认为空
   * @param useIMS3GPP 是否采用IMS的3gpp认证，参考服务器的注册要求
   * @param extraHeads 扩展消息格式标准为"X-HEAD_NAME1:head1 value;X-HEAD_NAME2:head2 value",头部消息间用";"分隔:
   *                   如:"X-SparkleComm-Call-DEVICE-ID: EF32D-98AF-9E01-882E;X-SparkleComm-Call-MSG2:this is other message"
   */
  public void registerToSipServer(String serverAddr, int port, String userID, String userPWD, int accountID,
                                  String domain, boolean authWithDomain, String idUri, boolean useIMS3GPP, String extraHeads);

注销SIP服务器
------------------------
::

   /**
   * 反注册SIP服务器
   * @param accountID
   */
  public void unRegister(int accountID)

拨打电话
------------------------
::

  /**
   * 发起带扩展消息的语音呼叫
   * @param calleeAdd 对方号码
   * @param extraData 扩展消息格式标准为"X-HEAD_NAME1:head1 value;X-HEAD_NAME2:head2 value",头部消息间用";"分隔:
   *                  如:"X-SparkleComm-Call-DEVICE-ID: EF32D-98AF-9E01-882E;X-SparkleComm-Call-MSG2:this is other message"
   * @param accountID 呼叫帐号编号(用于多账户注册)
   */
  public void makeVoiceCall(String calleeAdd, String extraData, int accountID);

挂断电话
------------------------
::

  /**
   * 挂断电话
   * @param callID
   * @param accountID
   */
  public void releaseCall(int callID, int accountID);

启用视频
------------------------
::

  /**
   * 是否打开视频通话功能
   * @param enable
   */
  public void enableVideo(boolean enable);

设置视频窗口
------------------------
::

  /**
   * 设置视频窗口
   * @param preview 预览窗口
   * @param remote 对方窗口
   */
  public void setVideoDisplays(Surface preview, Surface remote);
  
设置视频分辨率
------------------------
::

  /**
   * 设置视频显示分辨率
   */
  public void setVideoResolution(VIDEO_RES res);

旋转摄像头角度
------------------------
::

  /**
   * 旋转摄像头角度
   * @param angle
   */
  public void setVideoCaptureOrient(int angle);
  
开始录音
------------------------
::

  /**
   * 开始录音
   * @param path 录音文件存放目录
   * @param fileName 录音文件名
   * @param accountID 账户ID
   * @param callID 通话ID
   * @return 录音ID
   */
  public int startRecord(String path, String fileName, int accountID, int callID);

停止录音
------------------------
::

  /**
   * 停止录音：可以主动调用此函数停止录音，通话结束后也会自动停止录音
   * @param recordId 录音ID
   * @param accountID 账户ID
   * @param callID 通话ID
   * @return
   */
  public boolean stopRecord(int recordId, int accountID, int callID);

显示预览视频
------------------------
::

  /**
   * 显示预览视频
   */
  public void showVideoPrevieWindow();
  
隐藏预览视频
------------------------
::

  /**
   * 隐藏预览视频
   */
  public void hideVideoPrevieWindow();

呼叫保持
------------------------
::

  /**
   * 呼叫保持
   * @param accountID
   * @param callID
   */
  public void callHold(int accountID, int callID);

取消呼叫保持
------------------------
::

  /**
   * 取消呼叫保持
   * @param accountID
   * @param callID
   */
  public void releaseHold(int accountID, int callID);
  
接听
------------------------
::

  /**
   * 接听
   * @param callID
   * @param accountID
   */
  public void answerCall(int callID, int accountID);
  
发送DTMF消息
------------------------
::

  /**
   * 发送DTMF消息
   * @param digits
   */
  public void sendDtmf(String digits);

静音
------------------------
::

  /**
   * 设置MIC静音
   * @param on true=静音 false=取消静音
   */
  public void setMicMute(boolean on);
  
呼叫转移
------------------------
::

  /**
   * 呼叫转移
   * @param addr 目标地址
   * @param callID
   * @param accountID
   */
  public void callTransfer(String addr, int callID, int accountID);

获取所有通话信息
------------------------
::

  /**
   * 获取所有通话信息，通过ISipEvent.onUpdateCallInfo(List<CallInfo> callList);接口返回
   */
  public void getAllCallInfo();
  
是否在通话中
------------------------
::

  /**
   * 是否在通话中
   * @return
   */
  public boolean isInCalling();

切换摄像头
------------------------
::

  /**
   * 切换摄像头
   * @param toFront true=切换到前置摄像头  false=切换到后置摄像头
   */
  public void cameraSwitch(boolean toFront);

关闭视频
------------------------
::

  /**
   * 关闭视频：在视频通话时，去掉视频，只保留语音通话
   */
  public void closeVideo();

打开视频
------------------------
::

  /**
   * 打开视频，在进行语音通话时，增加视频
   */
  public void openVideo();


