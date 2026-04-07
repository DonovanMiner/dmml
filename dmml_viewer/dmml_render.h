#pragma once

//#include <GL\glew.h>
//#include <gl/GL.h>


namespace dmml {
	namespace viewer {

		class dmml_render {
		

			dmml_render();
			~dmml_render();

			void DrawTriangle();


		private:

			unsigned int vertArray_m, vertBuffer_m, idxBuffer_m = 0;

		
		};
	}
}


