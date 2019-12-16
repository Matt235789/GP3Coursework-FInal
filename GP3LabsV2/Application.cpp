#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"
#include "Physics.h"
#include "BoxShape.h"
#include "SphereShape.h"
#include "RigidBody.h"

#include <string>

Application* Application::m_application = nullptr;

Application::Application() {

}

void Application::Init()
{
	//initialisation
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError(), errType::ERROR);
		exit(-1);

	}

	//if less than one controller output an error
	if (SDL_NumJoysticks() < 1)
		std::cout << "No Controller" << std::endl;
	else
		SDL_Joystick* joystick = SDL_JoystickOpen(0);

	//setting openGL to v 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//creating a window
	m_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);

	//enable fog
	glEnable(GL_FOG);

	SDL_CaptureMouse(SDL_TRUE);
	OpenGlinit();
	GameInit();

}

void Application::OpenGlinit()
{
	//creating context
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();

	SDL_GL_SetSwapInterval(1);

	//initialising glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);

	}

	//Smooth Shading 
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));

	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));

	//sets less or equal func for depth testing

	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));

	//enable blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glCullFace(GL_BACK));


	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);


}

void Application::Loop()
{
	m_appState = AppState::RUNNING;

	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	RigidBody* addPush = new RigidBody();
	glm::vec3 xAxis;
	xAxis = glm::vec3(1, 0, 0);

	xAxis = glm::normalize(xAxis);
	glm::quat rot;
	float rotateA = 15.0f;
	float rotateD = -15.0f;
	glm::quat currentRot;
	//glm::quat result;

	while (m_appState != AppState::QUITTING)
	{
		//poll SDL events
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;

			case SDL_KEYDOWN:
				INPUT->GetKeyDown(event.key.keysym.sym);
				INPUT->SetKey(event.key.keysym.sym, true);

				break;
				//record when the user presses a key
			case SDL_KEYUP:
				INPUT->GetKeyUp(event.key.keysym.sym);
				INPUT->SetKey(event.key.keysym.sym, false);
				break;
				// contrls the joystick button inputs
			case SDL_JOYBUTTONDOWN:
				if (event.jbutton.button == 0)
				{
					if (event.jbutton.which == 1)
					{
						if (event.jbutton.button == 0)
						{
							m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0.f, 1.f, 0.f));
						}
					}
				}
				break;
			}
			
			/*
			if (event.type == SDL_JOYBUTTONDOWN)
			{
				if (event.jbutton.which == 1)
				{
					if (event.jbutton.button == 0)
					{
						m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0.f, 1.f, 0.f));
					}
				}
			}
			*/

			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				Quit();
				break;

			case SDLK_w:
				m_entities.at(3)->GetTransform()->AddPosition(glm::vec3(0.f, 1.f,0.f));
				break;

			case SDLK_s:
				m_entities.at(3)->GetTransform()->AddPosition(glm::vec3(0.f, -1.f, 0.f));
				break;

			case SDLK_a:
				rot = glm::angleAxis(glm::radians(rotateA), xAxis);
				//currentRot = m_entities.at(0)->GetTransform()->GetRotation();
				m_entities.at(4)->GetTransform()->AddRotation((rot));



				break;
			case SDLK_d:
				rot = glm::angleAxis(glm::radians(rotateD), xAxis);
				m_entities.at(4)->GetTransform()->AddRotation((rot));
				

				break;

			case SDLK_UP:
				m_entities.at(6)->GetTransform()->AddPosition(glm::vec3(0.0f, 0.08f, 0.0f));


				break;

			case SDLK_DOWN:
				m_entities.at(6)->GetTransform()->AddPosition(glm::vec3(0.0f, -0.08f, 0.0f));


				break;

			case SDLK_LEFT:
				m_entities.at(6)->GetTransform()->AddRotation(glm::vec3(0.0f, 0.08f, 0.0f));


				break;

			case SDLK_RIGHT:
				m_entities.at(6)->GetTransform()->AddRotation(glm::vec3(0.0f, -0.08f, 0.0f));


				break;

			case SDL_MOUSEMOTION:
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				break;


			}

		}

		auto currentTicks = std::chrono::high_resolution_clock::now();

		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks -
			prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		Physics::GetInstance()->Update(deltaTime);
		//update and render all entities 
		Update(deltaTime);
		Render();

		//LOG_DEBUG(std::to_string(deltaTime), errType::TRACE);
		//LOG_DEBUG("err", errType::ERROR);
		//LOG_DEBUG("warning", errType::WARNING);

		SDL_GL_SwapWindow(m_window);
	}
}

void Application::Quit()
{
	//Close SDL 
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}

Application::~Application() {

}

Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}

void Application::Run()
{
	Init();
	Loop();
	Quit();
}

void Application::Update(float deltaTime)
{

	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}

void Application::Render()
{
	//Clear Context
	glClearColor(0.5f, 0.5f, 0.5f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// This array stores the colour used for the fog
	GLfloat fogColour[] = { 0.6f, 0.4f, 0.6f, 1 };
	glFogfv(GL_FOG_COLOR, fogColour);

	//fog strength will increase in a linear fashion
	glFogi(GL_FOG_MODE, GL_LINEAR);

	//Where the fog will start drawing from the camera and when it will stop
	glFogf(GL_FOG_START, 5.0f);
	glFogf(GL_FOG_END, 45.0f);

	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}

void Application::GameInit()
{
	//Loading all resources
	Resources::GetInstance()->AddModel("cube.obj");
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddModel("peanut.obj");
	Resources::GetInstance()->AddTexture("peanut.jpg");
	Resources::GetInstance()->AddModel("ball.obj");
	Resources::GetInstance()->AddTexture("basketball.jpg");
	Resources::GetInstance()->AddModel("container.obj");
	Resources::GetInstance()->AddTexture("container.jpg");
	Resources::GetInstance()->AddModel("eyeball.obj");
	Resources::GetInstance()->AddTexture("eyeball.jpg");


	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH +
		"simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"), "simple");

	Entity* e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
	);
	e->GetTransform()->SetPosition(glm::vec3(0, -10, -10));


	MeshRenderer* m = e->GetComponent<MeshRenderer>();
	e->GetTransform()->SetPosition(glm::vec3(0, -10.0f, -20.0f));
	e->AddComponent<RigidBody>();
	e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.0f,
		100.0f)));
	e->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	e->GetTransform()->SetScale(glm::vec3(100.0f, 1.0f, 100.0f));

	//peanut object
	e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("peanut.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("peanut.jpg"))
	);
	e->AddComponent<RigidBody>();
	e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.6f, 0.6f,
		1.0f)));
	e->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	e->GetTransform()->SetPosition(glm::vec3(0, 5, -30));
	e->GetTransform()->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));

	//ball object
	e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("ball.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("basketball.jpg"))
	);
	e->AddComponent<RigidBody>();
	e->GetComponent<RigidBody>()->Init(new SphereShape(2.4f));
	//create new cpp and header file for sphere
	e->GetComponent<RigidBody>()->Get()->setMassProps(0.001, btVector3());
	e->GetTransform()->SetPosition(glm::vec3(0, -2, -20));
	e->GetTransform()->SetScale(glm::vec3(9.0f, 9.0f, 9.0f));


	//container object
	e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("container.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("container.jpg"))
	);
	e->AddComponent<RigidBody>();
	e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(8.0f, 3.40f,
		3.0f)));
	e->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	e->GetTransform()->SetPosition(glm::vec3(0, -9, -22));
	e->GetTransform()->SetScale(glm::vec3(0.025f, 0.02f, 0.02f));

	//container 2
	e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("container.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("container.jpg"))
	);
	e->AddComponent<RigidBody>();
	e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(8.0f, 3.40f,
		3.0f)));
	e->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	e->GetTransform()->SetPosition(glm::vec3(0, -9, -18));
	e->GetTransform()->SetScale(glm::vec3(0.025f, 0.02f, 0.02f));

	//eyeball object
	e = new Entity();
	m_entities.push_back(e);
	e->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("eyeball.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("eyeball.jpg"))
	);
	e->AddComponent<RigidBody>();
	e->GetComponent<RigidBody>()->Init(new SphereShape(2.7f));
	e->GetComponent<RigidBody>()->Get()->setMassProps(0.01, btVector3());
	e->GetTransform()->SetPosition(glm::vec3(0, 0, -20));
	e->GetTransform()->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));

	e = new Entity();

	m_entities.push_back(e);
	CameraComp* cc = new CameraComp();
	e->AddComponent(cc);
	cc->Start();
	e->GetTransform()->SetPosition(glm::vec3(0, 0, 7));
	

	for (int i = 0; i < 20; i++)
	{
		Entity* e = new Entity();
		m_entities.push_back(e);
		e->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("cube.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Wood.jpg"))
		);
		e->GetTransform()->SetPosition(glm::vec3(0, 2.0f * i, -20.0f));
		e->AddComponent<RigidBody>();
		e->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.0f,
			1.0f, 1.0f)));
		e->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));


	}

	
	
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}
