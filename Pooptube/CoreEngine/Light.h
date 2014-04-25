#pragma once
#include "Node.h"

namespace pooptube {

	class Light : public Node{
	
	public:
		Light ();
		~Light ();

		static std::shared_ptr<Light> Create();

		bool Init();

		void Render();
		void Update(float dTime);


	private:



	};

}