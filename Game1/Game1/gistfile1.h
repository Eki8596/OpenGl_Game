#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cube.h"
#include "textured_cube.h"
#include "Shader.h"
#include "Camera.h"
#include "Player.h"
#include "Guy.h"
#include "stb_image.h"
#include "Wall.h"
#include <vector>
#include <iostream>
#include <list>
#include "mapdata.h"
#define _USE_MATH_DEFINES
#include <math.h>
#ifndef GISTFILE1_H
#define  GISTFILE1_H
struct vertexStruct
{
	float Normal[3];
	float Texcoord[2];
	float Vertex[3];


};
struct vao
{
	unsigned int VAO;
	float size;

};

class gistfile1 {
public:
	std::vector <vao> SVAO;
	//unsigned int  sphereVertexArrayObject;
	//unsigned int sphereVertexBuffers[2];
	float res1 = 0, res2 = 0;
	void setupSphere() {
		std::vector<vertexStruct> verticesVector;
		std::vector<int> indicesVector;
		double latitudeBands = 30;
		double longitudeBands = 30;
		double radius = 2;
		unsigned int  sphereVertexArrayObject;
		unsigned int sphereVertexBuffers[2];
		for (double latNumber = 0; latNumber <= latitudeBands; latNumber++) {
			double theta = latNumber * M_PI / latitudeBands;
			double sinTheta = sin(theta);
			double cosTheta = cos(theta);

			for (double longNumber = 0; longNumber <= longitudeBands; longNumber++) {
				double phi = longNumber * 2 * M_PI / longitudeBands;
				double sinPhi = sin(phi);
				double cosPhi = cos(phi);

				vertexStruct vs;
				vs.Normal[0] = cosPhi * sinTheta;   // x
				vs.Normal[1] = cosTheta;            // y
				vs.Normal[2] = sinPhi * sinTheta;   // z
				vs.Texcoord[0] = 1 - (longNumber / longitudeBands); // u
				vs.Texcoord[1] = 1 - (latNumber / latitudeBands);   // v
				vs.Vertex[0] = radius * vs.Normal[0];
				vs.Vertex[1] = radius * vs.Normal[1];
				vs.Vertex[2] = radius * vs.Normal[2];

				verticesVector.push_back(vs);
			}

			for (int latNumber = 0; latNumber < latitudeBands; latNumber++) {
				for (int longNumber = 0; longNumber < longitudeBands; longNumber++) {
					int first = (latNumber * (longitudeBands + 1)) + longNumber;
					int second = first + longitudeBands + 1;

					indicesVector.push_back(first);
					indicesVector.push_back(second);
					indicesVector.push_back(first + 1);

					indicesVector.push_back(second);
					indicesVector.push_back(second + 1);
					indicesVector.push_back(first + 1);

				}
			}
		}
			int verticesCount = verticesVector.size();
			vertexStruct * vertices = new vertexStruct[verticesCount];
			int indicesCount = indicesVector.size();
			int * indices = new int[indicesCount];


			copy(verticesVector.begin(), verticesVector.end(), vertices);
			copy(indicesVector.begin(), indicesVector.end(), indices);

		

			// compass
			glGenVertexArrays(1, &sphereVertexArrayObject);
			glBindVertexArray(sphereVertexArrayObject);

			glGenBuffers(2, &sphereVertexBuffers[0]);
			glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBuffers[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), (GLvoid*)offsetof(vertexStruct, Texcoord));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), (GLvoid*)offsetof(vertexStruct, Normal));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), (GLvoid*)offsetof(vertexStruct, Vertex));
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereVertexBuffers[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

			res1 += verticesVector.size();
			res2 += indicesVector.size();

			//std::cout << res1 + res2 << std::endl;
			//std::cout << sizeof(sphereVertexArrayObject) << std::endl;
			vao push;
			push.size = res1 + res2;
			push.VAO = sphereVertexArrayObject;
			SVAO.push_back(push);
		

 	}
	void printSphere(GLFWwindow * window)
	{
		//std::cout << sizeof(sphereVertexArrayObject) << std::endl;

		for (int i = 0; i < SVAO.size(); i++)
		{
			glBindVertexArray(SVAO[i].VAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,30);

		}
	}
};
#endif