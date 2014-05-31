#pragma once
#include "Node.h"

namespace pooptube {

	class Light : public Node{
	
	public:
		Light ();
		virtual ~Light ();

		static Light *Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update(float dTime);

		void	LightOnOff(bool OnOff) { mLightSwitch = OnOff; }
		DWORD	GetIndex() const { return mIndex; }

		void	SetRange(float newRange) { mD3DLight.Range = newRange; }
		float	GetRange() { return mD3DLight.Range; }

		void	  SetSpecular(D3DCOLORVALUE value) { mD3DLight.Specular = value; }
		D3DCOLORVALUE GetSpecular() { return mD3DLight.Specular; }

		void	  SetAmbient(D3DCOLORVALUE value) { mD3DLight.Ambient = value; }
		D3DCOLORVALUE GetAmbient() { return mD3DLight.Ambient; }

		void	  SetDiffuse(D3DCOLORVALUE value) { mD3DLight.Diffuse = value; }
		D3DCOLORVALUE GetDiffuse() { return mD3DLight.Diffuse; }

		void		SetDirection(D3DXVECTOR3 value) { mD3DLight.Direction = value; }
		D3DXVECTOR3 GetDirection() { return mD3DLight.Direction; }

	protected:
		D3DLIGHT9		mD3DLight;

	private:

		DWORD			mIndex = 0;
		static DWORD	mCountIndex;
		float			mRange = 5.f;

		bool			mLightSwitch = true;
	};

}