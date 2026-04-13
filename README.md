# SOMEHASH

a lightweight modern hash variant implementation

---

## Quick View

```bash
$ somehashsum gen_pbox.py gen_sbox.py
d212aa2faba8d9697337420735363a79c86b649cdbb315a07637006f8fb9bb46  gen_pbox.py
2f905d02538cf0529ee03e5f45266bd720c42d7440fdd1172c0687063ebd4820  gen_sbox.py
$ somehashsum gen_pbox.py gen_sbox.py > sum
$ somehashsum -c sum
gen_pbox.py: OK
gen_sbox.py: OK
```

## Usage

```bash
somehashsum [-h|--help] [-c|--check] [<FILE>]...
```

command line argument meaning stay consistent with md5sum/sha1sum/sha256sum.

## Performance

```bash
$ python3 avalanche_test.py
message length: 10 bytes
bits flipped mean: 127.1300
bits flipped std: 8.2641
test times: 1000

message length: 50 bytes
bits flipped mean: 127.0500
bits flipped std: 8.0774
test times: 1000

message length: 500 bytes
bits flipped mean: 127.2367
bits flipped std: 7.8740
test times: 1000

message length: 1000 bytes
bits flipped mean: 127.6225
bits flipped std: 8.0384
test times: 1000

message length: 2000 bytes
bits flipped mean: 128.0400
bits flipped std: 8.0784
test times: 1000

message length: 4000 bytes
bits flipped mean: 128.0817
bits flipped std: 8.2252
test times: 1000

message length: 8000 bytes
bits flipped mean: 128.1771
bits flipped std: 8.1249
test times: 1000
```

it takes 5.805s to process a 975.97MB single file (168.1MB/s) on my system (AMD Ryzen 9 8945HX), and the program automatically use $\text{max}(1, \lfloor\frac 3 4\times\text{＃vCPU}\rfloor )$ of vCPU for multithread compression.

## Detail

- **Output length**: 256‑bit hash value.
- **Block size**: Processes input in 512‑bit blocks.
- **Streaming support**: Accepts `-` as a filename to read from standard input, enabling pipeline use.
- **Merkel Damgard structure**: each block is compressed independently, then `h = round_func(h, block)` is executed.
- **SPN and ARX mixing round function**: In round function, applies S-box and P-box at the beginning, then each of the 64 rounds applies a nonlinear mixing step using `ch` and `maj` functions, XOR with a round constant, and rotation‑based mixing, then applies S-box and P-box and the end.
- **S‑box properties**: 8×8 S‑box derived from the GF(2^8) inverse with seed b'114514'; nonlinearity = 112, differential uniformity = 4, no fixed points.
- **P‑box properties**: 128‑bit single cycle permutation with an average displacement distance of 96.38, use true random source `/dev/urandom` as seed.
- **Round constant**: state XORed with $\frac{2^{256}}{\phi} \times (r+1)$ in each round to avoid symmetry (where $\phi=\frac{\sqrt 5 + 1}{2}$ is the golden ratio).
- **Initialization vectors**: `H0` and `STATE0` randomly generated from `/dev/urandom`.
- **Finalization**: After processing all blocks, the total number of bytes (as a 64‑bit integer) is XORed into both the lower and upper 128 bits of the state, followed by eight applications of the round function to produce the final hash.

## Compile

require `c++>=17`

```bash
make all
```
