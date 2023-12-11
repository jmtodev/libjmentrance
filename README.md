# libjmentrance - Library Data Entrance Jasa Marga

**`libjmentrance`** merupakan library untuk melakukan **decrypt** data entrance tol yang ditulis oleh peralatan tol
dengan software **Toll Collector Jasa Marga**.

Library tersedia untuk `Linux` dalam `4` arsitektur, diantaranya:
* `amd64` - Intel/AMD x86 dengan 64bit
* `aarch64` - ARM 64bit
* `armv7` - ARM 32bit
* `i386` - Intel 32bit

Sedangkan untuk `QNX/NTO` hanya disediakan arsitektur `i386` (Intel/AMD x86 dengan 32bit)

## Binary Build Library
`libjmentrance` dibangun sebagai `static library` untuk `C/C++` yang dapat digunakan pada environment `gcc`.

File library terdiri dari 2 jenis extension `*.a` (pada direktori `lib`) atau `*.o` (pada direktori `obj`).
Aplikasi **Toll Collector** dapat memilih salah-satu format sesuai dengan kebutuhannya.

Berikut adalah nama-nama file binary untuk library beserta keterangan arsitektur dan platformnya:
* `libjmentrance-amd64.a` / `libjmentrance-amd64.o` - Linux x86 64bit
* `libjmentrance-aarch64.a` / `libjmentrance-aarch64.o` - Linux ARM 64bit
* `libjmentrance-armv7.a` / `libjmentrance-armv7.o` - Linux ARMv7
* `libjmentrance-i386.a` / `libjmentrance-i386.o` - Linux x86 32bit
* `libjmentrance-i386-qnx.a` / `libjmentrance-i386-qnx.o` - QNX/NTO x86 32bit

## Test Program
Test program dapat dilihat pada file `example/libjmentrance-test.c`, dan dapat di-build dengan `build_test.sh` (Hanya untuk Linux). Berikut command untuk melakukan build sesuai dengan arsitektur yang dituju:

``` bash
# Build untuk x86 64bit
./build_test.sh

# Build untuk arm-64
./build_test.sh aarch64

# Build untuk arm-v7 32bit
./build_test.sh armv7

# Build untuk x86 32bit
./build_test.sh i386
```

# Library Usage

## Penyimpanan Data
Entrance tol Jasa Marga yang telah di-enkripsi akan memiliki panjang `16 bytes` atau `32 hex-characters`, sehingga
dapat disimpan kedalam `satu` block mifare (BCA), atau pada penyimpanan data etoll yang dapat menampung sampai `44` karakter.

Untuk etoll (non-bca) `libjmentrance` hanya akan memproses `16 byte` awal dan mengacuhkan sisanya, misalnya:

```
DATA DALAM KARTU   = 615870B6B93F44ABB07EEADCB581CBC6FFFFFFFFFFFF
YANG AKAN DIPROSES = 615870B6B93F44ABB07EEADCB581CBC6
```

Sehingga penggunaan library seperti ini tetap valid:
```c
const char* sn = "7546000012122780";
const char* data = "615870B6B93F44ABB07EEADCB581CBC6FFFFFFFFFFFF";
char out[45] = {0};
ret = jmentrance_decrypt(jmentrance_jjs_pubkey, jmentrance_jjs_cluster_id,
                          sn, data, out, 45);
```

## Block Data BCA
Data entrance akan disimpan pada block `44`  (dalam satu block saja). Dan parameter `sn` pada fungsi
`jmentrance_decrypt` diisi dengan `hex-uuid` (Bukan SN Kartu).

Block `45` akan berisi owner code pada byte pertama dengan konten `0x91` untuk entrance yang belum melakukan exit dan
`0x90` untuk entrance yang telah melakukan exit.

Block `46` tidak digunakan.

## Block Data Kartu Dinas
Data entrance akan disimpan pada block `16`  (dalam satu block saja). Dan parameter `sn` pada fungsi
`jmentrance_decrypt` diisi dengan `hex-uuid` (Bukan SN Kartu).

Block `17` akan berisi owner code pada byte pertama dengan konten `0x91` untuk entrance yang belum melakukan exit dan
`0x90` untuk entrance yang telah melakukan exit. 

Block `18` tidak digunakan.

Key untuk block `16`, `17` dan `18` (Sektor `4`) adalah:

- `Key A` Sector `4` : `0CB0FFDCB72E`
- `Key B` Sector `4` : `0C6C47B41A72`


## Keys
Entrance tol Jasa Marga menggunakan enkripsi `public` & `private` key. Untuk melakukan `decrypt`
library memerlukan `public-key` dan `cluster-id-key`, dimana untuk ruas **JJS** data key & cluster
adalah sebagai berikut:

**PUBLIC KEY**
```
bad265420de99f9f78435d2207e44859ca4eba4af53650d50ed63466e3657225cf07967277e5093e25af511eeb9f1aabf61646db59df1e9722ad901851ffca3d
```

**CLUSTER ID KEY**
```
ba24cc34
```

## Struktur Output Data
Output dari `jmentrance_decrypt` merupakan `hex-string` dengan format sebagai berikut:
| LOKASI | UKURAN | KETERANGAN | DETAIL | 
| ------ | ------ | ------ | ------ |
| 0 | 1 | NO CABANG | - |
| 1 | 1 | NO GERBANG | - |
| 2 | 1 | NO GARDU | - |
| 3 | 1 | NO GERBANG EXIT | `00` Bila belum exit |
| 4-5 | 2 | INST ID | `0003` untuk Jasa Marga |
| 6 | 1 | TANGGAL TRANSAKSI | `DD` |
| 7 | 1 | BULAN TRANSAKSI | `MM` |
| 8 | 1 | TAHUN TRANSAKSI | `YY` 23 untuk 2023|
| 9 | 1 | JAM TRANSAKSI | `HH` format 24 jam |
| 10 | 1 | MENIT TRANSAKSI | `MM` |
| 11 | 1 | DETIK TRANSAKSI | `SS` |
| 12 | 1 | STATUS | `00` Belum exit, `01` Sudah exit |
| 13 | 1 | JENIS | Tidak dipakai |
| 14 | 1 | GOLONGAN | `1` s/d `5` |
| 15-17 | 4 | ID PETUGAS | - |
| 18 | 1 | Direction | Tidak dipakai |
| 19-21 | 3 | CHECKSUM | - |

## Test Output
```
TEST BCA/DINAS
UUID      : AB56EE01
ENCRYPTED : 05A4B618A982AA55D61C1284684923E7
DECRYPTED : 25610400000329112315375500000212553200001569
ERR-CODE  : 0

TEST ETOLL NON BCA (BELUM EXIT)
UUID      : 6013500130684476
ENCRYPTED : 4D4619FADD5E111F2604AF9453A52D9D
DECRYPTED : 25640100000329112315375500000110101000042189
ERR-CODE  : 0

TEST ETOLL NON BCA (SUDAH EXIT)
UUID      : 7546000012122780
ENCRYPTED : 615870B6B93F44ABB07EEADCB581CBC6FFFFFFFFFFFF
DECRYPTED : 25640163000329112315375501000312121200051726
ERR-CODE  : 0
```

# Referensi Library
Berikut adalah referensi lengkap dari `libjmentrance` yang terbagi kedalam `2 segment` diantaranya:
- **Function** - Merupakan fungsi-fungsi yang terdapat dalam library.
- **Constants** - Merupakan kumpulan nilai konstanta yang digunakan pada library

---
## Daftar Isi

- **[Functions](#functions)**
	- [`jmentrance_decrypt()`](#jmentrance_decrypt)
- **[Constants](#constants)**
	- [Error Code](#error-code)

---
## Functions

### `jmentrance_decrypt()`
Decrypt `raw data` (hex string) yang didapatkan dari kartu menjadi output standar data entrance (hex string) 
#### Arguments
- `pubkey` : Public-key untuk decrypt
- `cluster_id` : Cluster-id key
- `sn` : Nomor kartu etoll atau `uuid` untuk BCA
- `input` : Hex string input
- `output` : Hex string output
- `output_length` : Length dari output buffer
 
#### Return Value
- Error Code. Lihat `LIBJJS_OK` atau `LIBJJS_ERR_*`

``` c
int jmentrance_decrypt(
    const char* pubkey,
    const char* cluster_id,
    const char* sn,
    const char* input,
    char* output,
    size_t output_length
);
```




---
## Constants

### Error Code
| NAME | VALUE | Keterangan |
| ------ | ------ | ------ |
|`LIBJJS_OK`|`0`|Tidak terjadi error|
|`LIBJJS_ERR_LENGTH`|`1`|Length data error|
|`LIBJJS_ERR_CHECKSUM`|`2`|Checksum error|
|`LIBJJS_ERR_SIG`|`3`|Signature error|
|`LIBJJS_ERR_DATA`|`4`|Input Data error|
|`LIBJJS_ERR_BUFSZ`|`5`|not enough buffer size|



