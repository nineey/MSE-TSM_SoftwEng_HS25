# Docker file for a CI/CD pipeline

FROM ubuntu:24.04

# folder for downloaded files
WORKDIR /apps
RUN mkdir -p /apps

# Install prerequisites
RUN \
    apt update \
    && apt install -y git python3 wget unzip \
    && apt install -y cmake build-essential ninja-build \
    && apt install -y gcc-arm-none-eabi gdb-arm-none-eabi libnewlib-arm-none-eabi 

# Install nano editor
RUN apt-get install -y nano

# Install doxygen and graphviz
RUN apt-get install -y doxygen graphviz

# Install ping utility
RUN apt-get update && apt-get install -y iputils-ping

# Install curl and others, needed to get SEGGER files
RUN apt-get install -y apt-transport-https ca-certificates curl software-properties-common
RUN apt-get install -y libx11-xcb1 libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-randr0 libxcb-render-util0 libxcb-shape0 libxcb-sync1 libxcb-util1 libxcb-xfixes0 libxcb-xkb1 libxkbcommon-x11-0 libxkbcommon0 xkb-data
RUN apt-get install -y udev

# Get a specific version of the arm toolchain
# x86:              xpack-arm-none-eabi-gcc-13.2.1-1.1-linux-x64.tar.gz
# Apple silicon:    xpack-arm-none-eabi-gcc-13.2.1-1.1-linux-arm64.tar.gz
RUN \
    cd /apps \
    && wget https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/download/v13.2.1-1.1/xpack-arm-none-eabi-gcc-13.2.1-1.1-linux-x64.tar.gz -O gcc-arm-none-eabi.tar.gz \
    && mkdir /opt/gcc-arm-none-eabi-13.2.1-1.1 \
    && tar -xvzf gcc-arm-none-eabi.tar.gz -C /opt/gcc-arm-none-eabi-13.2.1-1.1 --strip-components 1 \
    && rm gcc-arm-none-eabi.tar.gz \
    && ln -s /opt/gcc-arm-none-eabi-13.2.1-1.1/bin/* /usr/local/bin

# for gcov, have to use the arm-none-eabi-gcov one
RUN \
    rm /usr/bin/gcov \
    && cd /usr/bin \
    && ln -s /opt/gcc-arm-none-eabi-13.2.1-1.1/bin/arm-none-eabi-gcov gcov

# Install gcovr (needs pip) and virtual environment
RUN \
  apt install -y python3-pip python3.12-venv

RUN \
    cd /apps \
    && python3 -m venv venv \
    && ./venv/bin/pip install gcovr

# Install SEGGER
# for x86:           JLink_Linux_V810g_x86_64.deb
# for Apple silicon: JLink_Linux_V810g_arm64.deb
RUN \
    cd /apps \
    && curl -d "accept_license_agreement=accepted&submit=Download+software" -X POST -O "https://www.segger.com/downloads/jlink/JLink_Linux_V810g_x86_64.deb" 
# in case issue with mismatch between J-Link version between host and container/image: use matching version
# issue with udev, see https://forum.segger.com/index.php/Thread/8953-SOLVED-J-Link-Linux-installer-fails-for-Docker-containers-Error-Failed-to-update/
RUN \
    cd /apps \
    && dpkg --unpack JLink_Linux_V810g_x86_64.deb \
    && rm -f /var/lib/dpkg/info/jlink.postinst \
    && dpkg --configure jlink \
    && apt install -yf

# Install Pico SDK
RUN \
    cd /apps \
    && git clone https://github.com/raspberrypi/pico-sdk.git --branch master \
    && cd pico-sdk/ \
    && git checkout tags/2.0.0 \
    && git submodule update --init
    
# Set the Pico SDK environment variable
ENV PICO_SDK_PATH=/apps/pico-sdk/

# Patch the SDK for semihosting file I/O (needed for gcov)
COPY .devcontainer/newlib_interface.c.patched      ${PICO_SDK_PATH}/src/rp2_common/pico_clib_interface/newlib_interface.c

# Build picotool so we don't have to build it for the project
RUN \
    cd /apps \
    && git clone https://github.com/raspberrypi/picotool.git --branch master \
    && cd picotool/ \
    && git checkout tags/2.0.0 \
    && git submodule update --init \
    && mkdir build && cd build && cmake ../ && make -j8 \
    && cmake --install .

# Copy needed project files into image
COPY CMakePresets.json          /project/
COPY CMakeLists.txt             /project/
COPY McuLib                     /project/McuLib/
COPY gcovr                      /project/gcovr/
COPY doxy                       /project/doxy/
COPY src                        /project/src/

# Build project
RUN \
  cd /project \
  && cmake --preset Debug \
  && cmake --build --preset app-debug \
  && cmake --preset Release \
  && cmake --build --preset app-release \
  && cmake --preset Test \
  && cmake --build --preset app-test

# Create documentation
RUN \
  cd /project/doxy \
  && doxygen Doxyfile

# run tests: possible latency issues with running on git server: do it manually
#RUN \
#    cd /project \
#    && ctest -v --test-dir build/Test --timeout 120 --output-on-failure

# Command that will be invoked when the container starts
ENTRYPOINT ["/bin/bash"]
