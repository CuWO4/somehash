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
test times: 1000
message length: 10 bytes
bits flipped mean: 128.0300
bits flipped std: 7.8476

test times: 1000
message length: 50 bytes
bits flipped mean: 128.0300
bits flipped std: 7.3880

test times: 1000
message length: 500 bytes
bits flipped mean: 127.9533
bits flipped std: 7.5703

test times: 1000
message length: 1000 bytes
bits flipped mean: 128.0925
bits flipped std: 7.8468

test times: 1000
message length: 2000 bytes
bits flipped mean: 127.7860
bits flipped std: 7.9288

test times: 1000
message length: 4000 bytes
bits flipped mean: 127.6183
bits flipped std: 7.7222

test times: 1000
message length: 8000 bytes
bits flipped mean: 127.7400
bits flipped std: 7.8020
```

it takes 2.665s to process a 95.52MB file (35.8MB/s) on my system (AMD Ryzen 9 8945HX).

## Detail

- **Output length**: 256‑bit hash value.
- **Block size**: Processes input in 512‑bit blocks.
- **Streaming support**: Accepts `-` as a filename to read from standard input, enabling pipeline use.
- **Merkel Damgard structure**: each block is compressed independently, then `h = round_func(h, block)` is executed.
- **SPN and ARX mixing round function**: In round function, applies S-box and P-box at the beginning, then each of the 64 rounds applies a nonlinear mixing step using `ch` and `maj` functions, XOR with a round constant, and rotation‑based mixing, then applies S-box and P-box and the end.
- **Round constant**: state XORed with $\frac{2^{256}}{\phi} \times (r+1)$ in each round to avoid symmetry (where $\phi=\frac{\sqrt 5 + 1}{2}$ is the golden ratio).
- **Initialization vectors**: `H0` and `STATE0` randomly generated from `/dev/urandom`.
- **Finalization**: After processing all blocks, the total number of bytes (as a 64‑bit integer) is XORed into both the lower and upper 128 bits of the state, followed by eight applications of the round function to produce the final hash.


## Compile

require `c++>=17`

```bash
make all
```
