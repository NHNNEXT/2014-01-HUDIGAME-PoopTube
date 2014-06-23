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
#include "PooMath.h"
#include "EndScene.h"
#include "GameoverScene.h"

Stage::Stage()
{
}
Stage::~Stage()
{
	mBgm->stop( FMOD_STUDIO_STOP_IMMEDIATE );
	mBgm->release();
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

	mLightOrbList.clear();
	mCreatureList.clear();

	SoundManager::GetInstance()->LoadBank( PATH_SOUND_BANK );
	SoundManager::GetInstance()->LoadBank( PATH_SOUND_BANK_STRING );

	mCharacter = MainCharacter::Create( this );
	AddChild( mCharacter );
	ThirdPersonCamera* tCamera = ThirdPersonCamera::Create( mCharacter );
	tCamera->SetFarPlane(200.f);
	mCamera = tCamera;
	AddChild( tCamera );

	/*mSkyBox = pooptube::SkyBox::Create(PATH_SKYBOX_UP,
		PATH_SKYBOX_DOWN,
		PATH_SKYBOX_FRONT,
		PATH_SKYBOX_BACK,
		PATH_SKYBOX_LEFT,
		PATH_SKYBOX_RIGHT);

	mSkyBox->SetTarget(mCharacter);
	AddChild(mSkyBox);*/

	//파일 로드
	mFileName = filename; // 테스트용
	_LoadFile( filename );
	
	// UI 부분(class로 따로 빼야겠음)
	float x = Application::GetInstance()->GetScreenSize().x;
	float y = Application::GetInstance()->GetScreenSize().y;

	for (int i = 0; i < mOrbCount; ++i)
	{
		mClearPoint[i] = nullptr;
		mClearPoint[i] = Sprite::Create(PATH_RING);
		AddChild(mClearPoint[i]);

		mYellow[i] = nullptr;
		mYellow[i] = Sprite::Create(PATH_YELLOW);
		AddChild(mYellow[i]);

		/*mPink[i] = nullptr;
		mPink[i] = pooptube::Sprite::Create(PATH_PINK);
		AddChild(mPink[i]);*/

		mClearPoint[i]->Translate(x / 30.f + static_cast<float>(i * 70), y / 30.f);
		mClearPoint[i]->ApplyTransform();
		mClearPoint[i]->SetVisible(true);

		mYellow[i]->Translate(x / 30.f + static_cast<float>(i * 70), y / 30.f);
		mYellow[i]->ApplyTransform();
		mYellow[i]->SetVisible(false);

		/*mPink[i]->Translate(x / 30.f + static_cast<float>(i * 70), y / 30.f);
		mPink[i]->ApplyTransform();
		mPink[i]->SetVisible(false);*/
	}
	

	return true;
}

void Stage::Render()
{
	Node::Render();

	float cameraPos[3];
	D3DXVECTOR3 cPos = mCamera->GetPosition();
	cameraPos[0] = cPos.x;
	cameraPos[1] = cPos.y;
	cameraPos[2] = cPos.z;

	pooptube::ResourceManager::GetInstance()->LoadHLSL(L"Shader\\SkinnedMesh.fx")->SetFloatArray("mCamaraPos", cameraPos, 3);

	for (int i = 0; i < mOrbCount; ++i) {
		mClearPoint[i]->Draw(NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
		mYellow[i]->Draw(NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
		//mPink[i]->Draw(NULL, &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void Stage::Update( float dTime )
{
	Node::Update( dTime );
	UpdateInput();
	if( this != pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene() )
		return;

	//2초마다 한번씩
	if( mTimeForFPS > 2.f ) {
		printf_s( "FPS : %f\n", Application::GetInstance()->GetFps() );
		printf_s( "DPCall : %d\n", Application::GetInstance()->GetCountDPCall() );
		mTimeForFPS = 0.f;
	}
	mTimeForFPS += dTime;

	SoundManager::GetInstance()->Update();

 	
// 	printf("Creature count : %d \n", mCreatureList.size());

	int orbCount = 0;

	// yellow 그리기
	std::list<LightOrb*>::iterator lightOrbListItor= mLightOrbList.begin();
	for (; lightOrbListItor != mLightOrbList.end(); ++lightOrbListItor) {
		if (!((*lightOrbListItor)->IsRender())) {
			++orbCount;
			if ((*lightOrbListItor)->IsHealable()) {
				mCharacter->InCreaseHP(1);
				(*lightOrbListItor)->SetHealable();
			}
		}
	}
	for (int i = 0; i < orbCount; ++i) {
		mYellow[i]->SetVisible(true);
	}

	// pink 그리기
	//std::list<Creature*>::iterator creatureListItor = mCreatureList.begin();
	//for (; creatureListItor != mCreatureList.end(); ++creatureListItor) {
	//	if ((*creatureListItor)->GetState() == RAGE && (*creatureListItor)->DoRage(dTime) && mCharacter->GetHP() >= 0) {
	//		//mTotalDamage += (*creatureListItor)->GetTotalDamage();
	//		mCharacter->DecreaseHP(1);
	//		//printf("HP!!!!!! : %d\n", mCharacter->GetHP());
	//	}
	//}
	//mCharacter->SetHP(orbCount - mTotalDamage);

	/*for (int i = 0; i < mOrbCount; ++i) {
		if (i < mCharacter->GetHP())
		mPink[i]->SetVisible(true);
		else
		mPink[i]->SetVisible(false);
		}*/

	// HP에 따른 메인케릭터 빛의 범위 변화
// 	if (mCharacter->GetLight()->GetRange() <= static_cast<float>(mCharacter->GetHP()) * 5.f) {
// 		mLightEnhanceTime += dTime;
// 		mCharacter->GetLight()->SetRange(mLightEnhanceTime * 2.f);
// 	}
// 	else
// 	{
// 		mLightEnhanceTime -= dTime;
// 		mCharacter->GetLight()->SetRange(mLightEnhanceTime * 2.f);
// 	}

	std::list<Creature*>::iterator creatureListItor = mCreatureList.begin();
	for (; creatureListItor != mCreatureList.end(); ++creatureListItor) {
		(*creatureListItor)->IncreaseIdleDistance(orbCount);
		//printf("%f\n", (*creatureListItor)->GetIdleDistance());
		if ((*creatureListItor)->GetState() == RAGE && (*creatureListItor)->DoRage(dTime)) {
			//mTotalDamage += (*creatureListItor)->GetTotalDamage();
			mCharacter->DecreaseHP(1);
		}
	}

	if (mCharacter->GetHP() < 0) {
		pooptube::CollisionManager::GetInstance()->ClearCollsionBoxList();
		GameoverScene* pGameoverScene = GameoverScene::Create();
		pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pGameoverScene);
	}

	if (orbCount == mOrbCount)
	{
		pooptube::CollisionManager::GetInstance()->ClearCollsionBoxList();
		EndScene *pEndScene = EndScene::Create();
		pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pEndScene);
	}
	
	/*printf("LightOrb count : %d \n", orbCount);
	printf("HP : %d\n", mCharacter->GetHP());
	printf("DMG : %d\n", mTotalDamage);*/

	if( mBgm != nullptr )
		SoundManager::GetInstance()->PlayOnce( *mBgm );
}

void Stage::UpdateInput()
{
	if (GetInputManager().KeyState('K') == KeyState::KEY_PRESSED) {// stage skip용
		pooptube::CollisionManager::GetInstance()->ClearCollsionBoxList();
		EndScene *pEndScene = EndScene::Create();
		pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pEndScene);
	}
}

void Stage::_LoadFile( std::string& filename )
{
	Json::Value stageData = JsonParser::FileToJson( filename )["Map"];
	_LoadToTarget( this, stageData );
}

void Stage::_LoadToTarget( Node* target, Json::Value& jsonData )
{
	for( auto& data : jsonData ){
		if( data["Class"] == "Tree1" )
			_LoadTree1( target, data );
		else if( data["Class"] == "Tree2" )
			_LoadTree2( target, data );
		else if( data["Class"] == "Tree3" )
			_LoadTree3( target, data );
		else if( data["Class"] == "BillBoard" )
			_LoadBillBoard( target, data );
		else if( data["Class"] == "Creature" )
			_LoadCreature( target, data );
		else if( data["Class"] == "Ground" )
			_LoadGround( target, data );
		else if( data["Class"] == "Camera" )
			_LoadCamera( target, data );
		else if( data["Class"] == "SunLight" )
			_LoadSunLight( target, data );
		else if( data["Class"] == "Node" )
			_LoadNode( target, data );
		else if( data["Class"] == "LightOrb" )
			_LoadLightOrb( target, data );
		else if( data["Class"] == "CollisionBox" )
			_LoadCollisionBox( target, data );
		else if( data["Class"] == "SoundBox" )
			_LoadSoundBox( target, data );
		else if( data["Class"] == "Bgm" )
			_LoadBgm( target, data );
		printf_s( "Load %s\n", data["Class"] );
	}
}

void Stage::_LoadGround( Node* target, Json::Value& jsonData )
{
	std::string str( jsonData.get( "HeightMapFile", "MODEL\\test.jpg" ).asString() );
	std::wstring wStr;
	wStr.assign( str.begin(), str.end() );
	float amp = static_cast<float>(jsonData.get( "HeightAmp", 0.1f ).asDouble());
	mGround = Ground::Create( wStr, amp );
	D3DXVECTOR3 minArea(
		static_cast<float>(jsonData["MinArea"].get( 0u, 0.f ).asDouble()),
		static_cast<float>(jsonData["MinArea"].get( 1u, 0.f ).asDouble()),
		static_cast<float>(jsonData["MinArea"].get( 2u, 0.f ).asDouble())
		);
	D3DXVECTOR3 maxArea(
		static_cast<float>(jsonData["MaxArea"].get( 0u, 0.f ).asDouble()),
		static_cast<float>(jsonData["MaxArea"].get( 1u, 0.f ).asDouble()),
		static_cast<float>(jsonData["MaxArea"].get( 2u, 0.f ).asDouble())
		);
	mGround->SetValidArea( minArea, maxArea );

	_SetCommonData( mGround, jsonData );
	target->AddChild( mGround );
	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( mGround, jsonData["Child"] );
	}
}

void Stage::_LoadCamera( Node* target, Json::Value& jsonData )
{
	Camera *tCamera = Camera::Create();
	_SetCommonData( tCamera, jsonData );
	target->AddChild( tCamera );
	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( tCamera, jsonData["Child"] );
	}
}

void Stage::_LoadSunLight( Node* target, Json::Value& jsonData )
{
	SunLight *tSunLight = SunLight::Create();
	_SetCommonData( tSunLight, jsonData );
	target->AddChild( tSunLight );
	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( tSunLight, jsonData["Child"] );
	}
}

void Stage::_LoadCreature( Node* target, Json::Value& jsonData )
{
	Creature *tCreature = Creature::Create();
	_SetCommonData( tCreature, jsonData );
	tCreature->pss = mCharacter;
	target->AddChild( tCreature );

	tCreature->SetInitialPosition(tCreature->GetPosition());

	mCreatureList.push_back(tCreature);

	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( tCreature, jsonData["Child"] );
	}
}

void Stage::_LoadLightOrb( Node* target, Json::Value& jsonData )
{
	LightOrb *tLightOrb = LightOrb::Create();
	_SetCommonData( tLightOrb, jsonData );

	D3DXVECTOR3 LightOrbPos = tLightOrb->GetPosition();
	float		MapHeight = mGround->GetHeight(LightOrbPos.x, LightOrbPos.z);
	LightOrbPos.y = MapHeight + 1.5f;
	tLightOrb->SetPosition(LightOrbPos);

	target->AddChild( tLightOrb );

	//auto iter = mLightOrbList.begin();
	mLightOrbList.push_back(tLightOrb);

	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( tLightOrb, jsonData["Child"] );
	}
}

void Stage::_LoadBillBoard( Node* target, Json::Value& jsonData )
{
	BillBoard *tBillBoard = BillBoard::Create();
	_SetCommonData( tBillBoard, jsonData );
	std::string texture( jsonData.get( "Texture", "Model\\logo.jpg" ).asString() );
	std::wstring wStr;
	wStr.assign( texture.begin(), texture.end() );
	tBillBoard->SetTexture( wStr );
	target->AddChild( tBillBoard );

	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( tBillBoard, jsonData["Child"] );
	}
}

// 나중에 구조를 좀 바꿔야...
void Stage::_LoadTree1( Node* target, Json::Value& jsonData )
{
	Tree1 *tTree = Tree1::Create();
	_SetCommonData( tTree, jsonData ); 

	D3DXVECTOR3 TreePos = tTree->GetPosition();
	float		MapHeight = mGround->GetHeight(TreePos.x, TreePos.z);
	TreePos.y = MapHeight;
	tTree->SetPosition(TreePos);

	target->AddChild( tTree );

	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( tTree, jsonData["Child"] );
	}
}
void Stage::_LoadTree2( Node* target, Json::Value& jsonData )
{
	Tree2 *tTree = Tree2::Create();
	_SetCommonData( tTree, jsonData );

	D3DXVECTOR3 TreePos = tTree->GetPosition();
	float		MapHeight = mGround->GetHeight(TreePos.x, TreePos.z);
	TreePos.y = MapHeight;
	tTree->SetPosition(TreePos);

	target->AddChild( tTree );

	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( tTree, jsonData["Child"] );
	}
}
void Stage::_LoadTree3( Node* target, Json::Value& jsonData )
{
	Tree3 *tTree = Tree3::Create();
	_SetCommonData( tTree, jsonData );

	D3DXVECTOR3 TreePos = tTree->GetPosition();
	float		MapHeight = mGround->GetHeight(TreePos.x, TreePos.z);
	TreePos.y = MapHeight;
	tTree->SetPosition(TreePos);

	target->AddChild( tTree );

	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( tTree, jsonData["Child"] );
	}
}

void Stage::_LoadSoundBox( Node* target, Json::Value& jsonData )
{
	std::string soundID( jsonData.get( "Sound", "" ).asString() );
	FMOD::Studio::EventInstance* eventInstance = pooptube::SoundManager::GetInstance()->GetSound( soundID );
	pooptube::SoundBox* soundBox = pooptube::SoundBox::Create( eventInstance );
	_SetCommonData( soundBox, jsonData );

	target->AddChild( soundBox );

	if( jsonData.isMember( "Child" ) ){
		_LoadToTarget( soundBox, jsonData["Child"] );
	}
}

void Stage::_LoadBgm( Node* target, Json::Value& jsonData )
{
	std::string soundID( jsonData.get( "Sound", "" ).asString() );
	mBgm = pooptube::SoundManager::GetInstance()->GetSound( soundID );
}

void Stage::_LoadCollisionBox( Node* target, Json::Value& jsonData )
{
	pooptube::CollisionBox* cBox = pooptube::CollisionBox::Create( target );
	_SetCommonData( cBox, jsonData );

	cBox->SetAxisLenX( static_cast<float>(jsonData["AxisLen"].get( 0u, 0.5f ).asDouble()) );
	cBox->SetAxisLenY( static_cast<float>(jsonData["AxisLen"].get( 1u, 0.5f ).asDouble()) );
	cBox->SetAxisLenZ( static_cast<float>(jsonData["AxisLen"].get( 2u, 0.5f ).asDouble()) );

	target->AddChild( cBox );
}

void Stage::_LoadNode( Node* target, Json::Value& jsonData )
{
	// 임시
	_LoadTree1( target, jsonData );
}

void Stage::_SetCommonData( Node* target, Json::Value& jsonData )
{
	target->SetObjectName( jsonData["Name"].asString() );
	target->SetPosition(
		static_cast<float>(jsonData["Position"][0u].asDouble()),
		static_cast<float>(jsonData["Position"][1u].asDouble()),
		static_cast<float>(jsonData["Position"][2u].asDouble())
		);
	target->SetScale(
		static_cast<float>(jsonData["Scale"][0u].asDouble()),
		static_cast<float>(jsonData["Scale"][1u].asDouble()),
		static_cast<float>(jsonData["Scale"][2u].asDouble())
		);

	D3DXVECTOR3 jsonVec(
		static_cast<float>(jsonData["FrontVector"][0u].asDouble()),
		static_cast<float>(jsonData["FrontVector"][1u].asDouble()),
		static_cast<float>(jsonData["FrontVector"][2u].asDouble())
		);
	D3DXVECTOR3 zVec( 0.f, 0.f, 1.f );
	D3DXVECTOR3 tmpVec, zeroVec(0.f, 0.f, 0.f);
	target->SetFrontVector( zVec );
	if( jsonVec == zVec ) return;
	
	float angle;
	
// 	tmpVec = jsonVec;
// 	tmpVec.x = 0.f;
// 	if( tmpVec != zeroVec ){
// 		angle = GetAngle( zVec, tmpVec );
// 		target->RotationX( angle );
// 	}
	tmpVec = jsonVec;
	tmpVec.y = 0.f;
	if( tmpVec != zeroVec ){
		angle = CalculateAngleSignedByY( zVec, tmpVec );
		target->RotationY( angle );
	}
// 	tmpVec = jsonVec;
// 	tmpVec.z = 0.f;
// 	if( tmpVec != zeroVec ){
// 		angle = GetAngle( zVec, tmpVec );
// 		target->RotationZ( angle );
// 	}
}
