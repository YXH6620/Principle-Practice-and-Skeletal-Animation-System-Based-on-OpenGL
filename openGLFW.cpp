#incl ude "Base.h"
#include "Shader.h"

//Shader
Shader _shader;
//1）获取VBO index
unsigned int VBO = 0;
//VAO index
unsigned int VAO = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void	initShader(const char* _vertexPath, const char* _fragPath)
{
    _shader.initShader(_vertexPath,_fragPath);
}
void initModel()
{
    float vertices[] = {
    //顶点               //颜色 
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    //创建VAO
    glGenVertexArrays(1, &VAO);
    //绑定VAO，VBO此时属于VAO的管理范围，管理对应的VBO锚定点
    glBindVertexArray(VAO);

    //构建EBO
    unsigned int EBO=0;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //（2）绑定VBO index
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //（3）给VBO分配显存空间 传输数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //（4）告诉shader数据解析方式
    //顶点信息，跨度为6
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //颜色信息，跨度为6
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3));
    //（5）激活锚点
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //解绑VAO
    glBindVertexArray(0); 
}

void rend()
{
    _shader.start();
    glBindVertexArray(VAO);
    //用哪个shader程序
    //glUseProgram(shaderProgram);
    //绘图模式、起始顶点，顶点数
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    _shader.end();
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Core", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    initModel();
    initShader("vertexShader.glsl" , "fragmentShader.glsl");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        rend();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}