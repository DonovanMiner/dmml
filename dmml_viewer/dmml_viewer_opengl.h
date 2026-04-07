#pragma once


#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_opengl3_loader.h"
#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/emscripten_mainloop_stub.h"

#include <SDL3/SDL.h>

#include <stdio.h>
#include <memory>




namespace dmml {

	namespace viewer {


		class dmml_viewer {
		public:


			dmml_viewer();
			~dmml_viewer();

			int CreateDMMLWindow();

			void InitImGuiContext();

			bool MainLoop();

			int CleanupWindow();




		private:
			
			//std::unique_ptr<SDL_Window> window_m;
			SDL_Window* window_m = nullptr;
			SDL_GLContext glContext_m = nullptr;

			float displayScale_m = 0.00f;
			const char* glslVersion_m = nullptr;
			


		};
	}
}
