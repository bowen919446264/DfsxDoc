手机编辑框架XEdit说明

1.工程介绍
1）xengine -- 引擎（可以包括时间线引擎），负责渲染、合成等任务。UI层只调用它提供的接口
2）xproject -- 工程，包括了工程相关的一系列接口，可以供xengine调用

2.开发工具
建议采用JetBrains Clion，因为他可以跨Windows、MacOS、Linux等众多平台，调试更加方便。

3.编译工具
采用CMake编译，在JetBrains Clion中可以无缝使用CMake

4.辅助编译脚本
1）单目标系统，单CPU架构编译
	configure -- 配置目标系统和目标CPU架构
	build -- 编译指定目标系统（如Android）指定CPU架构（如arm64-v8a），生成目标动（静）态库
2）一键编译iOS、Android、Windows的所有CPU架构的目标动（静）态库
	调用buildAll脚本即可

5.编译环境依赖
1）CMake
2）Cygwin等Windows上的linux模拟环境