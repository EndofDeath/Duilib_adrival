
这是Duilib的一个改进副本。
改进部分简述：
1.为控件CEditUI增加readonly背景色属性，以提供；
2.为控件CTextUI增加editcontrol属性，以改进多行显示；
3.修复控件CRichEditUI关于输入法的数个bug；

4.扩展了xml中<Default/>标签功能，例如：
  添加Default标签，name属性内容自定义
  <Default shared="true" name="testbtn" value="textcolor=&quot;#000000&quot; hottextcolor=&quot;#FFFFFF&quot;" />
  在控件标签中使用default属性
  <Button name="btn1" default="testbtn" />
  <Button name="btn2" default="testbtn" />
  等同于
  <Button name="btn1" textcolor="#000000" hottextcolor="#FFFFFF"/>
  <Button name="btn2" textcolor="#000000" hottextcolor="#FFFFFF"/>
  
 5.增加窗口接管类CWndDUI，以便于接管已有的window窗口的窗口过程及消息转发。详见MFCdemo；
 6.实现了右键菜单的改进版本，详见MenuDemo。
 
