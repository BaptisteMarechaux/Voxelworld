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
}

void Render() 
{
	/*
	glm::mat4 model, proj, mvp, view;

	model = glm::mat4(1.0);
	proj = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);
	view = glm::lookAt(
		glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	mvp = proj * view * model;

	glUseProgram(program);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glProgramUniform4fv(program, color_location, 1, fragmentColor);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glVertexAttribPointer(0, 3 , GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	//std::cout << vertexBufferPoints << std::endl;
	glDisableVertexAttribArray(0);
	*/
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
		ImGui::Text("Add a Voxel");
		ImGui::DragFloat3("Position", (float*)&newVoxelPosition);
		if (ImGui::Button("Add A Voxel")) addAVoxel ^= 1;

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

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		Render();
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
    return 0;
}

