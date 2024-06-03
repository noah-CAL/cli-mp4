FROM ubuntu:24.04
# Necessary for compiling FFmpeg from source
# https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu
RUN apt update -qq && apt -y install \
    bear \
    ffmpeg \
    gcc \
    gdb \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libavfilter-dev \
    libavdevice-dev \
    libswresample-dev \
    libswscale-dev \
    make

WORKDIR /src/
