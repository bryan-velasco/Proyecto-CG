#ifndef PARTICLES_H
#define PARTICLES_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

struct Particle {
	glm::vec3   p0; //DEFNIMOS EL TIEMPO EN EL QUE VAN A APARECER ESTAS PARTICULAS 
	glm::vec3	position;
	glm::vec3	velocity;
	glm::vec3	acceleration;
	glm::vec3	force;
	float		mass;
};

class Particles
{
public:
	// Material Attributes
	vector<Particle> particles;

	Particles(unsigned int numParticles) {
		// Inicializa la semilla usando el tiempo actual
		srand(time(NULL));

		for (unsigned int i = 0; i < numParticles; i++) {
			Particle P;

			double min_valuex = -8.0;
			double max_valuex = 8.0;
			double rangex = max_valuex - min_valuex;
			double random_valuex = ((double)rand() / RAND_MAX) * rangex + min_valuex;
			P.p0.x = random_valuex;

			double min_valuey = -2.0;
			double max_valuey = 1.6;
			double rangey = max_valuey - min_valuey;
			double random_valuey = ((double)rand() / RAND_MAX) * rangey + min_valuey;
			P.p0.y = random_valuey;


			double min_valuez = -21.73;
			double max_valuez = -5.5;
			double rangez = max_valuez - min_valuez;
			double random_valuez = ((double)rand() / RAND_MAX) * rangez + min_valuez;
			P.p0.z = random_valuez;


			P.position = P.p0;
			P.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			P.acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
			P.force = glm::vec3(0.0f, 0.0f, 0.0f);
			P.mass = 0.001f;

			particles.push_back(P);
		}
	}
	~Particles() {}

	void setGravity(glm::vec3 g) { gravity = g; }
	glm::vec3 getGravity() { return gravity; }

	void UpdatePhysics(float deltaTime) {
		for (int i = 0; i < (int)(particles.size()); i++) {
			Particle* P = &particles.at(i);

			// Aplica una gravedad hacia arriba para simular burbujas ascendentes
			P->force = P->mass * gravity;
			P->acceleration = P->force / P->mass;
			P->velocity += P->acceleration * deltaTime;
			P->position += P->velocity * deltaTime;

			if (P->position.y >= 1.6) {
				// Cuando una partícula llega a la parte superior, regresa a la posición inicial en la parte inferior
				P->position = P->p0;
				P->velocity = glm::vec3(0.0f, 0.0f, 0.0f);  // Resetea la velocidad
			}
		}
	}
	

private:

	//glm::vec3 gravity = glm::vec3(0.0f, -0.1f, 0.0f);
	glm::vec3 gravity = glm::vec3(0.0f, 0.3, 0.0f);
	float k = 0.25f; //VALOR DE RIGIDEZ O SITIFFNES
	float Damping = 0.1f; // VARIABLAE DE AMORTIGUAMINETO O DE DAMPING

};

#endif