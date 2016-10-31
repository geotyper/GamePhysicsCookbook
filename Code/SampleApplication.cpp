#define WIN32_LEAN_AND_MEAN 
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
#include <cstdlib>

#ifdef USE_GLEW
#include <GL\glew.h>
#include <GL\wglew.h>
#endif
#include <gl\GL.h>

#include "SampleApplication.h"
#include"FixedFunctionPrimitives.h"

static SampleApplication debugInstance("Sample Application", 800, 600);


// TODO: REMOVE
mat4 perspView;
#include <iostream>

//Triangle triangle(Point(-1.0f, 5.0f, 0.0f), Point(2.0f, 2.0f, -3.0f), Point(5.0f, 5.0f, 0.0f));
Mesh meshObject;
// END TODO

float SampleApplication::random(float min, float max) {
	// this  function assumes max > min, you may want 
	// more robust error checking for a non-debug build
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = max - min;
	return (random*range) + min;
}

void SampleApplication::OnInitialize() {
	GLWindow::OnInitialize();
	cameraDist = -10.0f;
	matView = LookAt(vec3(cameraPos.x, cameraPos.y, cameraDist), vec3(), vec3(0.0f, 1.0f, 0.0f));
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);

	glPointSize(3.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);
	val[0] = 0.0f; val[1] = 1.0f; val[2] = 0.0f; val[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, val);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, val);
	val[0] = 1.0f; val[2] = 1.0f;
	glLightfv(GL_LIGHT0, GL_SPECULAR, val);

	perspView = GetView();
	LoadMesh("suzane.obj", &meshObject);
	AccelerateMesh(meshObject);
}

void SampleApplication::OnShutdown() {
	FreeMesh(&meshObject);
	FreeBVHNode(meshObject.accelerator);
	delete meshObject.accelerator;
}

void SampleApplication::OnRender() {
	GLWindow::OnRender();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0f, 0.0f, 1.0f);
	Render(meshObject);

	glColor3f(1.0f, 0.0f, 0.0f);
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Render(*meshObject.accelerator);
	glEnable(GL_LIGHTING);

	FixedFunctionOrigin(true, false);
}

void SampleApplication::OnUpdate(float deltaTime) {
	GLWindow::OnUpdate(deltaTime);

	if (KeyDown(KEY_ONE)) {
		cameraDist = -10.0f;
		cameraPos = vec2();
	}
	else if (KeyDown(KEY_TWO)) {
		cameraPos = vec2(14, 14);
		cameraDist = -14.0f;
	}
	else if (KeyDown(KEY_THREE)) {
		float size = 5.5f;
		SetOrtho(-size, size, -size, size, 100, -100);
		//SetOrtho(0, m_nWidth, 0, m_nHeight, 0.01f, 1000.0f);
	}
	else if (KeyDown(KEY_FOUR)) {
		SetPerspective(60.0f, 0.01f, 1000.0f);
	}

	if (MouseButonDown(MOUSE_LEFT)) {
		cameraDist += 3.0f * deltaTime;
	} else if (MouseButonDown(MOUSE_RIGHT)) {
		cameraDist -= 3.0f * deltaTime;
	}

	if (KeyDown(KEY_DOWN_ARROW)) {
		cameraPos.y += -5.0f * deltaTime;
	}
	else if (KeyDown(KEY_UP_ARROW)) {
		cameraPos.y += 5.0f * deltaTime;
	}

	if (KeyDown(KEY_LEFT_ARROW)) {
		cameraPos.x += -5.0f * deltaTime;
	}
	else if (KeyDown(KEY_RIGHT_ARROW)) {
		cameraPos.x += 5.0f * deltaTime;
	}

	matView = LookAt(vec3(cameraPos.x, cameraPos.y, cameraDist), vec3(), vec3(0.0f, 1.0f, 0.0f));
}