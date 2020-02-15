# 使用 GNU gettext 实现本地化

## 安装

* 从[官网](http://www.gnu.org/software/gettext/)下载压缩包
* 编译

    ```bash
    ./configure
    make
    make install
    ```

## 示例

```c
#include <stdio.h>
#include <locale.h>
#include <libintl.h>

#define _(str) gettext(str)

int main(int argc, char *argv[]) {
  setlocale (LC_ALL, "");
  bindtextdomain ("helloworld", "locale");
  textdomain ("helloworld");
  printf (_("Hello world!\n"));
  return 0;
}
```

## 说明

* `<locale.h>` 头文件包含 `setlocale()` 函数声明
* `<libintl.h>` 头文件包含 `gettext` 所使用的函数声明
* 第 8 行 `setlocale()` 进行区域设置，第二个参数为 `""` 使用当前操作系统默认的地域设置
* 第 9 行 `bindtextdomain()` 设置包含给定 domain 的语言文件的基目录，这里 domain 设置为 `helloworld`，基目录是当前目录下的 `locale` 目录，也可以是系统目录 `/usr/share/locale`
* 第 10 行 `textdomain()` 设置当前程序的 domain，需要与 `bindtextdomain` 中的 domain 保持一致（这里是 `helloworld`）
* 将所有要翻译的字符串写成 `gettext("some string.")` 的形式，如程序中原来的 `printf ("Hello world!\n");` 要写成 `printf (gettext ("Hello world!\n");` 根据 GNU 标准也可以使用 `_` 代替 `gettext`，这样可以少写几个字

## 编译

* 编译源文件生成 `helloworld` 程序：

  ```bash
  gcc -o helloworld helloworld.c
  ```

* 生成翻译文件 `helloworld.po`，将 `_` 当成 Keyword，并使用 `-d helloworld` 参数设置 `default-domain`：

  ```bash
  xgettext -k_ -d helloworld helloworld.c
  ```

按照 `bindtextdomain()` 中的参数，建立目录结构：

* 在 helloworld 当前目录下建立 locale 目录，locale 目录下建立你的语言目录，如：zh_CN、zh_TW 等，所有可以使用的名字参考 `setlocale()` 的第二个参数。在所有的语言目录下建立 `LC_MESSAGES` 目录。下面建立目录结构，假设你已经位于 helloworld 的当前目录下：

  ```bash
  mkdir -p locale/zh_CN/LC_MESSAGES
  mkdir -p locale/zh_TW/LC_MESSAGES
  ```

* 将 `helloworld.po` 拷贝到所有语言目录下：

  ```bash
  cp helloworld.po locale/zh_CN/LC_MESSAGES
  cp helloworld.po locale/zh_TW/LC_MESSAGES
  ```

* 编辑各个语言文件 `*.po`，具体如何翻译，可以自己查阅资料进行翻译。翻译完成后生成 `*.mo` 文件：

  ```bash
  msgfmt -o locale/zh_CN/LC_MESSAGES/helloworld.mo helloworld.po
  msgfmt -o locale/zh_TW/LC_MESSAGES/helloworld.mo helloworld.po
  ```

* 下面运行 `helloworld`，其中的 `Hello world!\n` 已经显示翻译后的词了！

  ```bash
  LANGUAGE=zh_CN.UTF-8 ./helloworld
  ```

## 参考资料

1. [GNU gettext](http://www.gnu.org/software/gettext/)
2. [红孩儿你好](https://www.cnblogs.com/274914765qq/archive/2015/11/17/4972315.html)
3. [liutao的博客](http://blog.sina.com.cn/s/blog_593abc890100noo7.html)
