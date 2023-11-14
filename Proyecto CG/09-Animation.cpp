/*
* Laboratorio de Computación Gráfica e Interacción Humano-Computadora
* PROYECTO FINAL
*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <material.h>
#include <light.h>
#include <cubemap.h>
#include <particles.h>

#include <irrKlang.h>
using namespace irrklang;


// Max number of bones
#define MAX_RIGGING_BONES 100

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

float     rotateCharacter = 0.0f;
glm::vec3 position(0.0f, 0.0f, 0.0f);
glm::vec3 forwardView(0.0f, 0.0f, 1.0f);

float     scaleV = 0.005f;
float sineTime = 0.0f;


// Shaders
Shader* ourShader;
Shader* staticShader;
Shader *cubemapShader;
Shader* particlesShader;
Shader* wavesShader;
Shader* nenufarShader;
Shader* jellyFishShader;
Shader* boaShader;
Shader* phongShader;
Shader* fresnelShader;
Shader* proceduralShader;
Shader* proceduralShaderObjects;

// Partículas
Particles particlesSystem(70); // creamos 200 partículas

// Carga la información del modelo
Model *particleModel;

float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

// Cubemap
CubeMap* mainCubeMap;

// Audio
ISoundEngine* SoundEngine = createIrrKlangDevice();

// Carga la información del modelo

Model* lirio;		//1
Model* nenufar;		//2
Model* cocodrilo;	//3
Model* tortuga;		//4
Model* mangle;		//5
Model* cangrejo;	//6
Model* garza;		//7
Model* bolsa;		//8
Model* lataCoca;	//9
Model* lataFanta;	//9
Model* lataSprite;	//9
Model* ostra;		//10
Model* flamingo;	//11
Model* estrella;	//12
Model* medusa;		//13
Model* boa;			//14
Model* alga;		//15
Model* gavilan;		//16
Model* pez;			//17
Model* manati;		//18
Model* agua;		//19
Model* lancha;		//20
Model* arena;		//21
Model* roca;		//22
Model* ave;			//23

Model* WaterGridMesh;
Model* NenufarGridMesh;

Model* cubeenv;




// Pose inicial de Lancha
glm::mat4 gBones[MAX_RIGGING_BONES];
glm::mat4 gBonesBar[MAX_RIGGING_BONES];

// Pose inicial de Manati
glm::mat4 gBonesManati[MAX_RIGGING_BONES];

// Pose inicial de Tortuga
glm::mat4 gBonesTortuga[MAX_RIGGING_BONES];

float	fps = 30.0f;
int		keys = 0;

float	fpsManati = 30.0f;
int		keysManati = 0;

float	fpsTortuga = 30.0f;
int		keysTortuga = 0;

int		animationCount = 0;
int		animationCountManati = 0;
int		animationCountTortuga = 0;

float wavesTime = 0.0f;
float proceduralTime = 0.0f;


// selección de cámara
bool    activeCamera = 1; // activamos la primera cámara

// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	// Loop de renderizado
	while (!glfwWindowShouldClose(window))
	{

	}

	// glfw: Terminamos el programa y liberamos memoria
	glfwTerminate();
	return 0;
}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LA VIDA EN LOS MANGLARES (ESPECIES ENDEMICAS Y EN PELIGRO DE EXTINCIÓN", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	ourShader = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");
	staticShader = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs");
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	particlesShader = new Shader("shaders/13_particles.vs", "shaders/13_particles.fs");
	wavesShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/13_wavesAnimation.fs");
	nenufarShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/10_fragment_simple.fs");
	jellyFishShader = new Shader("shaders/14_jellyFishAnimation.vs", "shaders/10_fragment_simple.fs");
	boaShader = new Shader("shaders/14_BoaAnimation.vs", "shaders/10_fragment_simple.fs");
	proceduralShader = new Shader("shaders/12_ProceduralAnimation.vs", "shaders/12_ProceduralAnimation.fs");
	proceduralShaderObjects = new Shader("shaders/12_ProceduralAnimationObjects.vs", "shaders/12_ProceduralAnimation.fs");

	particleModel = new Model("models/snow/burbuja.fbx");

	// Máximo número de huesos: 100
	ourShader->setBonesIDs(MAX_RIGGING_BONES);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);


	////////////// M O D E L O S ///////////////////////////
	//MANGLE
	arena = new Model("models/arena/arena.fbx");
	mangle = new Model("models/mangle/mangle.fbx");
	lataCoca = new Model("models/lataCoca/lataCoca.fbx");
	lataFanta = new Model("models/lataCoca/lataFanta.fbx");
	lataSprite = new Model("models/lataCoca/lataSprite.fbx");
	bolsa = new Model("models/bolsa/bolsa.fbx");
	ostra = new Model("models/ostra/ostra.fbx");
	cocodrilo = new Model("models/cocodrilo/cocodrilo.fbx");
	estrella = new Model("models/estrella/estrella.fbx");
	flamingo = new Model("models/flamingo/flamingo.fbx");
	lancha = new Model("models/lancha/lancha.fbx");
	tortuga = new Model("models/tortuga/SeaTurtle.fbx");
	cangrejo = new Model("models/cangrejo/cangrejo.fbx");
	medusa = new Model("models/medusa/medusa.fbx");
	boa = new Model("models/boa/boa.fbx");
	manati = new Model("models/manati/manati.fbx");
	roca = new Model("models/roca/roca.fbx");
	garza = new Model("models/garza/garza.fbx");
	alga = new Model("models/alga/alga.fbx");
	ave = new Model("models/ave/ave.fbx");
	pez = new Model("models/pez/pez.fbx");
	gavilan = new Model("models/gavilan/gavilan.fbx");



	WaterGridMesh = new Model("models/agua/aguaBloque.fbx");
	NenufarGridMesh = new Model("models/nenufar/nenufar.fbx");


	// CUBO DE FONDO
	cubeenv = new Model("models/mycube.fbx");

	//SoundEngine->play2D("sound/mar.mp3", true);

	//Inicialización Lancha
	lancha->SetPose(0.0f, gBones);
	fps = (float)lancha->getFramerate();
	keys = (int)lancha->getNumFrames();

	//Inicialización Manati
	manati->SetPose(0.0f, gBonesManati);
	fpsManati = (float)manati->getFramerate();
	keysManati = (int)manati->getNumFrames();

	//Inicialización Tortuga
	tortuga->SetPose(0.0f, gBonesTortuga);
	fpsTortuga = (float)tortuga->getFramerate();
	keysTortuga = (int)tortuga->getNumFrames();

	camera3rd.Position = position;
	camera3rd.Position.y += 1.7f;
	camera3rd.Position -= forwardView;
	camera3rd.Front = forwardView;

	return true;
}

bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	elapsedTime += deltaTime;
	if (elapsedTime > 1.0f / fps) {
		elapsedTime = 0.0f;
		particlesSystem.UpdatePhysics(deltaTime);

		animationCount++;
		if (animationCount > keys - 1) {
			animationCount = 0;
		}
		lancha->SetPose((float)animationCount, gBones);

		animationCountManati++;
		if (animationCountManati > keysManati - 1) {
			animationCountManati = 0;
		}
		manati->SetPose((float)animationCountManati, gBonesManati);

		animationCountTortuga++;
		if (animationCountTortuga > keysTortuga - 1) {
			animationCountTortuga = 0;
		}
		tortuga->SetPose((float)animationCountTortuga, gBonesTortuga);

		elapsedTime = 0.0f;
	}


	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(0);


	// MANGLAR
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);


		////////////////////////MODELOS ESTATICOS/////////////////////////////////////////////
		// ARENA
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.47129, -2.1, -13.738553)); // translate it down so it's at the center of the 
		model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		staticShader->setMat4("model", model);

		arena->Draw(*staticShader);

		// GAVILAN
		glm::mat4 modelGavilan = glm::mat4(1.0f);
		modelGavilan = glm::translate(modelGavilan, glm::vec3(0.4, -0.19, -13.7)); // translate it down so it's at the center of the 
		modelGavilan = glm::rotate(modelGavilan, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelGavilan = glm::scale(modelGavilan, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader->setMat4("model", modelGavilan);

		gavilan->Draw(*staticShader);

		// MANGLE 1 --> EL DE LA DERECHA (EL PRIMERO DE LA DERECHA)
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(7.89733, -0.85, -21.0));
		model1 = glm::rotate(model1, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.4f, 0.6f, 0.5f));
		staticShader->setMat4("model", model1);

		mangle->Draw(*staticShader);

		// MANGLE 2
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(5.89733, -1.2, -20.0));
		model2 = glm::rotate(model2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model2 = glm::scale(model2, glm::vec3(0.3f, 0.4f, 0.35f));
		staticShader->setMat4("model", model2);

		mangle->Draw(*staticShader);


		// MANGLE 3
		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::translate(model3, glm::vec3(3.89733, -0.85, -21.0));
		model3 = glm::rotate(model3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model3 = glm::scale(model3, glm::vec3(0.4f, 0.6f, 0.5f));
		staticShader->setMat4("model", model3);

		mangle->Draw(*staticShader);

		// MANGLE 4
		glm::mat4 model4 = glm::mat4(1.0f);
		model4 = glm::translate(model4, glm::vec3(1.89733, -0.95, -20.0));
		model4 = glm::rotate(model4, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model4 = glm::scale(model4, glm::vec3(0.3f, 0.4f, 0.35f));
		staticShader->setMat4("model", model4);

		mangle->Draw(*staticShader);


		// MANGLE 5
		glm::mat4 model5 = glm::mat4(1.0f);
		model5 = glm::translate(model5, glm::vec3(0.0, -0.85, -21.0));
		model5 = glm::rotate(model5, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model5 = glm::scale(model5, glm::vec3(0.4f, 0.6f, 0.5f));
		staticShader->setMat4("model", model5);

		mangle->Draw(*staticShader);


		// MANGLE 6
		glm::mat4 model6 = glm::mat4(1.0f);
		model6 = glm::translate(model6, glm::vec3(-1.89733, -0.85, -20.0));
		model6 = glm::rotate(model6, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model6 = glm::scale(model6, glm::vec3((0.3f, 0.4f, 0.35f)));
		staticShader->setMat4("model", model6);

		mangle->Draw(*staticShader);

		// MANGLE 7
		glm::mat4 model7 = glm::mat4(1.0f);
		model7 = glm::translate(model7, glm::vec3(-3.8, -0.85, -21.0));
		model7 = glm::rotate(model7, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model7 = glm::scale(model7, glm::vec3((0.4f, 0.6f, 0.5f)));
		staticShader->setMat4("model", model7);

		mangle->Draw(*staticShader);


		// MANGLE 8
		glm::mat4 model8 = glm::mat4(1.0f);
		model8 = glm::translate(model8, glm::vec3(-5.5, -0.85, -20.0));
		model8 = glm::rotate(model8, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model8 = glm::scale(model8, glm::vec3((0.3f, 0.4f, 0.35f)));
		staticShader->setMat4("model", model8);

		mangle->Draw(*staticShader);



		// MANGLE 9
		glm::mat4 model9 = glm::mat4(1.0f);
		model9 = glm::translate(model9, glm::vec3(-7.0, -0.85, -20.0));
		model9 = glm::rotate(model9, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model9 = glm::scale(model9, glm::vec3((0.4f, 0.6f, 0.5f)));
		staticShader->setMat4("model", model9);

		mangle->Draw(*staticShader);


		// LATA DE COCA 1
		glm::mat4 model12 = glm::mat4(1.0f);
		model12 = glm::translate(model12, glm::vec3(0.047129, -1.841403, -13.738553));
		model12 = glm::rotate(model12, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model12 = glm::scale(model12, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader->setMat4("model", model12);

		lataCoca->Draw(*staticShader);


		// LATA DE COCA 2
		glm::mat4 model13 = glm::mat4(1.0f);
		model13 = glm::translate(model13, glm::vec3(4.047129, -1.81403, -9.738553));
		model13 = glm::rotate(model13, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model13 = glm::scale(model13, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader->setMat4("model", model13);

		lataCoca->Draw(*staticShader);

		// LATA DE COCA 3
		glm::mat4 model14 = glm::mat4(1.0f);
		model14 = glm::translate(model14, glm::vec3(-3.047129, -1.81403, -11.738553));
		model14 = glm::rotate(model14, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model14 = glm::scale(model14, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader->setMat4("model", model14);

		lataCoca->Draw(*staticShader);


		// LATA DE FANTA 3
		glm::mat4 model15 = glm::mat4(1.0f);
		model15 = glm::translate(model15, glm::vec3(-3.047129, -1.9999, -15.738553));
		model15 = glm::rotate(model15, glm::radians(-50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model15 = glm::scale(model15, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader->setMat4("model", model15);

		lataFanta->Draw(*staticShader);

		// LATA DE SPRITE 3
		glm::mat4 model16 = glm::mat4(1.0f);
		model16 = glm::translate(model16, glm::vec3(4.047129, -1.81403, -18.738553));
		model16 = glm::rotate(model16, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model16 = glm::scale(model16, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader->setMat4("model", model16);

		lataSprite->Draw(*staticShader);

		//COCODRILO
		glm::mat4 model17 = glm::mat4(1.0f);
		model17 = glm::translate(model17, glm::vec3(2.11384, -0.5603, -11.50047));
		model17 = glm::rotate(model17, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model17 = glm::rotate(model17, glm::radians(-8.28166f), glm::vec3(1.0f, 1.0f, 0.0f));
		model17 = glm::scale(model17, glm::vec3(3.0f, 3.0f, 4.0f));
		staticShader->setMat4("model", model17);

		cocodrilo->Draw(*staticShader);

		//COCODRILO BEBE
		glm::mat4 model18 = glm::mat4(1.0f);
		model18 = glm::translate(model18, glm::vec3(2.11384, -0.5, -9.50047));
		model18 = glm::rotate(model18, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model18 = glm::rotate(model18, glm::radians(-8.28166f), glm::vec3(1.0f, 1.0f, 0.0f));
		model18 = glm::scale(model18, glm::vec3(1.5f, 1.5f, 2.5f));
		staticShader->setMat4("model", model18);

		cocodrilo->Draw(*staticShader);


		//COCODRILO 2
		glm::mat4 model19 = glm::mat4(1.0f);
		model19 = glm::translate(model19, glm::vec3(-1.9733, -0.551403, -14.50047));
		model19 = glm::rotate(model19, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model19 = glm::rotate(model19, glm::radians(63.8f), glm::vec3(0.0f, 0.0f, 1.0f));
		model19 = glm::rotate(model19, glm::radians(-4.97354f), glm::vec3(0.0f, 1.0f, 0.0f));
		model19 = glm::scale(model19, glm::vec3(2.5f, 2.5f, 3.5f));
		staticShader->setMat4("model", model19);

		cocodrilo->Draw(*staticShader);


		//OSTRA 
		glm::mat4 model20 = glm::mat4(1.0f);
		model20 = glm::translate(model20, glm::vec3(2.047129, -1.9876, -16.738553));
		model20 = glm::rotate(model20, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model20 = glm::scale(model20, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader->setMat4("model", model20);

		ostra->Draw(*staticShader);


		//OSTRA 2
		glm::mat4 model21 = glm::mat4(1.0f);
		model21 = glm::translate(model21, glm::vec3(4.047129, -1.91826, -12.738553));
		model21 = glm::rotate(model21, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model21 = glm::rotate(model21, glm::radians(-50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model21 = glm::scale(model21, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader->setMat4("model", model21);

		ostra->Draw(*staticShader);

		//OSTRA 3
		glm::mat4 model22 = glm::mat4(1.0f);
		model22 = glm::translate(model22, glm::vec3(-2.047129, -1.9876, -16.738553));
		model22 = glm::rotate(model22, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model22 = glm::rotate(model22, glm::radians(-80.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model22 = glm::scale(model22, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader->setMat4("model", model21);

		ostra->Draw(*staticShader);


		//OSTRA 4
		glm::mat4 model23 = glm::mat4(1.0f);
		model23 = glm::translate(model23, glm::vec3(-2.047129, -1.91826, -10.738553));
		model23 = glm::rotate(model23, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model23 = glm::rotate(model23, glm::radians(-50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model23 = glm::scale(model23, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader->setMat4("model", model23);

		ostra->Draw(*staticShader);

		//OSTRA 5
		glm::mat4 model24 = glm::mat4(1.0f);
		model24 = glm::translate(model24, glm::vec3(-4.047129, -1.91826, -16.738553));
		model24 = glm::rotate(model24, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model24 = glm::rotate(model24, glm::radians(-130.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model24 = glm::scale(model24, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader->setMat4("model", model24);

		ostra->Draw(*staticShader);


		//OSTRA 5
		glm::mat4 model25 = glm::mat4(1.0f);
		model25 = glm::translate(model25, glm::vec3(0.0, -1.9, -16.0));
		model25 = glm::rotate(model25, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model25 = glm::rotate(model25, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model25 = glm::scale(model25, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader->setMat4("model", model25);

		ostra->Draw(*staticShader);

		//ESTRELLA DE MAR 1
		glm::mat4 model26 = glm::mat4(1.0f);
		model26 = glm::translate(model26, glm::vec3(2.5, -2.0, -15.0));
		model26 = glm::rotate(model26, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model26 = glm::scale(model26, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader->setMat4("model", model26);

		estrella->Draw(*staticShader);

		//ESTRELLA DE MAR 2
		glm::mat4 model27 = glm::mat4(1.0f);
		model27 = glm::translate(model27, glm::vec3(-2.5, -2.0, -12.0));
		model27 = glm::rotate(model27, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model27 = glm::scale(model27, glm::vec3(1.5f, 1.5f, 1.5f));
		staticShader->setMat4("model", model27);

		estrella->Draw(*staticShader);

		//ESTRELLA DE MAR 3
		glm::mat4 model28 = glm::mat4(1.0f);
		model28 = glm::translate(model28, glm::vec3(-0.5, -2.0, -18.0));
		model28 = glm::rotate(model28, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model28 = glm::scale(model28, glm::vec3(1.5f, 1.5f, 1.5f));
		staticShader->setMat4("model", model28);

		estrella->Draw(*staticShader);

		//ESTRELLA DE MAR 4
		glm::mat4 model29 = glm::mat4(1.0f);
		model29 = glm::translate(model29, glm::vec3(0.0, -2.0, -11.0));
		model29 = glm::rotate(model29, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model29 = glm::scale(model29, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader->setMat4("model", model29);

		estrella->Draw(*staticShader);

		//ESTRELLA DE MAR 5
		glm::mat4 model30 = glm::mat4(1.0f);
		model30 = glm::translate(model30, glm::vec3(4.0, -2.0, -16.0));
		model30 = glm::rotate(model30, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model30 = glm::scale(model30, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader->setMat4("model", model30);

		estrella->Draw(*staticShader);

		//FLAMINGO 1
		glm::mat4 model31 = glm::mat4(1.0f);
		model31 = glm::translate(model31, glm::vec3(0.2, -3.3, -12.0));
		model31 = glm::rotate(model31, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model31 = glm::scale(model31, glm::vec3(0.5f, 0.55f, 0.55f));
		staticShader->setMat4("model", model31);

		flamingo->Draw(*staticShader);

		//FLAMINGO 2
		glm::mat4 model32 = glm::mat4(1.0f);
		model32 = glm::translate(model32, glm::vec3(0.7, -3.4, -12.0));
		model32 = glm::rotate(model32, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model32 = glm::scale(model32, glm::vec3(0.5f, 0.55f, 0.55f));
		staticShader->setMat4("model", model32);

		flamingo->Draw(*staticShader);

		//FLAMINGO 3
		glm::mat4 model33 = glm::mat4(1.0f);
		model33 = glm::translate(model33, glm::vec3(1.0, -3.3, -12.0));
		model33 = glm::rotate(model33, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model33 = glm::scale(model33, glm::vec3(0.5f, 0.55f, 0.55f));
		staticShader->setMat4("model", model33);

		flamingo->Draw(*staticShader);


		//FLAMINGO 4
		glm::mat4 model34 = glm::mat4(1.0f);
		model34 = glm::translate(model34, glm::vec3(4.8, -3.1, -5.0));
		model34 = glm::rotate(model34, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model34 = glm::scale(model34, glm::vec3(0.5f, 0.55f, 0.55f));
		staticShader->setMat4("model", model34);

		flamingo->Draw(*staticShader);

		//FLAMINGO 5
		glm::mat4 model35 = glm::mat4(1.0f);
		model35 = glm::translate(model35, glm::vec3(4.5, -3.1, -5.3));
		model35 = glm::rotate(model35, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model35 = glm::scale(model35, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader->setMat4("model", model35);

		flamingo->Draw(*staticShader);

		//FLAMINGO 6
		glm::mat4 model36 = glm::mat4(1.0f);
		model36 = glm::translate(model36, glm::vec3(4.1, -3.1, -5.0));
		model36 = glm::rotate(model36, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model36 = glm::scale(model36, glm::vec3(0.5f, 0.55f, 0.55f));
		staticShader->setMat4("model", model36);

		flamingo->Draw(*staticShader);

		//FLAMINGO 7
		glm::mat4 model37 = glm::mat4(1.0f);
		model37 = glm::translate(model37, glm::vec3(3.7, -3.0, -5.2));
		model37 = glm::rotate(model37, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model37 = glm::scale(model37, glm::vec3(0.5f, 0.55f, 0.5f));
		staticShader->setMat4("model", model37);

		flamingo->Draw(*staticShader);


		//FLAMINGO 8
		glm::mat4 model38 = glm::mat4(1.0f);
		model38 = glm::translate(model38, glm::vec3(-4.2, -3.05, -10.0));
		model38 = glm::rotate(model38, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model38 = glm::scale(model38, glm::vec3(0.5f, 0.55f, 0.55f));
		staticShader->setMat4("model", model38);

		flamingo->Draw(*staticShader);

		//FLAMINGO 9
		glm::mat4 model39 = glm::mat4(1.0f);
		model39 = glm::translate(model39, glm::vec3(-4.7, -2.90, -10.4));
		model39 = glm::rotate(model39, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model39 = glm::scale(model39, glm::vec3(0.5f, 0.6f, 0.55f));
		staticShader->setMat4("model", model39);

		flamingo->Draw(*staticShader);

		//FLAMINGO 10
		glm::mat4 model40 = glm::mat4(1.0f);
		model40 = glm::translate(model40, glm::vec3(-3.60, -2.90, -10.8));
		model40 = glm::rotate(model40, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model40 = glm::scale(model40, glm::vec3(0.5f, 0.4f, 0.55f));
		staticShader->setMat4("model", model40);

		flamingo->Draw(*staticShader);

		//CANGREJO 1
		glm::mat4 model41 = glm::mat4(1.0f);
		model41 = glm::translate(model41, glm::vec3(7.8, -2.0, -20.0));
		model41 = glm::rotate(model41, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model41 = glm::scale(model41, glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader->setMat4("model", model41);

		cangrejo->Draw(*staticShader);

		//CANGREJO 2
		glm::mat4 model42 = glm::mat4(1.0f);
		model42 = glm::translate(model42, glm::vec3(-4.8, -2.0, -6.0));
		model42 = glm::rotate(model42, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model42 = glm::scale(model42, glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader->setMat4("model", model42);

		cangrejo->Draw(*staticShader);

		//CANGREJO 3
		glm::mat4 model43 = glm::mat4(1.0f);
		model43 = glm::translate(model43, glm::vec3(-6.0, -2.0, -15.0));
		model43 = glm::rotate(model43, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model43 = glm::scale(model43, glm::vec3(0.7f, 0.7f, 0.7f));
		staticShader->setMat4("model", model43);

		cangrejo->Draw(*staticShader);


		//CANGREJO 4
		glm::mat4 model44 = glm::mat4(1.0f);
		model44 = glm::translate(model44, glm::vec3(0.0, -2.0, -20.0));
		model44 = glm::rotate(model44, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model44 = glm::scale(model44, glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader->setMat4("model", model44);

		cangrejo->Draw(*staticShader);

		//CANGREJO 5
		glm::mat4 model45 = glm::mat4(1.0f);
		model45 = glm::translate(model45, glm::vec3(3.8, -2.0, -20.0));
		model45 = glm::rotate(model45, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model45 = glm::scale(model45, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader->setMat4("model", model45);

		cangrejo->Draw(*staticShader);

		//CANGREJO 6
		glm::mat4 model46 = glm::mat4(1.0f);
		model46 = glm::translate(model46, glm::vec3(0.8, -2.0, -6.0));
		model46 = glm::rotate(model46, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model46 = glm::scale(model46, glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader->setMat4("model", model46);

		cangrejo->Draw(*staticShader);


		//MEDUSA 1
		glm::mat4 model47 = glm::mat4(1.0f);
		model47 = glm::translate(model47, glm::vec3(5.8, -1.0, -10.0));
		model47 = glm::rotate(model47, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model47 = glm::scale(model47, glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader->setMat4("model", model47);

		medusa->Draw(*staticShader);

		//MEDUSA 2
		glm::mat4 model48 = glm::mat4(1.0f);
		model48 = glm::translate(model48, glm::vec3(0.8, -1.0, -13.0));
		model48 = glm::rotate(model48, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model48 = glm::scale(model48, glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader->setMat4("model", model48);

		medusa->Draw(*staticShader);

		//MEDUSA 3
		glm::mat4 model49 = glm::mat4(1.0f);
		model49 = glm::translate(model49, glm::vec3(-4.8, -1.2, -15.0));
		model49 = glm::rotate(model49, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model49 = glm::scale(model49, glm::vec3(0.19f, 0.19f, 0.19f));
		staticShader->setMat4("model", model49);

		medusa->Draw(*staticShader);

		//MEDUSA 4
		glm::mat4 model50 = glm::mat4(1.0f);
		model50 = glm::translate(model50, glm::vec3(0.0, -1.0, -12.0));
		model50 = glm::rotate(model50, glm::radians(-150.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model50 = glm::scale(model50, glm::vec3(0.15f, 0.15f, 0.15f));
		staticShader->setMat4("model", model50);

		medusa->Draw(*staticShader);

		//MEDUSA 5
		glm::mat4 model51 = glm::mat4(1.0f);
		model51 = glm::translate(model51, glm::vec3(4.8, -1.0, -18.0));
		model51 = glm::rotate(model51, glm::radians(-120.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model51 = glm::scale(model51, glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader->setMat4("model", model51);

		medusa->Draw(*staticShader);

		//MEDUSA 6
		glm::mat4 model52 = glm::mat4(1.0f);
		model52 = glm::translate(model52, glm::vec3(-7.0, -1.1, -19.5));
		model52 = glm::rotate(model52, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model52 = glm::scale(model52, glm::vec3(0.25f, 0.25f, 0.25f));
		staticShader->setMat4("model", model52);

		medusa->Draw(*staticShader);


		//ROCA 1
		glm::mat4 model53 = glm::mat4(1.0f);
		model53 = glm::translate(model53, glm::vec3(1.2, -2.0, -15.2));
		model53 = glm::rotate(model53, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model53 = glm::scale(model53, glm::vec3(0.344f, 0.344f, 0.344f));
		staticShader->setMat4("model", model53);

		roca->Draw(*staticShader);

		//ROCA 2
		glm::mat4 model54 = glm::mat4(1.0f);
		model54 = glm::translate(model54, glm::vec3(2.0, -2.0, -15.5));
		model54 = glm::rotate(model54, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model54 = glm::scale(model54, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader->setMat4("model", model54);

		roca->Draw(*staticShader);

		//ROCA 3
		glm::mat4 model55 = glm::mat4(1.0f);
		model55 = glm::translate(model55, glm::vec3(1.5, -2.0, -15.5));
		model55 = glm::rotate(model55, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model55 = glm::scale(model55, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader->setMat4("model", model55);

		roca->Draw(*staticShader);


		//ROCA 4
		glm::mat4 model56 = glm::mat4(1.0f);
		model56 = glm::translate(model56, glm::vec3(5.8, -2.0, -8.4));
		model56 = glm::rotate(model56, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model56 = glm::scale(model56, glm::vec3(0.3f, 0.6f, 0.57f));
		staticShader->setMat4("model", model56);

		roca->Draw(*staticShader);


		//ROCA 5
		glm::mat4 model57 = glm::mat4(1.0f);
		model57 = glm::translate(model57, glm::vec3(4.8, -2.0, -8.6));
		model57 = glm::rotate(model57, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model57 = glm::scale(model57, glm::vec3(0.6f, 0.8f, 0.6f));
		staticShader->setMat4("model", model57);

		roca->Draw(*staticShader);


		//ROCA 6
		glm::mat4 model58 = glm::mat4(1.0f);
		model58 = glm::translate(model58, glm::vec3(-3.0, -2.0, -13.2));
		model58 = glm::rotate(model58, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model58 = glm::scale(model58, glm::vec3(0.6f, 0.8f, 0.7f));
		staticShader->setMat4("model", model58);

		roca->Draw(*staticShader);


		//ROCA 7
		glm::mat4 model59 = glm::mat4(1.0f);
		model59 = glm::translate(model59, glm::vec3(-4.0, -2.0, -13.9));
		model59 = glm::rotate(model59, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model59 = glm::scale(model59, glm::vec3(0.5f, 0.9f, 0.7f));
		staticShader->setMat4("model", model59);

		roca->Draw(*staticShader);

		//ROCA 8
		glm::mat4 model60 = glm::mat4(1.0f);
		model60 = glm::translate(model60, glm::vec3(-2.5, -2.0, -13.2));
		model60 = glm::rotate(model60, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model60 = glm::scale(model60, glm::vec3(0.3f, 0.9f, 0.7f));
		staticShader->setMat4("model", model60);

		roca->Draw(*staticShader);

		//ROCA 9
		glm::mat4 model61 = glm::mat4(1.0f);
		model61 = glm::translate(model61, glm::vec3(8.0, -1.9, -13.2));
		model61 = glm::rotate(model61, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model61 = glm::scale(model61, glm::vec3(0.4f, 0.9f, 0.7f));
		staticShader->setMat4("model", model61);

		roca->Draw(*staticShader);

		//ROCA 10
		glm::mat4 model62 = glm::mat4(1.0f);
		model62 = glm::translate(model62, glm::vec3(8.0, -1.9, -13.2));
		model62 = glm::rotate(model62, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model62 = glm::scale(model62, glm::vec3(0.4f, 0.9f, 1.2f));
		staticShader->setMat4("model", model62);

		roca->Draw(*staticShader);

		//GARZA 1
		glm::mat4 model63 = glm::mat4(1.0f);
		model63 = glm::translate(model63, glm::vec3(7.9, -0.65, -13.1));
		model63 = glm::rotate(model63, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model63 = glm::rotate(model63, glm::radians(-90.0f), glm::vec3(0.0f, .0f, 1.0f));
		model63 = glm::scale(model63, glm::vec3(0.8f, 0.8, 0.8f));
		staticShader->setMat4("model", model63);

		garza->Draw(*staticShader);

		//GARZA 2
		glm::mat4 model64 = glm::mat4(1.0f);
		model64 = glm::translate(model64, glm::vec3(7.65, -0.85, -13.1));
		model64 = glm::rotate(model64, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model64 = glm::rotate(model64, glm::radians(-90.0f), glm::vec3(0.0f, .0f, 1.0f));
		model64 = glm::scale(model64, glm::vec3(0.8f, 0.8, 0.8f));
		staticShader->setMat4("model", model64);

		garza->Draw(*staticShader);

		//ALGA 1
		glm::mat4 model65 = glm::mat4(1.0f);
		model65 = glm::translate(model65, glm::vec3(4.47129, -2.1, -18.738553));
		model65 = glm::rotate(model65, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model65 = glm::rotate(model65, glm::radians(-90.0f), glm::vec3(0.0f, .0f, 1.0f));
		model65 = glm::scale(model65, glm::vec3(0.8f, 0.8, 1.6f));
		staticShader->setMat4("model", model65);

		alga->Draw(*staticShader);

		//ALGA 2
		glm::mat4 model66 = glm::mat4(1.0f);
		model66 = glm::translate(model66, glm::vec3(-5.47129, -2.1, -7.738553));
		model66 = glm::rotate(model66, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model66 = glm::rotate(model66, glm::radians(-90.0f), glm::vec3(0.0f, .0f, 1.0f));
		model66 = glm::scale(model66, glm::vec3(0.8f, 0.8, 1.5f));
		staticShader->setMat4("model", model66);

		alga->Draw(*staticShader);

		//ALGA 3
		glm::mat4 model67 = glm::mat4(1.0f);
		model67 = glm::translate(model67, glm::vec3(6.0, -2.1, -19.738553));
		model67 = glm::rotate(model67, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model67 = glm::rotate(model67, glm::radians(-90.0f), glm::vec3(0.0f, .0f, 1.0f));
		model67 = glm::scale(model67, glm::vec3(0.8f, 0.8, 1.6f));
		staticShader->setMat4("model", model67);

		alga->Draw(*staticShader);

		//ALGA 4
		glm::mat4 model68 = glm::mat4(1.0f);
		model68 = glm::translate(model68, glm::vec3(- 4.0, -2.1, -19.738553));
		model68 = glm::rotate(model68, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model68 = glm::rotate(model68, glm::radians(-90.0f), glm::vec3(0.0f, .0f, 1.0f));
		model68 = glm::scale(model68, glm::vec3(0.8f, 0.8, 1.6f));
		staticShader->setMat4("model", model68);

		alga->Draw(*staticShader);

		//ALGA 5
		glm::mat4 model69 = glm::mat4(1.0f);
		model69 = glm::translate(model69, glm::vec3(7.0, -2.1, -6.1));
		model69 = glm::rotate(model69, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model69 = glm::rotate(model69, glm::radians(-90.0f), glm::vec3(0.0f, .0f, 1.0f));
		model69 = glm::scale(model69, glm::vec3(0.8f, 0.8, 0.8f));
		staticShader->setMat4("model", model69);

		alga->Draw(*staticShader);

		//ALGA 6
		glm::mat4 model70 = glm::mat4(1.0f);
		model70 = glm::translate(model70, glm::vec3(-7.0, -2.1, -18.0));
		model70 = glm::rotate(model70, glm::radians(-90.0f), glm::vec3(1.0, 0.0f, 0.0f));
		model70 = glm::rotate(model70, glm::radians(-90.0f), glm::vec3(0.0f, .0f, 1.0f));
		model70 = glm::scale(model70, glm::vec3(0.8f, 0.8, 0.8f));
		staticShader->setMat4("model", model70);

		alga->Draw(*staticShader);



	}


	{
		// Activamos el shader del plano
		staticShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		cubeenv->Draw(*staticShader);
	}


	// Animación de partículas
	{
		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		glm::mat4 projection;
		glm::mat4 view;

		if (activeCamera) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
		}
		else {
			projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3rd.GetViewMatrix();
		}

		for (int psc = 0; psc < particlesSystem.particles.size(); psc++) {
			Particle p_i = particlesSystem.particles.at(psc);

			// Activación del shader de las partículas
			particlesShader->use();
			particlesShader->setMat4("projection", projection);
			particlesShader->setMat4("view", view);

			// Aplicamos transformaciones del modelo
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, p_i.position); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down

			particlesShader->setMat4("model", model);

			// Dibujamos el modelo
			particleModel->Draw(*particlesShader);
		}

	}

	glUseProgram(0);

	/*OLEAJE*/
	{
		// Activamos el shader de Phong
		wavesShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		wavesShader->setMat4("projection", projection);
		wavesShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.8, -1.0, -13.738553));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.6f, 2.0f));
		wavesShader->setMat4("model", model);

		wavesShader->setFloat("time", wavesTime);
		wavesShader->setFloat("radius", 5.0f);
		wavesShader->setFloat("height", 5.0f);

		WaterGridMesh->Draw(*wavesShader);
		wavesTime += 0.01;

	}

	glUseProgram(0);

	/*NENÚFAR*/
	{
		// Activamos el shader de Phong
		nenufarShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		nenufarShader->setMat4("projection", projection);
		nenufarShader->setMat4("view", view);

		//Nenúfar 1
		glm::mat4 modelNenufar1 = glm::mat4(1.0f);
		modelNenufar1 = glm::translate(modelNenufar1, glm::vec3(0.47129, -0.33, -13.738553));
		modelNenufar1 = glm::rotate(modelNenufar1, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelNenufar1 = glm::scale(modelNenufar1, glm::vec3(1.5f, 1.5f, 1.5f));
		nenufarShader->setMat4("model", modelNenufar1);

		nenufarShader->setFloat("time", wavesTime);
		nenufarShader->setFloat("radius", 5.0f);
		nenufarShader->setFloat("height", 5.0f);

		NenufarGridMesh->Draw(*nenufarShader);

		//Nenúfar 2
		glm::mat4 modelNenufar2 = glm::mat4(1.0f);
		modelNenufar2 = glm::translate(modelNenufar2, glm::vec3(3.0, -0.295, -7.5));
		modelNenufar2 = glm::rotate(modelNenufar2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelNenufar2 = glm::scale(modelNenufar2, glm::vec3(1.5f, 1.5f, 1.5f));
		nenufarShader->setMat4("model", modelNenufar2);

		nenufarShader->setFloat("time", wavesTime + 1.57);
		nenufarShader->setFloat("radius", 5.0f);
		nenufarShader->setFloat("height", 5.0f);

		NenufarGridMesh->Draw(*nenufarShader);

		//Nenúfar 3
		glm::mat4 modelNenufar3 = glm::mat4(1.0f);
		modelNenufar3 = glm::translate(modelNenufar3, glm::vec3(-4.0, -0.265, -15.0));
		modelNenufar3 = glm::rotate(modelNenufar3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelNenufar3 = glm::scale(modelNenufar3, glm::vec3(1.5f, 1.5f, 1.5f));
		nenufarShader->setMat4("model", modelNenufar3);

		nenufarShader->setFloat("time", wavesTime + 1.5707);
		nenufarShader->setFloat("radius", 5.0f);
		nenufarShader->setFloat("height", 5.0f);

		NenufarGridMesh->Draw(*nenufarShader);

		//Nenúfar 4
		glm::mat4 modelNenufar4 = glm::mat4(1.0f);
		modelNenufar4 = glm::translate(modelNenufar4, glm::vec3(-3.0, -0.205, -9.0));
		modelNenufar4 = glm::rotate(modelNenufar4, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelNenufar4 = glm::scale(modelNenufar4, glm::vec3(1.5f, 1.5f, 1.5f));
		nenufarShader->setMat4("model", modelNenufar4);

		nenufarShader->setFloat("time", wavesTime + 1.5707);
		nenufarShader->setFloat("radius", 5.0f);
		nenufarShader->setFloat("height", 5.0f);

		NenufarGridMesh->Draw(*nenufarShader);

		//Nenúfar 5
		glm::mat4 modelNenufar5 = glm::mat4(1.0f);
		modelNenufar5 = glm::translate(modelNenufar5, glm::vec3(6.0, -0.24, -12.0));
		modelNenufar5 = glm::rotate(modelNenufar5, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelNenufar5 = glm::scale(modelNenufar5, glm::vec3(1.5f, 1.5f, 1.5f));
		nenufarShader->setMat4("model", modelNenufar5);

		nenufarShader->setFloat("time", wavesTime + 1.5707);
		nenufarShader->setFloat("radius", 5.0f);
		nenufarShader->setFloat("height", 5.0f);

		NenufarGridMesh->Draw(*nenufarShader);

	}

	/*MEDUSAS*/
	{
		// Activamos el shader de Phong
		jellyFishShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		jellyFishShader->setMat4("projection", projection);
		jellyFishShader->setMat4("view", view);

		//Medusa 1
		glm::mat4 modelMedusa1 = glm::mat4(1.0f);
		modelMedusa1 = glm::translate(modelMedusa1, glm::vec3(5.8, -1.3, -10.0));
		modelMedusa1 = glm::rotate(modelMedusa1, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMedusa1 = glm::scale(modelMedusa1, glm::vec3(0.2f, 0.2f, 0.2f));
		jellyFishShader->setMat4("model", modelMedusa1);

		jellyFishShader->setFloat("time", wavesTime);
		jellyFishShader->setFloat("radius", 5.0f);
		jellyFishShader->setFloat("height", 5.0f);

		medusa->Draw(*jellyFishShader);

		//Medusa 2
		glm::mat4 modelMedusa2 = glm::mat4(1.0f);
		modelMedusa2 = glm::translate(modelMedusa2, glm::vec3(0.0, -1.5, -7.0));
		modelMedusa2 = glm::rotate(modelMedusa2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMedusa2 = glm::scale(modelMedusa2, glm::vec3(0.3f, 0.3f, 0.3f));
		jellyFishShader->setMat4("model", modelMedusa2);

		jellyFishShader->setFloat("time", wavesTime);
		jellyFishShader->setFloat("radius", 5.0f);
		jellyFishShader->setFloat("height", 5.0f);

		medusa->Draw(*jellyFishShader);

		//Medusa 3
		glm::mat4 modelMedusa3 = glm::mat4(1.0f);
		modelMedusa3 = glm::translate(modelMedusa3, glm::vec3(-4.8, -1.5, -13.0));
		modelMedusa3 = glm::rotate(modelMedusa3, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMedusa3 = glm::scale(modelMedusa3, glm::vec3(0.19f, 0.19f, 0.19f));
		jellyFishShader->setMat4("model", modelMedusa3);

		jellyFishShader->setFloat("time", wavesTime);
		jellyFishShader->setFloat("radius", 5.0f);
		jellyFishShader->setFloat("height", 5.0f);

		medusa->Draw(*jellyFishShader);

		//Medusa 4
		glm::mat4 modelMedusa4 = glm::mat4(1.0f);
		modelMedusa4 = glm::translate(modelMedusa4, glm::vec3(0.0, -1.3, -11.0));
		modelMedusa4 = glm::rotate(modelMedusa4, glm::radians(-150.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMedusa4 = glm::scale(modelMedusa4, glm::vec3(0.15f, 0.15f, 0.15f));
		jellyFishShader->setMat4("model", modelMedusa4);

		jellyFishShader->setFloat("time", wavesTime);
		jellyFishShader->setFloat("radius", 5.0f);
		jellyFishShader->setFloat("height", 5.0f);

		medusa->Draw(*jellyFishShader);

		//Medusa 5
		glm::mat4 modelMedusa5 = glm::mat4(1.0f);
		modelMedusa5 = glm::translate(modelMedusa5, glm::vec3(4.8, -1.3, -16.0));
		modelMedusa5 = glm::rotate(modelMedusa5, glm::radians(-120.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMedusa5 = glm::scale(modelMedusa5, glm::vec3(0.2f, 0.2f, 0.2f));
		jellyFishShader->setMat4("model", modelMedusa5);

		jellyFishShader->setFloat("time", wavesTime);
		jellyFishShader->setFloat("radius", 5.0f);
		jellyFishShader->setFloat("height", 5.0f);

		medusa->Draw(*jellyFishShader);
		
		//Medusa 6
		glm::mat4 modelMedusa6 = glm::mat4(1.0f);
		modelMedusa6 = glm::translate(modelMedusa6, glm::vec3(-7.0, -1.4, -17.5));
		modelMedusa6 = glm::rotate(modelMedusa6, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMedusa6 = glm::scale(modelMedusa6, glm::vec3(0.25f, 0.25f, 0.25f));
		jellyFishShader->setMat4("model", modelMedusa6);

		jellyFishShader->setFloat("time", wavesTime);
		jellyFishShader->setFloat("radius", 5.0f);
		jellyFishShader->setFloat("height", 5.0f);

		medusa->Draw(*jellyFishShader);
	}

	glUseProgram(0);

	/*BOA*/
	{
		// Activamos el shader de Phong
		boaShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		boaShader->setMat4("projection", projection);
		boaShader->setMat4("view", view);

		//Boa
		glm::mat4 modelBoa = glm::mat4(1.0f);
		modelBoa = glm::translate(modelBoa, glm::vec3(-3.0, -2.0, -17.5));
		modelBoa = glm::rotate(modelBoa, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBoa = glm::scale(modelBoa, glm::vec3(0.2f, 0.2f, 0.2f));
		boaShader->setMat4("model", modelBoa);

		boaShader->setFloat("time", wavesTime*5);
		boaShader->setFloat("radius", 5.0f);
		boaShader->setFloat("height", 5.0f);

		boa->Draw(*boaShader);
	}

	glUseProgram(0);

	//ANIMACIONES
	{
		// Activación del shader del personaje
		ourShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		glm::mat4 projection;
		glm::mat4 view;

		if (activeCamera) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
		}
		else {
			projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3rd.GetViewMatrix();
		}

		ourShader->setMat4("projection", projection);
		ourShader->setMat4("view", view);

		/*LANCHA*/
		glm::mat4 modelLancha = glm::mat4(1.0f);
		modelLancha = glm::translate(modelLancha, glm::vec3(-4.5, -0.45, -7.0));
		modelLancha = glm::rotate(modelLancha, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelLancha = glm::scale(modelLancha, glm::vec3(0.7f, 0.7f, 0.7f));
		ourShader->setMat4("model", modelLancha);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones);

		// Dibujamos el modelo
		lancha->Draw(*ourShader);

		// MANATI
		glm::mat4 modelManati = glm::mat4(1.0f);
		modelManati = glm::translate(modelManati, glm::vec3(6.0, -1.4, -17.0)); // translate it down so it's at the center of the 
		modelManati = glm::rotate(modelManati, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelManati = glm::scale(modelManati, glm::vec3(0.09f, 0.09f, 0.09f));
		ourShader->setMat4("model", modelManati);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesManati);

		manati->Draw(*ourShader);

		// MANATI BEBE
		glm::mat4 modelManatiBebe = glm::mat4(1.0f);
		modelManatiBebe = glm::translate(modelManatiBebe, glm::vec3(5.0, -1.4, -15.0)); // translate it down so it's at the center of the 
		modelManatiBebe = glm::rotate(modelManatiBebe, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelManatiBebe = glm::scale(modelManatiBebe, glm::vec3(0.03f, 0.03f, 0.03f));
		ourShader->setMat4("model", modelManatiBebe);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesManati);

		manati->Draw(*ourShader);

		// TORTUGA
		glm::mat4 modelTortuga = glm::mat4(1.0f);
		modelTortuga = glm::translate(modelTortuga, glm::vec3(-5.5, -1.4, -8.0)); // translate it down so it's at the center of the 
		modelTortuga = glm::rotate(modelTortuga, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTortuga = glm::scale(modelTortuga, glm::vec3(0.1f, 0.1f, 0.1f));
		ourShader->setMat4("model", modelTortuga);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesTortuga);

		tortuga->Draw(*ourShader);

		// TORTUGA BEBE
		glm::mat4 modelTortugaBebe = glm::mat4(1.0f);
		modelTortugaBebe = glm::translate(modelTortugaBebe, glm::vec3(-4.7, -1.4, -8.0)); // translate it down so it's at the center of the 
		modelTortugaBebe = glm::rotate(modelTortugaBebe, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTortugaBebe = glm::scale(modelTortugaBebe, glm::vec3(0.05f, 0.05f, 0.05f));
		ourShader->setMat4("model", modelTortugaBebe);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesTortuga);

		tortuga->Draw(*ourShader);

		// TORTUGA 2
		glm::mat4 modelTortuga2 = glm::mat4(1.0f);
		modelTortuga2 = glm::translate(modelTortuga2, glm::vec3(7.5, -1.5, -18.0)); // translate it down so it's at the center of the 
		modelTortuga2 = glm::rotate(modelTortuga2, glm::radians(13.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTortuga2 = glm::scale(modelTortuga2, glm::vec3(0.04f, 0.04f, 0.03f));
		ourShader->setMat4("model", modelTortuga2);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesTortuga);

		tortuga->Draw(*ourShader);

	}

	{
		//Activamos el shader de Phong
		proceduralShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		proceduralShader->setMat4("projection", projection);
		proceduralShader->setMat4("view", view);

		// BOLSA
		glm::mat4 modelBolsa = glm::mat4(1.0f);
		modelBolsa = glm::translate(modelBolsa, glm::vec3(-3.0, -1.6, -15.5)); // translate it down so it's at the center of the 
		modelBolsa = glm::rotate(modelBolsa, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBolsa = glm::scale(modelBolsa, glm::vec3(0.2f, 0.2f, 0.2f));
		proceduralShaderObjects->setMat4("model", modelBolsa);

		proceduralShaderObjects->setFloat("time", proceduralTime);
		proceduralShaderObjects->setFloat("radius", 0.15f);
		proceduralShaderObjects->setFloat("height", 0.0f);

		bolsa->Draw(*proceduralShaderObjects);

		// BOLSA 2
		glm::mat4 modelBolsa2 = glm::mat4(1.0f);
		modelBolsa2 = glm::translate(modelBolsa2, glm::vec3(3.0, -1.6, -12.0)); // translate it down so it's at the center of the 
		modelBolsa2 = glm::rotate(modelBolsa2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBolsa2 = glm::scale(modelBolsa2, glm::vec3(0.1f, 0.1f, 0.1f));
		proceduralShaderObjects->setMat4("model", modelBolsa2);

		proceduralShaderObjects->setFloat("time", proceduralTime + 20);
		proceduralShaderObjects->setFloat("radius", 0.1f);
		proceduralShaderObjects->setFloat("height", 0.0f);

		bolsa->Draw(*proceduralShaderObjects);

		// BOLSA 3
		glm::mat4 modelBolsa3 = glm::mat4(1.0f);
		modelBolsa3 = glm::translate(modelBolsa3, glm::vec3(2.0, -1.6, -18.0)); // translate it down so it's at the center of the 
		modelBolsa3 = glm::rotate(modelBolsa3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBolsa3 = glm::scale(modelBolsa3, glm::vec3(0.25f, 0.25f, 0.25f));
		proceduralShaderObjects->setMat4("model", modelBolsa3);

		proceduralShaderObjects->setFloat("time", proceduralTime + 100);
		proceduralShaderObjects->setFloat("radius", 0.05f);
		proceduralShaderObjects->setFloat("height", 0.0f);

		bolsa->Draw(*proceduralShaderObjects);


		// PEZ
		glm::mat4 modelPez = glm::mat4(1.0f);
		modelPez = glm::translate(modelPez, glm::vec3(-3.0, -1.0, -10.0)); // translate it down so it's at the center of the 
		modelPez = glm::rotate(modelPez, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPez = glm::scale(modelPez, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralShaderObjects->setMat4("model", modelPez);

		proceduralShaderObjects->setFloat("time", proceduralTime + 200);
		proceduralShaderObjects->setFloat("radius", 0.02f);
		proceduralShaderObjects->setFloat("height", 0.0f);

		pez->Draw(*proceduralShaderObjects);

		// PEZ 2
		glm::mat4 modelPez2 = glm::mat4(1.0f);
		modelPez2 = glm::translate(modelPez2, glm::vec3(-2.8, -1.2, -10.0)); // translate it down so it's at the center of the 
		modelPez2 = glm::rotate(modelPez2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPez2 = glm::scale(modelPez2, glm::vec3(0.6f, 0.6f, 0.6f));
		proceduralShaderObjects->setMat4("model", modelPez2);

		proceduralShaderObjects->setFloat("time", proceduralTime + 200);
		proceduralShaderObjects->setFloat("radius", 0.02f);
		proceduralShaderObjects->setFloat("height", 0.0f);

		pez->Draw(*proceduralShaderObjects);

		// PEZ 3
		glm::mat4 modelPez3 = glm::mat4(1.0f);
		modelPez3 = glm::translate(modelPez3, glm::vec3(0.0, -1.0, -8.2)); // translate it down so it's at the center of the 
		modelPez3 = glm::rotate(modelPez3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPez3 = glm::scale(modelPez3, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralShaderObjects->setMat4("model", modelPez3);

		proceduralShaderObjects->setFloat("time", proceduralTime + 200);
		proceduralShaderObjects->setFloat("radius", 0.02f);
		proceduralShaderObjects->setFloat("height", 0.0f);

		pez->Draw(*proceduralShaderObjects);

		// PEZ 4
		glm::mat4 modelPez4 = glm::mat4(1.0f);
		modelPez4 = glm::translate(modelPez4, glm::vec3(0.2, -1.2, -8.0)); // translate it down so it's at the center of the 
		modelPez4 = glm::rotate(modelPez4, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPez4 = glm::scale(modelPez4, glm::vec3(0.6f, 0.6f, 0.6f));
		proceduralShaderObjects->setMat4("model", modelPez4);

		proceduralShaderObjects->setFloat("time", proceduralTime + 200);
		proceduralShaderObjects->setFloat("radius", 0.02f);
		proceduralShaderObjects->setFloat("height", 0.0f);

		pez->Draw(*proceduralShaderObjects);


		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.47129, 2.1, -13.738553));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 5.0f);
		proceduralShader->setFloat("height", 0.0f);

		ave->Draw(*proceduralShader);
		proceduralTime += 0.01;

		// Aplicamos transformaciones del modelo
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(0.7, 1.3, -17.738553));
		model1 = glm::rotate(model1, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::scale(model1, glm::vec3(0.09f, 0.09f, 0.09f));
		proceduralShader->setMat4("model", model1);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 1.0f);
		proceduralShader->setFloat("height", 0.1f);

		ave->Draw(*proceduralShader);
		proceduralTime += 0.01;

		// Aplicamos transformaciones del modelo
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(-6.7, 1.3, -9.738553));
		model2 = glm::rotate(model2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model2 = glm::scale(model2, glm::vec3(0.07f, 0.07f, 0.07f));
		proceduralShader->setMat4("model", model2);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 0.3f);
		proceduralShader->setFloat("height", 0.2f);

		ave->Draw(*proceduralShader);
		proceduralTime += 0.01;


		// Aplicamos transformaciones del modelo
		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::translate(model3, glm::vec3(6.7, 0.9, -3.738553));
		model3 = glm::rotate(model3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model3 = glm::scale(model3, glm::vec3(0.06f, 0.06f, 0.06f));
		proceduralShader->setMat4("model", model3);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 0.5f);
		proceduralShader->setFloat("height", 0.01f);

		ave->Draw(*proceduralShader);
		proceduralTime += 0.001;

	}

	glUseProgram(0);

	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);



}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}
