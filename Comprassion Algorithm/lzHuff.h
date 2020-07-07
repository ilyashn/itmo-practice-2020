
#ifdef __cplusplus
extern "C" {
#endif
#ifndef COMPRESSIONALG_LZHUFF_H
#define COMPRESSIONALG_LZHUFF_H

#include <cstdint>
#include <cstddef>

int64_t compress_block(char *inBuf, size_t inSize, char *outBuf, size_t outSize, size_t level);

int64_t decompress_block(char *inBuf, size_t inSize, char *outBuf, size_t outSize, size_t level);

#endif //COMPRESSIONALG_LZHUFF_H
#ifdef __cplusplus
}
#endif
