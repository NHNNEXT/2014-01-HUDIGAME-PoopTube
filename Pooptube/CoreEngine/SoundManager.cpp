#include "stdafx.h"
#include "SoundManager.h"
#include "Node.h"

namespace pooptube {
// 	void( *Common_Private_Error )(FMOD_RESULT);
// 
// 	void ERRCHECK( FMOD_RESULT result )
// 	{
// 		if( result != FMOD_OK )
// 		{
// 			if( Common_Private_Error )
// 			{
// 				Common_Private_Error( result );
// 			}
// 			printf_s( "FMOD error %d - %s\n", result, FMOD_ErrorString( result ) );
// 		}
// 	}

	SoundManager* SoundManager::mInstance = nullptr;

	SoundManager::SoundManager(){
	}

	SoundManager::~SoundManager(){
		for( auto& bank : mBankList ){
			bank->unload();
		}
	}

	SoundManager* pooptube::SoundManager::GetInstance()
	{
		if( mInstance == nullptr ) {
			mInstance = new SoundManager();

			mInstance->mSystem = nullptr;
			FMOD::Studio::System::create( &mInstance->mSystem );
//			ERRCHECK( mInstance->mSystem->initialize( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0 ) );
// 			mInstance->mSystem->initialize( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0 );

			ObjectManager::GetInstance()->AddObject( mInstance );
		}

		return mInstance;
	}

	void SoundManager::ReleaseInstance() {
		if( mInstance != nullptr )
			delete mInstance;
	}

	void SoundManager::LoadBank( std::string filename )
	{
		FMOD::Studio::Bank* tempBank = nullptr;
//		ERRCHECK( mSystem->loadBankFile( filename.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &tempBank ) );
		mSystem->loadBankFile( filename.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &tempBank );
		mBankList.push_back( tempBank );
	}

	FMOD::Studio::EventDescription* SoundManager::GetEvent( std::string eventID )
	{
		if( mEventList.find( eventID ) == mEventList.end() ){
			FMOD::Studio::ID tempID = { 0 };
//			ERRCHECK( mSystem->lookupID( eventID.c_str(), &tempID ) );
			mSystem->lookupID( eventID.c_str(), &tempID );
			
			FMOD::Studio::EventDescription* eventDescription = nullptr;
//			ERRCHECK( mSystem->getEvent( &tempID, FMOD_STUDIO_LOAD_BEGIN_NOW, &eventDescription ) );
			mSystem->getEvent( &tempID, FMOD_STUDIO_LOAD_BEGIN_NOW, &eventDescription );

			return eventDescription;
		}
		else{
			return mEventList[eventID];
		}
	}

	FMOD::Studio::EventInstance* SoundManager::GetSound( std::string eventID )
	{
		FMOD::Studio::EventInstance* tempInstance(nullptr);
//		ERRCHECK( GetEvent( eventID )->createInstance( &tempInstance ) );
		GetEvent( eventID )->createInstance( &tempInstance );
		return tempInstance;
	}

	void SoundManager::SetListener( FMOD_3D_ATTRIBUTES* listener )
	{
// 		FMOD_3D_ATTRIBUTES* tempListner(nullptr);
// 		ERRCHECK( mSystem->getListenerAttributes( tempListner ) );
// 
// 		return tempListner;
//		ERRCHECK( mSystem->setListenerAttributes( listener ) );
		mSystem->setListenerAttributes( listener );
	}

	void SoundManager::DxVecToFmodVec( D3DXVECTOR3& dxVec, FMOD_VECTOR& fmodVec )
	{
		fmodVec.x = dxVec.x;
		fmodVec.y = dxVec.y;
		fmodVec.z = dxVec.z;
	}

	void SoundManager::NodeToFmod3DAttribute( const Node& node, FMOD_3D_ATTRIBUTES& fmodAttr )
	{
		DxVecToFmodVec( node.GetPosition(), fmodAttr.position );
		DxVecToFmodVec( node.GetFrontVector(), fmodAttr.forward );
		DxVecToFmodVec( node.GetUpVector(), fmodAttr.up );
	}

	void SoundManager::PlayOnce( FMOD::Studio::EventInstance& sound )
	{
		FMOD_STUDIO_PLAYBACK_STATE curState;
		sound.getPlaybackState( &curState );
		if( curState != FMOD_STUDIO_PLAYBACK_PLAYING )
			sound.start();
	}

}