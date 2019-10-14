#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "cube.h"
#include "textured_cube.h"
#include "Shader.h"
#include "Camera.h"
#include "Player.h"
#include "Guy.h"
#include "stb_image.h"
#include "Wall.h"
#include <vector>
#include <iostream>
#include <list>
#include <string>
#include "mapdata.h"
#include "gistfile1.h"
#include "items.h"

struct EnemyData;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void CreateBullets(GLFWwindow* window);
void createWall(GLFWwindow* window, Wall the_wall, unsigned int VAO, unsigned int texture);
void createGround(GLFWwindow *window, unsigned int VAO, unsigned int texture);
void updatePlayerData(GLFWwindow* window);
void drawCursor(GLFWwindow* window, unsigned int VAO);
void mapGenerate(GLFWwindow* window, unsigned int VAO, unsigned int texture);
void CreateEnemyBullets(GLFWwindow* window, unsigned int texture, unsigned int VAO);
void createItemTest(GLFWwindow* window, std::vector<unsigned int> VAO);
unsigned int textureSetup(const char name[],int mod,int id);
void createThings(GLFWwindow* window, unsigned int VAO, unsigned int texture1, unsigned int texture2);
void createSightTest(GLFWwindow* window, unsigned int VAO);
unsigned int setupSightTest(GLFWwindow* window);
void setupMap();
void itemSetup();
void setupEnemyBullet();
void bulletStartData();
void reset();
void LoadEnemyBullet();
void updateRotationSpeed();
void detectCollusionPlayer();
void detectCollusionBullets();
void createOutside(GLFWwindow* window,  unsigned int VAO, unsigned int texture);
bool CheckCollision(glm::vec3 wall_size, glm::vec3 wall_pos, glm::vec3 unit_Size, glm::vec3 unit_pos);
void createOneEnemy(GLFWwindow* window, unsigned int VAO, unsigned int texture1, unsigned int texture2, EnemyData monster);
void createOneCodex(GLFWwindow* window, unsigned int VAO, unsigned int texture1, EnemyData codex);
void endCond(GLFWwindow* window);
std::vector<unsigned int> setupBarTest(GLFWwindow* window);
//data of bullet
struct BulletData {
	float shooting_time;
	glm::vec3  BulletPos;
	glm::vec3 BulletDirection;
	glm::vec3 BulletSize;
	bool firstTime = true;
	bool allowFire = true;
	BulletData(glm::vec3 bulposs,glm::vec3 bulldir, glm::vec3 bullsize):BulletPos(bulposs), BulletDirection(bulldir), BulletSize(bullsize)
	{

	}
	BulletData()
	{

	}
};
struct EnemyData{
	glm::vec3 enemyPos;
	glm::vec3 enemyDir;
	glm::vec3 enemySize;
	bool enemyExist;
	int enemyType;
	EnemyData(glm::vec3 bulposs, glm::vec3 bulldir = glm::vec3(0.0f),int type = 0,glm::vec3 size = glm::vec3(0.25f)) 
		: enemyPos(bulposs), enemyDir(bulldir), enemyType(type),enemySize(size),enemyExist(true)
	{

	}
	EnemyData()
	{

	}
};

unsigned int BulletNumber = 0;
bool checkCodex = false;
bool isFirstPushCodex = true;
int codexCollected = 0;
bool putBullet = true;
glm::vec3 StartPosBullet;
glm::vec3 StartFrontLookBullet;
// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
//player
Player guy1(glm::vec3(0.0f, -2.0f, 0.0f));
glm::vec3 player_pos;
bool isRight = false;
bool isLeft = false;
bool isForward = false;
bool isBack = false;
//PLAYERS MODEL
bool guy1first = true;
float yaw;
Guy playerModel(glm::vec3(0.0f), guy1);
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
int cameraMod = 1;
bool changeCam = true;
// timing
float deltaTime = 0.0f;
float loadTime = 0.0f;
float lastFrame = 0.0f;
Shader lightingShader("C:/Users/ekin/source/repos/Game1/color_vs.txt", "C:/Users/ekin/source/repos/Game1/color_fs.txt");
Shader lampShader("C:/Users/ekin/source/repos/Game1/lamp_vs.txt", "C:/Users/ekin/source/repos/Game1/lamp_fs.txt");
Shader textureShader("C:/Users/ekin/source/repos/Game1/texture_vs.txt", "C:/Users/ekin/source/repos/Game1/texture_fs.txt");
Shader oneDShader("C:/Users/ekin/source/repos/Game1/1d_vs.txt", "C:/Users/ekin/source/repos/Game1/1d_fs.txt");
Shader worldTextureShader("C:/Users/ekin/source/repos/Game1/texture_vs.txt", "C:/Users/ekin/source/repos/Game1/world_fs.txt");
// lighting test
//enemy 
float speedEnemy = 0.0f;
float speedEnemy2 = 0.0f;
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f); 
glm::vec3 lightPos(0.0f, 5.0f, -40.0f);
//glm::vec3 lightPos(0.0f, -2.0f, -2.0f);
std::vector<Wall> MapArray;
std::vector<EnemyData> EnemyArray;
std::vector<EnemyData> CodexArray;
std::vector<BulletData> BulletArray;
std::vector<std::vector<BulletData>> EnemyBulletArray;
std::vector<bool> PutBullet(9,true);
//circle data

//

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	/*glfwSetScrollCallback(window, scroll_callback);*/

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	// build and compile our shader zprogram
	// -----
	float verticesTriangle[] = {
	-0.05f, -0.05f, 0.0f,
	 0.05f, -0.05f, 0.0f,
	 0.0f,  0.05f, 0.0f
	}; 

	lightingShader.Setup();
	lampShader.Setup();
	textureShader.Setup();
	oneDShader.Setup();
	worldTextureShader.Setup();
	// set up oned


	//set bullet array

	unsigned int SIGHTVAO = setupSightTest(window);
	unsigned int VBO,TVBO, VAO, lVAO, TVAO,ONE_VBO,ONE_VAO;
	unsigned int SVAO;
	std::vector<unsigned int> BARVAOS = setupBarTest(window);
	gistfile1 sphere;
    sphere.setupSphere();
	//RETUP ONE D
	glGenVertexArrays(1, &ONE_VAO);
	glGenBuffers(1, &ONE_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, ONE_VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangle), verticesTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//SETUP REGULAR
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cube::size, cube::vertices, GL_STATIC_DRAW);
		
	glBindVertexArray(VAO);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// color attribute
	glGenVertexArrays(1, &lVAO);
	glBindVertexArray(lVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//texture VAO
	glGenVertexArrays(1, &TVAO);
	glGenBuffers(1, &TVBO);

	glBindBuffer(GL_ARRAY_BUFFER, TVBO);
	glBufferData(GL_ARRAY_BUFFER, textured_cube::size, textured_cube::vertices, GL_STATIC_DRAW);

	glBindVertexArray(TVAO);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// -----------
	
	float lastBulletFrame = glfwGetTime();
	// ----------------------------------------------------------------
	//setup texture of wall
	unsigned int textureWall = textureSetup("C:/Users/ekin/source/repos/Game1/wall.jpg", 1,0);
	unsigned int textureOut = textureSetup("C:/Users/ekin/source/repos/Game1/outside.png",1,1);
	unsigned int textureGround = textureSetup("C:/Users/ekin/source/repos/Game1/ground.jpg",2,2);
	unsigned int textureEnemy = textureSetup("C:/Users/ekin/source/repos/Game1/enemy1.png", 1,3);
	unsigned int textureAmmo = textureSetup("C:/Users/ekin/source/repos/Game1/ammo2.jpg",2,4);
	unsigned int texturePurple = textureSetup("C:/Users/ekin/source/repos/Game1/purple.jpg", 2,5);
	unsigned int textureCharacter = textureSetup("C:/Users/ekin/source/repos/Game1/body.jpg", 2, 6);
	//unsigned int textureTenticle = textureSetup("C:/Users/ekin/source/repos/Game1/enemyTent.jpg", 1,6);
	textureShader.use();
	textureShader.setInt("ourTexture", 0);
	worldTextureShader.use();
	worldTextureShader.setInt("ourTexture", 0);
	//Wall another_wall(glm::vec3(0.0f, -5.0f, 8.0f), glm::vec3(10.0f, 10.0f, 0.2f));
	loadTime = glfwGetTime() - 2.1;
	setupMap();
	itemSetup();
	setupEnemyBullet();
	while (!glfwWindowShouldClose(window))
	{   //DELTA TIME
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//if (lastBulletFrame + 0.2 <= currentFrame)
		//{
		//	lastBulletFrame = currentFrame;
		//	putBullet = true;
		//}
		processInput(window);
		updateRotationSpeed();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 


		//sTUFF
		LoadEnemyBullet();//creates enemy projectiles ebery 2 sec
		mapGenerate(window,TVAO, textureWall);	// creates map
		bulletStartData();//for some experimantation

		glActiveTexture(GL_TEXTURE0);//bullet texture
		glBindTexture(GL_TEXTURE_2D, textureAmmo);
		CreateBullets(window);//create bullets of player
		CreateEnemyBullets(window, textureAmmo, TVAO);//creates enemy projectiles
		createGround(window, TVAO, textureGround);// creates ground	
		createOutside(window, TVAO, textureOut);//creates sky
		//EnemyData a(glm::vec3(0.0f), glm::vec3(0.0f),1);
		//createOneEnemy(window, TVAO, textureEnemy, texturePurple,a);
		createThings(window, TVAO, textureEnemy, texturePurple);
		detectCollusionPlayer(); //collusion detection player
		detectCollusionBullets(); // collusion detect'on bullets
		
		//stuff
		


		///WASTE
		lightingShader.use();
		lightingShader.setVec3("objectColor", 2.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", lightPos);
		glm::mat4 projection1 = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
		glm::mat4 view;
		if (cameraMod == 0)
		{
			view = camera.GetViewMatrix();
			lightingShader.setVec3("viewPos", camera.Position);
		}
		else if (cameraMod == 1)
		{
			view = guy1.LookLock();
			lightingShader.setVec3("viewPos", guy1.Position);
		}
		lightingShader.setMat4("projection", projection1);
		lightingShader.setMat4("view", view);
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(5.0f));
		lightingShader.setMat4("model", model);
		//waste
		//createCircleTest(window, VAO);
		//sphere.printSphere(window);
		//player model
		glBindVertexArray(TVAO);
		playerModel.updateUser(window, guy1, textureShader, textureCharacter,deltaTime);


		///
		lightingShader.use();
		lightingShader.setVec3("objectColor", 0.1f, 0.1f, 0.1f);
		glm::mat4 modelL;
		modelL = glm::translate(modelL, lightPos);
		modelL = glm::scale(modelL, glm::vec3(2.0f)); // a smaller cube
		lightingShader.setMat4("model", modelL);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		createSightTest(window, SIGHTVAO);
		createItemTest(window, BARVAOS);
		endCond(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lVAO);
	glDeleteVertexArrays(1, &TVAO);
	glDeleteVertexArrays(1, &ONE_VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &ONE_VBO);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
void createGround(GLFWwindow *window,unsigned int VAO, unsigned int texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	textureShader.use();
	textureShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	textureShader.setVec3("lightPos", lightPos);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -5.0f, 8.0f));
	model = glm::scale(model, glm::vec3(200.0f, 0.2f, 200.0f));

	glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
	glm::mat4 view;
	if (cameraMod == 0)
	{
		view = camera.GetViewMatrix();
		lightingShader.setVec3("viewPos", camera.Position);
	}
	else if (cameraMod == 1)
	{
		view = guy1.LookLock();
		textureShader.setVec3("viewPos", guy1.Position);
	}
	textureShader.setMat4("projection", projection2);
	textureShader.setMat4("view", view);
	textureShader.setMat4("model", model);
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void processInput(GLFWwindow *window)
{
	float currentTime = glfwGetTime();

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (putBullet)
		{
			BulletData a;
			a.shooting_time = currentTime;
			BulletArray.push_back(a);
			putBullet = false;
		}
		
	
	}
	else
	{
		putBullet = true;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if (isFirstPushCodex)
		{
			checkCodex = true;
			isFirstPushCodex = false;
		}
		else
		{
			checkCodex = false;
		}

	}
	else
	{
		isFirstPushCodex = true;
		checkCodex = false;
	}


		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			if (cameraMod == 1)
				guy1.ProcessKeyboardP(FORWARD_P, deltaTime);
			else if(cameraMod == 0)
				camera.ProcessKeyboard(FORWARD, deltaTime);

			isForward = true;
		}
		else
			isForward = false;
	
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			if (cameraMod == 1)
				guy1.ProcessKeyboardP(BACKWARD_P, deltaTime);
			else if (cameraMod == 0)
				camera.ProcessKeyboard(BACKWARD, deltaTime);
			isBack = true;
		}
		else
			isBack = false;
	
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			if (cameraMod == 1)
				guy1.ProcessKeyboardP(LEFT_P, deltaTime);
			else if (cameraMod == 0)
				camera.ProcessKeyboard(LEFT, deltaTime);
			isLeft = true;
		}
		else
			isLeft = false;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			if (cameraMod == 1)
				guy1.ProcessKeyboardP(RIGHT_P, deltaTime);
			else if (cameraMod == 0)
				camera.ProcessKeyboard(RIGHT, deltaTime);
			isRight = true;
		}
		else
		{
			isRight = false;
		}
	


	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		if (changeCam)
		{
			
			if (cameraMod == 1 )
			{
				
				cameraMod = 0;
			}
			else 
			{
				
				cameraMod = 1;
			}
			changeCam = false;
		}
	}
	else
	{
		changeCam = true;
	}

}
void CreateBullets(GLFWwindow* window)
{
	float speed = 10.5f;
	float velocity = speed * deltaTime;
	float calibration = 25;
	int currentTime = glfwGetTime();

	for (int i = 0; i < BulletArray.size(); i++)
	{

		if (  BulletArray[i].shooting_time + 6 > currentTime && BulletArray[i].allowFire)
		{
			if (BulletArray[i].firstTime)
			{


				BulletArray[i].BulletDirection = StartFrontLookBullet;

				BulletArray[i].BulletPos = StartPosBullet + BulletArray[i].BulletDirection*0.3f;
				BulletArray[i].BulletSize = glm::vec3(0.1f);
				BulletArray[i].firstTime = false;
			}
			glm::mat4 BulletModel;
			BulletModel = glm::translate(BulletModel, BulletArray[i].BulletPos);	
			BulletModel = glm::scale(BulletModel, glm::vec3(0.02f));
			
			lightingShader.setMat4("model", BulletModel);
			lightingShader.setVec3("objectColor", 0.2f, 1.7f, 2.31f);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			BulletArray[i].BulletPos += BulletArray[i].BulletDirection*velocity;
			//BulletArray[i].BulletPos += BulletArray[i].BulletDirection*velocity;
		}
		else 
		{
			BulletArray.erase(BulletArray.begin() + i);
		}

	}


}
void CreateEnemyBullets(GLFWwindow* window,unsigned int texture,unsigned int VAO)
{
	float speed = 3.5f;
	float velocity = speed * deltaTime;
	float calibration = 25;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	textureShader.use();
	textureShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	textureShader.setVec3("lightPos", lightPos);
	int currentTime = glfwGetTime();

	for (int j = 0; j < EnemyBulletArray.size(); j++)
	{
		for (int i = 0; i < EnemyBulletArray[j].size(); i++)
		{

			if (EnemyBulletArray[j][i].shooting_time + 6 >= currentTime)
			{

				glm::mat4 BulletModel;

				BulletModel = glm::translate(BulletModel, EnemyBulletArray[j][i].BulletPos);
				BulletModel = glm::scale(BulletModel, glm::vec3(0.2f));
				textureShader.setMat4("model", BulletModel);
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				EnemyBulletArray[j][i].BulletPos += EnemyBulletArray[j][i].BulletDirection*velocity;
				//BulletArray[i].BulletPos += BulletArray[i].BulletDirection*velocity;
			}
			else if (EnemyBulletArray[j][i].shooting_time + 3 < currentTime)
			{
				EnemyBulletArray[j].erase(EnemyBulletArray[j].begin() + i);
			}

		}
	}


}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	//if (cameraMod == 0)
		camera.ProcessMouseMovement(xoffset, yoffset);
	//else
	
	guy1.MouseMovement(xoffset, yoffset);
	playerModel.uptadeRotation(xoffset, yoffset,guy1,deltaTime);
}
void setupMap()
{
	float posx,posy = -5.0f,posz;
	float scalex,scaley= 10.0f,scalez;
	std::cout << mapdata::size;
	for(int i = 0; i < mapdata::size; i++)
	{
		if(i % 2 == 0)
		{
			posx = mapdata::WallPoints[i];
			scalex = mapdata::WallScales[i];
			
		}
		else
		{
			posz = mapdata::WallPoints[i];
			scalez = mapdata::WallScales[i];

			MapArray.push_back(Wall(glm::vec3(posx, posy, posz), glm::vec3(scalex, scaley, scalez),i/2));
			//std::cout << i / 2 << std::endl;
			//std::cout << posx <<' ' << posy << " " << posz << std::endl;
			//std::cout << scalex << ' ' << scaley << " " <<  scalez << std::endl;
		}

	}


}
void mapGenerate(GLFWwindow* window, unsigned int VAO, unsigned int texture)
{

	for (int i = 0; i < MapArray.size(); i++)
	{
		createWall(window, MapArray[i], VAO, texture);

	}
}
void bulletStartData()
{
	StartPosBullet = guy1.GetPos();
	StartFrontLookBullet = guy1.GetFrontLook();
}
void createWall(GLFWwindow* window, Wall the_wall, unsigned int VAO, unsigned int texture)
	{
	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, texture);
		textureShader.use();
		textureShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		textureShader.setVec3("lightPos", lightPos);
		glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
		glm::mat4 view;
		if (cameraMod == 0)
		{
			view = camera.GetViewMatrix();
			lightingShader.setVec3("viewPos", camera.Position);
		}
		else if (cameraMod == 1)
		{
			view = guy1.LookLock();
			textureShader.setVec3("viewPos", guy1.Position);
		}
		textureShader.setMat4("projection", projection2);
		textureShader.setMat4("view", view);
		textureShader.setMat4("model", the_wall.GetModel());
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);

	}
void createOutside(GLFWwindow* window, unsigned int VAO, unsigned int texture)
{
		glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, texture);
		worldTextureShader.use();
		worldTextureShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		worldTextureShader.setVec3("lightPos", lightPos);
		glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
		glm::mat4 view;
		if (cameraMod == 0)
		{
			view = camera.GetViewMatrix();
			worldTextureShader.setVec3("viewPos", camera.Position);
		}
		else if (cameraMod == 1)
		{
			view = guy1.LookLock();
			worldTextureShader.setVec3("viewPos", guy1.Position);
		}
		worldTextureShader.setMat4("projection", projection2);
		worldTextureShader.setMat4("view", view);
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f));
		model = glm::scale(model, glm::vec3(150.0f,80.0f,150.0f));

		worldTextureShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
}
bool CheckCollision(glm::vec3 wall_size,glm::vec3 wall_pos,glm::vec3 unit_Size,glm::vec3 unit_pos) // AABB - AABB collision
{
	float x_person = unit_Size.x;
	float x_wall = wall_size.x;
    float y_person = unit_Size.y;
	float y_wall = wall_size.y;
	float z_person = unit_Size.z;
	float z_wall = wall_size.z;
	glm::vec3 person_position = unit_pos;
	glm::vec3 wall_position = wall_pos;
	if ((person_position.x > wall_position.x && person_position.x - x_person <= wall_position.x + x_wall) || 
		 (person_position.x < wall_position.x && person_position.x + x_person >= wall_position.x - x_wall) || person_position.x == wall_position.x)
	{
		if ((person_position.y > wall_position.y && person_position.y - y_person <= wall_position.y + y_wall) ||
			(person_position.y < wall_position.y && person_position.y + y_person >= wall_position.y - y_wall) || person_position.y == wall_position.y)
		{
			if ((person_position.z > wall_position.z && person_position.z - z_person <= wall_position.z + z_wall) ||
				(person_position.z < wall_position.z && person_position.z + z_person >= wall_position.z - z_wall) || person_position.z == wall_position.z)
			{
				//std::cout << wall.id << std::endl;
				return true;
			}

		}

	}

	return false;
}
//setup codex and enemy
void itemSetup()
{
	float posx, posy , posz;
	float dirx, diry, dirz;
	int j = 0;
	int type;
	for (int i = 0; i < items::enemysize; i++)
	{
		if (i % 3 == 0)
		{
			posx = items::enemyPos[i];
			dirx = items::enemyLook[i];

		}
		else if (i % 3 == 1)
		{
			posy = items::enemyPos[i];
			diry = items::enemyLook[i];

		}
		else if (i % 3 == 2)
		{
			posz = items::enemyPos[i];
			dirz = items::enemyLook[i];
			glm::vec3 pos(posx, posy, posz);
			glm::vec3 dir(dirx, diry, dirz);
			type = items::enemyType[j];
			j++;
			EnemyArray.push_back(EnemyData(pos, dir,type));

		}
	}
	for (int i = 0; i < items::codexsize; i++)
	{
		if (i % 3 == 0)
		{
			posx = items::codexPos[i];


		}
		else if (i % 3 == 1)
		{
			posy = items::codexPos[i];


		}
		else if (i % 3 == 2)
		{
			posz = items::codexPos[i];
			glm::vec3 pos(posx, posy, posz);
			CodexArray.push_back(EnemyData(pos));

		}
	}

} 
unsigned int textureSetup(const char name[],int mod,int id)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, nrChannels;
	unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (mod == 1)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		
		
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture "  << id << std::endl;
	}
	stbi_image_free(data);

	return texture;
}
void createOneEnemy(GLFWwindow* window, unsigned int VAO, unsigned int texture1, unsigned int texture2,EnemyData monster)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	if (speedEnemy > 360)
	{
		speedEnemy -= 360;
	}
	if (speedEnemy2 > 360)
	{
		speedEnemy2 -= 360;
	}

	textureShader.use();
	textureShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	textureShader.setVec3("lightPos", lightPos);
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
	glm::mat4 view;
	if (cameraMod == 0)
	{
		view = camera.GetViewMatrix();
		lightingShader.setVec3("viewPos", camera.Position);
	}
	else if (cameraMod == 1)
	{
		view = guy1.LookLock();
		textureShader.setVec3("viewPos", guy1.Position);
	}
	textureShader.setMat4("projection", projection);
	textureShader.setMat4("view", view);

	glm::mat4 body;	
	body = glm::translate(body, monster.enemyPos);
	if (monster.enemyType == 1)
	{
		body = glm::rotate(body, glm::radians(speedEnemy2), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else
	{
		body = glm::rotate(body, glm::radians(speedEnemy2), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	body = glm::scale(body, glm::vec3(0.3f));
	textureShader.setMat4("model", body);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glm::mat4 tent1;
	glm::vec3 pos = monster.enemyPos;
	tent1 = glm::translate(tent1, pos);
	
	if (monster.enemyType == 1)
	{
		tent1 = glm::rotate(tent1, glm::radians(-speedEnemy), glm::vec3(0.0f, 0.0f, 1.0f));
		tent1 = glm::translate(tent1, glm::vec3(0.5f,0.0f,0.0f));
		tent1 = glm::scale(tent1, glm::vec3(0.4f, 0.1f, 0.1f));
	}
	else
	{
		tent1 = glm::rotate(tent1, glm::radians(-speedEnemy), glm::vec3(1.0f, 0.0f, 0.0f));
		tent1 = glm::translate(tent1, glm::vec3(0.0f, 0.0f, 0.5f));
		tent1 = glm::scale(tent1, glm::vec3(0.1f, 0.1f, 0.4f));
		
	}
	textureShader.setMat4("model", tent1);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glm::mat4 tent2;
	tent2 = glm::translate(tent2, pos);

	if (monster.enemyType == 1)
	{
		tent2 = glm::rotate(tent2, glm::radians(-speedEnemy), glm::vec3(0.0f, 0.0f, 1.0f));
		tent2 = glm::translate(tent2, glm::vec3(-0.5f, 0.0f, 0.0f));
		tent2 = glm::scale(tent2, glm::vec3(0.4f, 0.1f, 0.1f));

	}
	else
	{
		tent2 = glm::rotate(tent2, glm::radians(-speedEnemy), glm::vec3(1.0f, 0.0f, 0.0f));
		tent2 = glm::translate(tent2, glm::vec3(0.0f, 0.0f, -0.5f));
		tent2 = glm::scale(tent2, glm::vec3(0.1f, 0.1f, 0.4f));

	}
	textureShader.setMat4("model", tent2);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glm::mat4 tent3;
	tent3 = glm::translate(tent3, pos);

	if (monster.enemyType == 1)
	{
		tent3 = glm::rotate(tent3, glm::radians(-speedEnemy), glm::vec3(0.0f, 0.0f, 1.0f));
		tent3 = glm::translate(tent3, glm::vec3(0.0f, -0.5f, 0.0f));
		tent3 = glm::scale(tent3, glm::vec3(0.1f, 0.4f, 0.1f));

	}
	else
	{
		tent3 = glm::rotate(tent3, glm::radians(-speedEnemy), glm::vec3(1.0f, 0.0f, 0.0f));
		tent3 = glm::translate(tent3, glm::vec3(0.0f, -0.5f, 0.0f));
		tent3 = glm::scale(tent3, glm::vec3(0.1f, 0.4f, 0.1f));

	}
	textureShader.setMat4("model", tent3);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glm::mat4 tent4;
	tent4 = glm::translate(tent4, pos);

	if (monster.enemyType == 1)
	{
		tent4 = glm::rotate(tent4, glm::radians(-speedEnemy), glm::vec3(0.0f, 0.0f, 1.0f));
		tent4 = glm::translate(tent4, glm::vec3(0.0f, 0.5f, 0.0f));
		tent4 = glm::scale(tent4, glm::vec3(0.1f, 0.4f, 0.1f));
	}
	else
	{
		tent4 = glm::rotate(tent4, glm::radians(-speedEnemy), glm::vec3(1.0f, 0.0f, 0.0f));
		tent4 = glm::translate(tent4, glm::vec3(0.0f, 0.5f, 0.0f));
		tent4 = glm::scale(tent4, glm::vec3(0.1f, 0.4f, 0.1f));

	}
	textureShader.setMat4("model", tent4);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);


}
//create codex and enemy
void createThings(GLFWwindow* window, unsigned int VAO, unsigned int texture1, unsigned int texture2)
{

	for (int i = 0; i < EnemyArray.size(); i++)
	{
		if(EnemyArray[i].enemyExist)
			createOneEnemy(window, VAO, texture1, texture2, EnemyArray[i]);

	}
	for (int i = 0; i < CodexArray.size(); i++)
	{
		if(CodexArray[i].enemyExist)
			createOneCodex(window, VAO, texture2, CodexArray[i]);

	}


}
void createOneCodex(GLFWwindow* window, unsigned int VAO, unsigned int texture1, EnemyData codex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	textureShader.use();
	textureShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	textureShader.setVec3("lightPos", lightPos);
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
	glm::mat4 view;
	if (cameraMod == 0)
	{
		view = camera.GetViewMatrix();
		lightingShader.setVec3("viewPos", camera.Position);
	}
	else if (cameraMod == 1)
	{
		view = guy1.LookLock();
		textureShader.setVec3("viewPos", guy1.Position);
	}
	
	textureShader.setMat4("projection", projection);
	textureShader.setMat4("view", view);
	glm::mat4 model;
	model = glm::translate(model, codex.enemyPos);
	model = glm::scale(model, glm::vec3(0.5f));
	model = glm::rotate(model, glm::radians(speedEnemy), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.2f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	
	
	textureShader.setMat4("model", model);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);



}
void LoadEnemyBullet()
{
	int current = glfwGetTime();
	if (current - loadTime > 2)
	{
		for (int i = 0; i < EnemyBulletArray.size(); i++)
		{
			if (PutBullet[i])
			{
				BulletData aBullet(EnemyArray[i].enemyPos, EnemyArray[i].enemyDir, glm::vec3(0.1f));
				aBullet.shooting_time = current;
				aBullet.firstTime = false;
				EnemyBulletArray[i].push_back(aBullet);
			}
		}
		//std::cout << EnemyBulletArray.size() << std::endl;
		loadTime = current;
	}

}
void detectCollusionPlayer()
{
	bool detected = false;
	bool detected2 = false;
	glm::vec3 guypos = guy1.GetPos();
	glm::vec3 guysize = guy1.Size;
	if ((guypos.x < 8 && guypos.z > -24) || (guypos.x < 13.90 && guypos.z > -24 && guypos.z < -12))//in first area
	{
		for (int i = 0; i < 9 && !detected; i++)//for walls
		{
			detected = CheckCollision(MapArray[i].size, MapArray[i].Position, guysize, guypos);
		}
		for (int i = 0; i < 2 && !detected; i++)//for enemy and its bullets
		{
			if(EnemyArray[i].enemyExist)
				detected = CheckCollision(EnemyArray[i].enemySize, EnemyArray[i].enemyPos, guysize, guypos);
			for (int j = 0; j < EnemyBulletArray[i].size() && !detected;j++)
			{
				detected = CheckCollision(EnemyBulletArray[i][j].BulletSize, EnemyBulletArray[i][j].BulletPos, guysize, guypos);
				//std::cout << "detected 1 j" << std::endl;
			}

		}
		for (int i = 0; i < 1 && checkCodex && !detected2; i++)//look codexes 
		{
			detected2 = CheckCollision(CodexArray[i].enemySize, CodexArray[i].enemyPos, guysize, guypos);
			if (detected2)
			{
				CodexArray[i].enemyExist = false;
				codexCollected += 1;
			}
		}
	}
	else if ((guypos.x >= 14 && guypos.x < 24 && guypos.z > -33.90) || (guypos.x >= 8 && guypos.z > -12))//in second area
	{
		for (int i = 9; i < 25 && !detected; i++)//for walls
		{
			detected = CheckCollision(MapArray[i].size, MapArray[i].Position, guysize, guypos);
		}
		for (int i = 2; i < 4 && !detected; i++)//for enemy
		{
			if (EnemyArray[i].enemyExist)
				detected = CheckCollision(EnemyArray[i].enemySize, EnemyArray[i].enemyPos, guysize, guypos);
			for (int j = 0; j < EnemyBulletArray[i].size() && !detected; j++)
			{
				detected = CheckCollision(EnemyBulletArray[i][j].BulletSize, EnemyBulletArray[i][j].BulletPos, guysize, guypos);

			}

		}
		for (int i = 1; i < 2 && checkCodex && !detected2; i++)//look codexes 
		{
			
			detected2 = CheckCollision(CodexArray[i].enemySize, CodexArray[i].enemyPos, guysize, guypos);

			if (detected2)
			{
				CodexArray[i].enemyExist = false;
				codexCollected += 1;
			}
		}
		
	}
	else if ((guypos.z <= -34) || (guypos.z <= -22 && guypos.x > 24))//in third area
	{
		for (int i = 25; i < MapArray.size() && !detected; i++)//for walls
		{
			detected = CheckCollision(MapArray[i].size, MapArray[i].Position, guysize, guypos);
		}
		for (int i = 4; i < EnemyArray.size() && !detected; i++)//for enemy
		{
			if (EnemyArray[i].enemyExist)
				detected = CheckCollision(EnemyArray[i].enemySize, EnemyArray[i].enemyPos, guysize, guypos);
			for (int j = 0; j < EnemyBulletArray[i].size() && !detected; j++)
			{
				detected = CheckCollision(EnemyBulletArray[i][j].BulletSize, EnemyBulletArray[i][j].BulletPos, guysize, guypos);

			}
		}
		for (int i = 2; i < CodexArray.size() && checkCodex && !detected2; i++)//look codexes 
		{
			detected2 = CheckCollision(CodexArray[i].enemySize, CodexArray[i].enemyPos, guysize, guypos);
			if (detected2)
			{
				CodexArray[i].enemyExist = false;
				codexCollected += 1;

			}
		}


	}
	if (detected)
	{

		reset();
	}

}
void detectCollusionBullets()
{
	bool detected = false;

	for (int j = 0; j < BulletArray.size(); j++)
	{
		glm::vec3 bullpos = BulletArray[j].BulletPos;
		glm::vec3 bullsize = BulletArray[j].BulletSize;
		if ((bullpos.x < 8 && bullpos.z > -24) || (bullpos.x < 13.90 && bullpos.z > -24 && bullpos.z < -12))//in first area
		{
			for (int i = 0; i < 9 && !detected; i++)//for walls
			{
				detected = CheckCollision(MapArray[i].size, MapArray[i].Position, bullsize, bullpos);

				if (detected)
				{
					BulletArray.erase(BulletArray.begin() + j);
				}
			}
			for (int i = 0; i < 2 && !detected; i++)//for enemy 
			{
				detected = CheckCollision(EnemyArray[i].enemySize, EnemyArray[i].enemyPos, bullsize, bullpos);
				if (detected)
				{
					BulletArray.erase(BulletArray.begin() + j);
					EnemyArray[i].enemyExist = false;
					PutBullet[i] = false;
				}

			}

		}
		else if ((bullpos.x >= 14 && bullpos.x < 24 && bullpos.z > -33.90) || (bullpos.x >= 8 && bullpos.z > -12))//in second area
		{
			for (int i = 9; i < 25 && !detected; i++)//for walls
			{
				detected = CheckCollision(MapArray[i].size, MapArray[i].Position, bullsize, bullpos);
				if (detected)
				{
					BulletArray.erase(BulletArray.begin() + j);
				}
			}
			for (int i = 2; i < 4 && !detected; i++)//for enemy
			{
				detected = CheckCollision(EnemyArray[i].enemySize, EnemyArray[i].enemyPos, bullsize, bullpos);
				if (detected)
				{
					BulletArray.erase(BulletArray.begin() + j);
					EnemyArray[i].enemyExist = false;
					PutBullet[i] = false;
				}

			}

		}
		else if ((bullpos.z <= -34) || (bullpos.z <= -22 && bullpos.x > 24))//in third area
		{
			for (int i = 25; i < MapArray.size() && !detected; i++)//for walls
			{
				detected = CheckCollision(MapArray[i].size, MapArray[i].Position, bullsize, bullpos);
				if (detected)
				{
					BulletArray.erase(BulletArray.begin() + j);
				}
			}
			for (int i = 4; i < EnemyArray.size() && !detected; i++)//for enemy
			{
				detected = CheckCollision(EnemyArray[i].enemySize, EnemyArray[i].enemyPos, bullsize, bullpos);
				if (detected)
				{
					BulletArray.erase(BulletArray.begin() + j);
					EnemyArray[i].enemyExist = false;
					PutBullet[i] = false;;
				}

			}

		}
	}

}
void setupEnemyBullet()
{
	for (int i = 0; i < EnemyArray.size(); i++)
	{
		std::vector<BulletData> a;
		
		EnemyBulletArray.push_back(a);
	}


}
void reset()
{
	guy1= Player(glm::vec3(0.0f, -2.0f, 0.0f));
	playerModel= Guy(glm::vec3(0.0f),guy1);
	for (int i = 0; i < BulletArray.size(); i++)
	{
		BulletArray[i].allowFire = false;

	}
	for (int i = 0; i < EnemyArray.size(); i++)
	{
		PutBullet[i] = true;
		EnemyArray[i].enemyExist = true;
		
	}
	for (int i = 0; i < CodexArray.size(); i++)
	{
		CodexArray[i].enemyExist = true;
	}
	codexCollected = 0;
}
void endCond(GLFWwindow* window)
{
	if (codexCollected == 7)
	{
		glfwSetWindowShouldClose(window, true);

	}


}
unsigned int setupSightTest(GLFWwindow* window)
{
	float vertices[] = {
	 0.004f,  0.004f, 0.0f,  // top right
	 0.004f, -0.004f, 0.0f,  // bottom right
	-0.004f, -0.004f, 0.0f,  // bottom left
	-0.004f,  0.004f, 0.0f   // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 3, 1,  // first Triangle
		3, 2, 1   // second Triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return VAO;
}
std::vector<unsigned int> setupBarTest(GLFWwindow* window)
{
	std::vector<unsigned int> ret;
	float vertices1[] = {
	0.935f,  0.94f, 0.0f,  // top right
	0.960f, 0.85f, 0.0f,  // bottom right
	0.935f, 0.76f, 0.0f,  // bottom left
	0.910f,  0.85f, 0.0f   // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 3, 1,  // first Triangle
		3, 2, 1   // second Triangle
	};
	for (int i = 0; i < 7; i++)
	{
		unsigned int VBO, VAO, EBO;
		for (int j = 0; j < sizeof(vertices1) / sizeof(float); j++)
		{
			if(j % 3 == 0)
				vertices1[j] -= 0.055;
			std::cout << vertices1[j] << std::endl;
		}
		
			
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);


	
			ret.push_back(VAO);
	}



	return ret;
}
void createSightTest(GLFWwindow* window, unsigned int VAO)
{

	glDisable(GL_DEPTH_TEST);
	oneDShader.use();
	oneDShader.setVec4("color", glm::vec4(0.7f, 0.7f, 0.0f, 1.0f));
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);
}
void updateRotationSpeed()
{

	speedEnemy += 200.5*deltaTime;

	if (speedEnemy > 360)
		speedEnemy -= 360;

	speedEnemy2 += 100.5*deltaTime;

	if (speedEnemy2 > 360)
		speedEnemy2 -= 360;

}
void createItemTest(GLFWwindow* window, std::vector<unsigned int> VAO)
{
	glDisable(GL_DEPTH_TEST);
	for (int i = 0; i < codexCollected; i++)
	{
		
		oneDShader.use();
		oneDShader.setVec4("color", glm::vec4(0.4f, 0.0f, 0.4f, 1.0f));
		glBindVertexArray(VAO[i]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}
	glEnable(GL_DEPTH_TEST);
}
