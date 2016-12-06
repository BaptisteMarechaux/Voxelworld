#include "Scene.h"



Scene::Scene()
{
	Initialize();
}


Scene::~Scene()
{
}

void Scene::Initialize()
{
	glGenBuffers(1, &vertexBufferPoints);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

	
}

void Scene::Render()
{
	model = glm::mat4(1.0);
	proj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	view = glm::lookAt(
		glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	mvp = proj * view * model;

	std::list<Voxel*>::iterator vox = voxelItems.begin();
	for (; vox != voxelItems.end(); ++vox)
	{
		glUseProgram((*vox)->program);
		glUniformMatrix4fv((*vox)->mvp_location, 1, GL_FALSE, &mvp[0][0]);
		glProgramUniform4fv((*vox)->program, (*vox)->color_location, 1, defaultFragmentColor);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	}
	glDisableVertexAttribArray(0);
}

void Scene::Update()
{
}

void Scene::AddVoxelAtPosition(glm::vec3 pos, GLint program)
{
	Voxel* voxel = new Voxel();
	voxel->SetPosition(pos);

	std::vector<GLfloat> points = voxel->getPoints();
	g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), points.begin(), points.end());

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(voxel->position_location);
	glVertexAttribPointer(voxel->position_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
}

void Scene::TranslateCamera(glm::vec3 v)
{
}
