#include "Scene2.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"	

#include "shader.hpp"


Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	//Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID,
		"MVP");
	// Use our shader
	glUseProgram(m_programID);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Init Real-time transformation
	rotateAngle = 0;
	translateX = 0;
	scaleAll = 0;

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	// Triangle 1
	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	// An array of 3 vectors which represents 3 vertices
	static const GLfloat vertex_buffer_data1[] = { 
	-1.0f, 0.0f, 0.0f, //vertex 0 of triangle
	1.0f, 0.0f, 0.0f, //vertex 1 of triangle
	0.0f, 1.0f, 0.0f, //vertex 2 of triangle
	};
	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	// Transfer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data1), vertex_buffer_data1, GL_STATIC_DRAW);
	
	// An array of 3 vectors which represents the colors of the 3 vertices
		static const GLfloat color_buffer_data1[] = {
		1.0f, 0.0f, 0.0f, //color of vertex 0
		0.0f, 1.0f, 0.0f, //color of vertex 1
		0.0f, 0.0f, 1.0f, //color of vertex 2
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data1), color_buffer_data1, GL_STATIC_DRAW);

	// Triangle 2
	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[1]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[1]);
	// An array of 3 vectors which represents 3 vertices
	static const GLfloat vertex_buffer_data2[] = {
	-1.0f, 0.0f, 0.0f, //vertex 0 of triangle
	1.0f, 0.0f, 0.0f, //vertex 1 of triangle 
	0.0f, -1.0f, 0.0f, //vertex 2 of triangle 
	};
	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	// Transfer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data2), vertex_buffer_data2, GL_STATIC_DRAW);

	// An array of 3 vectors which represents the colors of the 3 vertices
	static const GLfloat color_buffer_data2[] = {
	1.0f, 0.0f, 0.0f, //color of vertex 0
	0.0f, 1.0f, 0.0f, //color of vertex 1
	0.0f, 0.0f, 1.0f, //color of vertex 2
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data2),
		color_buffer_data2, GL_STATIC_DRAW);


}

int i = 69;
void Scene2::Update(double dt)
{
	if (Application::IsKeyPressed(VK_SPACE))
	{
		rotateAngle += (float)(i * dt);

		translateX += (float)(20 * dt);
		if (translateX > 10) { translateX = -10; }

		scaleAll += (float)(3 * dt);
		if (scaleAll > 2) { scaleAll = 0; }
	}
	if (rotateAngle > 90) { i = -69; }
	if (rotateAngle < -90) { i = 69; }
}

void Scene2::Render()
{
	// Render VBO here
	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity(); //no need camera for now, set it at World's origin
	projection.SetToOrtho(-10, +10, -10, +10, -10, +10); //Our world is a cube defined by these boundaries

	scale.SetToScale(2, 2, 2);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(0, 0, 0);
	model = translate * rotate * scale; //scale, followed by rotate, then lastly translate
		MVP = projection * view * model; // Remember, matrix multiplication is the other way around
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]); //update the shader with new MVP
	

	// Triangle 1
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(
		0, // attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		0 // array buffer offset
	);
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices = 1 triangle
	
	// Triangle 2
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(
		0, // attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		0 // array buffer offset
	);
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices = 1 triangle
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	
		translate.SetToIdentity();
		rotate.SetToIdentity();
		scale.SetToIdentity();
		model.SetToIdentity();
		view.SetToIdentity(); //no need camera for now, set it at World's origin
		projection.SetToOrtho(-10, +10, -10, +10, -10, +10); //Our world is a cube defined by these boundaries

		scale.SetToScale(1, 1, 1);
		rotate.SetToRotation(45, 0, 0, 1);
		translate.SetToTranslation(translateX, 5, 0);
		model = translate * rotate * scale; //scale, followed by rotate, then lastly translate
		MVP = projection * view * model; // Remember, matrix multiplication is the other way around
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]); //update the shader with new MVP


	// Triangle 1
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(
		0, // attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		0 // array buffer offset
	);
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices = 1 triangle

	// Triangle 2
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(
		0, // attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		0 // array buffer offset
	);
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices = 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity(); //no need camera for now, set it at World's origin
	projection.SetToOrtho(-10, +10, -10, +10, -10, +10); //Our world is a cube defined by these boundaries

	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-5, -5, 0);
	model = translate * rotate * scale; //scale, followed by rotate, then lastly translate
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP


	// Triangle 1
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(
		0, // attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		0 // array buffer offset
	);
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices = 1 triangle

	// Triangle 2
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(
		0, // attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		0 // array buffer offset
	);
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices = 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene2::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[1]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[1]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteVertexArrays(2, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}
