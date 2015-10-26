#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{};

	PhysBody(b2Body* body, const SDL_Rect& rect);
	~PhysBody();

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
	void Turn(int degrees);
	void Push(float x, float y);

public:
	int width, height;
	b2Body* body;
	Module* listener;
	SDL_Texture* texture;

private:
	SDL_Rect rect;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangle(const SDL_Rect& rect);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* CreateCircleStatic(int x, int y, int radius);
	PhysBody* CreateChainStatic(int x, int y, int* points, int size);
	PhysBody* CreateFlipper(const SDL_Rect& rect, int* points, uint size, float density, float restitution, bool ccd, bool isSensor, SDL_Texture*);

	PhysBody* AddWall(int x, int y, int* points, int size);
	PhysBody* AddFlipper(int x, int y, int* points, int size, SDL_Texture*);

	void RevoluteJoint(PhysBody* body1, PhysBody* body2, int x_pivot1 , int y_pivot1 , int x_pivot2 , int y_pivot2 , int max_angle, int min_angle);
	void LineJoint(PhysBody* body1, PhysBody* body2, int x_pivot1 = 0, int y_pivot1 = 0, int x_pivot2 = 0, int y_pivot2 = 0, float frequency = 10.0f, float damping = 0.5f);
	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	void DestroyBody(PhysBody* body);

	void Turn(int degrees);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2RevoluteJoint* m_joint;
	b2DistanceJoint* dis_joint;
	b2Body* ground;
	b2Body* click_body;
	p2List<PhysBody*> bodies;

	float32 time_step;
	int32 velocity_iter;
	int32 position_iter;
	
};