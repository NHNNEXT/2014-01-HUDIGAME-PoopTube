#include "stdafx.h"
#include "Stage.h"
#include "Application.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "CollisionBox.h"
#include "SkinnedMesh.h"
#include "ThirdPersonCamera.h"
#include "MainCharacter.h"
#include "SkyBox.h"
#include "Light.h"
#include "SunLight.h"
#include "Ground.h"
#include "Creature.h"
#include "LightOrb.h"
#include "XMesh.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "SoundBox.h"
#include "InputManager.h"
#include "ResourceDef.h"
#include "Tree1.h"
#include "Tree2.h"
#include "Tree3.h"
#include "BillBoard.h"

Stage::Stage()
{
}
Stage::~Stage()
{
}

Stage* Stage::Create( std::string filename ) {
	Stage* pStage = new Stage;
	if( pStage->Init( filename ) ) {
		ObjectManager::GetInstance()->AddObject( pStage );
	}
	else {
		delete pStage;
		pStage = nullptr;
	}

	return pStage;
}

bool Stage::Init( std::string filename )
{
	Scene::Init();

	SoundManager::GetInstance()->LoadBank( PATH_SOUND_BANK );
	SoundManager::GetInstance()->LoadBank( PATH_SOUND_BANK_STRING );

	mCharacter = MainCharacter::Create();
	AddChild( mCharacter );
	
	AddChild( ThirdPersonCamera::Create( mCharacter ) );

	//파일 로드
	Json::Value stageData = JsonParser::FileToJson( filename )["Map"];
	
	for( auto data : stageData ){
		if( data["Class"] == "Ground" )
			_LoadGround( data );
// 		else if( data["Class"] == "Camera" )
// 			_LoadCamera( data );
		else if( data["Class"] == "SunLight" )
			_LoadSunLight( data );
		else if( data["Class"] == "Creature" )
			_LoadCreature( data );
		else if( data["Class"] == "LightOrb" )
			_LoadLightOrb( data );
		else if( data["Class"] == "BillBoard" )
			_LoadBillBoard( data );
		else if( data["Class"].asString().find( "Tree" ) != std::string::npos )
			_LoadTree( data );
		else if( data["Class"] == "Node" )
			_LoadNode( data );
		printf_s( "Load %s\n", data["Class"] );
	}
	
	return true;
}

void Stage::Render()
{
	Node::Render();
}

void Stage::Update( float dTime )
{
	Node::Update( dTime );

	//2초마다 한번씩
	if( mTimeForFPS > 2.f ) {
		printf_s( "FPS : %f\n", Application::GetInstance()->GetFps() );
		printf_s( "DPCall : %d\n", Application::GetInstance()->GetCountDPCall() );
		mTimeForFPS = 0.f;
	}
	mTimeForFPS += dTime;

	SoundManager::GetInstance()->Update();
}

void Stage::UpdateInput()
{
	if( GetInputManager().KeyState( 'R' ) == KeyState::KEY_PRESSED )
		mGround->_SetBuffer();
	if( GetInputManager().KeyState( 'T' ) == KeyState::KEY_PRESSED )
		mCharacter->Move( 0.1f, 0.f );
	if( GetInputManager().KeyState( 'G' ) == KeyState::KEY_PRESSED )
		mCharacter->Move( -0.1f, 0.f );
	if( GetInputManager().KeyState( 'F' ) == KeyState::KEY_PRESSED )
		mCharacter->Move( 0.f, 0.1f );
	if( GetInputManager().KeyState( 'H' ) == KeyState::KEY_PRESSED )
		mCharacter->Move( 0.f, -0.1f );
}

void Stage::_LoadGround( Json::Value& jsonData )
{
	mGround = Ground::Create( PATH_HEIGHTMAP ); // json파일에 파일 정보가 필요하다...
	_SetCommonData( mGround, jsonData );
	AddChild( mGround );
}

void Stage::_LoadCamera( Json::Value& jsonData )
{
	Camera *tCamera = Camera::Create();
	_SetCommonData( tCamera, jsonData );
	AddChild( tCamera );
}

void Stage::_LoadSunLight( Json::Value& jsonData )
{
	SunLight *tSunLight = SunLight::Create();
	_SetCommonData( tSunLight, jsonData );
	AddChild( tSunLight );
}

void Stage::_LoadCreature( Json::Value& jsonData )
{
	Creature *tCreature = Creature::Create();
	_SetCommonData( tCreature, jsonData );
	tCreature->pss = mCharacter;
	AddChild( tCreature );
}

void Stage::_LoadLightOrb( Json::Value& jsonData )
{
	LightOrb *tLightOrb = LightOrb::Create();
	_SetCommonData( tLightOrb, jsonData );
	AddChild( tLightOrb );
}

void Stage::_LoadBillBoard( Json::Value& jsonData )
{
	BillBoard *tBillBoard = BillBoard::Create();
	_SetCommonData( tBillBoard, jsonData );
	tBillBoard->SetTexture( L"Model\\logo.jpg" ); // json파일에 파일 정보가 필요하다...
	AddChild( tBillBoard );
}

void Stage::_LoadTree( Json::Value& jsonData )
{
	// 나중에 파일 형식을 좀 바꿔야...
	if( jsonData["Class"] == "Tree1" ){
		Tree1 *tTree = Tree1::Create();
		_SetCommonData( tTree, jsonData );
		AddChild( tTree );
	}
	else if( jsonData["Class"] == "Tree2" ){
		Tree2 *tTree = Tree2::Create();
		_SetCommonData( tTree, jsonData );
		AddChild( tTree );
	}
	else if( jsonData["Class"] == "Tree3" ){
		Tree3 *tTree = Tree3::Create();
		_SetCommonData( tTree, jsonData );
		AddChild( tTree );
	}
}

void Stage::_LoadNode( Json::Value& jsonData )
{
	// 임시
	Tree3 *tTree = Tree3::Create();
	_SetCommonData( tTree, jsonData );
	AddChild( tTree );
}

void Stage::_SetCommonData( Node* target, Json::Value& jsonData )
{
	target->SetObjectName( jsonData["Name"].asString() );
	target->SetPosition( jsonData["Position"][0u].asDouble(), jsonData["Position"][1u].asDouble(), jsonData["Position"][2u].asDouble() );
	target->SetScale( jsonData["Scale"][0u].asDouble(), jsonData["Scale"][1u].asDouble(), jsonData["Scale"][2u].asDouble() );
	target->SetFrontVector( jsonData["FrontVector"][0u].asDouble(), jsonData["FrontVector"][1u].asDouble(), jsonData["FrontVector"][2u].asDouble() );
}
