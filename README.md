node-clearsilver
================

clearsilver local addon


  npm -g install node-gyp

  #安装特定版本的编译文件
  node-gyp install 0.6.21
  
  #使用指定版本的文件来编译扩展
  node-gyp configure --target=0.6.21
  
  #编译
  node-gyp build --debug
