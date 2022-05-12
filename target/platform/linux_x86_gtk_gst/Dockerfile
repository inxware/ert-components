From debian:9
RUN git checkout inxware/ert-build-support
RUN git checkout inxware/ert-contrib-middleware
COPY configure /configure
ENTRYPOINT [./configure] CMD[linux_x86_gts_gst]
RUN make
RUN make targetenv
RUN make targetenv_deb
