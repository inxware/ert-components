#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 


yajl_alloc.h : api/yajl_common.h

yajl_buf.h : api/yajl_common.h yajl_alloc.h

yajl_bytestack.h : api/yajl_common.h

yajl_encode.h : yajl_buf.h api/yajl_gen.h

yajl_lex.h : api/yajl_common.h

yajl_ : api/yajl_parse.h yajl_bytestack.h yajl_buf.h yajl_lex.h

yajl_alloc.$(OBJ) : yajl_alloc.h

yajl_buf.$(OBJ) : yajl_buf.h

yajl.$(OBJ) : api/yajl_parse.h yajl_lex.h yajl_ yajl_alloc.h

yajl_encode.$(OBJ) : yajl_encode.h

yajl_gen.$(OBJ) : api/yajl_gen.h yajl_buf.h yajl_encode.h

yajl_lex.$(OBJ) : yajl_lex.h yajl_buf.h

yajl_parser.$(OBJ) : api/yajl_parse.h yajl_lex.h yajl_ yajl_encode.h yajl_bytestack.h

yajl_tree.$(OBJ) : api/yajl_tree.h api/yajl_parse.h yajl_
