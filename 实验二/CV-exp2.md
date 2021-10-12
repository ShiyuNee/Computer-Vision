### 调用摄像头一般格式

- 创建对象 VideoCapture capture
- 打开摄像头 capture.open(0)
- 传输每一帧的数据 capture >> frame
- 关闭摄像头 capture.release( )

### 输出视频一般格式

- 创建对象 VideoWriter writer

- 配置对象

  - 存储路径 ， 编码方式 ， 帧率（每秒的帧数） ， 视频大小

  ```c++
  writer.open("D:\\cLion\\project\\output01.avi" , VideoWriter::fourcc('M', 'J', 'P', 'G'), 5, Size(frame_width , frame_height));
  ```

- 保存每一帧的数据 writer.write(frame)
- 关闭输出 writer.release( ) **不要忘了关，要不然输出的视频没法看**

### waitKey

- waitKey(0) 将无限地显示窗口，直到任何按键按下(它适合于图像显示)
- waitKey(25) 将显示一个框架。25毫秒后，显示将自动关闭
- 如果waitKey处于一个循环中，里面的参数将显示视频读取的帧
- 当等待时间内无任何操作时，等待结束后，返回 -1
- 当等待时间内有输入字符时，返回返回字符的ASCII码对应的十进制值