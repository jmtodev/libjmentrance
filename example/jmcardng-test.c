/*******************************************************************************
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
 * Filename    : jmcardng-test.c
 * Description : Example program libjmentrance
 *
 * + This is part of libjmentrance
 * + 2026 - Author(s): Ahmad Amarullah
 *
 */

#include <libjmentrance.h>
#include <stdio.h>

// Structure data jmcard-ng
// typedef struct {
//   uint32_t ruas1;   /* ruas byte 1 */
//   uint32_t ruas2;   /* ruas byte 2 */
//   char ruas[17];    /* ruas string */
//   char expire[9];   /* expire date string */
//   int tipe;         /* jenis kartu 1. operasi, 2. karyawan, 3.mitra */
//   char nokartu[32]; /* nomor kartu string */
//   char uid[9];      /* uid string */
//   uint8_t status;   /* status error validasi kartu. */
// } jmcard_ng_t;

// Fungsi untuk menampilkan data jmcard-ng
void dump_jmcard_ng(jmcard_ng_t* jmcard) {
  printf("- RUAS BYTE 1: %08X\n", jmcard->ruas1);
  printf("- RUAS BYTE 2: %08X\n", jmcard->ruas2);
  printf("- RUAS STRING : %s\n", jmcard->ruas);
  printf("- EXPIRE DATE : %s\n", jmcard->expire);
  printf("- TIPE KARTU  : %d\n", jmcard->tipe);
  printf("- NO KARTU    : %s\n", jmcard->nokartu);
  printf("- UID         : %s\n", jmcard->uid);
  printf("- STATUS      : %u\n", jmcard->status);
  printf("\n");
}

// Fungsi untuk test parsing jmcard-ng
void test_parsing_jmcard_ng() {
  jmcard_ng_t jmcard = {0};

  // UUID Kartu: 33627705
  const char* uuid = "33627705";

  // Mifare Sector-1 Block 0, 1, 2
  // Baca sektor-1 blok 0, 1, 2 menggunakan fungsi pembaca kartu mifare,
  // lalu masukkan data blok tersebut ke fungsi jmcard_ng untuk didekripsi.
  // Gunakan key A berikut: "2177A6F53421"
  const char* block0 = "8B55DA5B230324EEF53281FDA91CF617";
  const char* block1 = "7FE86B1B9EB8E52C4240F52AA89362FE";
  const char* block2 = "856AD4EAC610A53F317CAB5573568138";

  // Decrypt jmcard-ng data
  uint8_t ret = jmcard_ng(&jmcard, uuid, block0, block1, block2);

  // Tampilkan hasil decrypt
  printf("DECRYPTED DATA %s\n", ret == 0 ? "SUCCESS" : "FAILED");
  if (ret == 0) {
    // Decrypt berhasil, tampilkan data jmcard-ng
    dump_jmcard_ng(&jmcard);

    // Cek validasi ruas
    int ruas_valid = jmcard_ng_cek_ruas(jmcard.ruas1, jmcard.ruas2);
    printf("CEK RUAS:\n");
    printf("- JUMLAH RUAS VALID: %d\n", ruas_valid);
    printf("- STATUS RUAS.     : %s\n", ruas_valid > 0 ? "OK" : "RUAS LAIN");
    printf("\n");
  } else {
    // Decrypt gagal, tampilkan error
    printf("DECRYPT ERROR : %u\n", ret);
    switch (ret) {
      case JMCARD_NG_ERR_CKSUM:
        printf("- INVALID CHECKSUM\n");
        break;
      case JMCARD_NG_ERR_SIG:
        printf("- UUID NOT MATCH / SIGNATURE INVALID\n");
        break;
      case JMCARD_NG_ERR_DECRYPT:
        printf("- INVALID DECRYPT RESULT\n");
        break;
      case JMCARD_NG_ERR_DATA:
        printf("- INVALID RAW DATA\n");
        break;
      case JMCARD_NG_ERR_READ:
        printf("- ERROR READING DATA\n");
        break;
      case JMCARD_NG_ERR_AUTH:
        printf("- AUTHENTICATION ERROR\n");
        break;
    }
  }
}

void whitelist_validation(const char* uuid, const char* whitelist_signature) {
  // Decrypt signature whitelist
  char out[128] = {0};  // Buffer untuk menyimpan hasil decrypt
  uint8_t r = jmcard_ng_decrypt(whitelist_signature, out);

  // Tampilkan hasil decrypt
  printf("UUID: %s\n", uuid);
  printf("DECRYPT WHITELIST SIGNATURE %s\n", r == 1 ? "SUCCESS" : "FAILED");
  if (r == 1) {
    printf("- DECRYPTED SIGNATURE: %s\n", out);
  } else {
    printf("- DECRYPT ERROR: %u\n", r);
  }
  printf("\n");
}

void test_whitelist_ktp() {
  // Test whitelist signature untuk beberapa UUID
  whitelist_validation("7D6FD112",
                       "7c030aa5f13280567ae477fba52232f2cfbf2c813053100499df33d"
                       "5956264ae72344b7708b9995d4ebe9a1b666d40ea");
  whitelist_validation("2595D4A0",
                       "8d578b5884c43ab546d10716edef65f37eb26612394b688e753340b"
                       "99544bcce108bde269094a90e842aa89a6b31afb6");
  whitelist_validation("DC7425D4",
                       "5ec00535d1c2effda1c28005bac1f576de2f16abab1c7355c719576"
                       "f1c87c29aba05c55f65658b14f484bc3cc589cf77");
}

int main() {
  jmcard_ng_t jmcard = {0};

  // Konfigurasi Ruas pada GTO
  // Silahkan baca dokumentasi `jmcard_ng_config_ruas` untuk format konfigurasi
  // ruas yang benar.
  //
  // Contoh: Harus ada ruas 1, atau ruas 2 dan 3, atau ruas 4
  jmcard_ng_config_ruas("1, 2+3, 4");

  // Test parsing jmcard-ng dengan data contoh. Data ini diambil dari kartu asli
  // yang sudah didekripsi menggunakan alat dekripsi lain. Pastikan data blok
  // yang digunakan benar agar hasil parsing sesuai dengan data asli kartu.
  test_parsing_jmcard_ng();

  // Test validasi whitelist KTP dengan beberapa UUID dan signature whitelist
  test_whitelist_ktp();

  return 0;
}
