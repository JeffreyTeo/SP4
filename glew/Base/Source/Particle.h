#ifndef PARTICLE_H
#define PARTICLE_H

#include "SpriteAnimation.h"
#include "MyMath.h"
#include <iostream>
#include <vector>
#include "Vector3.h"

using namespace std;

class Particle
{

public:
	Particle();
	~Particle();
	void ParticleInit(int size, float x, float y,int style);
	void SpritePushBack(SpriteAnimation* Holder, float m_windowheight, float m_windowwidth);
	void Update(double dt);
	void Render();
	int GetSize();
	float GetX();
	float GetY();
	int Getparticlestyle();
	void SetConfettiRightSide(bool confettiRightside);
	vector<SpriteAnimation*> theSpriteHolder;
private:
	bool confettiRightside;
	int size;
	float x;
	float y;
	int particlestyle;
	
};

#endif