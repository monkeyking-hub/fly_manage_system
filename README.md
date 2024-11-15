# 重新修改了一下调度类

## 写了一个system类
新写好一个前端就在system的start函数里面注册进去
然后在构造函数里面取消注册

可以修改main函数的来使得第一张展示的的界面,可以用来查看自己写的前端界面
```cpp

InterfaceManager::instance()->switchToPage("/MainWindow/Beta");

```


后续也可以通过这个函数来跳转,只要输入当初注册的名字就行
