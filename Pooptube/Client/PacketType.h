#pragma once

#define MAX_CHAT_LEN	1024

#define MAX_NAME_LEN	30
#define MAX_COMMENT_LEN	40

#include "../../Pooptube/CoreEngine/PacketHeader.h"

enum PacketTypes
{
	PKT_NONE = 0,

	PKT_CS_LOGIN = 1,
	PKT_SC_LOGIN = 2,

	PKT_CS_ACCELERATION = 3,
	PKT_SC_ACCELERATION = 4,

	PKT_CS_STOP = 5,
	PKT_SC_STOP = 6,

	PKT_CS_ROTATION = 7,
	PKT_SC_ROTATION = 8,

	PKT_CS_SYNC = 9,
	PKT_SC_SYNC = 10,

	PKT_SC_NEW = 11, // 새 플레이어나 오브젝트 생성

	PKT_MAX = 1024
};

#pragma pack(push, 1)

// 로그인 요청
struct LoginRequest : public PacketHeader
{
	LoginRequest()
	{
		mSize = sizeof(LoginRequest);
		mType = PKT_CS_LOGIN;
		mPlayerId = -1;
	}

	int	mPlayerId;
};

struct LoginResult : public PacketHeader
{
	LoginResult()
	{
		mSize = sizeof(LoginResult);
		mType = PKT_SC_LOGIN;
		mPlayerId = -1;
		memset(mName, 0, MAX_NAME_LEN);
	}

	int		mPlayerId;
	double	mPosX;
	double	mPosY;
	double	mPosZ;
	char	mName[MAX_NAME_LEN];
};

#pragma pack(pop)