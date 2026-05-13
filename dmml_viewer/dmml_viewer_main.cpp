//#include "dmml_viewer_vulk.h"
//
//int main() {
//
//	dmml::viewer::dmml_viewer App = dmml::viewer::dmml_viewer();
//	App.CreateWindow();
//	App.InitContext();
//	App.InitBackends();
//	App.MainLoop();
//	App.CleanupWindow();
//	App.~dmml_viewer();
//
//	return 0;
//}

//#include "dmml_viewer_opengl.h"
//
//int main() {
//
//	auto app = dmml::viewer::dmml_viewer::dmml_viewer();
//	app.CreateDMMLWindow();
//	app.InitImGuiContext();
//	app.MainLoop();
//	app.CleanupWindow();
//
//	
//
//	return 0;
//}



//#include "dmml_render.h"
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <SDL3/SDL_opengl.h>
#include <GL/GL.h>
#include <iostream>

int main() {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "Error: SDL_Init(): %s\n";
		return 1;
	}

	//change these?
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	float displayScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

	SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	SDL_Window* window = SDL_CreateWindow("DMML Viewer", (int)(1280 * displayScale), (int)(800 * displayScale), window_flags);


	if (window == nullptr) {
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return 1;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
		return 1;
	}

	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW Err\n";
	}

	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	std::cout << "OpenGL Ver: " << glGetString(GL_VERSION) << std::endl;



	float pos[6] = { -0.5f, -0.5f,
					  0.0f, 0.5f,
					  0.5f, -0.5f };
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, pos, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);


	unsigned int vertArray_m, vertBuffer_m, idxBuffer_m = 0;
	//glGenVertexArrays(1, &vertArray_m);
	//glBindVertexArray(vertArray_m);

	//glGenBuffers(1, &vertBuffer_m);
	//glBindBuffer(GL_ARRAY_BUFFER, vertBuffer_m);

	//float vertices[3 * 3] = {
	//	-0.5f, -0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.0f, 0.5f, 0.0f
	//};

	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

	//glGenBuffers(1, &idxBuffer_m);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBuffer_m);

	//unsigned int indices[3] = { 0, 1, 2 };
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);




	bool done = false;
	while (!done) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
			SDL_Delay(10);
			continue;
		}

		glDrawArrays(GL_TRIANGLES, 0, 3);
		


		SDL_GL_SwapWindow(window);

	}
	
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}