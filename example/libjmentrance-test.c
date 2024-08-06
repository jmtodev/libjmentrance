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
 * Filename    : libjmentrance-test.c
 * Description : Example program libjmentrance
 *
 * + This is part of libjmentrance
 * + 2023 - Author(s): Ahmad Amarullah
 *
 */

#include <libjmentrance.h>
#include <stdio.h>

/* Public Key untuk JJS */
const char* jmentrance_jjs_pubkey =
    "bad265420de99f9f78435d2207e44859ca4eba4af53650d50ed63466e3657225cf07967277"
    "e5093e25af511eeb9f1aabf61646db59df1e9722ad901851ffca3d";

/* Cluster-ID Key untuk JJS */
const char* jmentrance_jjs_cluster_id = "ba24cc34";

/* Public Key untuk JMJ */
const char* jmentrance_jmj_pubkey =
    "830d659aa4500b4fe5d0dd5c8e38e367a564a6fe846ed758b34884ee40dbfaefb5454b1f3b"
    "250eed2e52a93abbef99c5c142a9218f58198fda9da4e035ccdb3b";

/* Cluster-ID Key untuk JMJ */
const char* jmentrance_jmj_cluster_id = "ba5cee4e";

/* Print Info */
void print_info(const char* title, const char* uid, const char* enc,
                const char* dec, int ecd) {
  printf(
      "%s\n"
      "  UUID      : %s\n"
      "  ENCRYPTED : %s\n"
      "  DECRYPTED : %s\n"
      "  ERR-CODE  : %i\n\n",
      title, uid, enc, dec, ecd);
}

/* Test Data JJS */
void test_jjs() {
  int ret;

  /* JJS */
  const char* pubkey = jmentrance_jjs_pubkey;
  const char* clusterid = jmentrance_jjs_cluster_id;

  /*
  TEST BCA/DINAS (BELUM EXIT)
  ---------------------------
  CB / GB / GD     : 25 / 61 / 04
  Status / GB Exit : 00 / 00
  Jenis / IID      : 00 / 0003
  Dir / Petugas    : 00 / 125532
  Golongan         : 02
  Tanggal          : 2023-11-29 15:37:55
  Checksum         : 0
  SN               : AB56EE01
  ENCRYPTED-DATA   : 05A4B618A982AA55D61C1284684923E7
  */
  const char* sn_bca = "AB56EE01"; /* BCA Gunakan UUID */
  const char* data_bca = "05A4B618A982AA55D61C1284684923E7";
  char out_bca[45] = {0};
  ret = jmentrance_decrypt(pubkey, clusterid, sn_bca, data_bca, out_bca, 45);
  print_info("TEST BCA/DINAS", sn_bca, data_bca, out_bca, ret);

  /*
  TEST ETOLL NON BCA (BELUM EXIT)
  -------------------------------
  CB / GB / GD     : 25 / 64 / 01
  Status / GB Exit : 00 / 00
  Jenis / IID      : 00 / 0003
  Dir / Petugas    : 00 / 101010
  Golongan         : 01
  Tanggal          : 2023-11-29 15:37:55
  Checksum         : 0
  SN               : 6013500130684476
  ENCRYPTED-DATA   : 4D4619FADD5E111F2604AF9453A52D9D
  */
  const char* sn_etoll1 = "6013500130684476"; /* Non-BCA Gunaan SN */
  const char* data_etoll1 = "4D4619FADD5E111F2604AF9453A52D9D";
  char out_etoll1[45] = {0};
  ret = jmentrance_decrypt(pubkey, clusterid, sn_etoll1, data_etoll1,
                           out_etoll1, 45);
  print_info("TEST ETOLL NON BCA (BELUM EXIT)", sn_etoll1, data_etoll1,
             out_etoll1, ret);

  /*
  TEST ETOLL NON BCA (SUDAH EXIT)
  -------------------------------
  CB / GB / GD     : 25 / 64 / 01
  Status / GB Exit : 01 / 63
  Jenis / IID      : 00 / 0003
  Dir / Petugas    : 00 / 121212
  Golongan         : 03
  Tanggal          : 2023-11-29 15:37:55
  Checksum         : 0
  SN               : 7546000012122780
  ENCRYPTED-DATA   : 615870B6B93F44ABB07EEADCB581CBC6FFFFFFFFFFFF
  */
  const char* sn_etoll2 = "7546000012122780"; /* Non-BCA Gunaan SN */
  const char* data_etoll2 = "615870B6B93F44ABB07EEADCB581CBC6FFFFFFFFFFFF";
  char out_etoll2[45] = {0};
  ret = jmentrance_decrypt(pubkey, clusterid, sn_etoll2, data_etoll2,
                           out_etoll2, 45);
  print_info("TEST ETOLL NON BCA (SUDAH EXIT)", sn_etoll2, data_etoll2,
             out_etoll2, ret);
}

/* Test Data JMJ */
void test_jmj() {
  int ret;

  /* JMJ */
  const char* pubkey = jmentrance_jmj_pubkey;
  const char* clusterid = jmentrance_jmj_cluster_id;

  /* Variabel */
  char* sn;
  char* enc;
  char out[45] = {0};

  /*
    --------------------------------------------
    CB / GERBANG / GARDU  : 45 / 30 / 01
    GOLONGAN              : 1
    TGL_ENTRANCE          : 06/08/2024 08:18:14
    PETUGAS               : 151498
    IID                   : 0003
    STAT / JENIS / DIR    : 00 / 00 / 00
    GERBANG EXIT          : 00
    UID/SN                : 6032982864461171
    ____________________________________________
    METODA BAYAR : MANDIRI
    SN           : 6032982864461171
    DATA ENCRYPT : 7813822AEBB9A40540A6B9E87306F195FFFFFFFFFFFF
  */
  sn = "6032982864461171";
  enc = "7813822AEBB9A40540A6B9E87306F195FFFFFFFFFFFF";
  ret = jmentrance_decrypt(pubkey, clusterid, sn, enc, out, 45);
  print_info("JJS MANDIRI", sn, enc, out, ret);

  /*
    --------------------------------------------
    CB / GERBANG / GARDU  : 45 / 30 / 01
    GOLONGAN              : 2
    TGL_ENTRANCE          : 06/08/2024 08:18:30
    PETUGAS               : 151498
    IID                   : 0003
    STAT / JENIS / DIR    : 00 / 00 / 00
    GERBANG EXIT          : 00
    UID/SN                : 15CC8081
    ____________________________________________
    METODA BAYAR : BCA
    UID          : 15CC8081
    DATA ENCRYPT : 71634CB6147414803B8E57A42E26FEF2
  */
  sn = "15CC8081";
  enc = "71634CB6147414803B8E57A42E26FEF2";
  ret = jmentrance_decrypt(pubkey, clusterid, sn, enc, out, 45);
  print_info("JJS BCA", sn, enc, out, ret);

  /*
    --------------------------------------------
    CB / GERBANG / GARDU  : 45 / 30 / 01
    GOLONGAN              : 3
    TGL_ENTRANCE          : 06/08/2024 08:18:43
    PETUGAS               : 151498
    IID                   : 0003
    STAT / JENIS / DIR    : 00 / 00 / 00
    GERBANG EXIT          : 00
    UID/SN                : 6013500418244902
    ____________________________________________
    METODA BAYAR : BRI
    SN           : 6013500418244902
    DATA ENCRYPT : B31818BEB247BDB46CF4AD672FD0973DFFFFFFFFFFFF
  */
  sn = "6013500418244902";
  enc = "B31818BEB247BDB46CF4AD672FD0973DFFFFFFFFFFFF";
  ret = jmentrance_decrypt(pubkey, clusterid, sn, enc, out, 45);
  print_info("JJS BRI", sn, enc, out, ret);

  /*
    --------------------------------------------
    CB / GERBANG / GARDU  : 45 / 30 / 01
    GOLONGAN              : 4
    TGL_ENTRANCE          : 06/08/2024 08:18:53
    PETUGAS               : 151498
    IID                   : 0003
    STAT / JENIS / DIR    : 00 / 00 / 00
    GERBANG EXIT          : 00
    UID/SN                : 7546130008699754
    ____________________________________________
    METODA BAYAR : BNI
    SN           : 7546130008699754
    DATA ENCRYPT : 92BAAF2F1BF8226473031612D12362B4FFFFFFFFFFFF
  */
  sn = "7546130008699754";
  enc = "92BAAF2F1BF8226473031612D12362B4FFFFFFFFFFFF";
  ret = jmentrance_decrypt(pubkey, clusterid, sn, enc, out, 45);
  print_info("JJS BNI", sn, enc, out, ret);
}

int main() {
  printf("TEST DATA JJS\n");
  printf("=============\n");
  test_jjs();

  printf("\n\n");
  printf("TEST DATA JMJ\n");
  printf("=============\n");
  test_jmj();
  return 0;
}