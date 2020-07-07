#include "lzHuff.h"
#include "LZ.h"
#include "FastLZ.h"
#include "DeepLZ.h"
#include "Huffman.h"
#include <cstdint>

//compress block of data
int64_t compress_block(char *inBuf, size_t inSize, char *outBuf, size_t outSize, size_t level) {
    LZ *lz;
	if (level == 0) 
        lz = new FastLZ();
	else
        lz = new DeepLZ();
	
   lz->compress(inBuf, inSize);
    Huffman huffman(lz->getOut());
    huffman.compressString();
    string out = huffman.getEncodedString();

    if (out.size() > outSize) return 0;
    memcpy(outBuf, out.c_str(), out.size());
    return out.size();
}

//decompress block of data
int64_t decompress_block(char *inBuf, size_t inSize, char *outBuf, size_t outSize, size_t level) {  
    LZ *lz;

    if (inSize == 0) return 0;
    if (level == 0)
        lz = new FastLZ();
    else
        lz = new DeepLZ();

    Huffman huffman;

    string str(inBuf, inSize);

    huffman.decompressString(str);
    string encodedStr = huffman.getDecodedString();
    lz->strToEncoded(encodedStr);
    string out = lz->decompress();
    if (out.size() > outSize) return 0;
    memcpy(outBuf, out.c_str(), out.size());

    return out.size();
}
