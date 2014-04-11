
#pragma once

#include "Node.h"


namespace pooptube {

	class Mesh : public Node {
	public:
		Mesh();
		virtual ~Mesh();

		static Mesh* Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update( float dTime );


	protected:
		

	private:

	};
}