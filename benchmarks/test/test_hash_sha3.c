
#include "riscvcrypto/share/test.h"
#include "riscvcrypto/share/util.h"

#include "riscvcrypto/crypto_hash/sha3/fips202.h"

unsigned char seed[CRYPTO_HASH_SHA3_512_OUTPUT_LENGTH] = {0x56,0xac,0x4f,0x68,0x45,0xa4,0x51,0xda,0xc3,0xe8,0x88,0x6f,0x97,0xf7,0x02,0x4b,0x64,0xb1,0xb1,0xe9,0xc5,0x18,0x1c,0x05,0x9b,0x57,0x55,0xb9,0xa6,0x04,0x2b,0xe6,0x53,0xa2,0xa0,0xd5,0xd5,0x6a,0x9e,0x1e,0x77,0x4b,0xe5,0xc9,0x31,0x2f,0x48,0xb4,0x79,0x80,0x19,0x34,0x5b,0xea,0xe2,0xff,0xcc,0x63,0x55,0x4a,0x3c,0x69,0x86,0x2e};

unsigned char expect[CRYPTO_HASH_SHA3_512_OUTPUT_LENGTH] = {0x5f,0xde,0x5c,0x57,0xa3,0x1f,0xeb,0xb9,0x80,0x61,0xf2,0x7e,0x45,0x06,0xfa,0x5c,0x24,0x55,0x06,0x33,0x6e,0xe9,0x0d,0x59,0x5c,0x91,0xd7,0x91,0xa5,0x97,0x5c,0x71,0x2b,0x3a,0xb9,0xb3,0xb5,0x86,0x8f,0x94,0x1d,0xb0,0xae,0xb4,0xc6,0xd2,0x83,0x7c,0x44,0x47,0x44,0x2f,0x84,0x02,0xe0,0xe1,0x50,0xa9,0xdc,0x0e,0xf1,0x78,0xdc,0xa8};

int sha3_kat() {
    unsigned char       hash_signature  [CRYPTO_HASH_SHA3_512_OUTPUT_LENGTH];
    FIPS202_SHA3_512(
        seed                                ,
        CRYPTO_HASH_SHA3_512_OUTPUT_LENGTH  ,
        hash_signature 
    );
    
    for(int i = 0; i < CRYPTO_HASH_SHA3_512_OUTPUT_LENGTH; i ++) {
        if(hash_signature[i] != expect[i]) {
            printf("Expected output byte %d = %x, got %x\n",i,
                expect[i],
                hash_signature[i]);
            return 1;
        }
    }

    return 0;
}


int main(int argc, char ** argv) {

    if(sha3_kat() != 0) {
        return 1;
    }

    printf("Running SHA3 " STR(TEST_NAME) " benchmark...\n");

    unsigned char       hash_input      [TEST_HASH_INPUT_LENGTH  ];
    unsigned long long  hash_input_len = TEST_HASH_INPUT_LENGTH   ;
    unsigned char       hash_signature  [CRYPTO_HASH_SHA3_512_OUTPUT_LENGTH];

    printf("Reading %d random bytes as input...\n", TEST_HASH_INPUT_LENGTH);
    test_rdrandom(hash_input, TEST_HASH_INPUT_LENGTH);

    const uint64_t start_instrs   = test_rdinstret();

    FIPS202_SHA3_512(
        seed          ,
        hash_input_len,
        hash_signature 
    );
    
    const uint64_t end_instrs     = test_rdinstret();

    const uint64_t final_instrs   = end_instrs - start_instrs;


    printf("PERF: "STR(TEST_NAME) " instrs: 0x");
    puthex64(final_instrs); printf("\n");

    return 0;
}
