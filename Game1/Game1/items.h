#pragma once
#ifndef ITEMS_H
#define ITEMS_H
#include <assert.h>

namespace items {
	const float enemyPos[] = {
		-13.0f, -3.0f,-3.0f,
		13.0f,-3.0f, -21.0f,
		15.0f,-3.0f, -7.0f,
		15.0f,-3.0f, -33.0f,
		27.0f, -3.0f, -49.0f,
		-3.0f,-3.0f,-55.0f,
		11.0f,-3.0f,-47.0f,
		15.0f,-3.0f,-52.0f,
		-7.0f,-3.0f,-50.0f
	};
	const float  enemyLook[] = {
		0.0f, 0.0f, -1.0f,
		-1.0f,0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

	};
	const float codexPos[] =
	{
		-13.0f, -3.0f, 1.0f,
		21.0f,-3.0f, -13.0f,
		27.0f,-3.0f,-31.0f,
		13.0f,-3.0f,-47.0f,
		-5.0f,-3.0f,-47.0f,
		-5.0f,-3.0f,-55.0f,
		13.0f,-3.0f,-55.0f
	};
	const int enemyType[] = { 1,2,1,1,1,1,1,2,2};
	const float enemysize = sizeof(enemyPos) / sizeof(float);
	const float codexsize = sizeof(codexPos) / sizeof(float);


}
#endif
