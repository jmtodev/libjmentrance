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
 * STRUCT `jmcard_ng_t`
 * Struktur data JMCARD_NG yang akan digunakan untuk menyimpan
 * data hasil decrypt kartu JMCard-NG
 */
typedef struct {
  uint32_t ruas1;   /* ruas segment 1 */
  uint32_t ruas2;   /* ruas segment 2 */
  char ruas[17];    /* ruas string */
  char expire[9];   /* expire date string */
  int tipe;         /* jenis kartu 1. operasi, 2. karyawan, 3.mitra */
  char nokartu[32]; /* nomor kartu string */
  char uid[9];      /* uid string */
  uint8_t status;   /* status error validasi kartu. */
} jmcard_ng_t;

/**
 * DEFINE Error Code
 */
#define JMENTRANCE_OK 0           /* Tidak terjadi error */
#define JMENTRANCE_ERR_LENGTH 1   /* Length data error */
#define JMENTRANCE_ERR_CHECKSUM 2 /* Checksum error */
#define JMENTRANCE_ERR_SIG 3      /* Signature error */
#define JMENTRANCE_ERR_DATA 4     /* Input Data error */
#define JMENTRANCE_ERR_BUFSZ 5    /* not enough buffer size */

#define JMCARD_NG_OK 0          /* Tidak terjadi error */
#define JMCARD_NG_ERR_CKSUM 1   /* INVALID CRC32 */
#define JMCARD_NG_ERR_SIG 2     /* UUID NOT MATCH */
#define JMCARD_NG_ERR_DECRYPT 4 /* INVALID DECRYPT RESULT */
#define JMCARD_NG_ERR_DATA 8    /* INVALID RAW DATA INVALID */
#define JMCARD_NG_ERR_READ 16   /* INVALID RAW DATA INVALID */
#define JMCARD_NG_ERR_AUTH 32   /* INVALID RAW DATA INVALID */

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
int jmentrance_decrypt(const char *pubkey, const char *cluster_id,
                       const char *sn, const char *input, char *output,
                       size_t output_length);

/**
 * Decrypt `raw data` (hex string) yang didapatkan dari data
 * whitelist untuk JM Card
 * @param ciphersrc String cipher hex untuk di-decrypt
 * @param out Buffer untuk menyimpan hasil (min 256 bytes)
 * @return 0 gagal, 1 berhasil
 */
uint8_t jmcard_ng_decrypt(const char *ciphersrc, char *out);

/**
 * Decrypt `raw data` (hex string) yang didapatkan dari block kartu mifare
 * JMCard-NG menjadi struktur data jmcard_ng_t
 * @param pubkey Public-key untuk decrypt
 * @param cluster_id Cluster-id key
 * @param sn Nomor kartu etoll atau `uuid` untuk BCA
 * @param input Hex string input
 * @param output Hex string output
 * @param output_length Length dari output buffer
 * @return Error Code. Lihat `JMCARD_NG_OK` atau <b>JMCARD_NG_ERR_*</b>
 */
uint8_t jmcard_ng(jmcard_ng_t *out, const char *uuid, const char *block0,
                  const char *block1, const char *block2);

/**
 * Konfigurasi list ruas yang valid untuk kartu JMCard-NG. Fungsi ini digunakan
 * untuk menentukan ruas mana saja yang dianggap valid saat melakukan validasi
 * kartu.
 *
 * Gunakan koma untuk memisahkan ruas, dan gunakan tanda plus untuk kombinasi
 * ruas. Contoh:
 * - "1, 2, 3" -> hanya ruas 1, 2, dan 3 yang valid
 * - "1+2, 3" -> harus ada ruas 1 dan 2, atau memikiki ruas 3
 * @param config_ruas String daftar ruas dan kombinasi ruas
 * @return void
 */
void jmcard_ng_config_ruas(const char *config_ruas);

/**
 * Konfigurasi list ruas yang valid untuk kartu JMCard-NG. Fungsi ini digunakan
 * untuk menentukan ruas mana saja yang dianggap valid saat melakukan validasi
 * kartu.
 * @param ruas1 Ruas segment 1 yang didapat dari kartu (jmcard_ng_t)
 * @param ruas2 Ruas segment 2 yang didapat dari kartu (jmcard_ng_t)
 * @return int Jumlah kombinasi ruas yang valid, 0 jika tidak ada yang valid
 */
int jmcard_ng_cek_ruas(uint32_t ruas1, uint32_t ruas2);

/* [END-DOC] */
/* end of extern for cpp */
#ifdef __cplusplus
}
#endif
#endif /* __libjmentrance_h__ */