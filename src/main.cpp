#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image/stb_image.h"

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "GLDebug.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Light.hpp"
#include "Transform.hpp"
#include "OutlineRenderer.hpp"
#include "PickingTexture.hpp"
#include "FrameBuffers.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <memory>

void processInput(GLFWwindow *window, Camera& camera); 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool firstMouse = true;

float lastX = 400.0f, lastY = 300.0f;

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
    glfwSetCursorPosCallback(window, mouse_callback);

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

/*  -----   Shader  -----   */
    Shader planetShader("res/shaders/PlanetShader.shader");
    Shader starShader("res/shaders/StarShader.shader");
    Shader hdrShader("res/shaders/HDRShader.shader");
    Shader blurShader("res/shaders/BlurShader.shader");
/*  -----   -----  -----   */

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplOpenGL3_Init("#version 410");

    // Setup Style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    bool show_demo_window = false;


/*          ****    ****    ****        */
/*  ----- vertices indices textures --- */
/*          ****    ****    ****        */

    // model
    Model earthModel("res/models/earth/earth.obj");
    Model sunModel("res/models/sun/sun.obj");


/*  -----   define light uniform   -----   */
    glm::vec3 sun_position = glm::vec3( 0.0f,  0.0f,  0.0f);
    glm::vec3 sun_color = glm::vec3(1.0f, 1.0f, 1.0f);
    DirectionalLight sunLight(sun_position, sun_color);


/*  -----   -------   -----   */

    // Camera
    camera = Camera(glm::vec3(2.0f, 10.0f, 10.0f));

    float scaleFactor = 0.25f;

    Transform earthTrans(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    Transform sunTrans(sun_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));

    blurShader.Use();
    blurShader.setInt("image", 0);
    hdrShader.Use();
    hdrFrameBuffer.SetBufferToTexture(&hdrShader, 0);
    pingpongFrameBuffer.SetBufferToTexture(&hdrShader, 1);

/*          ****    ****    ****        */
/*       -----   Render Loop  -----     */
    Renderer renderer;
    while(!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window, camera);

        earthTrans.SetRotation(glm::vec3(0.0f, -30.0f + glfwGetTime() * 2.0f, 23.5f));
        sunTrans.SetRotation(glm::vec3(0.0f, glfwGetTime() * 0.1f, 0.0f));

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.f);
        glm::mat4 earth_model = earthTrans.GetModelMatrix();
        glm::mat4 sun_model = sunTrans.GetModelMatrix();

        glm::vec3 earth_location = glm::vec3(earth_model[3]);
        sunLight.updatgeTarget(earth_location);

        renderer.Clear();
        hdrFrameBuffer.Bind();
        hdrFrameBuffer.Clear();
            planetShader.Use();
            planetShader.setMat4("model", earth_model);
            planetShader.setMat4("projection", projection);
            planetShader.setMat4("view", view);
            planetShader.setVec3("viewPos", camera.Position);
            planetShader.UnUse();
            
            sunLight.SetupShader(&planetShader); 
            
            starShader.Use();
            starShader.setMat4("model", sun_model);
            starShader.setMat4("projection", projection);
            starShader.setMat4("view", view);
            starShader.UnUse();

            earthModel.Render(&planetShader);
            sunModel.Render(&starShader);
        hdrFrameBuffer.Unbind();

        // blur
        bool horizontal = true, first_iteration = true;
        unsigned int rounds = 10;
        
        for (int i = 0; i < rounds; i ++) {
            blurShader.Use();
            pingpongFrameBuffer.Bind(horizontal);
            blurShader.setInt("horizontal", horizontal);
            if (first_iteration) {
                hdrFrameBuffer.BindTexture();
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

        // render to default framebuffer
        hdrFrameBuffer.Clear();
        hdrShader.Use();
        hdrFrameBuffer.ActiveTexture(0);
        hdrFrameBuffer.BindTexture(0);
        pingpongFrameBuffer.ActiveTexture(1);
        pingpongFrameBuffer.BindTexture(!horizontal);
        hdrFrameBuffer.SetupShader(&hdrShader, 1.0f);
        hdrFrameBuffer.RenderBufferToScreen(&hdrShader);

        // poll IO events
        glfwPollEvents();

        // swap buffers ( from back to front screen)
        glfwSwapBuffers(window);

    }
/*  -----   -----   -----   -----   */
    
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;

}

// handling user input for a given window 
// query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, Camera& camera) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, deltaTime);
    }
}

// this callback function executes whenever the window size changed 
//  adjust the OpenGL viewport to match the new window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    camera.processMouse(xoffset, yoffset);
}