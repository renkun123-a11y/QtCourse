# QtCourse

东莞理工学院《Qt 程序设计》课程作业仓库。

## 目录结构

```
QtCourse/
└── samp2_4App/          # 示例 2-4：基于 Qt 的简易文本编辑器（教学示例 + 课后改造）
    ├── main.cpp
    ├── qwmainwind.h / .cpp / .ui
    ├── res.qrc
    ├── samp2_4.pro
    └── images/
```

## samp2_4App 说明

原程序是教材示例 2-4 的简易文本编辑器，具备新建 / 打开 / 剪切 / 复制 / 粘贴 /
清空 / 字体设置 / 粗体 / 斜体 / 下划线以及状态栏提示、工具栏字体字号控件等功能。

本仓库在原示例基础上做了如下改造：

**新增“关于”工具按钮**：在主工具栏末尾增加了一个“关于”按钮，
点击后弹出 About 对话框，显示作者姓名与学号。

- 动作名：`actAbout`
- 图标：`images/about.svg`（取自 [Remix Icon](https://github.com/Remix-Design/RemixIcon) 的 `information-line`）
- 槽函数：`QWMainWind::on_actAbout_triggered()`

## 开发环境

| 项目 | 版本 |
| --- | --- |
| Qt | 6.7.2 (MinGW 64-bit) |
| 编译器 | MinGW-w64 GCC 13.1.0 |
| 构建工具 | qmake + mingw32-make |
| 操作系统 | Windows |

## 编译与运行

```bash
mkdir build && cd build
qmake ../samp2_4App/samp2_4.pro
mingw32-make -j4
./release/samp2_4.exe
```

## 关于

- 姓名：张梓鸿
- 学号：2024414300227
