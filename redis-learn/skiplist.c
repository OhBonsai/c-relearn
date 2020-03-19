//
// Created by 盆栽 on 2020/3/19.
//

#include "skiplist.h"
#include "zmalloc.h"
#include "stdlib.h"



skiplistNode* slCreateNode(int level, double score, robj *obj) {
    skiplistNode *n = zmalloc(sizeof(*n) + level * sizeof(struct skiplistLevel));
    n->score = score;
    n->obj = obj;

    return n;
}

skiplist *slCreate(void) {
    int j;
    skiplist *sl = zmalloc(sizeof(*sl));

    sl->level = 1;
    sl->length = 0;


    sl->header = slCreateNode(ZSKIPLIST_MAXLEVEL, 0, NULL);
    for (j =0; j< ZSKIPLIST_MAXLEVEL; j++) {
        sl->header->level[j].forward = NULL;
        sl->header->level[j].span = 0;
    }

    sl->header->backward = NULL;
    sl->tail = NULL;
    return sl;
}


void slFreeNode(skiplistNode *node) {
    decrRefCount(node->obj);
    zfree(node);
}


void slFree(skiplist *sl) {
    skiplistNode *node = sl->header->level[0].forward, *next;
    zfree(sl->header);

    while (node) {
        next = node->level[0].forward;
        slFreeNode(node);
        node = next;
    }
    zfree(sl);
}


int zslRandomLevel(void) {
    int lvl = 1;
    while ((random() & 0xffff ) < (ZSKIPLIST_P * 0xffff))
        lvl += 1;

    return (lvl < ZSKIPLIST_MAXLEVEL) ? lvl : ZSKIPLIST_MAXLEVEL;
}

skiplistNode *slInsert(skiplist *sl, double score, robj *obj) {
    skiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    unsigned int rank[ZSKIPLIST_MAXLEVEL];

    int i, level;

    x = sl->header;
    for (i = sl->level -1; i>=0; i--) {
        rank[i] = i == (sl->level-1) ? 0 : rank[i+1];
    }

}


//int slDelete(skiplist *sl, double score, robj *obj);
//skiplistNode *slFirstInRange(skiplist *sl, rangespec *range);
//skiplistNode *slLastInRange(skiplist *sl, rangespec *range);
//unsigned long slGetRank(skiplist *sl, double score, robj *o);