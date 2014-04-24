#pragma once

#pragma pack(push, 1)

struct PacketHeader
{
	PacketHeader() : mSize(0), mType(0)	{}
	unsigned short	mSize;
	short mType;
};

#pragma pack(pop)

