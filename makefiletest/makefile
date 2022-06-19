# Version 1.0
# calc:add.c  div.c  main.c  mult.c  sub.c
#         gcc add.c  div.c  main.c  mult.c  sub.c -o calc

# Version 2.0 自动推导
# calc:add.o div.o main.o mult.o sub.o
# 	gcc add.o div.o main.o mult.o sub.o -o calc


# # 模式匹配 -> 通过一个公式, 代表若干个满足条件的规则
# # 依赖有一个, 后缀为.c, 生成的目标是一个 .o 的文件, % 是一个通配符, 匹配的是文件名
# # 模式匹配
# %.o:%.c
# 	gcc $< -c


# Version 3.0 自定义变量
# target = calc
# obj = add.o div.o sub.o mult.o main.o
# $(target): $(obj)
# 	gcc $(obj) -o $(target)
# %.o:%.c
# 	gcc $< -c

# Version 4.0 添加自定义变量 -> makefile中注释前 使用 #
# # 使用函数搜索当前目录下的源文件 .c
# src=$(wildcard *.c)
# # 将源文件的后缀替换为 .o
# # % 匹配的内容是不能被替换的, 需要替换的是第一个参数中的后缀, 替换为第二个参数中指定的后缀
# # obj=$(patsubst %.cpp, %.o, $(src)) 将src中的关键字 .cpp 替换为 .o
# obj=$(patsubst %.c, %.o, $(src))
# target=calc

# $(target):$(obj)
# 	gcc $(obj)  -o $(target)

# %.o:%.c
# 	gcc $< -c

# # Version last
target = calc
#搜索磁盘的源文件
src = $(wildcard *.c)
#后缀的替换
obj = $(patsubst %.c, %.o, $(src))
$(target):$(obj)
	gcc $(obj) -o $(target)
%.o:%.c
	gcc $< -c
#clear
.PHONY:clean
clean:
    # shell命令前的 - 表示强制这个指令执行, 如果执行失败也不会终止
	-rm $(obj) $(target)
	echo "hello, 我是测试字符串"
