QT +=xml

DEFINES +=USING_QT_ENV
INCLUDEPATH += \
      $$PWD/include/



win32{
message("using win32")
LIBPATH=$$PWD/lib/win32/
#LIBS +=$$PWD/lib/win32/softphonesdk.lib
LIBS +=$$LIBPATH/softphonesdk.lib
}
BUILD_LIB2=for_debug
android{
message("using android")
contains(BUILD_LIB2,for_debug){
LIBPATH=/data/soft-develop/leiqiangsoftphone/build-softphone_lib2-Android_for_armeabi_GCC_4_9_Qt_5_5_0_cafe3f-Debug/android-build/libs/armeabi
}
contains(BUILD_LIB2,for_product){
LIBPATH=$$PWD/lib/android/armv5
}
LIBS +=-L$$LIBPATH
LIBS +=-lsoftphonesdk
}

macx{
message("using mac os x")

LIBPATH=$$PWD/lib/Darwin/x86_64
LIBS +=-L$$LIBPATH
LIBS +=-lsoftphonesdkv4
}
linux!android{
message("using linux")
LINUX_DISTRIBUTION = $$system(lsb_release -is)
LINUX_CODENAME = $$system(lsb_release -cs)
LIBPATH=$$PWD/lib$$QMAKE_HOST.os/$$LINUX_DISTRIBUTION"_"$$LINUX_CODENAME/$$QMAKE_HOST.arch
LIBS +=-L$$LIBPATH
LIBS +=-lsoftphonesdkv4
}
ios{
message("using ios")
    contains(BUILD_LIB2,for_debug){
    LIBPATH=/data/soft-develop/leiqiangsoftphone/build-softphone_lib2-iphoneos_clang_Qt_5_5_1_for_iOS-Debug
    }
    contains(BUILD_LIB2,for_product){
    LIBPATH=$$PWD/lib/android/armv5
    }
LIBS +=-L$$LIBPATH
LIBS +=-lsoftphonesdk -lresolv -framework  CFNetwork
}
macx{
message("using mac os x")
LIBS +=-lz -lresolv -lresolv
}
message(LIBPATH=$$LIBPATH)

