#pragma once
#ifndef WALL_H
#define WALL_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "shader.h"

class Wall
{
public:
	glm::vec3 Position;
	glm::vec3 size;
	glm::mat4 model;
	int id;
	float x, y, z;
	Wall(glm::vec3 position, glm::vec3 scaler,int idGet)
	{

		Position = position;
		model = glm::translate(model, Position);
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 0.2f));
		model = glm::scale(model, scaler);
		id = idGet;
		x = scaler.x / 2;
		z = scaler.z / 2;
		y = scaler.y / 2;
		if (x > z)
		{
			z = 0.3f;
		}
		else
		{
			x = 0.3f;
		}
		size = glm::vec3(x, y, z);
	}
	~Wall()
	{

	}
	glm::mat4 GetModel()
	{

		return model;
	}

};
#endif