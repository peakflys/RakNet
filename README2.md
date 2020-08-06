# Raknet接入项目整理

------

为了便于项目中快速接入可靠UDP，目前直接选择了第三方开源的实现**RakNet**

官网github地址：https://github.com/facebookarchive/RakNet.git

此项目从2014年后就再无更新维护信息，在此之前的几次更新过程中，文档没有做跟进更新，所以在移植到三个平台（Windows，Android，IOS）时，均出现了一些问题，目前内部代码分支版本为修复版本，同时结合本项目的需求对源码也做了一些增删。 
现整理如下：

> * 源码所做更改
> * Windows动态库编译
> * Android动态库编译
> * IOS静态库编译
> * Unity项目代码
> * 交互过程

------

## 源码所做更改

RakNet源码均在Source目录下，除修复编译报错的一些必要文件外，主要还做了如下更改

### 1. 文件增加
新增**ClientConnections.cpp、ClientConnections.h、ClientConnectionsWrap.cpp**三个文件，用于裁剪实现可靠UDP传输，封装基本的库接口，便于项目接入

### 2. bug修复
- [x] 修复可靠传输分包发送后组包过程中可能出错的问题（涉及文件ReliabilityLayer.h、ReliabilityLayer.cpp）
- [x] 修复IOS下解包序列化异常的问题（涉及文件ReliabilityLayer.cpp）
- [x] 修复IOS下特定路由器DNS解析等待时间过长问题（涉及文件RakPeer.cpp）
- [x] 修复特定情况下网络超时无返回问题（涉及文件RakPeer.cpp）
- [x] 修复连接过程中，连接请求队列多线程重入问题（涉及文件RakPeer.cpp）
- [x] 其他一些小改动（涉及文件BitStream.h、RakString.cpp）

## Windows动态库编译
工程文件RakNet_VS2008，目前项目中使用的动态库为VS2015编译产生。
选择解决方案中的DLL项目，生成Release版本的X86以及X64的dll文件，从生成目录（Lib文件夹）里复制对应的dll文件，并改名为RakNet.dll，放入项目中对应的插件平台目录x86以及x86_64

## Android动态库编译
使用NDK进行编译，所使用的Android.mk、Application.mk文件内容如下：
### Android.mk
```powershell
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := RakNet
MY_PREFIX := $(LOCAL_PATH)/RakNetSources/
MY_SOURCES := $(wildcard $(MY_PREFIX)*.cpp)
LOCAL_SRC_FILES += $(MY_SOURCES:$(MY_PREFIX)%=RakNetSources/%) 
include $(BUILD_SHARED_LIBRARY)
```
### Application.mk
```powershell
APP_PLATFORM := android-12
APP_STL := gnustl_static
APP_ABI := armeabi-v7a
```
把编译出来的动态库放入项目中安卓对应指令集目录（Plugins\Android\libs\armeabi-v7a\）

## IOS静态库编译
直接打开XCode工程RakNet_IOS_LIB.xcodeproj来编译执行，注意编译优化选项为-O2，使用默认的-Os或者更高的-O3编译优化，在连接断开时会触发多处内存不可用问题，怀疑为新编译器在编译这些老代码时，高优化级别下编译出的代码问题。

## Unity项目代码
项目文件及用途如下：
#### NetConnection.cs 
连接基类实现定义和实现了链接共有的数据和接口，UDPConnection和TCPConnection均继承自此基类
#### UDPConnection.cs 
实现UDP逻辑的类，主要是实现RakNet接口，并提供解封包以及同上层游戏代码所需接口的逻辑
#### RakNetWrapper.cs 
RakNet库接口的定义封装类

## 交互过程 

```seq
Client->RakNetLib: Connect
RakNetLib->Server: Connect Request
Note right of RakNetLib: handleshake
Note right of RakNetLib: ...... 
Server-->RakNetLib: Connect Result
RakNetLib-->Client: Receive/Timeout
Client->RakNetLib: Send Data
Note right of RakNetLib: Send in Queue
RakNetLib->Server: Send Data in Layer
Server->RakNetLib: Send Data
RakNetLib->Client: Receive Data(ID_USER_PACKET_ENUM)
Client->RakNetLib: Disconnect
RakNetLib->Server: Disconnect
Server-->RakNetLib: Disconnect
RakNetLib-->Client: Receive Data(ID_CONNECTION_LOST)
```

作者 [@jefftian]    
2018 年 06月 05日
