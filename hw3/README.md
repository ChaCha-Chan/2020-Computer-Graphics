# 作业3  使用GLSL

要求：绘制一个小球并使用GLSL实现不同着色模型

**必要：**

- 实现环境光反射，漫反射，镜面反射
- 比较三种着色模型的效果
  - Flat， Gouraud， Phong
- 比较其他绘制参数变化时，绘制结果的变化
  - 如不同反射类型的光照强度，镜面反射中的指数$\alpha$等
- 比较小球细分程度不同时，绘制结果的变化

**可选：**

- 对小球进行形变，使用多个光源
- 根据顶点在三维空间中的位置指明颜色 (已实现)
- 根据片元在二位屏幕空间上的法向量变化指明颜色
