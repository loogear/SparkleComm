#ifndef MEDIACALLBACK
#define MEDIACALLBACK
class MediaCallBack{
public:
virtual int playBufferCallBack(char *buffer,size_t len)=0;
virtual   int readDataFromMicCallBack(char *buffer,size_t len)=0;

};
#endif // MEDIACALLBACK

