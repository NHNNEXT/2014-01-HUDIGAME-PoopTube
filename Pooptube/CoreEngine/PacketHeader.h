#pragma once

struct PacketHeader
{
	PacketHeader() : mSize(0), mType(0)	{}
	unsigned short	mSize;
	short mType;
};
