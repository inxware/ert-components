#
# deps.mk - dependencies for the current makefile
# 
# Automatically Generated. DO NOT EDIT
# 
# Created by makedeps.sh on Sun 17 Apr 23:35:18 BST 2022


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