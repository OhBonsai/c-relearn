//
// Created by 盆栽 on 2020/3/21.
//

#include <stdio.h>
#include <string.h>
#include "intset.h"
#include "zmalloc.h"

#define INTSET_ENC_INT16 (sizeof(int16_t))
#define INTSET_ENC_INT32 (sizeof(int32_t))
#define INTSET_ENC_INT64 (sizeof(int64_t))

static uint8_t _intsetValueEncoding(int64_t v) {
    if (v < INT32_MIN || v > INT32_MAX)
        return INTSET_ENC_INT64;
    else if (v < INT16_MIN || v > INT16_MAX)
        return INTSET_ENC_INT32;
    else
        return INTSET_ENC_INT16;
}

static int64_t _intsetGetEncoded(intset *is, int pos, uint8_t enc) {
    int64_t v64;
    int32_t v32;
    int16_t v16;

    if (enc == INTSET_ENC_INT64) {
        memcpy(&v64, ((int64_t*)is->contents)+pos, sizeof(v64));
        return v64;
    } else if (enc == INTSET_ENC_INT32) {
        memcpy(&v32,((int32_t*)is->contents)+pos,sizeof(v32));
        return v32;
    } else {
        memcpy(&v16,((int16_t*)is->contents)+pos,sizeof(v16));
        return v16;
    }
}

static int64_t _intsetGet(intset *is, int pos) {
    return _intsetGetEncoded(is, pos, is->encoding);
}

static void _intsetSet(intset *is, int pos, int64_t value) {
    uint32_t encoding = is->encoding;
    if (encoding == INTSET_ENC_INT64) {
        ((int64_t*)is->contents)[pos] = value;
    } else if (encoding == INTSET_ENC_INT32) {
        ((int32_t*)is->contents)[pos] = value;
    } else {
        ((int16_t*)is->contents)[pos] = value;
    }
}


intset *intsetNew(void) {
    intset *is = zmalloc(sizeof(intset));
    is->encoding = INTSET_ENC_INT16;
    is->length = 0;
    return is;
}


static uint8_t intsetSearch(intset *is, int64_t value, uint32_t *pos) {
    int min =0, max= (is->length) -1, mid= -1;
    int64_t cur = -1;

    if (is->length == 0) {
        if(pos) *pos = 0;
        return 0;
    } else {
        if (value > _intsetGet(is, is->length-1)) {
            if (pos) *pos = is->length;
            return 0;
        } else if (value < _intsetGet(is, 0)) {
            if (pos) *pos = 0;
            return 0;
        }
    }

    while (max >= min) {
        mid = (min+max)/2;
        cur = _intsetGet(is, mid);
        if (value > cur) {
            min = mid + 1;
        } else if (value < cur) {
            max = mid - 1;
        } else {
            break;
        }

    }

    if (value == cur) {
        if (pos) *pos = mid;
        return 1;
    } else {
        if (pos) *pos = min;
        return 0;
    }
}

static intset *intsetResize(intset *is, uint32_t len) {
    uint32_t size = len * is->encoding;
    is = zrealloc(is, sizeof(struct intset) + size);
    return is;
}


static intset *intsetUpgradeAndAdd(intset *is, int64_t value) {

    uint8_t curenc = is->encoding;
    uint8_t newenc = _intsetValueEncoding(value);
    int length = is->length;

    int prepend = value < 0 ? 1 : 0;

    is->encoding = newenc;
    is = intsetResize(is, is->length+1);
    while (length--)
        _intsetSet(is, length+prepend, _intsetGetEncoded(is, length, curenc));


    if (prepend)
        _intsetSet(is, 0, value);
    else
        _intsetSet(is, is->length, value);

    is->length = is->length + 1;
    return is;
}






