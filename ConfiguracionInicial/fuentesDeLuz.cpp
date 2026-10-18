//Practica 9
//Sánchez Luján César Ricardo
//Fecha de entrega: 18 de octubre del 2026
//No. Cuenta: 321195414

// Std. Includes
#include <string>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 3.6f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -12.0f);
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 solPos(0.0f, 0.0f, 0.0f);
glm::vec3 lunaPos(0.0f, 0.0f, 0.0f);
float radio = 7.2f;
// Zona para ver el partido: banco a la izquierda y pantalla a la derecha.
glm::vec3 bancoPos(-3.5f, -1.005681f, -1.2f);
glm::vec3 pantallaPos(4.2f, -1.005681f, -2.0f);
float giroPantalla = -55.0f;
// Lampara de calle: base sobre el piso y foco bajo la pantalla.
glm::vec3 lamparaPos(-4.0f, -1.005681f, -3.2f);
// Centro de la parte emisiva del OBJ, con la misma escala del modelo.
// Se baja ligeramente para que la luz salga debajo del foco.
glm::vec3 focoLamparaPos = lamparaPos + glm::vec3(-0.000279f, 2.594924f, 0.162006f) * 1.2f;
// Antorcha: luz puntual en la punta del modelo.
glm::vec3 antorchaPos(4.4f, -1.005681f, 5.2f);
glm::vec3 luzAntorchaPos = antorchaPos + glm::vec3(0.0f, 0.99f, 0.0f);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 60.0f;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9 Cesar Sanchez", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model escenario((char*)"Models/EscenarioP8.obj");
    Model sol((char*)"Models/Sol.obj");
    Model luna((char*)"Models/Luna.obj");
    Model lampara((char*)"Models/lamparaCalle.obj");
    Model antorcha((char*)"Models/antorchaMinecraft.obj");
    Model banco((char*)"Models/bancoParque.obj");
    Model pantalla((char*)"Models/pantallaPartido.obj");
    Model imagenPartido((char*)"Models/imagenPartido.obj");
    GLuint emisionLampara = TextureFromFile("Street_lamp_Emissive.png", "Models");
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Orbita alrededor del centro del piso, que se dibuja con escala 3.
        solPos = glm::vec3(0.28f + radio * cos(glm::radians(rot)),
            -1.005681f + radio * sin(glm::radians(rot)), -0.54f);
        lunaPos = glm::vec3(0.28f - radio * cos(glm::radians(rot)),
            -1.005681f - radio * sin(glm::radians(rot)), -0.54f);

        lightingShader.Use();

        // Posicion de la camara.
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z);

        // Direcciones desde los astros hacia el centro del escenario.
        glm::vec3 direccionSol = glm::normalize(
            glm::vec3(0.28f, -1.005681f, -0.54f) - solPos);

        glm::vec3 direccionLuna = glm::normalize(
            glm::vec3(0.28f, -1.005681f, -0.54f) - lunaPos);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"),
            direccionSol.x, direccionSol.y, direccionSol.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLightLuna.direction"),
            direccionLuna.x, direccionLuna.y, direccionLuna.z);

        // Apagar ambas luces antes de elegir cual ilumina.
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),
            0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),
            0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),
            0.0f, 0.0f, 0.0f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLightLuna.ambient"),
            0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLightLuna.diffuse"),
            0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLightLuna.specular"),
            0.0f, 0.0f, 0.0f);

        // Encender solamente el astro que esta arriba del piso.
        if (solPos.y >= -1.005681f)
        {
            // Sol: luz calida.
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),
                0.22f, 0.17f, 0.10f);

            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),
                1.0f, 0.78f, 0.48f);

            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),
                0.45f, 0.32f, 0.18f);
        }
        else
        {
            // Luna: luz fria y tenue.
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLightLuna.ambient"),
                0.10f, 0.13f, 0.20f);

            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLightLuna.diffuse"),
                0.30f, 0.42f, 0.60f);

            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLightLuna.specular"),
                0.03f, 0.05f, 0.08f);
        }

        // Reflector de la camara: linterna del usuario.
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x,
            camera.GetFront().y,
            camera.GetFront().z);

        // Color e intensidad.
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"),
            0.0f, 0.0f, 0.0f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"),
            2.2f, 2.2f, 2.2f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),
            0.7f, 0.7f, 0.7f);

        // Atenuacion: pierde intensidad con la distancia.
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"),
            1.0f);

        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"),
            0.014f);

        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"),
            0.0007f);

        // Apertura del cono y borde suave.
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
            cos(glm::radians(7.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
            cos(glm::radians(10.0f)));

        // Reflector fijo de la lampara de calle.
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.position"),
            focoLamparaPos.x, focoLamparaPos.y, focoLamparaPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.direction"),
            0.0f, -1.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.ambient"),
            0.015f, 0.012f, 0.006f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.diffuse"),
            1.20f, 0.90f, 0.50f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.specular"),
            0.35f, 0.27f, 0.15f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.cutOff"),
            cos(glm::radians(18.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLightLampara.outerCutOff"),
            cos(glm::radians(26.0f)));

        // Quinta fuente: luz puntual calida de la antorcha.
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            luzAntorchaPos.x, luzAntorchaPos.y, luzAntorchaPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
            0.05f, 0.022f, 0.005f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            2.10f, 0.98f, 0.25f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
            0.60f, 0.29f, 0.08f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.07f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.022f);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 0);

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"),
            16.0f);

        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "esLampara"), 0);
        escenario.Draw(lightingShader);

        // Banco de madera iluminado por las fuentes de la escena.
        model = glm::mat4(1.0f);
        model = glm::translate(model, bancoPos);
        model = glm::rotate(model, glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        banco.Draw(lightingShader);

        // Estructura y soporte de la pantalla orientada hacia el perro.
        model = glm::mat4(1.0f);
        model = glm::translate(model, pantallaPos);
        model = glm::rotate(model, glm::radians(giroPantalla), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pantalla.Draw(lightingShader);

        // Dibujar la lampara con su base apoyada en el piso.
        model = glm::mat4(1.0f);
        model = glm::translate(model, lamparaPos);
        model = glm::scale(model, glm::vec3(1.2f));
        model = glm::translate(model, glm::vec3(-1.948107f, -0.060057f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, emisionLampara);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "texturaEmisionLampara"), 7);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "esLampara"), 1);
        lampara.Draw(lightingShader);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "esLampara"), 0);
        glActiveTexture(GL_TEXTURE0);

        // Dibujar Sol y Luna con sus texturas, como en Carga de Modelos.
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //escenario.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, solPos);
        model = glm::scale(model, glm::vec3(4.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        sol.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lunaPos);
        model = glm::scale(model, glm::vec3(3.8f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        luna.Draw(shader);

        // Antorcha con su textura, visible como fuente encendida.
        model = glm::mat4(1.0f);
        model = glm::translate(model, antorchaPos);
        model = glm::scale(model, glm::vec3(1.6f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        antorcha.Draw(shader);

        // Imagen estatica del partido, visible como una pantalla encendida.
        model = glm::mat4(1.0f);
        model = glm::translate(model, pantallaPos);
        model = glm::rotate(model, glm::radians(giroPantalla), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        imagenPartido.Draw(shader);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (keys[GLFW_KEY_X])
    {
        rot += 40.0f * deltaTime;
    }
    if (keys[GLFW_KEY_Z])
    {
        rot -= 40.0f * deltaTime;
    }
    if (rot >= 360.0f) rot -= 360.0f;
    if (rot < 0.0f) rot += 360.0f;

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }


}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

