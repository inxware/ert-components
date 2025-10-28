

EHS_APRILTAG_PATH=$(EHS_TARGET_COMPONENT_HAL_PATH)/image_processing/apriltag
INC_DIRS+=$(EHS_APRILTAG_PATH)
VPATH+=$(EHS_APRILTAG_PATH)
VPATH+=$(EHS_APRILTAG_PATH)/common

TOOLCHAIN_ROOT_PATH :=/usr
TOOLCHAIN_PREFIX := x86_64-linux-gnu-
OBJ += o

CC := ${TOOLCHAIN_ROOT_PATH}/bin/${TOOLCHAIN_PREFIX}gcc
AR := ${TOOLCHAIN_ROOT_PATH}/bin/${TOOLCHAIN_PREFIX}ar
RANLIB := ${TOOLCHAIN_ROOT_PATH}/bin/${TOOLCHAIN_PREFIX}ranlib


APRILTAG_C_SRC := $(wildcard *.c)
APRILTAG_C_SRC += $(wildcard common/*.c)
APRILTAG_C_EXECUTABLE := $(APRILTAG_C_SRC:.c=.$(OBJ))

INCLUDE_FLAG := -I/home/xiaosheng/Github_Repos/apriltag

all:$(APRILTAG_C_EXECUTABLE)
	${AR} qc libapriltag.a $(APRILTAG_C_EXECUTABLE)
	${RANLIB} libapriltag.a

$(APRILTAG_C_EXECUTABLE):$(APRILTAG_C_SRC)
	${CC} -O3 -DNDEBUG -std=gnu99 -fPIC -Wall -Wextra -Werror -Wpedantic -Wno-shift-negative-value ${INCLUDE_FLAG} -c $< -o $@

clean:
	find . -name "*.${OBJ}" -delete
	rm libapriltag.a

# OBJECTS+=apriltag.$(OBJ)
# OBJECTS+=apriltag_pose.$(OBJ)
# OBJECTS+=apriltag_pywrap.$(OBJ)
# OBJECTS+=apriltag_quad_thresh.$(OBJ)
# OBJECTS+=tag16h5.$(OBJ)
# OBJECTS+=tag25h9.$(OBJ)
# OBJECTS+=tag36h10.$(OBJ)
# OBJECTS+=tag36h11.$(OBJ)
# OBJECTS+=tagCircle21h7.$(OBJ)
# OBJECTS+=tagCircle49h12.$(OBJ)
# OBJECTS+=tagCustom48h12.$(OBJ)
# OBJECTS+=tagStandard41h12.$(OBJ)
# OBJECTS+=tagStandard52h13.$(OBJ)
# 
# OBJECTS+=g2d.$(OBJ)
# OBJECTS+=getopt.$(OBJ)
# OBJECTS+=homography.$(OBJ)
# OBJECTS+=image_u8.$(OBJ)
# OBJECTS+=image_u8x3.$(OBJ)
# OBJECTS+=image_u8x4.$(OBJ)
# OBJECTS+=matd.$(OBJ)
# OBJECTS+=pam.$(OBJ)
# OBJECTS+=pjpeg-idct.$(OBJ)
# OBJECTS+=pjpeg.$(OBJ)
# OBJECTS+=pnm.$(OBJ)
# OBJECTS+=pthreads_cross.$(OBJ)
# OBJECTS+=string_util.$(OBJ)
# OBJECTS+=svd22.$(OBJ)
# OBJECTS+=time_util.$(OBJ)
# OBJECTS+=unionfind.$(OBJ)
# OBJECTS+=workerpool.$(OBJ)
# OBJECTS+=zarray.$(OBJ)
# OBJECTS+=zhash.$(OBJ)
# OBJECTS+=zmaxheap.$(OBJ)
