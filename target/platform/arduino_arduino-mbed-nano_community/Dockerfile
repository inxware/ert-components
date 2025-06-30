# Use Debian 12 as the base image
FROM debian:12

# Set non-interactive mode for package installation
ENV DEBIAN_FRONTEND=noninteractive
ENV ARDUINO_CLI_VERSION=1.2.0
ENV ARDUINO_CLI_HOME=/home/inxware/.arduino15

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    wget \
    curl \
    git \
    zip \
    && apt-get clean

# Create a user and group with specific UID/GID
RUN addgroup --gid 1000 inxware && \
    adduser --uid 1000 --gid 1000 --disabled-password --gecos "" inxware

# Switch to the non-root user
USER inxware
WORKDIR /home/inxware

# Download and install the latest Arduino CLI
RUN wget -qO- https://downloads.arduino.cc/arduino-cli/arduino-cli_${ARDUINO_CLI_VERSION}_Linux_64bit.tar.gz | tar xzf - && \
    chmod +x arduino-cli

# Add Arduino CLI to PATH
ENV PATH="/home/inxware:$PATH"

# Update Arduino CLI and install cores
RUN arduino-cli core update-index && \
    arduino-cli core install arduino:megaavr@1.8.8 && \
    arduino-cli core install arduino:mbed_nano@4.2.2

# Add Arduino Libs
RUN arduino-cli lib install WiFiNINA@1.9.0 && \
    arduino-cli lib install Arduino_LSM6DS3@1.0.3 && \
    arduino-cli lib install ArduinoMqttClient
