#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // Same thing as src = &source[0]. Just returns a pointer to the first element of a string to mimic a raw string.
    glShaderSource(id, 1, &src, nullptr); // Specifies the source of the shader.
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); //a C function that let's you dynamically allocate on the stack rather than the heap.
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //provide a string version of a vertexShader and fragmentShader for our program to compile and link together and return a unique identifier

    unsigned int program = glCreateProgram(); //creates a shader program within OpenGL that returns 
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
        -0.5f, -0.5,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer; // variable that will hold the ID of a vertex buffer that we create
    glGenBuffers(1, &buffer); // first parameter determines the amount of buffers created; second parameter passes the pointer where the ID of the buffer created is located
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //Bind in the context of OpenGL means "Select" the buffer.
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //2nd param is size of buffer; 3rd param is the array pointer to the data; 4th param is the type of Draw you want 

    glEnableVertexAttribArray(0); //this enables the vertex attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    //first (index): the location of the attribute within each vertex
    //second (size): the component count of the attribute 
    //third (type): the data type of the attribute
    //fourth (isNormalized): if true openGL normalizes the data in the attribute meaning if you have a value that can scaled from 0-255 openGL normalizes it to 0-1
    //fifth (stride): amount of bytes between each vertex 
    //sixth (pointer): the starting point of the attribute being handled

    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3); //2nd param is starting point. 3rd param is the number of indeces/in this case vertices to be rendered

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}