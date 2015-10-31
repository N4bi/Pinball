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
	void Turn(int degrees);
	void Push(float x, float y);
	double GetAngle() const;


public:
	int width, height;
	b2Body* body;
	Module* listener;
	SDL_Texture* texture;
	

private:
	SDL_Rect rect;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener 
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
	PhysBody* CreateFlipper(int flipper_pos_x, int flipper_pos_y, int pivot_x, int pivot_y, int* points, uint size, int anchorA_x, int anchorA_y, int AnchorB_x, int AnchorB_y, float lower_angle, float upper_angle, float density, float restitution, bool ccd, bool isSensor, SDL_Texture*);

	PhysBody* AddWall(int x, int y, int* points, int size, float restitution = 0.0f);
	PhysBody* AddSpring(int x_box, int y_box, SDL_Texture* texture, float restitution = 0.0f);

	
	// b2ContactListener ---

	void BeginContact(b2Contact* contact);
	void DestroyBody(PhysBody* body);

	


private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2Body* click_body;
	p2List<PhysBody*> bodies;

public:
	float32 time_step;
	int32 velocity_iter;
	int32 position_iter;
	
};