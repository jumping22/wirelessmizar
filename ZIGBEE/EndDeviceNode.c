#include "EndDeviceNode.h"
#include "HashTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



static int _update(EndDeviceNode *node);
static void _delete(EndDeviceNode *node);
static EndDeviceNode* _getByID(uint16_t id);


static HashTable* ht_EDev;
static EndDeviceNode_class node;

static int _update(EndDeviceNode *node)
{
	char int2str_buf[32];
	sprintf(int2str_buf, "%d", node->id);
    return hash_table_put(ht_EDev, int2str_buf, (void*)&(node->data));
}

static void _delete(EndDeviceNode *node)
{
	char int2str_buf[32];
	sprintf(int2str_buf, "%d", node->id);
    hash_table_rm(ht_EDev, int2str_buf);
}

static EndDeviceNode* _getByID(uint16_t id)
{
	char int2str_buf[32];
	sprintf(int2str_buf, "%d", id);
    return (EndDeviceNode*)hash_table_get(ht_EDev, int2str_buf);
}


EndDeviceNode_class* EndDeviceNode_Init(void)
{
    ht_EDev = hash_table_new();
    if (NULL == ht_EDev) {
        return NULL;
    }

    node.update = _update;
    node.delete = _delete;
    node.getByID = _getByID;

	return &node;
}





