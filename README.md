# TSM_PicoW_Blinky
Project using the Pico-W RP2040 board with VS Code, implementing a simple blinky program.

It includes as well the 'sample' program, used for the 'scientific debugging' part. The program implements the ShellSort to sort a list of numbers.
Connect with a terminal to the VCOM of the USB port of the board.

## Build
Project has `Debug`, `Release` and `Test` targets, using CMake Presets. 

Configure:
```
cmake --list-presets
cmake --preset Debug
cmake --preset Release
cmake --preset Test
```

Build:
```
cmake --build --list-presets
cmake --build --preset app-debug
cmake --build --preset app-release
cmake --build --preset app-test
```

Test:
```
cmake --preset Test
cmake --build --preset app-test
ctest --list-presets
ctest --test-dir build/Test -R Led_1
```

## Debugging
Use `J-Link cortex-debug' launch config for a direct USB J-Link connection.

If using a DevContainer, use the 'inside DevContainer' launch config. You have to launch the debug server on the host first with
```
JLinkGDBServerCL -select USB -device RP2040_M0_0 -endian little -if SWD -rtos GDBServer/RTOSPlugin_FreeRTOS
```

- Note 1: On a Linux host, it is JLinkGDBServerCLExe
- Note 2: Make sure you have selected the correct build target (VS Code bottom toolbar)
- Note 3: If using semihosting or coverage, the current directory *has to be the project folder*. Otherwise, the data files are not stored in the correct location.
- Note 4: if using an IP address for the debug probe, configure `.devcontainer\devcontainer.env`

## Scientific Debugging
The application includes the 'sample' program of the 'scientific debugging' part, enabled with the `PL_CONFIG_USE_SAMPLE` setting in `platform.h.

Open a serial terminal (Termite/PuTTy or the 'Serial Monitor' in VS Code). Connect to the VCOM (115200, CRLF at the end). Type in 'help' to get some guidance.
Example in using Serial Monitor in VS Code:
```
---- Opened the serial port COM181 ----
---- Sent utf8 encoded message: "help\r\n" ----
Sample program. Type in numbers, separated by spaces. Press <ENTER> at the end.
---- Sent utf8 encoded message: "20 5 3\r\n" ----
Output: 3 5 20 
```

## Doxygen
Run task: Menu Terminal -> Run Task... -> doxygen

## Coverage
Enable `PL_CONFIG_USE_GCOV` in `src/platform.h` and use the `Debug` build.
Run the application with the debugger.

Note: with DevContainer, the current directory of the GDB server needs to be the **project root folder**:
```
JLinkGDBServerCL -select USB -device RP2040_M0_0 -endian little -if SWD -rtos GDBServer/RTOSPlugin_FreeRTOS
```

If using DevContainer, you have to install `gcovr` with `pip`.
First we need to create a new local python environment in a folder (`penv`):
```
cd /workspace
python3 -m venv penv
./penv/bin/pip install gcovr
```
After that, one can use 'gcovr' with the python `penv` environment:
```
./penv/bin/gcovr .
```

To generate a text report (use `./penv/bin/gcovr` with a virtual environment):
```
gcovr .
```
To generate a HTML report:
```
gcovr --html-details -o ./gcovr/main.html
```
or use Terminal -> Run Task... -> gcovr

You can use the `gcov` utility on a file like this:
```
gcov build/debug/srcLib/CMakeFiles/srcLib.dir/main.c.gcda
```

To see coverage in VS Code: `ctrl`+`shift`+`p`: `Gcov Viewer: Show`

## Testing
Build  with `Test` build configuration. Disable `PL_CONFIG_USE_GCOV` in `src/platform.h`.

If using DevContainer, you *have* to use an IP connection to the debug probe/server on the host.
For this, start a GDB server on the host for debugging (`JLinkGDBServerCLExe` on Linux):
```
JLinkRemoteServerCL
```
The file `src/tests/CMakeLists.txt` uses an environment variable `PICO_SEGGER_IP` (if defined). This is the IP address of the host with the J-Link debug probe. You can set that IP address in `.devcontainer\devcontainer.env`.


Manual Test Runner run (e.g. Windows host with J-Link attached), from the project root folder:
```
JRun --device RP2040_M0_0 --pc off --sp off --if SWD --args "Led_1" build/Test/TSM_PicoW_Blinky.elf
```
To run it from a DevContainer with the address of the host:
```
JRun --device RP2040_M0_0 --pc off --sp off --if SWD --ip "192.168.65.254" --args "Led_1" build/Test/TSM_PicoW_Blinky.elf
```
Manual CTest run:
```
ctest --extra-verbose --test-dir build/Test --timeout 15 --output-on-failure -R Led_1
```

Otherwise, you have to use a tunnel service:
```
JLinkRemoteServerCL -UseTunnel -TunnelBySN -select="801048148"
```

