# inxware/esp32s3_ubuntu22.04-build-essential
FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
    apt-get install -y \
	build-essential git \
	python3 python3-venv \
	python3-pip libffi-dev libbsd-dev \
	libssl-dev dfu-util libpython2.7 \
	libusb-1.0-0-dev flex libncurses-dev \
	gperf python3-setuptools gperf \
	git git-lfs curl zip unzip lcov \
	wget bison ca-certificates check \
	cmake ninja-build ccache && \
    apt-get autoremove -y && \
    update-alternatives --install /usr/bin/python python /usr/bin/python3 10
RUN python -m pip install  --upgrade pip
RUN wget https://dl.espressif.com/dl/esp-idf/espidf.constraints.v5.1.txt -O /opt/espidf.constraints.v5.1.txt
RUN python3 -m venv /opt/python_env
ENV PATH="/opt/python_env/bin:$PATH"
ENV IDF_PYTHON_ENV_PATH="/opt/python_env"
ENV IDF_TOOLS_PATH="/opt"
RUN pip install "setuptools>=21" "click>=7.0" "pyserial>=3.3" "future>=0.15.2" "cryptography>=2.1.4 --only-binary cryptography" \
    "pyparsing>=2.0.3,<2.4.0" "pyelftools>=0.22" "idf-component-manager~=1.2" "urllib3<2" "gdbgui==0.13.2.0" "pygdbmi<=0.9.0.2" \
	"python-socketio<5" "jinja2<3.1" "itsdangerous<2.1" "kconfiglib==13.7.1" "reedsolo>=1.5.3,<=1.5.4" "bitstring>=3.1.6,<4" \
	"ecdsa>=0.16.0" "construct==2.10.54" "littlefs-python>=0.9.1" "esptool>=4.0.0"
RUN pip install -U -r /opt/espidf.constraints.v5.1.txt
RUN addgroup --gid 1000 inxware && adduser --uid 1000 --gid 1000 inxware
RUN apt-get install -y xxd
#RUN pip install bitstring==3.1.9 Brotli==1.0.9 certifi==2022.5.18.1 cffi==1.15.0 charset-normalizer==2.0.12 click==8.1.3 colorama==0.4.4 construct==2.10.54 contextlib2==21.6.0 cryptography==37.0.2 ecdsa==0.17.0 Flask==0.12.5 Flask-Compress==1.12 Flask-SocketIO==2.9.6 future==0.18.2 gdbgui==0.13.2.0 gevent==1.5.0 greenlet==1.1.2 idf-component-manager==1.1.1 idna==3.3 itsdangerous==2.0.1 Jinja2==3.0.3 kconfiglib==13.7.1 MarkupSafe==2.1.1 pycparser==2.21 pyelftools==0.28 pygdbmi==0.9.0.2 Pygments==2.12.0 pyparsing==2.3.1 pyserial==3.5 python-engineio==3.14.2 python-socketio==4.6.1 PyYAML==6.0 reedsolo==1.5.4 requests==2.27.1 requests-toolbelt==0.9.1 schema==0.7.5 six==1.16.0 tqdm==4.64.0 urllib3==1.26.9 Werkzeug==0.16.1
