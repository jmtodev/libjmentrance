/*
 * Copyright (C) 2023 JMTO (https://jmto.co.id/)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *______________________________________________________________________________
 *
 * Filename    : libjmentrance.h
 * Description : Jasa marga entrance library header
 *
 * + This is part of limjmentrance
 * + 2023 - Author(s): Ahmad Amarullah
 *
 */

#ifndef __libjmentrance_h__
#define __libjmentrance_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * DEFINE Error Code
 */
#define JMENTRANCE_OK 0           /* Tidak terjadi error */
#define JMENTRANCE_ERR_LENGTH 1   /* Length data error */
#define JMENTRANCE_ERR_CHECKSUM 2 /* Checksum error */
#define JMENTRANCE_ERR_SIG 3      /* Signature error */
#define JMENTRANCE_ERR_DATA 4     /* Input Data error */
#define JMENTRANCE_ERR_BUFSZ 5    /* not enough buffer size */

/**
 * Decrypt `raw data` (hex string) yang didapatkan dari kartu menjadi
 * output standar data entrance (hex string)
 * @param pubkey Public-key untuk decrypt
 * @param cluster_id Cluster-id key
 * @param sn Nomor kartu etoll atau `uuid` untuk BCA
 * @param input Hex string input
 * @param output Hex string output
 * @param output_length Length dari output buffer
 * @return Error Code. Lihat `JMENTRANCE_OK` atau <b>JMENTRANCE_ERR_*</b>
 */
int jmentrance_decrypt(const char* pubkey, const char* cluster_id,
                       const char* sn, const char* input, char* output,
                       size_t output_length);

/* [END-DOC] */
/* end of extern for cpp */
#ifdef __cplusplus
}
#endif
#endif /* __libjmentrance_h__ */