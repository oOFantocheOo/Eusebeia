========
Eusebeia
========

Eusebeia 是一款智能门卫。


在线浏览 Eusebeia 文档
----------------------

访问： https://eusebeia.readthedocs.io/zh_CN/latest/


如何编写该项目的代码
--------------------

见具体实现中的贡献指南 (CONTRIBUTING.rst)

*预备工作：学习 Git 的基本操作*


如何生成该文档 (HTML, PDF)
--------------------------

0. 正确安装 Python_ (3)

   .. _Python: https://www.python.org
#. 创建虚拟环境并进入：

     `python3 -m venv .venv`

     `source .venv/bin/activate` (\*nix)

     `.venv\\Scripts\\activate.bat` (Windows)

#. 安装 Sphinx：
     
     `pip install Sphinx`

#. 构建文档：

     `make *fotmat*` （输入 `make` 查看所有选项）

   文档将在 _build 文件夹下生成。

