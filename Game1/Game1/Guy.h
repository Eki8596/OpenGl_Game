#pragma once
#ifndef GUY_H
#define GUY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "shader.h"
#include "Player.h"
enum WhereToLook {
	X_VECTOR,
	Z_VECTOR,
	X_IN_VECTOR,
	Z_IN_VECTOR,
};
class Guy
{
public:
	glm::vec3 Position;
	bool guyFirst = true;
	float yaw;
	float pitch;
	float rotationSpeed = 0.0f;
	float rotationDegree = 0.0f;

	glm::vec3 frontOld;
	glm::vec3 frontNew;
	glm::vec3 prevFrontGo;
	Guy(glm::vec3 position, Player user)
	{

		Position = position;
		prevFrontGo = user.GetFrontGo();

	}
	~Guy()
	{}
	void updateUser(GLFWwindow* window, Player user, Shader lightingShader,unsigned int texture,float deltaTime)
	{

		rotationSpeed += deltaTime * 100.0f;
		if (rotationSpeed > 360)
		{
			rotationSpeed -= 360;
		}
		glActiveTexture(GL_TEXTURE0);//bullet texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glm::vec3 currentFrontGo = user.GetFrontGo();
		lightingShader.use();
		float distance = 2;
		glm::vec3 ObjPos = user.GetPos()- user.GetFrontGo()*0.3f;
		glm::vec3 CharPos;
		glm::vec3 ObjY = glm::vec3(0.0f, -1.0f, 0.0f);
		ObjPos.y = 0.0f;
		float rotate = user.GetYaw() + 90.0f;
		float look = user.GetPitch();


		glm::mat4 model_head;
		model_head = glm::translate(model_head, ObjPos);
		
		model_head = glm::rotate(model_head, glm::radians(rotate), ObjY);
		
		model_head = glm::translate(model_head, glm::vec3(0.0f, -1.4f, 0.0f));
		model_head = glm::rotate(model_head, glm::radians(look), glm::vec3(1.0f, 0.0f, 0.0f));
		model_head = glm::scale(model_head, glm::vec3(0.6f));
		lightingShader.setMat4("model", model_head);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model_body;
		model_body = glm::translate(model_body, ObjPos);
		model_body = glm::rotate(model_body, glm::radians(rotate), ObjY);

		model_body = glm::translate(model_body, glm::vec3(0.0f, -2.6f, 0.0f));
		model_body = glm::scale(model_body, glm::vec3(0.9f));
		model_body = glm::scale(model_body, glm::vec3(1.0f, 2.0f, 0.5f));
		lightingShader.setMat4("model", model_body);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model_hand_right;
		model_hand_right = glm::translate(model_hand_right, ObjPos);
		model_hand_right = glm::rotate(model_hand_right, glm::radians(rotate), ObjY);
		model_hand_right = glm::translate(model_hand_right, glm::vec3(0.55f, -2.1f, -0.25f));
		model_hand_right = glm::rotate(model_hand_right, glm::radians(look), glm::vec3(1.0, 0.0, 0.0));
		model_hand_right = glm::scale(model_hand_right, glm::vec3(0.2f, 0.2f, 3.0f));
		lightingShader.setMat4("model", model_hand_right);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model_hand_left;
		model_hand_left = glm::translate(model_hand_left, ObjPos);

		model_hand_left = glm::rotate(model_hand_left, glm::radians(rotate), ObjY);
		model_hand_left = glm::translate(model_hand_left, glm::vec3(-0.55f, -2.1f, -0.25f));		
		model_hand_left = glm::rotate(model_hand_left, glm::radians(look), glm::vec3(1.0, 0.0, 0.0));		
		model_hand_left = glm::scale(model_hand_left, glm::vec3(0.2f, 0.2f, 3.0f));
		lightingShader.setMat4("model", model_hand_left);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model_leg0;
		model_leg0 = glm::translate(model_leg0, ObjPos);
		model_leg0 = glm::rotate(model_leg0, glm::radians(rotationSpeed), ObjY);

		model_leg0 = glm::translate(model_leg0, glm::vec3(0.5f, -3.8f, 0.0f));
		model_leg0 = glm::scale(model_leg0, glm::vec3(0.2f, 0.7f, 0.2f));
		lightingShader.setMat4("model", model_leg0);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model_leg1;
		model_leg1 = glm::translate(model_leg1, ObjPos);
		model_leg1 = glm::rotate(model_leg1, glm::radians(rotationSpeed), ObjY);
		model_leg1 = glm::translate(model_leg1, glm::vec3(-0.5f, -3.8f, 0.0f));
		model_leg1 = glm::scale(model_leg1, glm::vec3(0.2f, 0.7f, 0.2f));
		lightingShader.setMat4("model", model_leg1);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model_leg2;
		model_leg2 = glm::translate(model_leg2, ObjPos);
		model_leg2 = glm::rotate(model_leg2, glm::radians(rotationSpeed), ObjY);
		model_leg2 = glm::translate(model_leg2, glm::vec3(0.0f, -3.8f, 0.5f));
		model_leg2 = glm::scale(model_leg2, glm::vec3(0.2f, 0.7f, 0.2f));
		lightingShader.setMat4("model", model_leg2);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model_leg3;
		model_leg3 = glm::translate(model_leg3, ObjPos);
		model_leg3 = glm::rotate(model_leg3, glm::radians(rotationSpeed), ObjY);
		model_leg3 = glm::translate(model_leg3, glm::vec3(0.0f, -3.8f, -0.5f));
		model_leg3 = glm::scale(model_leg3, glm::vec3(0.2f, 0.7f, 0.2f));
		lightingShader.setMat4("model", model_leg3);

		glDrawArrays(GL_TRIANGLES, 0, 36);


		//glm::mat4 model_legdown_right;
		//model_legdown_right = glm::translate(model_legdown_right, ObjPos);
		//model_legdown_right = glm::rotate(model_legdown_right, glm::radians(rotate), ObjY);
		//model_legdown_right = glm::translate(model_legdown_right, glm::vec3(0.3f, -4.5f, 0.0f));
		//model_legdown_right = glm::scale(model_legdown_right, glm::vec3(0.2f, 0.7f, 0.2f));
		//lightingShader.setMat4("model", model_legdown_right);
		//
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//glm::mat4 model_legdown_left;
		//model_legdown_left = glm::translate(model_legdown_left, ObjPos);

		//model_legdown_left = glm::rotate(model_legdown_left, glm::radians(rotate), ObjY);
		//model_legdown_left = glm::translate(model_legdown_left, glm::vec3(-0.3f, -4.5f, 0.0f));
		//model_legdown_left = glm::scale(model_legdown_left, glm::vec3(0.2f, 0.7f, 0.2f));
		//lightingShader.setMat4("model", model_legdown_left);
		//
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//glm::mat4 model_feet_right;
		//model_feet_right = glm::translate(model_feet_right, ObjPos);
		//model_feet_right = glm::rotate(model_feet_right, glm::radians(rotate), ObjY);
		//model_feet_right = glm::translate(model_feet_right, glm::vec3(-0.3f, -4.8f, -0.08f));
		//model_feet_right = glm::scale(model_feet_right, glm::vec3(0.3f, 0.15f, 0.4f));
		//lightingShader.setMat4("model", model_feet_right);
		//
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//glm::mat4 model_feet_left;
		//model_feet_left = glm::translate(model_feet_left, ObjPos);
		//model_feet_left = glm::rotate(model_feet_left, glm::radians(rotate), ObjY);
		//model_feet_left = glm::translate(model_feet_left, glm::vec3(0.3f, -4.8f, -0.08f));
		//model_feet_left = glm::scale(model_feet_left, glm::vec3(0.3f, 0.15f, 0.4f));
		//lightingShader.setMat4("model", model_feet_left);
		//
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
	}
	void uptadeRotation(float xoffset,float yoffset,Player user,float deltatime)
	{
		float velocity = rotationSpeed * deltatime;
		xoffset *= rotationSpeed;
		yoffset *= rotationSpeed;
		yaw += xoffset;
		pitch += yoffset;
		rotationDegree += user.GetLastRotationDegree()*velocity;

		//std::cout << rotationDegree << std::endl;
	}
};


#endif