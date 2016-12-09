#include "Scene.h"

Scene::Scene()
{
	model = glm::mat4(1.0);
	proj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	view = glm::lookAt(
		glm::vec3(4, 3, 10), // Camera is at this position, in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	mvp = proj * view * model;

	input = Input();
	g_vertex_buffer_data = std::vector<glm::vec3>();
	indices = std::vector<GLuint>();
	normals = std::vector<glm::vec3>();


	Initialize();
}


Scene::~Scene()
{
}

void Scene::Initialize()
{
	glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	program = LoadShaders("..\\shaders\\simple.vs", "..\\shaders\\simple.fs");
	mvp_location = glGetUniformLocation(program, "MVP");
	ModelMatrixID = glGetUniformLocation(program, "M");
	ViewMatrixID = glGetUniformLocation(program, "V");
	position_location = glGetAttribLocation(program, "position");
	color_location = glGetUniformLocation(program, "vertexColor");
	LightID = glGetUniformLocation(program, "lightPos");
	//AddVoxelAtPosition(glm::vec3(0, 0, 0)); //Utiliser cette ligne pour instancier un voxel. il fera appel à la fonction Update pour mettre a jour les buffers de la scene la scene

	glGenVertexArrays(1, &VertexArrayID);
	glGenBuffers(1, &vertexBufferPoints);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &voxelElementBuffer);

	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelElementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, 1800 * sizeof(glm::vec3), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, 400 * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 400 * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

}

void Scene::Render()
{
	
	glUseProgram(program);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);
	glProgramUniform4fv(program, color_location, 1, defaultFragmentColor);
	glProgramUniform4fv(program, LightID, 1, lightPos);

	glBindVertexArray(VertexArrayID);
	glPointSize(5);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, voxelItems.size());
	glBindVertexArray(0);

}

void Scene::UpdateBuffers()
{

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	//glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(glm::vec3), g_vertex_buffer_data.data(), GL_STATIC_DRAW);
	std::vector<glm::vec3>* pts = (std::vector<glm::vec3>*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	pts = &computedVertices;
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	//glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	std::vector<glm::vec3>* nml = (std::vector<glm::vec3>*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	nml = &computedNormals;
	glUnmapBuffer(GL_ARRAY_BUFFER);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelElementBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	std::vector<GLuint>* ind = (std::vector<GLuint>*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	ind = &computedIndices;
	glUnmapBuffer(GL_ARRAY_BUFFER);

}

void Scene::AddVoxelAtPosition(glm::vec3 pos)
{
	Voxel* voxel = new Voxel();
	voxel->SetPosition(pos);

	std::vector<glm::vec3> points = voxel->getPoints();
	//g_vertex_buffer_data.insert(g_vertex_buffer_data.end(), points.begin(), points.end());
	computedVertices.insert(computedVertices.end(), points.begin(), points.end());

	std::vector<GLuint> voxelIndices = voxel->getIndices(indices.size());
	/*indices.insert(indices.end(), voxelIndices.begin(), voxelIndices.end());*/
	computedIndices.insert(computedIndices.end(), voxelIndices.begin(), voxelIndices.end());

	std::vector<glm::vec3> voxelNormals = voxel->getNormals();
	/*normals.insert(normals.end(), voxelNormals.begin(), voxelNormals.end());*/
	computedNormals.insert(computedNormals.end(), voxelNormals.begin(), voxelNormals.end());

	voxelItems.push_back(voxel);
	
	UpdateBuffers();

	std::cout << "Scene Added at Position : " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	std::cout << "Vertex Buffer Size : " << g_vertex_buffer_data.size() << std::endl;
}

void Scene::TranslateCamera(glm::vec3 v)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	camPosition += v * deltaTime * camSpeed;

	view = glm::lookAt(
		camPosition,
		direction,
		glm::vec3(0, 1, 0)
	);
	mvp = proj * view * model;
}

int Scene::getVertexCount()
{
	return indices.size();
}

void Scene::computeMatrixes(int winWidth, int winHeight, double xPos, double yPos)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	horizontalAngle += mouseSpeed * deltaTime * float(winWidth / 2 - xPos);
	verticalAngle += mouseSpeed * deltaTime * float(winHeight / 2 - yPos);

	direction=glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	up = glm::cross(right, direction);

	proj = glm::perspective(initialFoV, 16.0f / 9.0f, 0.1f, 300.0f);
	view = glm::lookAt(
		camPosition,
		direction,
		up
	);
	mvp = proj * view * model;
}

void Scene::zoomFoV(float val)
{
	FoV = glm::radians( initialFoV - glm::radians(5 * val) );
	proj = glm::perspective(FoV, 16.0f / 9.0f, 0.1f, 300.0f);
}

void Scene::Destroy()
{
	glDeleteBuffers(1, &vertexBufferPoints);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &voxelElementBuffer);
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &VertexArrayID);
}
