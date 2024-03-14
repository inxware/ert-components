FROM inxware/ubuntu22.04

RUN apt-get update && \
    apt-get install -y mingw-w64 p7zip-full wget

#Installs the following:
#binutils-mingw-w64-i686/jammy,now 2.38-3ubuntu1+9build1 amd64 [installed,automatic]
#g++-mingw-w64-i686-posix/jammy,now 10.3.0-14ubuntu1+24.3 amd64 [installed,automatic]
#g++-mingw-w64-i686-win32/jammy,now 10.3.0-14ubuntu1+24.3 amd64 [installed,automatic]
#g++-mingw-w64-i686/jammy,jammy,now 10.3.0-14ubuntu1+24.3 all [installed,automatic]
#gcc-mingw-w64-i686-posix-runtime/jammy,now 10.3.0-14ubuntu1+24.3 amd64 [installed,automatic]
#gcc-mingw-w64-i686-posix/jammy,now 10.3.0-14ubuntu1+24.3 amd64 [installed,automatic]
#gcc-mingw-w64-i686-win32-runtime/jammy,now 10.3.0-14ubuntu1+24.3 amd64 [installed,automatic]
#gcc-mingw-w64-i686-win32/jammy,now 10.3.0-14ubuntu1+24.3 amd64 [installed,automatic]
#gcc-mingw-w64-i686/jammy,jammy,now 10.3.0-14ubuntu1+24.3 all [installed,automatic]
#mingw-w64-i686-dev/jammy,jammy,now 8.0.0-1 all [installed,automatic]

# Download NSIS and place makensis in $PATH
RUN mkdir -p /opt/nsis && \
    cd /opt/nsis && \
    wget https://github.com/electron-userland/electron-builder-binaries/releases/download/nsis-3.0.4.1/nsis-3.0.4.1.7z && \
    7z x nsis-3.0.4.1.7z && \
    rm nsis-3.0.4.1.7z && \
    ln -sv /opt/nsis/linux/makensis /usr/local/bin/makensis