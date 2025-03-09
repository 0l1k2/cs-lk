movl $0x4cd0e218, %eax   #设置返回值为cookie
leal 0x28(%esp), %ebp    #恢复原ebp的值
push $0x8048e3a        #返回后下一条指令的地址
ret                      #返回
