#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:18 BST 2022


deflate.h : zutil.h

pngconf.h : pngusr.h target.h

png.h : zlib.h pngconf.h

zlib.h : zconf.h

zutil.h : zlib.h

adler32.$(OBJ) : zlib.h

compress.$(OBJ) : zlib.h

crc32.$(OBJ) : zutil.h crc32.h

deflate.$(OBJ) : deflate.h

gzio.$(OBJ) : zutil.h

infback.$(OBJ) : zutil.h inftrees.h inflate.h inffast.h inffixed.h

inffast.$(OBJ) : zutil.h inftrees.h inflate.h inffast.h

inflate.$(OBJ) : zutil.h inftrees.h inflate.h inffast.h inffixed.h

inftrees.$(OBJ) : zutil.h inftrees.h

png.$(OBJ) : png.h hal_mem.h

pngerror.$(OBJ) : png.h

pngget.$(OBJ) : png.h

pngmem.$(OBJ) : png.h hal_mem.h

pngpread.$(OBJ) : png.h

pngread.$(OBJ) : png.h

pngrio.$(OBJ) : png.h ehs_types.h hal_file.h

pngrtran.$(OBJ) : png.h

pngrutil.$(OBJ) : png.h

pngset.$(OBJ) : png.h

pngtrans.$(OBJ) : png.h

pngvcrd.$(OBJ) : png.h

pngwio.$(OBJ) : png.h

pngwrite.$(OBJ) : png.h

pngwtran.$(OBJ) : png.h

pngwutil.$(OBJ) : png.h

trees.$(OBJ) : deflate.h trees.h

uncompr.$(OBJ) : zlib.h

zutil.$(OBJ) : zutil.h hal_mem.h