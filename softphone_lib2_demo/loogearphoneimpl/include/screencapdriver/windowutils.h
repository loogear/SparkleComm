#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H
#include <string>
#include <vector>
#include "screencapdriver_global.h"


class SCREENCAPDRIVER_EXPORT WindowUtils
{
public:
    WindowUtils();
    ~WindowUtils();
  bool listWindows();
  static std::string getTempDir();
 long getWindowList(std::vector<WindowInfo> &windowList,bool fetchIcon=true);


};

#endif // WINDOWUTILS_H
