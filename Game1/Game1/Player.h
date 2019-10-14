#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "shader.h"


enum Player_Movement {
	FORWARD_P,
	BACKWARD_P,
	LEFT_P,
	RIGHT_P
};
class Player
{
public:
	glm::vec3 Position;
	glm::vec3 FrontGo;
	glm::vec3 PrevFrontGo;
	glm::vec3 FrontLook;
	glm::vec3 Direction;
	glm::vec3 Up;
	glm::vec3 wUp;
	glm::vec3 Right;
	glm::vec3 Size =  glm::vec3(0.5f, 1.5f, 0.5f);
	float MovementSpeed = 3.5f;
	float MouseSpeed = 0.1f;
	float Yaw = -90.0f;
	float prewYaw = -90.0f;
	float Pitch = 0.0f;
	float lastRotationDegree;


	Player(glm::vec3 position)
	{
		Position = position;
		
		setup();
	}
	~Player()
	{

	}
	Player()
	{

	}
	glm::mat4 LookLock()
	{
		//glm::vec3 posdum = Position + FrontLook;
		return glm::lookAt(Position, Position +FrontLook, Up);

	}
	glm::vec3 GetFrontLook()
	{
		return FrontLook;

	}
	glm::vec3 GetFrontGo()
	{
		return FrontGo;

	}
	glm::vec3 GetPrevFrontGo()
	{
		return PrevFrontGo;

	}
	glm::vec3 GetPos()
	{
		return Position;

	}
	float GetMoveSpeed()
	{
		return MovementSpeed;
	}
	glm::vec3 GetRight()
	{
		return Right;
	}
	float GetYaw()
	{
		return Yaw;
	}
	float GetPitch()
	{
		return Pitch;
	}
	float GetLastRotationDegree()
	{
		return lastRotationDegree;
	}
	void ProcessKeyboardP(Player_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD_P)
			Position +=  FrontGo * velocity;
		if (direction == BACKWARD_P)
			Position -= FrontGo * velocity;
		if (direction == LEFT_P)
			Position -= Right * velocity;
		if (direction == RIGHT_P)
			Position += Right * velocity;

	}
	void MouseMovement(float xoffset, float yoffset, GLboolean constrainPitchYaw = true)
	{
		xoffset *= MouseSpeed;
		yoffset *= MouseSpeed;

	
		Pitch += yoffset;
		Yaw += xoffset;
		
		if (constrainPitchYaw)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;

		}
		

	   update();
	}
private:
	
	void setup()
	{
		wUp = glm::vec3(0.0f, 1.0f, 0.0f);
		FrontGo = glm::vec3(0.0f, 0.0f, -1.0f);
		PrevFrontGo = glm::vec3(0.0f, 0.0f, -1.0f);
		FrontLook = glm::vec3(0.0f, 0.0f, -1.0f);
		//Direction = glm::normalize(Position - Target);
		Right = glm::normalize(glm::cross(FrontGo, wUp));
		Up = glm::normalize(glm::cross(Right,FrontGo));
	}
	void update()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = FrontGo.y;
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		FrontGo = glm::normalize(front);
	    glm::vec3 frontl;
		frontl.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		frontl.y = sin(glm::radians(Pitch));
		frontl.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		FrontLook =  glm::normalize(frontl);
		Right = glm::normalize(glm::cross(FrontLook, wUp));
		Up = glm::normalize(glm::cross(Right,FrontLook));
	}
};
#endif
