# SOMEHASH

a lightweight modern hash variant implementation

---

## Quick View

```bash
$ somehashsum gen_pbox.py gen_sbox.py
6bb3491758ee64fd5273bc70b4eec1e0  gen_pbox.py
fccbd0d70110cdaaf120efa0cb00b870  gen_sbox.py
$ somehashsum gen_pbox.py gen_sbox.py > sum
$ somehashsum -c sum
gen_pbox.py: OK
gen_sbox.py: OK
```

takes 27.188s to process a 7.0 GB file (263MB/s) on my system (AMD Ryzen 9 8945HX, ZHITAITiPlus5000 3500MB/s seq read).

## Usage

```bash
somehashsum [-h|--help] [-c|--check] [<FILE>]...
```

command line argument meaning stay consistent with md5sum.

## Detail

- **Output length**: 128‑bit hash value.
- **Block size**: Processes input in 4096‑bit blocks.
- **Streaming support**: Accepts `-` as a filename to read from standard input, enabling pipeline use.
- **Sponge‑like byte absorption**: Each input byte is XORed into the state, then the state is multiplied by a round‑dependent constant and rotated.
- **SPN round function**: Each round applies an 8‑bit S‑box (substitution), a bitwise permutation (P‑box), a linear mixing step, and a multiplication by round constant.
- **S‑box properties**: 8×8 S‑box derived from the GF(256) inverse with seed b'114514'; nonlinearity 112, differential uniformity 4, no fixed points.
- **P‑box properties**: 128‑bit single cycle permutation with an average displacement distance of 50.02, generated from `/dev/urandom`.
- **Round constant**: state multiplied by the $\frac{2^{128}}{\phi}$ $\times$ round index in round function to avoid symmetry (where $\phi=\frac{\sqrt 5 - 1}{2}$ is the golden ratio).
- **Initialization vector**: `STATE0` randomly generated from `/dev/urandom`.
- **Finalization**: After processing all blocks, the total bit length is XORed into the state (both low and high 64‑bit halves), followed by one full round function to produce the hash.

## Compile

require `c++>=17`

```bash
make all
```
