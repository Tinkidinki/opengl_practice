// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "shader.hpp"

#define PI 3.14159265358979323846

typedef struct {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    }point;

point rotate_by(double angle, point inp_point){
    point new_point;
    new_point.x = inp_point.x*cos(angle) - inp_point.y*sin(angle);
    new_point.y = inp_point.x*sin(angle) + inp_point.y*cos(angle);
    return new_point;
}

void fill_array(int sides, GLfloat array[]){
    double angle = 2 * PI / sides;
    
    point vertex;
    vertex.x = 0.0f;
    vertex.y = 0.0f;
    vertex.z = 0.0f;

    point oldpoint;
    oldpoint.x = 0.0f;
    oldpoint.y = 0.5f;
    oldpoint.z = 0.0f;

    //For each triangle
    for(int i = 0; i < sides; i++){
        // setting the vertex
        array[9*i + 0] = vertex.x;
        array[9*i + 1] = vertex.y;
        array[9*i + 2] = vertex.z;

        // setting the oldpoint
        array[9*i + 3] = oldpoint.x;
        array[9*i + 4] = oldpoint.y;
        array[9*i + 5] = oldpoint.z;

        // making the new point
        point newpoint = rotate_by(angle, oldpoint);

        // setting the new point
        array[9*i + 6] = newpoint.x;
        array[9*i + 7] = newpoint.y;
        array[9*i + 8] = newpoint.z;

        // New point is now old
        oldpoint = newpoint;
    }

}


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 768, 768, "Tutorial 02 - Red triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

    static GLfloat g_vertex_buffer_data[500];
    //test_fill_array(g_vertex_buffer_data);
    int sides = 50; // CHANGE THIS FOR ANY NUMBER OF SIDES
    // The circle is not full for big numbers
    fill_array(sides, g_vertex_buffer_data);


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, sides*3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

