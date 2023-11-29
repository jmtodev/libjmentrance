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

int main() {
  int ret;

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
  ret = jmentrance_decrypt(jmentrance_jjs_pubkey, jmentrance_jjs_cluster_id,
                           sn_bca, data_bca, out_bca, 45);
  printf(
      "TEST BCA/DINAS\n"
      "UUID      : %s\n"
      "ENCRYPTED : %s\n"
      "DECRYPTED : %s\n"
      "ERR-CODE  : %i\n\n",
      sn_bca, data_bca, out_bca, ret);

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
  ret = jmentrance_decrypt(jmentrance_jjs_pubkey, jmentrance_jjs_cluster_id,
                           sn_etoll1, data_etoll1, out_etoll1, 45);
  printf(
      "TEST ETOLL NON BCA (BELUM EXIT)\n"
      "UUID      : %s\n"
      "ENCRYPTED : %s\n"
      "DECRYPTED : %s\n"
      "ERR-CODE  : %i\n\n",
      sn_etoll1, data_etoll1, out_etoll1, ret);

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

  *** Panjang encrypted data berisi lebih dari 32hex.
  */
  const char* sn_etoll2 = "7546000012122780"; /* Non-BCA Gunaan SN */
  const char* data_etoll2 = "615870B6B93F44ABB07EEADCB581CBC6FFFFFFFFFFFF";
  char out_etoll2[45] = {0};
  ret = jmentrance_decrypt(jmentrance_jjs_pubkey, jmentrance_jjs_cluster_id,
                           sn_etoll2, data_etoll2, out_etoll2, 45);
  printf(
      "TEST ETOLL NON BCA (SUDAH EXIT)\n"
      "UUID      : %s\n"
      "ENCRYPTED : %s\n"
      "DECRYPTED : %s\n"
      "ERR-CODE  : %i\n\n",
      sn_etoll2, data_etoll2, out_etoll2, ret);
  return 0;
}