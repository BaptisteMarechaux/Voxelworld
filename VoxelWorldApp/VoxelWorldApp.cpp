// VoxelWorldApp.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "imgui\imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <gl3w\GL\gl3w.h>
#include <glfw\include\GLFW\glfw3.h>

#include "ShaderManager.h"
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include "Scene.h"

GLFWwindow* window;
GLuint vertexBufferPoints, vaoPoints, colorbuffer;

int width, height;
int nbFrames = 0;
double lastTime = 0.0f;
GLfloat fragmentColor[4] = { 1, 0, 0, 1 };
float newVoxelPosition[3];
int newChunkSize=3;

Scene *mainScene;
double mouseX, mouseY;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void callbackMousePos(GLFWwindow* window, int button, int action, int mods)
{

}

void callbackMouseMove(GLFWwindow* window, double x, double y)
{
	mouseX = x;
	mouseY = y;
	if (ImGui::IsMouseDown(0))
	{
		mainScene->computeMatrixes(width, height, mouseX, mouseY);
	}
}

void Render() 
{
	
	mainScene->zoomFoV(ImGui::GetScrollY());
	if (ImGui::IsKeyDown(262)) //right
		mainScene->TranslateCamera(glm::vec3(1, 0, 0));
	if (ImGui::IsKeyDown(263)) //left
		mainScene->TranslateCamera(glm::vec3(-1, 0, 0));
	if (ImGui::IsKeyDown(264)) //down
		mainScene->TranslateCamera(CameraDirection::backward);
	if (ImGui::IsKeyDown(265)) //up
		mainScene->TranslateCamera(CameraDirection::forward);
	
	mainScene->Render();
}

void Initialize()
{
	//Setup de la fenêtre
	width = 1280;
	height = 720;

	glfwSetErrorCallback(error_callback);

	//Initialisation de GLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Voxel World", NULL, NULL);
	glfwMakeContextCurrent(window);
	gl3wInit();

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);
	//glfwSetCursorPosCallback(window, callbackMouseMove);
	mainScene = new Scene();

}

float frand_a_b(float a, float b) {
	return (rand() / (float)RAND_MAX) * (b - a) + a;
}

int main(int, char**)
{
	bool show_test_window = true;
	bool reset = false;
	bool addAVoxel = false;
	bool addChunk = false;
	bool addSpherizedChunk = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);

	Initialize();
	

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Menu");
		ImGui::Text("This window is here to use the application!");
		ImGui::Columns(2, "mixed");
		ImGui::Separator();

		ImGui::Text("Column");
		

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("Add a Voxel/Chunk");
		ImGui::DragFloat3("Position", (float*)&newVoxelPosition);
		if (ImGui::Button("Add A Voxel")) addAVoxel ^= 1;
		ImGui::DragInt("Chunk Size", (int*)&newChunkSize);
		if (ImGui::Button("Add A Chunk")) addChunk ^= 1;
		if (ImGui::Button("Add A Spheric Chunk")) addSpherizedChunk ^= 1;

		ImGui::Separator();
		if (ImGui::Button("Reset")) reset ^= 1;
		ImGui::ColorEdit3("Clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		ImGui::End();

		static bool p_open = true;
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		if (!ImGui::Begin("Example: Fixed Overlay", &p_open, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return 0;
		}
		ImGui::Text("FPS Counter");
		ImGui::Separator();
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::Text("Vertex Count : %d", mainScene->getVertexCount());
		ImGui::End();

		if (show_test_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&show_test_window);
		}

		if (addAVoxel)
		{
			glm::vec3 pos = glm::vec3(newVoxelPosition[0], newVoxelPosition[1], newVoxelPosition[2]);
			mainScene->AddVoxelAtPosition(pos);
			addAVoxel = false;
		}

		if (addChunk)
		{
			glm::vec3 pos = glm::vec3(newVoxelPosition[0], newVoxelPosition[1], newVoxelPosition[2]);
			mainScene->AddChunkAtPosition(pos, newChunkSize);
			addChunk = false;
		}

		if (addSpherizedChunk)
		{
			glm::vec3 pos = glm::vec3(newVoxelPosition[0], newVoxelPosition[1], newVoxelPosition[2]);
			mainScene->AddSpherizedChunkAtPosition(pos, newChunkSize);
			//mainScene->AddChunkAtPosition(pos);
			addSpherizedChunk = false;
		}

		if (reset)
		{
			mainScene->resetScene();
			reset = false;
		}

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render();
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	// Cleanup
	mainScene->Destroy();
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
    return 0;
}

