# Prototype V2.1

## Lampu
Hanya dinyalakan sebelum mengirim serial dan dimatikan setelah mulai menerima serial.
Cek suhu untuk penggunaan berulang yang lama, karena lampu bisa melelehkan plastik dan membakar papan.

## API
| Perintah      | Keterangan                            | Format Output                                     |
|---------------|---------------                        |--------------------------------------             |
| I             | Output spektrum yang diterima sensor  | i: indeks (int)                                   |
|               |                                       | wl: pemetaan panjang gelombang (float 400-700)    |
|               |                                       | val: intensitas (float 0-255)                     |


| Perintah      | Keterangan                            | Format Output                                     |
|---------------|---------------                        |--------------------------------------             |
| B             | Output spektrum yang diterima sensor  | i: indeks (int)                                   |
|               |                                       | wl: pemetaan panjang gelombang (float 400-700)    |
|               |                                       | val: intensitas (float 0-255)                     |
Pengecekan spektrum lampu



B pengambilan blank

A
Absorbansi