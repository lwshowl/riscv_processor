# **tinyRV32**  
  
### 支持 RV32I 指令集 准备支持 CSR 指令  
### 多周期，非流水线设计，可以综合在 小熊猫 kintex7 base 开发板上  

<br>

## 资源 

**kintex-7**
|  LUT   | FIFO  |   BRAM  | LUTRAM | 
|  ----  | ----  |  ------ | ------ |
| 2609   | 1621  |   自定义 |  256   |

## 文件夹 

### 根文件夹
|  *.v                 |      Makefile        | 
|  ------------------  | -------------------- |  
|  测试用 verilog 代码  | verilator Makefile   |   

### **synthesis**

|  *.v                   | 
|  ------------------    |  
|  vivado 可综合的verilog  |    

### **test_benches**

|  *.cpp                   | 
|  ------------------      |  
|  verilator 测试用例        |

### **verify**

|  *.s                  | linker.ld          |   *.hex          |  *.disass
|  ------------------   |  ----------------  | ---------------- | -----------
|  汇编代码              |  gcc linker script |  用于测试的hex文件 |  反汇编文件
**verify/\***    gcc 测试代码


<br>
<br><br><br>

#### **support rv32i instruction set , csr instructions are scheduled**
#### **non piplelined multiple clock cycles per instruction**
#### **synthesisable for little panda kintex-7 base development board**

