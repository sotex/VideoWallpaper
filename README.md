# 视频壁纸

20210515 这个项目目前正在开发中，核心功能已经实现，剩下一些界面上的东西我还在慢慢做，后续时间充裕的情况下会慢慢加上。



#### 项目介绍

一个简单的用来在windows桌面壁纸层播放视频的软件。
详细介绍看这里[https://www.cnblogs.com/oloroso/p/9446299.html](https://www.cnblogs.com/oloroso/p/9446299.html)

当前仅仅在Windows10上做过测试，因为我也没有安装其他系统的机器了。
对于多显示器，当前会有点问题，会居中播放，而不是显示在某一个显示器。这个后期有时间了我再改。
还有字幕乱码的问题，这个也以后再研究怎么解决。

软件界面采用QT制作，视频播放使用libvlc进行。


#### 安装教程

编译后直接运行即可。
编译好的版本在这里[Win64](https://pan.baidu.com/s/1HN3wjQu8AX2L2x9JfV0UtQ)密码: 6bxk
20210515 上面的编译版本是之前 MFC 的版本，在新版本的 Windows 10 上可能无法使用。


#### 使用说明

![软件使用界面](doc/20210515.png)

#### 参与贡献

1. Fork 本项目
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request
