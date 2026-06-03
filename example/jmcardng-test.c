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

int main() {
  jmcard_ng_t jmcard = {0};

  // UUID Kartu: 33627705
  const char* uuid = "33627705";
  // Mifare Sector-1 Block 0, 1, 2
  const char* block0 = "8B55DA5B230324EEF53281FDA91CF617";
  const char* block1 = "7FE86B1B9EB8E52C4240F52AA89362FE";
  const char* block2 = "856AD4EAC610A53F317CAB5573568138";

  // Decrypt jmcard-ng data
  uint8_t ret = jmcard_ng(&jmcard, uuid, block0, block1, block2);

  printf("DECRYPTED DATA %s\n", ret == 0 ? "SUCCESS" : "FAILED");
  if (ret == 0) {
    printf("  UUID       : %s\n", uuid);
    printf("  RUAS BYTE 1: %u\n", jmcard.ruas1);
    printf("  RUAS BYTE 2: %u\n", jmcard.ruas2);
    printf("  RUAS STRING : %s\n", jmcard.ruas);
    printf("  EXPIRE DATE : %s\n", jmcard.expire);
    printf("  TIPE KARTU  : %d\n", jmcard.tipe);
    printf("  NO KARTU    : %s\n", jmcard.nokartu);
    printf("  UID         : %s\n", jmcard.uid);
    printf("  STATUS      : %u\n", jmcard.status);
  }
  return 0;
}
