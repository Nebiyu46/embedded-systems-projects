# Embedded Systems Projects Collection

Comprehensive collection of embedded C projects for ARM7 LPC2124/LPC2148 microcontrollers using IAR Embedded Workbench and various development environments.

## 🚀 **Project Categories**

### 📁 **IAR Projects** (`iar-projects/`)
Original IAR Embedded Workbench projects with complete workspace configurations.

#### **Featured Projects:**
- **Blinky** - Basic LED control and GPIO operations
- **LCD Projects** - Character LCD interfacing and display control

### 📁 **Workbench Projects** (`workbench-projects/`)
Advanced embedded system implementations with multiple project types.

#### **Project Types:**
- **LED Blink Variations** - Different approaches to LED control
- **LCD Display Systems** - Advanced LCD interfacing with 4-bit mode
- **Interrupt Handling** - Vectored Interrupt Controller (VIC) implementation
- **Temperature Sensing** - ADC-based sensor reading with LED indicators
- **Digital Signal Processing** - Basic noise filtering implementations

### 📁 **LED Blink Projects** (`led-blink-projects/`)
Specialized LED control implementations with various timing and pattern approaches.

## 🔧 **Hardware Platform**

### **Target Microcontrollers**
- **LPC2124** - ARM7TDMI-S core, 256KB Flash, 16KB RAM
- **LPC2148** - ARM7TDMI-S core, 512KB Flash, 40KB RAM

### **Development Boards**
- NXP LPC2148 Development Board
- Custom PCB implementations
- Breadboard prototypes

### **Peripheral Interfaces**
- **GPIO**: General Purpose Input/Output control
- **LCD**: 16x2 Character displays (4-bit mode)
- **ADC**: Analog-to-Digital Converter (10-bit)
- **VIC**: Vectored Interrupt Controller
- **UART**: Serial communication
- **Timer**: Hardware timer implementations

## 🛠️ **Development Environment**

### **Primary IDE: IAR Embedded Workbench**
- **Version**: ARM 7.x or higher
- **Compiler**: IAR C/C++ Compiler for ARM
- **Debugger**: Integrated JTAG/SWD debugging
- **Features**: Real-time debugging, memory analysis, performance profiling

### **Alternative Tools**
- **Keil µVision** - ARM development environment
- **Eclipse CDT** - Open-source alternative
- **Code Composer Studio** - TI development platform

### **Hardware Debuggers**
- **J-Link** - Segger JTAG debugger
- **LPC-Link** - NXP proprietary debugger
- **OpenOCD** - Open-source debugging solution

## 📚 **Technical Implementation Details**

### **Core Concepts Demonstrated**

#### **1. GPIO Control & LED Management**
```c
// Basic GPIO configuration
PINSEL0_bit.P0_0 = 0;    // Select GPIO function
IO0DIR_bit.P0_0 = 1;     // Set as output
IO0SET_bit.P0_0 = 1;     // Turn LED on
IO0CLR_bit.P0_0 = 1;     // Turn LED off
```

#### **2. LCD Interfacing (4-bit Mode)**
- **Data Lines**: P0.4-P0.7 (4-bit data bus)
- **Control Lines**: RS, Enable, R/W
- **Initialization**: 8-bit to 4-bit mode transition
- **Commands**: Clear display, cursor control, character writing

#### **3. Interrupt Handling (VIC Implementation)**
```c
// VIC configuration for interrupt handling
void install_IRQ(unsigned int channel, void (*ISR)(void), unsigned int priority);
void install_FIQ(unsigned int channel, void (*ISR)(void));
```

#### **4. ADC Temperature Sensing**
- **Channel Configuration**: Single-ended input
- **Conversion**: 10-bit resolution (0-1023 values)
- **Temperature Mapping**: Voltage to temperature conversion
- **Display**: LED pattern representation of temperature ranges

### **Memory Management**
- **Code Placement**: Flash memory organization
- **Data Structures**: RAM allocation strategies
- **Stack Management**: Proper stack pointer configuration
- **Interrupt Vectors**: Vector table setup and ISR linking

## 🏗️ **Project Architecture**

### **File Organization**
```
embedded-systems-projects/
├── iar-projects/           # Original IAR workspace projects
│   ├── blinky/            # Basic LED blink
│   └── lcd/               # LCD display projects
├── workbench-projects/     # Advanced multi-project workspace
│   ├── Blink/             # Enhanced LED control
│   ├── interrupt/         # VIC interrupt handling
│   ├── tempInclass/       # Temperature sensor ADC
│   └── teachLDC/          # LCD education projects
└── led-blink-projects/     # Specialized LED implementations
```

### **Common Header Files**
- `iolpc2124.h` - LPC2124 register definitions
- `interrupt.h` - VIC and ISR declarations
- `lcd.h` - LCD control function prototypes

## 🚀 **Getting Started**

### **1. Environment Setup**
```bash
1. Install IAR Embedded Workbench for ARM
2. Configure compiler for LPC21xx series
3. Set up hardware debugger (J-Link recommended)
4. Connect target board via JTAG
```

### **2. Project Compilation**
```bash
1. Open .eww workspace file in IAR
2. Select target configuration (Debug/Release)
3. Build project (F7)
4. Download to target (Ctrl+D)
```

### **3. Debugging Process**
```bash
1. Set breakpoints in source code
2. Start debug session (Ctrl+D)
3. Use step-by-step execution
4. Monitor variables and registers
5. Analyze peripheral states
```

## 🔬 **Advanced Features**

### **Real-Time Debugging**
- **Live Variables**: Monitor values during execution
- **Peripheral Registers**: Real-time register viewing
- **Memory Browser**: Dynamic memory analysis
- **Call Stack**: Function call hierarchy tracking

### **Performance Analysis**
- **Execution Profiling**: Function timing analysis
- **Code Coverage**: Execution path verification
- **Memory Usage**: RAM/Flash utilization reports

### **Hardware Integration**
- **JTAG Debugging**: Full chip control and monitoring
- **Flash Programming**: In-system programming capabilities
- **Peripheral Simulation**: Hardware behavior modeling

## 📊 **Project Complexity Levels**

### **Beginner Level**
- **Basic LED Blink**: Simple GPIO control
- **Switch Input**: Basic digital input handling
- **Delay Functions**: Software timing implementation

### **Intermediate Level**
- **LCD Display**: Character display with formatting
- **ADC Reading**: Analog sensor interfacing
- **Timer Usage**: Hardware timer configuration

### **Advanced Level**
- **Interrupt Systems**: Complex ISR implementations
- **Multi-tasking**: Basic RTOS concepts
- **Communication**: UART and SPI protocols

## 🔧 **Hardware Configuration**

### **Standard Pin Assignments**
```c
// LED connections
#define LED1_PIN    0    // P0.0
#define LED2_PIN    1    // P0.1

// LCD connections (4-bit mode)
#define LCD_RS      2    // P0.2
#define LCD_EN      3    // P0.3
#define LCD_D4      4    // P0.4-P0.7

// ADC input
#define TEMP_ADC    0    // AD0.0 (P0.27)
```

### **Clock Configuration**
- **Crystal**: 12MHz external crystal
- **PLL**: Phase-Locked Loop for frequency multiplication
- **CCLK**: Core clock configuration (up to 60MHz)
- **PCLK**: Peripheral clock settings

## 🤝 **Educational Value**

### **Learning Objectives**
- **Embedded C Programming**: Bare-metal development
- **Hardware Interfacing**: Direct register manipulation
- **Real-Time Systems**: Timing-critical applications
- **Debugging Skills**: Professional debugging techniques

### **Industry Relevance**
- **IoT Development**: Foundation for IoT devices
- **Automotive Systems**: ECU development basics
- **Industrial Control**: Process control applications
- **Medical Devices**: Embedded medical equipment

## 📝 **Documentation Standards**

All projects include:
- **Detailed Comments**: Explaining hardware interactions
- **Pin Diagrams**: Hardware connection documentation
- **Timing Specifications**: Critical timing requirements
- **Testing Procedures**: Verification and validation steps

## 🤝 **Contributing**

1. Fork the repository
2. Create a feature branch for your embedded project
3. Follow the existing code style and documentation standards
4. Test thoroughly on actual hardware
5. Include comprehensive documentation
6. Submit a pull request with detailed description

## 📚 **References and Documentation**

- **LPC2148 User Manual** - NXP Semiconductor
- **ARM7TDMI-S Technical Reference Manual** - ARM Limited  
- **IAR C/C++ Compiler User Guide** - IAR Systems
- **Embedded Systems Design Patterns** - Industry best practices

---

**Perfect for**: Embedded systems students, ARM developers, IoT enthusiasts, hardware engineers
