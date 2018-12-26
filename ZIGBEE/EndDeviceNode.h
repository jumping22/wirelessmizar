#ifndef __ENDDEVICE_NODE_H
#define __ENDDEVICE_NODE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
  uint8_t type;
  uint8_t data[4];
} NodeDataParam;

typedef struct {
    uint16_t id;
    NodeDataParam data;
}EndDeviceNode;

typedef struct {
    int (*update)(EndDeviceNode*);
    void (*delete)(EndDeviceNode*);
    EndDeviceNode* (*getByID)(uint16_t);
}EndDeviceNode_class;

/* EndDeviceNode_class function */
/*
 * 		int update(EndDeviceNode *node);
 *   	void delete(EndDeviceNode *node);
 *   	EndDeviceNode* getByID(uint16_t id);
 *
 *
 */

EndDeviceNode_class* EndDeviceNode_Init(void);



#ifdef __cplusplus
}
#endif

#endif
