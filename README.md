
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [NtSoftRender](#ntsoftrender)
  - [已实现功能](#已实现功能)
  - [尚未实现功能](#尚未实现功能)
  - [效果展示](#效果展示)
    - [经典的Blinn–Phong反射模型](#经典的blinnphong反射模型)
    - [线框模式](#线框模式)
    - [法线纹理应用](#法线纹理应用)
    - [阴影生成](#阴影生成)
    - [SSAO(屏幕环境光遮蔽)](#ssao屏幕环境光遮蔽)
  - [相关参考或书籍](#相关参考或书籍)

<!-- /code_chunk_output -->

# NtSoftRender

## 已实现功能

• 可编程的渲染管线，通过编写着色器（Shaders）来实现各种不同的效果
• 齐次裁剪(Homogeneous clipping)
• 透视插值(Perspective correct interpolation)
• 背面剔除
• 深度测试
• 经典的Blinn–Phong反射模型
• 切线空间法线贴图（Tangent space normal mapping）
• 阴影贴图（Shadow mapping）
• 透明混合
• 天空盒

## 尚未实现功能

• 折射和反射效果
• mtl文件格式解析，更多的3D文件格式读取
• 骨骼动画
• 多边形读入
• Physically based rendering（PBR）
• Image-based lighting（IBL）

## 效果展示

### 经典的Blinn–Phong反射模型

![avatar](\show\BP_diablo1.png "diablo")

![avatar](\show\BP_diablo2.PNG "diablo")

### 线框模式

![avatar](\show\wireframe_diablo.png "diablo")

### 法线纹理应用

无法线纹理
![avatar](\show\NT_BP_diablo.PNG "diablo")
法线纹理
![avatar](\show\TBP_diablo.PNG "diablo")

### 阴影生成

深度图

![avatar](\show\depth_diablo.PNG "diablo")

阴影图

![avatar](\show\shadow_diablo.PNG "diablo")

### SSAO(屏幕环境光遮蔽)

![avatar](\show\SSAO_diablo.PNG "diablo")

## 相关参考或书籍

<https://github.com/ssloy/tinyrenderer/wiki>
<https://www.zhihu.com/question/24786878/answer/820931418>
3D Game programming with DirectX11
Unity Shader入门精要