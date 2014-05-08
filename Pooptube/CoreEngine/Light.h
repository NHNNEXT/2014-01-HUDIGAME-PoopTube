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


	protected:
		D3DLIGHT9		mD3DLight;

	private:

		DWORD			mIndex = 0;
		static DWORD	mCountIndex;

		bool			mLightSwitch = true;
	};

}