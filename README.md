# 🧠 M-DSP RTOS

**A Real-Time Operating System Optimized for Digital Signal Processing**

---

## 🔍 Overview

**M-DSP RTOS** is a lightweight, modular real-time operating system designed for **real-time digital signal processing (DSP)** workloads in **embedded and edge computing**.
Unlike general-purpose RTOSes such as FreeRTOS, Zephyr, or RIOT that focus on multitasking and device abstraction, M-DSP is built around **deterministic task timing**, **low-latency execution**, and **energy-efficient processing of continuous signal data**.

Our focus is not just scheduling multiple tasks — it's enabling **real-time computation pipelines** for DSP-based applications such as **audio effects, FFT analysis, filtering, and control systems**.

---

## ⚙️ Key Features (Phase 1)

* **Minimal Scheduler Core:** Cooperative scheduler for deterministic control.
* **Static Task Model:** Predictable memory usage, no dynamic allocation.
* **Portable Design:** Written in standard C for easy porting to microcontrollers and FPGAs.
* **Extensible Kernel Hooks:** Foundation to add tick interrupts, context switching, and IPC in later phases.
* **Header Modularity:** Clean separation between `src/` (implementation) and `include/` (API headers).

---

## 🚀 How It Differs from Conventional RTOS

| Feature              | M-DSP RTOS                              | Traditional RTOS (FreeRTOS/Zephyr) |
| -------------------- | --------------------------------------- | ---------------------------------- |
| **Focus**            | Real-time DSP pipelines                 | Generic multitasking               |
| **Timing Model**     | Deterministic tick loop                 | Dynamic task preemption            |
| **Memory**           | Static allocation                       | Dynamic/heap allocation            |
| **Target Platforms** | FPGAs, audio DSP boards, MCU simulators | MCU boards and SoCs                |
| **Design Goal**      | Predictable latency for signal streams  | Task throughput                    |

---

## 🎧 Why DSP Focus?

Digital Signal Processing systems — like **audio equalizers**, **guitar effects**, or **real-time spectral analyzers** — demand **consistent timing** and **microsecond-level task precision**. Traditional RTOSes can introduce jitter or unpredictable latency due to preemptive scheduling.

M-DSP RTOS solves this by offering a **deterministic, cooperative execution model**, where DSP tasks can be scheduled in known time slices aligned with **sample processing windows**.

---

## 🧩 Future Extensions (Planned)

* [ ] Preemptive scheduling and context switching
* [ ] Tick timer ISR integration
* [ ] IPC and message queues for DSP task chains
* [ ] Real-time profiling and latency tracking
* [ ] Integration with I2S, ADC/DAC for real audio streams
* [ ] FPGA and STM32 demo targets

---

## 🧪 Build Instructions

```bash
# build for host simulator (Linux)
make clean && make                    # generates bin/mdsp_rtos
./bin/mdsp_rtos                      # run the ELF on your PC
```

### Flashing to an ESP32‑S3 device
```
# requirements: xtensa-esp32s3 toolchain, esptool.py (Python package)
# adjust PORT variable in Makefile or override on the command line
make clean && make                  # produces bin/kernel.elf.bin image
make flash                          # writes the image at 0x1000
# open a serial monitor (115200) to watch boot messages
```
> **Note:** the ROM on the S3 has a built‑in first‑stage loader. it looks
> for a valid ESP image header at the first 0x1000 boundary in flash. the
> `elf2image` step above adds that header. you do *not* need a separate
> ESP‑IDF bootloader for very simple apps, though you can flash one at
> 0x0 if you want to add features (partitions, encryption, etc.).
> just make sure the blob you flash contains a proper image header, or the
> boot messages will once again be `invalid header: 0xffffffff`.


Directory structure:

```
projectdir/
│
├── src/          # Core source files (scheduler, kernel, main)
├── include/      # Header files (task.h, mdsp_rtos.h, etc.)
├── build/        # Intermediate object files
├── bin/          # Final executable
└── Makefile
```

---

## 🧠 Phase 1 Goals (Completed)

* Basic kernel initialization
* Cooperative task loop with round-robin scheduling
* Modular source and header directory separation
* Working Linux simulation target

---

## 🛠️ Phase 2 & Beyond

* Implement **preemptive scheduler** using simulated tick ISR
* Add **priority scheduling and synchronization primitives**
* Create **audio processing tasks** (e.g., filter, FFT) as real-time workloads
* Support for **hardware drivers (I2S, UART, GPIO)**

---

## 💡 Use Cases

* Real-time **audio DSP** (e.g., guitar pedal, audio filter, FFT visualizer)
* **Sensor fusion** with precise sampling intervals
* **FPGA DSP pipelines** with software task coordination
* **Embedded control systems** with deterministic timing

---

## 🧩 Contributions

We welcome contributions in:

* Scheduler design and optimization
* DSP task library extensions
* Porting to microcontrollers or FPGA soft cores
* Integration with open-source audio/DSP frameworks

---

## 🧰 License

MIT License — open for learning, modification, and research.
