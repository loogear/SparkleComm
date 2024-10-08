#ifndef SCREENCAPDRIVER_GLOBAL_H
#define SCREENCAPDRIVER_GLOBAL_H
#include <string>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(SCREENCAPDRIVER_LIBRARY)
#  define SCREENCAPDRIVER_EXPORT Q_DECL_EXPORT
#else
#  define SCREENCAPDRIVER_EXPORT Q_DECL_IMPORT
#endif
typedef struct{
    int left=0;
    int top=0;
    int right=0;
    int bottom=0;
}WindowRect;
typedef struct{
    int x;
    int y;
}WindowPoint;
#define TITLE_MAX 255
typedef union{
    WindowPoint point;
    unsigned  long sourceID;
    char  title[TITLE_MAX];
}ExcludeData;
typedef enum{
    ExcludeType_None,
    ExcludeType_WinID,
    ExcludeType_WinPoint,
    ExcludeType_WinTitle,
    ExcludeType_MyprocessFirst,
}ExcludeType;
typedef struct{
    ExcludeType type;
    ExcludeData data;
}ExcludeWindow;

typedef struct{
    unsigned  long id;
    std::string title;
    unsigned long pid;
    std::string processname;
    std::string iconFile;
    int layer;
    WindowRect rect;
}WindowInfo;

typedef  enum{
    CAPTURE_SCREEN,
    CAPTURE_WINDOW,
    CAPTURE_MAX
}CAPTURE_TYPE;
typedef  enum{
    CAPTURE_FORMAT_VIDEO,
    CAPTURE_FORMAT_FILE,
    CAPTURE_FORMAT_MAX
}CAPTURE_FORMAT;
typedef  struct{
    size_t width=640;
    size_t height=480;
    size_t fps=15;
    long sourceID=0;
    WindowRect captureArea;
    CAPTURE_TYPE  captureType=CAPTURE_SCREEN;
    CAPTURE_FORMAT captureFormat=CAPTURE_FORMAT_VIDEO;
    ExcludeWindow excludeWindow;
}CaptueConfigure;
typedef struct {
    void (*frame_in)( uint8_t* data,size_t stride,size_t width,size_t height,
                      void *user_data);
}sreen_capture_op;
#endif // SCREENCAPDRIVER_GLOBAL_H
