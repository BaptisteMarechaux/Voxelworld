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

GLFWwindow* window;
GLuint vertexBufferPoints, vaoPoints;
GLuint mvp_location, position_location, color_location, program;
int width, height;
int nbFrames = 0;
double lastTime = 0.0f;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void Render() 
{
	glm::mat4x4 model, proj, mvp;

	model = glm::mat4(1.0);
	proj = glm::ortho(0.0f, float(width), 0.0f, float(height), 0.0f, 100.0f);
	mvp = proj * model;

	glUseProgram(program);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(vaoPoints);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
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

	program = LoadShaders("..\\shaders\\simple.vs", "..\\shaders\\simple.fs");

	GLfloat vertices[] = {
		width / 2, height, 0.0f,
		0.0f, 0.0, 0.0f,
		width, 0, 0.0f,
	};

	mvp_location = glGetUniformLocation(program, "MVP");
	position_location = glGetAttribLocation(program, "position");

	glGenVertexArrays(1, &vaoPoints);
	glBindVertexArray(vaoPoints);

	glGenBuffers(1, &vertexBufferPoints);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

int main(int, char**)
{
	bool show_test_window = true;
	bool reset = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);

	Initialize();
	glEnable(GL_CULL_FACE);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Convex Hull & Voronoi");
		ImGui::Text("This window is here to use the application!");
		ImGui::Columns(2, "mixed");
		ImGui::Separator();

		ImGui::Text("Enveloppe convexe");
		

		ImGui::Columns(1);
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
		ImGui::End();

		if (show_test_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&show_test_window);
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

