/*
 * zproto.c
 *
 *  Created on: Sep 24, 2014
 *      Author: toto
 */

#include "zproto.h"

int zproto_checkValidity(const void *data, uint8 size)
{
	ZHeader *header = (ZHeader*)data;
	if (header->signature != ZPROTO_HEADER_SIGNATURE)
		return -1;

	if (header->type == 0)
		return -1;

	if (header->length > size)
		return 0;

	ZTailer *tailer = (ZTailer*)data + header->length - 1;
	if (tailer->signature != ZPROTO_TAILER_SIGNATURE)
		return -1;
	return header->length;
}

int zproto_parseFromArray(ZProto *proto, void *data, uint8 size)
{
	int ret = zproto_checkValidity(data, size);
	if (ret <= 0)
		return ret;
	proto->data = data;
	proto->size = ret;
	return ret;
}

ZHeader* zproto_header(ZProto *proto)
{
	return (ZHeader*)proto->data;
}

int zproto_type(ZProto *proto)
{
	return zproto_header(proto)->type;
}
