# stm32_shell
This project aims to make a shell which can debug online using stm32f10x. For example, we can show a global variable's current value 
or execute a function manually. And for that, it relates to symbol table and i will use arm-none-eabi cross compiler toolchain.

1. cd ./build/ 
2. make 
3. download the hex file to stm32 board

# 2018.3.18 update：
1. Add USART function and make a customized printf function;
2. Add a simple OS which can realize fundamental schedule;
   
# 2018.2.25:
  Now i have already built software development environment, but only implement a simple led test function and no shell.
  
