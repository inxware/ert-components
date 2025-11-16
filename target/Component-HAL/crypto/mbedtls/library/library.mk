#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
# Makefile fragment to build the comms-specific code for EHS.

# Called by ../../platform/<platform-type>
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
INC_DIRS +=  $(EHS_TARGETS_ROOT_PATH)/Component-HAL/crypto/mbedtls/include
#  VPATH - where to look for source code
VPATH += $(EHS_TARGETS_ROOT_PATH)/Component-HAL/crypto/mbedtls/library/

OBJECTS += aes.$(OBJ)
OBJECTS += aesni.$(OBJ)
OBJECTS += arc4.$(OBJ)
OBJECTS += asn1parse.$(OBJ)
OBJECTS += asn1write.$(OBJ)
OBJECTS += base64.$(OBJ)
OBJECTS += bignum.$(OBJ)
OBJECTS += blowfish.$(OBJ)
OBJECTS += camellia.$(OBJ)
OBJECTS += ccm.$(OBJ)
OBJECTS += certs.$(OBJ)
OBJECTS += cipher.$(OBJ)
OBJECTS += cipher_wrap.$(OBJ)
OBJECTS += ctr_drbg.$(OBJ)
OBJECTS += mbedtlsdebug.$(OBJ)
OBJECTS += des.$(OBJ)
OBJECTS += dhm.$(OBJ)
OBJECTS += ecdh.$(OBJ)
OBJECTS += ecdsa.$(OBJ)
OBJECTS += ecjpake.$(OBJ)
OBJECTS += ecp.$(OBJ)
OBJECTS += ecp_curves.$(OBJ)
OBJECTS += entropy.$(OBJ)
OBJECTS += entropy_poll.$(OBJ)
OBJECTS += error.$(OBJ)
OBJECTS += gcm.$(OBJ)
OBJECTS += havege.$(OBJ)
OBJECTS += hmac_drbg.$(OBJ)
OBJECTS += md.$(OBJ)
OBJECTS += md2.$(OBJ)
OBJECTS += md4.$(OBJ)
OBJECTS += md5.$(OBJ)
OBJECTS += md_wrap.$(OBJ)
OBJECTS += memory_buffer_alloc.$(OBJ)
OBJECTS += oid.$(OBJ)
OBJECTS += padlock.$(OBJ)
OBJECTS += pem.$(OBJ)
OBJECTS += pk.$(OBJ)
OBJECTS += pk_wrap.$(OBJ)
OBJECTS += pkcs11.$(OBJ)
OBJECTS += pkcs12.$(OBJ)
OBJECTS += pkcs5.$(OBJ)
OBJECTS += pkparse.$(OBJ)
OBJECTS += pkwrite.$(OBJ)
OBJECTS += platform.$(OBJ)
OBJECTS += ripemd160.$(OBJ)
OBJECTS += rsa.$(OBJ)
OBJECTS += sha1.$(OBJ)
OBJECTS += sha256.$(OBJ)
OBJECTS += sha512.$(OBJ)
OBJECTS += ssl_cache.$(OBJ)
OBJECTS += ssl_ciphersuites.$(OBJ)
OBJECTS += ssl_cli.$(OBJ)
OBJECTS += ssl_cookie.$(OBJ)
OBJECTS += ssl_srv.$(OBJ)
OBJECTS += ssl_ticket.$(OBJ)
OBJECTS += ssl_tls.$(OBJ)
OBJECTS += threading.$(OBJ)
OBJECTS += timing.$(OBJ)
OBJECTS += version.$(OBJ)
OBJECTS += version_features.$(OBJ)
OBJECTS += x509.$(OBJ)
OBJECTS += x509_create.$(OBJ)
OBJECTS += x509_crl.$(OBJ)
OBJECTS += x509_crt.$(OBJ)
OBJECTS += x509_csr.$(OBJ)
OBJECTS += x509write_crt.$(OBJ)
OBJECTS += x509write_csr.$(OBJ)
OBJECTS += xtea.$(OBJ)
