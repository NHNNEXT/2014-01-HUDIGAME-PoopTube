/**
* @author 조익성
* @brief fmod 사운드 재생 박스
* @date 2014/05/11
* @file SoundBox.h
*/
#pragma once
#include "Node.h"
#include "SoundManager.h"

namespace pooptube {
	class CollisionBox;

	class SoundBox :
		public Node
	{
	public:
		SoundBox();
		virtual ~SoundBox();

		static SoundBox *Create( FMOD::Studio::EventInstance* soundInstance );
		bool Init( FMOD::Studio::EventInstance* soundInstance );

		virtual void Update( float dTime );

	private:
		FMOD::Studio::EventInstance* mSound = nullptr;
	};
}
