#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image/stb_image.h"

#include "GLDebug.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Model.hpp"
#include "Light.hpp"
#include "FrameBuffers.hpp"
#include "OtherMeshes.hpp"
#include "SceneObject.hpp"
#include "Particle.hpp"

/* include imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <memory>

void processInput(GLFWwindow *window, Camera& camera, Focus_Objects& flag);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
glm::mat4 GetLightSpaceMatrix(glm::vec3 position, glm::vec3 lookat, float near, float far);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;

const int CIRCLE_PTS = 192;
const int PARTICLE_NUM = 10000;

const int ACCUMULATE_FRAME = 5;



// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



Camera camera;

int main(){
    glfwInit();

    // set OpenGL version to 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    // set OpenGL profile to core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // set OpenGL forward compatibility to true, 
    // which means application uses only the modern features of OpenGL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS
    
    // creating window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // window -> context
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    /*  -----   GLobal Opengl  -----   */
    GLCall(glEnable(GL_DEPTH_TEST));

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // GLCall(glEnable(GL_FRAMEBUFFER_SRGB)); // gamma correction

    /* -----   Stencil Buffer for outlining -----  
    GLCall(glEnable(GL_STENCIL_TEST));
    GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
    GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
    */

/*  -----   -----   -----   -----   */

    // HDR Framebuffer
    HDRFrameBuffer hdrFrameBuffer(SCR_WIDTH, SCR_HEIGHT);
    PingpongFrameBuffer pingpongFrameBuffer(SCR_WIDTH, SCR_HEIGHT);
	
	// Depth FrameBUffer for shadow
	DepthMapFrameBuffer depthFrameBuffer(SHADOW_WIDTH, SHADOW_HEIGHT);

/*  -----   Shader  -----   */
    Shader planetShader("shader/PlanetShader.shader");
    Shader starShader("shader/StarShader.shader");
    Shader hdrShader("shader/HDRShader.shader");
    Shader blurShader("shader/BlurShader.shader");
	Shader depthShader("shader/DepthShader.shader");
	Shader circleShader("shader/CircleShader.shader");
	Shader particleShader("shader/ParticleShader.shader");
/*  -----   -----  -----   */

	/* Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplOpenGL3_Init("#version 410");

    // Setup Style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    bool show_demo_window = false;
	*/


/*          ****    ****    ****        */
/*  ----- ----- --- model --- ----- --- */
/*          ****    ****    ****        */
	
	float earth_radius = -50.0f;
	float moon_radius = 3.0f;
	
	glm::vec3 sun_position = glm::vec3(0.0f);
	glm::vec3 earth_position = glm::vec3(earth_radius, 0.0f, 0.0f);
	glm::vec3 moon_position = glm::vec3(earth_radius - moon_radius, 0.0f, 0.0f);
	
	// scale factors
	float sun_scale = 2.2f;
	float earth_scale = 0.05f;
	float moon_scale = 0.03f;
	
	// rotate factors
	float sun_rotate_speed_factor = 0.001f;
	float earth_rotate_speed_factor = 15.0f;
	float earth_orbit_speed_factor = 10 / 36.5f; // 1 / 365
	float moon_rotate_orbit_speed_factor = 10 / 2.73f; // 1 / 27.3f
	float earth_cloud_rotate_speed_factor = 5.0f;
	
	
    // model
	SceneObject Sun("resources/models/sun/sun.obj", sun_position, sun_scale);
	SceneObject Earth("resources/models/earth/earth.obj", earth_position, earth_scale);
	SceneObject Moon("resources/models/moon/moon.obj", moon_position, moon_scale);
	SceneObject EarthCloud("resources/models/earth/earth_cloud.obj", earth_position, earth_scale);
	
	// model transform - orbit
	Sun.transform->UpdateOrbit(sun_position, 0.0f);
	Earth.transform->UpdateOrbit(sun_position, earth_radius);
	Moon.transform->UpdateOrbit(earth_position, moon_radius);
	
	// set cloud transform according to earth transform
	EarthCloud.transform = Earth.transform->clone();
	EarthCloud.transform->UpdateScale(earth_scale + earth_scale * 0.01f);
	
	// Circle to represent orbit
	Circle CircleEarth(CIRCLE_PTS, glm::vec3(0.0f), earth_radius);
	Circle CircleMoon(CIRCLE_PTS, earth_position, moon_radius);
	
	CircleEarth.transform->UpdateOrbit(sun_position, 0.0f);
	CircleMoon.transform->UpdateOrbit(earth_position, 0.0f);

	
/*  -----   define light uniform   -----   */
    glm::vec3 sun_color = glm::vec3(1.0f, 1.0f, 1.0f);
    DirectionalLight sunLight(sun_position, sun_color);

/*  -----   -------   -----   */
	

    /* --- Camera --- */
	camera = Camera(glm::vec3(0.0f, 5.0f, 0.0f));
	/* --- --- --- */
	
	/* preset all texture index */
    blurShader.Use();
    blurShader.setInt("image", 0);
	
    hdrShader.Use();
    hdrFrameBuffer.SetBufferToTexture(&hdrShader, 0);
    pingpongFrameBuffer.SetBufferToTexture(&hdrShader, 1);
	
	planetShader.Use();
	// diffuse 0, specular 1, emission 2
	depthFrameBuffer.SetBufferToTexture(&planetShader, 3);
	/* --- --- --- */
	
	ParticleManager sunParticle(PARTICLE_NUM, sun_position, 22.0f, "res/sprite/sun_sprite.png");
	
	Focus_Objects focus_flag = Focus_Objects::EARTH;
	
/*          ****    ****    ****        */
/*       -----   Render Loop  -----     */
    Renderer renderer;
    while(!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		
		sunParticle.Update(deltaTime);

        // input
        processInput(window, camera, focus_flag);
		camera.update(deltaTime);
		
		glm::vec3 targetPlanet = earth_position;
		if (focus_flag == Focus_Objects::EARTH) {
			targetPlanet = earth_position;
		}
		if (focus_flag == Focus_Objects::MOON) {
			targetPlanet = moon_position;
		}
		if (focus_flag == Focus_Objects::SUN) {
			targetPlanet = sun_position;
		}

		camera.focusOn(targetPlanet);
		
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.f);
		glm::mat4 lightSpaceMat = GetLightSpaceMatrix(sun_position, earth_position, 0.01f, 50.0f);

		/* --- Transform --- */
		float time = glfwGetTime() * 0.1f;
		float sun_rotate_degree = time * sun_rotate_speed_factor;
		float earth_rotate_degree = -170.0f + time * earth_rotate_speed_factor; // -170
		float earth_orbit_degree = 25.0f + time * earth_orbit_speed_factor;
		float moon_rotate_degree = -90.0f + time *  moon_rotate_orbit_speed_factor;
		float moon_orbit_degree = 140.0f + time *  moon_rotate_orbit_speed_factor; // 30.0f
		float moon_orbit_incline_degree = std::abs(5.0f * glm::sin(glm::radians(moon_orbit_degree)));
		float earth_cloud_rotate_degree = 20.0f - time * earth_cloud_rotate_speed_factor;
		
        Sun.transform->UpdateRotation(glm::vec3(0.0f, sun_rotate_degree, 0.0f));
		
		Earth.transform->UpdateRotation(glm::vec3(23.5f, earth_rotate_degree, 23.5f));
		Earth.transform->UpdateOrbition(glm::vec3(0.0f, earth_orbit_degree, 0.0f));
		earth_position = Earth.transform->GetPosition();
		
		EarthCloud.transform = Earth.transform->clone();
		EarthCloud.transform->UpdateRotation(glm::vec3(23.5f, earth_cloud_rotate_degree, 0.0f));
		
		Moon.transform->UpdateRotation(glm::vec3(0.0f, moon_rotate_degree, 0.0f));
		Moon.transform->UpdateOrbition(glm::vec3(moon_orbit_incline_degree, moon_orbit_degree, 0.0f));
		Moon.transform->UpdateOrbitCenter(earth_position);
		moon_position = Moon.transform->GetPosition();
		
		CircleMoon.transform->UpdateOrbition(glm::vec3(2.0f, 0.0f, 0.0f));
		CircleMoon.transform->UpdateOrbitCenter(earth_position);
		/* --- --- --- */

        renderer.Clear();
        /* --- Depth Frame Buffer for shadow mapping --- */
		GLCall(glViewport(0, 0, SHADOW_WIDTH * 2, SHADOW_HEIGHT * 2));
		
		depthFrameBuffer.Bind();
		depthFrameBuffer.CullFrontFace();
		depthFrameBuffer.Clear();
		
			depthShader.Use();
			depthShader.setMat4("lightSpaceMatrix", lightSpaceMat);
			Earth.Render(&depthShader);
			Moon.Render(&depthShader);
		
		depthFrameBuffer.CullBackFace();
		depthFrameBuffer.Unbind();
		
		
		depthFrameBuffer.ActiveTexture(3);
		depthFrameBuffer.BindTexture();
		/* --- --- --- */
		
		/* --- HDR Frame Buffer normally render --- */
		GLCall(glViewport(0, 0, SCR_WIDTH * 2, SCR_HEIGHT * 2));
		hdrFrameBuffer.Bind();
        hdrFrameBuffer.Clear();

            // the Earth
            sunLight.updatgeTarget(earth_position);
            sunLight.SetupShader(&planetShader);
		
            planetShader.Use();
			planetShader.setMat4("lightSpaceMatrix", lightSpaceMat);
		
            planetShader.setMat4("projection", projection);
            planetShader.setMat4("view", view);
            planetShader.setVec3("viewPos", camera.Position);
            planetShader.UnUse();

            Earth.Render(&planetShader);
			EarthCloud.Render(&planetShader);
		

            // the Moon
            sunLight.updatgeTarget(moon_position);
            sunLight.SetupShader(&planetShader);

            planetShader.Use();
			planetShader.setMat4("lightSpaceMatrix", lightSpaceMat);
            planetShader.UnUse();

            Moon.Render(&planetShader);

            // the Sun
            starShader.Use();
            starShader.setMat4("projection", projection);
            starShader.setMat4("view", view);
            starShader.UnUse();

            Sun.Render(&starShader);
		
			// the Sun Particle
			particleShader.Use();
			particleShader.setMat4("projection", projection);
			particleShader.setMat4("view", view);
			particleShader.setVec3("cameraUp", camera.Up);
			particleShader.setVec3("cameraRight", camera.Right);
			sunParticle.Render(&particleShader);
		
			// circle
			circleShader.Use();
			circleShader.setMat4("projection", projection);
			circleShader.setMat4("view", view);
			circleShader.UnUse();
		
			CircleEarth.Render(&circleShader);
			CircleMoon.Render(&circleShader);

        hdrFrameBuffer.Unbind();
		/* --- --- --- */
		
        /* --- Blur --- */
        bool horizontal = true, first_iteration = true;
        unsigned int rounds = 16;
        
        for (int i = 0; i < rounds; i ++) {
            pingpongFrameBuffer.Bind(horizontal);
            blurShader.Use();
            blurShader.setInt("horizontal", horizontal);
            if (first_iteration) {
                hdrFrameBuffer.BindTexture(1); // use brightness texture
            } else {
                pingpongFrameBuffer.BindTexture(!horizontal);
            }
            hdrFrameBuffer.RenderBufferToScreen(&blurShader);
            horizontal = !horizontal;
            if (first_iteration) {
                first_iteration = false;
            }
        }
        pingpongFrameBuffer.Unbind();
		/* --- --- --- */

        /* --- render from texture to screen quad --- */
        hdrFrameBuffer.Clear();
        hdrShader.Use();
        // FrameBuffer Texture
		hdrFrameBuffer.ActiveTexture(0);
        hdrFrameBuffer.BindTexture(0);
        pingpongFrameBuffer.ActiveTexture(1);
        pingpongFrameBuffer.BindTexture(!horizontal);
		
		hdrFrameBuffer.SetupShader(&hdrShader, 1.0f);
		hdrFrameBuffer.RenderBufferToScreen(&hdrShader);
		/* --- --- --- */
		
        // poll IO events
        glfwPollEvents();

        // swap buffers ( from back to front screen)
        glfwSwapBuffers(window);

    }
/*  -----   -----   -----   -----   */
    
	/* Imgui end
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
	 */

}

// handling user input for a given window 
// query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, Camera& camera, Focus_Objects& flag) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, deltaTime);
    }
	
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.Position = glm::vec3(0.0f, 5.0f, 0.0f);
		camera.focusDistance = 1.0f;
		flag = Focus_Objects::EARTH;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.Position = glm::vec3(-60.0f, 15.0f, 0.0f);
		camera.focusDistance = 2.0f;
		flag = Focus_Objects::EARTH;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.Position = glm::vec3(-10.0f, 5.0f, 0.0f);
		camera.focusDistance = 0.7f;
		flag = Focus_Objects::MOON;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		camera.Position = glm::vec3(-100.0f, 1.5f, 0.0f);
		flag = Focus_Objects::EARTH;
		camera.focusDistance = 60.0f;
	}
	camera.targetFocusDistance = camera.focusDistance;
}

// this callback function executes whenever the window size changed 
//  adjust the OpenGL viewport to match the new window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

glm::mat4 GetLightSpaceMatrix(glm::vec3 position, glm::vec3 lookat, float near, float far) {
	glm::mat4 lightProjection, lightView;
	lightProjection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, near, far);
	lightView = glm::lookAt(position + lookat * 0.90f, lookat, glm::vec3(0.0, 1.0, 0.0));
	return lightProjection * lightView;
}
