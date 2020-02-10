
#include <stdint.h>
#include <stddef.h>

#include "riscvcrypto/share/util.h"

#ifndef __RISCV_CRYPTO_INTRINSICS__
#define __RISCV_CRYPTO_INTRINSICS__

#ifdef __ZSCRYPTO
#define EMULATE_ZSCRYPTO
#endif

#if __riscv_xlen == 32
#define RISCV_CRYPTO_RV32
#endif

#if __riscv_xlen == 64
#define RISCV_CRYPTO_RV64
#endif

//
// SHA256
//

#if (defined(__ZSCRYPTO) && (defined(RISCV_CRYPTO_RV32) || defined(RISCV_CRYPTO_RV64)))
static inline uint32_t _ssha256_s0 (uint32_t rs1) {uint32_t rd; __asm__ ("ssha256.s0  %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}
static inline uint32_t _ssha256_s1 (uint32_t rs1) {uint32_t rd; __asm__ ("ssha256.s1  %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}
static inline uint32_t _ssha256_s2 (uint32_t rs1) {uint32_t rd; __asm__ ("ssha256.s2  %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}
static inline uint32_t _ssha256_s3 (uint32_t rs1) {uint32_t rd; __asm__ ("ssha256.s3  %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}
#else 
static inline uint32_t _ssha256_s0 (uint32_t rs1) {return (ROTR32(rs1, 2) ^ ROTR32(rs1,13) ^ ROTR32(rs1,22));}
static inline uint32_t _ssha256_s1 (uint32_t rs1) {return (ROTR32(rs1, 6) ^ ROTR32(rs1,11) ^ ROTR32(rs1,25));}
static inline uint32_t _ssha256_s2 (uint32_t rs1) {return (ROTR32(rs1, 7) ^ ROTR32(rs1,18) ^    SHR(rs1, 3));}
static inline uint32_t _ssha256_s3 (uint32_t rs1) {return (ROTR32(rs1,17) ^ ROTR32(rs1,19) ^    SHR(rs1,10));}
#endif

//
// SHA512
//

#if defined(__ZSCRYPTO) && defined(RISCV_CRYPTO_RV64)
static inline uint64_t _ssha512_s0 (uint64_t rs1) {uint64_t rd; __asm__ ("ssha512.s0  %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}
static inline uint64_t _ssha512_s1 (uint64_t rs1) {uint64_t rd; __asm__ ("ssha512.s1  %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}
static inline uint64_t _ssha512_s2 (uint64_t rs1) {uint64_t rd; __asm__ ("ssha512.s2  %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}
static inline uint64_t _ssha512_s3 (uint64_t rs1) {uint64_t rd; __asm__ ("ssha512.s3  %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}
#else 
static inline uint64_t _ssha512_s0 (uint64_t rs1) {return (ROTR64(rs1,28) ^ ROTR64(rs1,34) ^ ROTR64(rs1,39));}
static inline uint64_t _ssha512_s1 (uint64_t rs1) {return (ROTR64(rs1,14) ^ ROTR64(rs1,18) ^ ROTR64(rs1,41));}
static inline uint64_t _ssha512_s2 (uint64_t rs1) {return (ROTR64(rs1, 1) ^ ROTR64(rs1, 8) ^ SHR(rs1,7))    ;}
static inline uint64_t _ssha512_s3 (uint64_t rs1) {return (ROTR64(rs1,19) ^ ROTR64(rs1,61) ^ SHR(rs1,6))    ;}
#endif

//
// SHA3
//

#if (defined(__ZSCRYPTO) && (defined(RISCV_CRYPTO_RV32) || defined(RISCV_CRYPTO_RV64)))
static inline int _ssha3_xy (int x, int y) { int rd; __asm__("ssha3.xy %0, %1, %2" : "=r"(rd) : "r"(x), "r"(y)); return rd;}
static inline int _ssha3_x1 (int x, int y) { int rd; __asm__("ssha3.x1 %0, %1, %2" : "=r"(rd) : "r"(x), "r"(y)); return rd;}
static inline int _ssha3_x2 (int x, int y) { int rd; __asm__("ssha3.x2 %0, %1, %2" : "=r"(rd) : "r"(x), "r"(y)); return rd;}
static inline int _ssha3_x4 (int x, int y) { int rd; __asm__("ssha3.x4 %0, %1, %2" : "=r"(rd) : "r"(x), "r"(y)); return rd;}
static inline int _ssha3_yx (int x, int y) { int rd; __asm__("ssha3.yx %0, %1, %2" : "=r"(rd) : "r"(x), "r"(y)); return rd;}

static inline uint64_t * _ssha3_idx (uint64_t * base, int idx) {uint64_t * rd; __asm__("add %0, %1, %2 ;" : "=r"(rd) : "r"(base), "r"(idx)); return rd;}
#else
static inline int _ssha3_xy (int x, int y) { return ((x  )%5)+(5*((      y)%5));}
static inline int _ssha3_x1 (int x, int y) { return ((x+1)%5)+(5*((      y)%5));}
static inline int _ssha3_x2 (int x, int y) { return ((x+2)%5)+(5*((      y)%5));}
static inline int _ssha3_x4 (int x, int y) { return ((x+4)%5)+(5*((      y)%5));}
static inline int _ssha3_yx (int x, int y) { return ((y  )%5)+(5*((2*x+3*y)%5));}

static inline uint64_t * _ssha3_idx (uint64_t * base, int idx) {uint64_t * rd; __asm__("add %0, %1, %2 ;" : "=r"(rd) : "r"(base), "r"(idx)); return rd;}
#endif

//
// AES
//

#if (defined(__ZSCRYPTO))
static inline uint32_t _saes_v1_enc(uint32_t rs1) {uint32_t rd; __asm__("saes.v1.enc %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}
static inline uint32_t _saes_v1_dec(uint32_t rs1) {uint32_t rd; __asm__("saes.v1.dec %0, %1" : "=r"(rd) : "r"(rs1)); return rd;}

static inline uint32_t _saes_v2_sub_enc   (uint32_t rs1, uint32_t rs2) {uint32_t rd; __asm__("saes.v2.sub.enc    %0, %1, %2" : "=r"(rd) : "r"(rs1), "r"(rs2)); return rd;}
static inline uint32_t _saes_v2_sub_encrot(uint32_t rs1, uint32_t rs2) {uint32_t rd; __asm__("saes.v2.sub.encrot %0, %1, %2" : "=r"(rd) : "r"(rs1), "r"(rs2)); return rd;}
static inline uint32_t _saes_v2_sub_dec   (uint32_t rs1, uint32_t rs2) {uint32_t rd; __asm__("saes.v2.sub.dec    %0, %1, %2" : "=r"(rd) : "r"(rs1), "r"(rs2)); return rd;}
static inline uint32_t _saes_v2_sub_decrot(uint32_t rs1, uint32_t rs2) {uint32_t rd; __asm__("saes.v2.sub.decrot %0, %1, %2" : "=r"(rd) : "r"(rs1), "r"(rs2)); return rd;}
static inline uint32_t _saes_v2_mix_enc   (uint32_t rs1, uint32_t rs2) {uint32_t rd; __asm__("saes.v2.mix.enc    %0, %1, %2" : "=r"(rd) : "r"(rs1), "r"(rs2)); return rd;}
static inline uint32_t _saes_v2_mix_dec   (uint32_t rs1, uint32_t rs2) {uint32_t rd; __asm__("saes.v2.mix.dec    %0, %1, %2" : "=r"(rd) : "r"(rs1), "r"(rs2)); return rd;}

static inline uint32_t _saes_v3_encs (uint32_t rs1, uint32_t rs2, int bs) {uint32_t rd; __asm__("saes.v3.encs  %0, %1, %2, %3" : "=r"(rd) : "r"(rs1), "r"(rs2), "i"(bs)); return rd;}
static inline uint32_t _saes_v3_encm (uint32_t rs1, uint32_t rs2, int bs) {uint32_t rd; __asm__("saes.v3.encm  %0, %1, %2, %3" : "=r"(rd) : "r"(rs1), "r"(rs2), "i"(bs)); return rd;}
static inline uint32_t _saes_v3_encsm(uint32_t rs1, uint32_t rs2, int bs) {uint32_t rd; __asm__("saes.v3.encsm %0, %1, %2, %3" : "=r"(rd) : "r"(rs1), "r"(rs2), "i"(bs)); return rd;}
static inline uint32_t _saes_v3_decs (uint32_t rs1, uint32_t rs2, int bs) {uint32_t rd; __asm__("saes.v3.decs  %0, %1, %2, %3" : "=r"(rd) : "r"(rs1), "r"(rs2), "i"(bs)); return rd;}
static inline uint32_t _saes_v3_decm (uint32_t rs1, uint32_t rs2, int bs) {uint32_t rd; __asm__("saes.v3.decm  %0, %1, %2, %3" : "=r"(rd) : "r"(rs1), "r"(rs2), "i"(bs)); return rd;}
static inline uint32_t _saes_v3_decsm(uint32_t rs1, uint32_t rs2, int bs) {uint32_t rd; __asm__("saes.v3.decsm %0, %1, %2, %3" : "=r"(rd) : "r"(rs1), "r"(rs2), "i"(bs)); return rd;}
#endif

//
// Bitmanip Instruction Intrinsics
//

#if (defined(__ZSCRYPTO))
static inline uint32_t _pack  (uint32_t rs1, uint32_t rs2) {uint32_t rd; __asm__("pack  %0, %1, %2" : "=r"(rd) : "r"(rs1), "r"(rs2)); return rd;}
static inline uint32_t _packu (uint32_t rs1, uint32_t rs2) {uint32_t rd; __asm__("packu %0, %1, %2" : "=r"(rd) : "r"(rs1), "r"(rs2)); return rd;}
static inline uint32_t _packh (uint32_t rs1, uint32_t rs2) {uint32_t rd; __asm__("packh %0, %1, %2" : "=r"(rd) : "r"(rs1), "r"(rs2)); return rd;}
#endif

#endif // __RISCV_CRYPTO_INTRINSICS__

