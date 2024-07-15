by NavyZhang 710605420@qq.com  
支持所有版本！请自行修复UE5.3.2之后引入的paper2D  bug，否则会陷入无限递归调用的bug。  修复如下图
![QQ_1721027944037](https://github.com/user-attachments/assets/5f06e880-22c4-40b4-9830-796dda3f13cf)

***无版权，放心使用***  
![UEAtlasLogo](https://github.com/hanbim520/UnrealAtlas/assets/7490792/0ca0b361-fbe8-4924-85fe-8e8634696c8f)  

使用方法：  
开启Paper2D及atlas功能  
1、参考plugin的content内AtlasTool工具  
2、图集说明  
    图集使用直接进行最小化尺寸计算，默认对于一个图集多张图的情况进行自动拆分为多张图对应多个图集，如果这个设定不喜欢，可以关闭DealMultipleTexturesToSingle函数。不过这个设定避免了一个图集多张图的最后一张图浪费的情况。  
  举例说明  
    比如Bag目录下有300个图片，图集采用2048x2046大小，可能需要3张2048x2046的图才能满足（如果是一个图集对应多个texture），插件优化为2张2048x2048加上最后一张的自适应尺寸，但是会有多个atlas产生。  
    a、自动化生成atlas和sprite  
    b、支持图利用率分析。  
    c、支持图集预览  

3、可以根据项目需要，添加到项目内的command的系统，避免使用第三方插件。    
有问题请反馈！  
QQ群  
![image](https://github.com/hanbim520/UnrealAtlas/assets/7490792/3f1799b2-e078-496b-bfca-21d8d21c8cff)
