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
	chunks = std::vector<Chunk>();
	//g_vertex_buffer_data = std::vector<glm::vec3>();
	positions = std::vector<glm::vec3>();
	//indices = std::vector<GLuint>();
	normals = std::vector<glm::vec3>();
	vertices = std::vector<glm::vec3>();


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
	deltaTimeID = glGetUniformLocation(program, "deltaTime");
	//AddVoxelAtPosition(glm::vec3(0, 0, 0)); //Utiliser cette ligne pour instancier un voxel. il fera appel à la fonction Update pour mettre a jour les buffers de la scene la scene

	glGenVertexArrays(1, &voxelVertexArrayID);
	glBindVertexArray(voxelVertexArrayID);
		glGenBuffers(1, &vertexBufferPoints);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//glGenBuffers(1, &voxelElementBuffer);
	glBindVertexArray(0);

	lastTime = glfwGetTime();

}

void Scene::Render()
{
	AutoRotateCamera(1);

	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);
	lightPos[0] = cosf(0.5f * 0.2f * deltaTime);
	lightPos[1] = sinf(0.5f * 0.2f * deltaTime);
	
	glUseProgram(program);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);
	glProgramUniform1f(program, deltaTimeID, deltaTime);
	glProgramUniform4fv(program, color_location, 1, defaultFragmentColor);
	glProgramUniform4fv(program, LightID, 1, lightPos);

	glBindVertexArray(voxelVertexArrayID);
	glPointSize(5);
	glDrawArrays(GL_TRIANGLES, 0,vertices.size());
	//glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	lastTime = glfwGetTime();

}

void Scene::UpdateBuffers()
{

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voxelElementBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

}

void Scene::AddVoxelAtPosition(glm::vec3 pos)
{
	Voxel* voxel = new Voxel();
	voxel->SetPosition(pos);

	std::vector<glm::vec3> points = voxel->getPoints();
	vertices.insert(vertices.end(), points.begin(), points.end());

	//std::vector<GLuint> voxelIndices = voxel->getIndices(indices.size());
	//indices.insert(indices.end(), voxelIndices.begin(), voxelIndices.end());

	std::vector<glm::vec3> voxelNormals = voxel->getNormals();
	normals.insert(normals.end(), voxelNormals.begin(), voxelNormals.end());

	UpdateBuffers();

	std::cout << "Scene Added at Position : " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	std::cout << "Vertex Buffer Size : " << vertices.size() << std::endl;
}

void Scene::AddChunkAtPosition(glm::vec3 pos, int chunkSize)
{
	chunks.push_back(Chunk(chunkSize, glm::vec3(0, 0, 0)));

	std::vector<glm::vec3> points = chunks[chunks.size()-1].getVertices();
	vertices.insert(vertices.end(), points.begin(), points.end());

	std::vector<glm::vec3> voxelNormals = chunks[chunks.size() - 1].getNormals();
	normals.insert(normals.end(), voxelNormals.begin(), voxelNormals.end());

	UpdateBuffers();

	std::cout << "Scene Added Chunk at Position : " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	std::cout << "Vertex Buffer Size : " << vertices.size() << std::endl;
}

void Scene::AddSpherizedChunkAtPosition(glm::vec3 pos, int chunkSize)
{
	chunks.push_back(Chunk(chunkSize, glm::vec3(0, 0, 0)));

	chunks[chunks.size() - 1].Spherize();

	std::vector<glm::vec3> points = chunks[chunks.size() - 1].getVertices();
	vertices.insert(vertices.end(), points.begin(), points.end());

	std::vector<glm::vec3> voxelNormals = chunks[chunks.size() - 1].getNormals();
	normals.insert(normals.end(), voxelNormals.begin(), voxelNormals.end());

	UpdateBuffers();

	std::cout << "Scene Added Chunk at Position : " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	std::cout << "Vertex Buffer Size : " << vertices.size() << std::endl;
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
	return vertices.size();
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
	proj = glm::perspective(FoV, 16.0f / 9.0f, 0.1f, 3000.0f);
}

void Scene::AutoRotateCamera(float speed)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	camPosition.x = 300 * cos(currentTime * speed);
	camPosition.z = 300 * sin(currentTime * speed);

	view = glm::lookAt(
		camPosition,
		direction,
		glm::vec3(0, 1, 0)
	);
	mvp = proj * view * model;
}

void Scene::Destroy()
{
	glDeleteBuffers(1, &vertexBufferPoints);
	glDeleteBuffers(1, &normalbuffer);
	//glDeleteBuffers(1, &voxelElementBuffer);
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &VertexArrayID);
}
