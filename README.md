# libjmentrance - Library Data Entrance Jasa Marga

**`libjmentrance`** merupakan library untuk melakukan **decrypt** data entrance tol yang ditulis oleh peralatan tol
dengan software **Toll Collector Jasa Marga**.

Library tersedia untuk `Linux` dalam `4` arsitektur, diantaranya:
* `amd64` - Intel/AMD x86 dengan 64bit
* `aarch64` - ARM 64bit
* `armv7` - ARM 32bit
* `i386` - Intel 32bit

Sedangkan untuk `QNX/NTO` hanya disediakan arsitektur `i386` (Intel/AMD x86 dengan 32bit)

Untuk `windows` tersedia dalam bentuk `dll` native dan managed (.net) disertai example untuk invoke pada c#, juga tersedia berupa cli-tools executable. *(Lihat folder `windows`)*

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

## Binary untuk ARM non Linux (Cortex-M non-linux)
Untuk keperluan library diluar linux, misalnya micro-controller berbasis arm, dapat menggunakan library
pada folder `lib` atau `obj` berikut:
* `libjmentrance-noneabi-arm.a`
* `libjmentrance-noneabi-arm.o`

## DLL untuk Windows dan C#
Library berupa DLL dan contoh penggunaan/pinvoke C# untuk windows dapat ditemukan pada folder windows
* `windows/jmentrance.dll` dll library
* `windows/Program.cs` contoh pinvoke dalam c#

Deklarasi method pada c#
```c#
/* PInvoke jmentrance */
[DllImport("jmentrance.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
internal static extern int jmentrance_decrypt([MarshalAs(UnmanagedType.LPStr)] string pubkey,
    [MarshalAs(UnmanagedType.LPStr)] string cluster_id,
    [MarshalAs(UnmanagedType.LPStr)] string sn,
    [MarshalAs(UnmanagedType.LPStr)] string input,
    [MarshalAs(UnmanagedType.LPStr)] StringBuilder output, int output_len);
```

Contoh fungsi
```c#
/* Output Buffer */
StringBuilder output = new StringBuilder(45);

/* HIMBARA */
string sn = "7546000012122780"; /* Gunakan sn kartu untuk HIMBARA */
string data = "615870B6B93F44ABB07EEADCB581CBC6FFFFFFFFFFFF";
int ret = jmentrance_decrypt(pubkey, clusterid, sn, data, output, 45);
Console.WriteLine("HIMBARA: "+ret+" = "+output);
```

Untuk status return, dapat dibaca lebih lanjut pada pembahasan *Constants - Error Code*.

## CLI-Tools untuk Windows
CLI-Tools untuk windows dapat dilihat pada folder windows
* `windows/jmentrance.exe` cli-tools
* `windows/test-windows.bat` example penggunaan cli tools

Format CLI-Tools
```
jmentrance <public-key> <cluster-id> <card-sn> <encrypted-data>
```

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
library memerlukan `public-key` dan `cluster-id`. Setiap ruas memiliki key dan cluster-id yang berbeda-beda.

Berikut adalah data key & cluster-id yang dapat digunakan:

### JJS
**CLUSTER ID :** `ba24cc34`

**PUBLIC KEY**
```
bad265420de99f9f78435d2207e44859ca4eba4af53650d50ed63466e3657225cf07967277e5093e25af511eeb9f1aabf61646db59df1e9722ad901851ffca3d
```


### JMJ
**CLUSTER ID :** `ba5cee4e`

**PUBLIC KEY**
```
830d659aa4500b4fe5d0dd5c8e38e367a564a6fe846ed758b34884ee40dbfaefb5454b1f3b250eed2e52a93abbef99c5c142a9218f58198fda9da4e035ccdb3b
```

## Penggunaan Library pada Multi Ruas
Karena berbasis key, maka library sudah mendukung implementasi multiruas, dengan menyiapkan seleksi kondisi baik pada runtime atau compile time.

**Contoh implementasi compile-time/macro**
```c
#ifdef TCT_JJS
  /* JJS */
  static const char* key =
    "bad265420de99f9f78435d2207e44859ca4eba4af53650d50ed63466e3657225cf07967277"
    "e5093e25af511eeb9f1aabf61646db59df1e9722ad901851ffca3d";
  static const char* cluster = "ba24cc34";
#endif

#ifdef TCT_JMJ
  /* JMJ */
  static const char* key =
    "830d659aa4500b4fe5d0dd5c8e38e367a564a6fe846ed758b34884ee40dbfaefb5454b1f3b"
    "250eed2e52a93abbef99c5c142a9218f58198fda9da4e035ccdb3b";
  static const char* cluster = "ba5cee4e";
#endif

/* Baca Data */
int status = jmentrance_decrypt(key, cluster, sn, data, output, 45);
```

**Contoh implementasi pada runtime**
```c
char* key ="";
char* cluster = "ba24cc34";

if (ini_tct_jjs){
  key =
    "bad265420de99f9f78435d2207e44859ca4eba4af53650d50ed63466e3657225cf07967277"
    "e5093e25af511eeb9f1aabf61646db59df1e9722ad901851ffca3d";
  cluster = "ba24cc34";
}
else if (ini_tct_jmj){
  key =
    "830d659aa4500b4fe5d0dd5c8e38e367a564a6fe846ed758b34884ee40dbfaefb5454b1f3b"
    "250eed2e52a93abbef99c5c142a9218f58198fda9da4e035ccdb3b";
  cluster = "ba5cee4e";
}

/* Baca Data */
int status = jmentrance_decrypt(key, cluster, sn, data, output, 45);

```

## Struktur Output Data
Output dari `jmentrance_decrypt` merupakan `hex-string` dengan format sebagai berikut:
| LOKASI | UKURAN | KETERANGAN | KODE | DETAIL | 
| ------ | ------ | ------ | ------ | ------ |
| 0 | 1 | NO CABANG | `CB` | - |
| 1 | 1 | NO GERBANG | `GB` | - |
| 2 | 1 | NO GARDU | `GD` | - |
| 3 | 1 | NO GERBANG EXIT | `EX` | `00` Bila belum exit |
| 4-5 | 2 | INST ID | `INST` | `0003` untuk Jasa Marga |
| 6 | 1 | TANGGAL TRANSAKSI | `DD` | `DD` |
| 7 | 1 | BULAN TRANSAKSI | `MM` | `MM` |
| 8 | 1 | TAHUN TRANSAKSI | `YY` | `YY` 23 untuk 2023|
| 9 | 1 | JAM TRANSAKSI | `HH` | `HH` format 24 jam |
| 10 | 1 | MENIT TRANSAKSI | `II` | `MM` |
| 11 | 1 | DETIK TRANSAKSI | `SS` | `SS` |
| 12 | 1 | STATUS | `ST` | `00` Belum exit, `01` Sudah exit |
| 13 | 1 | JENIS | `JN` | Tidak dipakai |
| 14 | 1 | GOLONGAN | `GL` | `1` s/d `5` |
| 15-17 | 4 | ID PETUGAS | `IDPETG` | - |
| 18 | 1 | Direction | `DR` | Tidak dipakai |
| 19-21 | 3 | CHECKSUM | `CHKSUM` | - |

## Test Output
```
TEST DATA JJS
=============
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



TEST DATA JMJ
=============
JJS MANDIRI
  UUID      : 6032982864461171
  ENCRYPTED : 7813822AEBB9A40540A6B9E87306F195FFFFFFFFFFFF
  DECRYPTED : 45300100000306082408181400000115149800006594
  ERR-CODE  : 0

JJS BCA
  UUID      : 15CC8081
  ENCRYPTED : 71634CB6147414803B8E57A42E26FEF2
  DECRYPTED : 45300100000306082408183000000215149800040738
  ERR-CODE  : 0

JJS BRI
  UUID      : 6013500418244902
  ENCRYPTED : B31818BEB247BDB46CF4AD672FD0973DFFFFFFFFFFFF
  DECRYPTED : 45300100000306082408184300000315149800043806
  ERR-CODE  : 0

JJS BNI
  UUID      : 7546130008699754
  ENCRYPTED : 92BAAF2F1BF8226473031612D12362B4FFFFFFFFFFFF
  DECRYPTED : 45300100000306082408185300000415149800009831
  ERR-CODE  : 0
```

# Contoh Parsing Data Output
```
DECRYPTED:      45300100000306082408185300000415149800009831
============================================================
CB GB DG EX INST DD MM YY HH II SS ST JN GL IDPETG DR CHKSUM
45 30 01 00 0003 06 08 24 08 18 53 00 00 04 151498 00 009831


DECRYPTED:      45300100000306082408184300000315149800043806
============================================================
CB GB DG EX INST DD MM YY HH II SS ST JN GL IDPETG DR CHKSUM
45 30 01 00 0003 06 08 24 08 18 43 00 00 03 151498 00 043806


DECRYPTED:      25640163000329112315375501000312121200051726
============================================================
CB GB DG EX INST DD MM YY HH II SS ST JN GL IDPETG DR CHKSUM
25 64 01 63 0003 29 11 23 15 37 55 01 00 03 121212 00 051726
         ^                          ^
         +--------------------------+
    Kartu sudah exit bila field ini bukan 00


DECRYPTED:      25610400000329112315375500000212553200001569
============================================================
CB GB DG EX INST DD MM YY HH II SS ST JN GL IDPETG DR CHKSUM
25 61 04 00 0003 29 11 23 15 37 55 00 00 02 125532 00 001569


DECRYPTED:      45300100000306082408181400000115149800006594
============================================================
CB GB DG EX INST DD MM YY HH II SS ST JN GL IDPETG DR CHKSUM
45 30 01 00 0003 06 08 24 08 18 14 00 00 01 151498 00 006594

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
- Error Code. Lihat `JMENTRANCE_OK` atau `JMENTRANCE_ERR_*`

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
|`JMENTRANCE_OK`|`0`|Tidak terjadi error|
|`JMENTRANCE_ERR_LENGTH`|`1`|Length data error|
|`JMENTRANCE_ERR_CHECKSUM`|`2`|Checksum error|
|`JMENTRANCE_ERR_SIG`|`3`|Signature error|
|`JMENTRANCE_ERR_DATA`|`4`|Input Data error|
|`JMENTRANCE_ERR_BUFSZ`|`5`|not enough buffer size|



