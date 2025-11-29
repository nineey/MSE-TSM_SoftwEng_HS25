# This is a docker file which will be used with GitHub actions on GitHub
# The GitHub workflow will copy this file to the root of the project
# and then build the image plus creates the container

# Fetch ubuntu image
FROM ubuntu:24.04

# Install prerequisites
RUN \
    apt update \
    && apt install -y git python3 \
    && apt install -y cmake gcc-arm-none-eabi gdb-arm-none-eabi libnewlib-arm-none-eabi build-essential ninja-build

# Install nano editor
RUN apt-get install -y nano

# Install doxygen and graphviz
RUN apt-get install -y doxygen graphviz

# Install ping utility
RUN apt-get update && apt-get install -y iputils-ping

# Install curl and others, needed to get SEGGER
RUN apt-get install -y apt-transport-https ca-certificates curl software-properties-common
RUN apt-get install -y libx11-xcb1 libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-randr0 libxcb-render-util0 libxcb-shape0 libxcb-sync1 libxcb-util1 libxcb-xfixes0 libxcb-xkb1 libxkbcommon-x11-0 libxkbcommon0 xkb-data
RUN apt-get install -y udev

# Install SEGGER
RUN \
    mkdir -p /project \
    && cd /project \
    && curl -d "accept_license_agreement=accepted&submit=Download+software" -X POST -O "https://www.segger.com/downloads/jlink/JLink_Linux_V810g_x86_64.deb" 
# in case issue with mismatch between J-Link version between host and container/image: use matching version
# issue with udev, see https://forum.segger.com/index.php/Thread/8953-SOLVED-J-Link-Linux-installer-fails-for-Docker-containers-Error-Failed-to-update/
RUN \
    cd /project \
    && dpkg --unpack JLink_Linux_V810g_x86_64.deb \
    && rm -f /var/lib/dpkg/info/jlink.postinst \
    && dpkg --configure jlink \
    && apt install -yf

# Install Pico SDK
RUN \
    mkdir -p /project/src/ \
    && cd /project \
    && git clone https://github.com/raspberrypi/pico-sdk.git --branch master \
    && cd pico-sdk/ \
    && git checkout tags/2.0.0 \
    && git submodule update --init
    
# Set the Pico SDK environment variable
ENV PICO_SDK_PATH=/project/pico-sdk/

# Copy project sources into image
COPY CMakePresets.json          /project/
COPY CMakeLists.txt             /project/
COPY gcovr                      /project/gcovr/
COPY doxy                       /project/doxy/
COPY McuLib                     /project/McuLib/
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
#    && ctest -v --test-dir build/debug-test --timeout 120 --output-on-failure

# Command that will be invoked when the container starts
ENTRYPOINT ["/bin/bash"]
