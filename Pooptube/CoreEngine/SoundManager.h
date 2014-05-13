/**
* @author 조익성
* @brief fmod 사운드 컨트롤
* @date 2014/05/11
* @file SoundManager.h
*/
#pragma once
#include "Object.h"
#include "..\Library\FMOD\fmod.hpp"
#include "..\Library\FMOD\fmod_studio.hpp"
#include "..\Library\FMOD\fmod_errors.h"
#include <unordered_map>

namespace pooptube {
	class Node;

	class SoundManager : public Object
	{
	public:
		static SoundManager* GetInstance();
		static void ReleaseInstance();

		void LoadBank( std::string filename );
		FMOD::Studio::EventInstance* GetSound( std::string eventID );
		void SetListener( FMOD_3D_ATTRIBUTES* listener );
		void Update() { mSystem->update(); }

		void DxVecToFmodVec( D3DXVECTOR3& dxVec, FMOD_VECTOR& fmodVec );
		void NodeToFmod3DAttribute( const Node& node, FMOD_3D_ATTRIBUTES& fmodVec );
		void PlayOnce( FMOD::Studio::EventInstance& sound );

	private:
		SoundManager();
		~SoundManager();

		FMOD::Studio::EventDescription* GetEvent( std::string eventID );

		static SoundManager* mInstance;

		FMOD::Studio::System* mSystem;
		std::vector<FMOD::Studio::Bank*> mBankList;
		std::unordered_map< std::string, FMOD::Studio::EventDescription* > mEventList;
	};
}
