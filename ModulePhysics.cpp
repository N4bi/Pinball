#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
	time_step = 0.0f;
	velocity_iter = 0;
	position_iter = 0.0f;
}

PhysBody::PhysBody(b2Body* body) : body(body), listener(NULL)
{
}

PhysBody::~PhysBody()
{
	body->GetWorld()->DestroyBody(body);
	body = NULL;
	listener = NULL;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);
	
	// We need this to create the mouse joint and the prismatic joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	time_step = 1.0f / 60.0f;
	velocity_iter = 6;
	position_iter = 2;

	world->Step(time_step,velocity_iter, position_iter);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

void ModulePhysics::DestroyBody(PhysBody* body)
{
	bodies.del(bodies.findNode(body));
	delete body;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, bool isSensor,bool ccd)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.bullet = ccd;

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.isSensor = isSensor;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;
	bodies.add(pbody);

	return pbody;
}

PhysBody* ModulePhysics::CreateCircleStatic(int x, int y, int radius, float density, float restitution, bool ccd, bool isSensor)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.bullet = ccd;

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.isSensor = isSensor;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;
	bodies.add(pbody);

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;
	bodies.add(pbody);

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;
	bodies.add(pbody);

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete[] p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;
	bodies.add(pbody);

	return pbody;
}

PhysBody* ModulePhysics::CreateChainStatic(int x, int y, int* points, int size, float density, float restitution, bool isSensor)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.isSensor = isSensor;

	b->CreateFixture(&fixture);

	delete[] p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;
	bodies.add(pbody);

	return pbody;
}


PhysBody* ModulePhysics::AddWall(int x, int y, int* points, int size, float restitution)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	delete[] p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;
	bodies.add(pbody);

	return pbody;
}

PhysBody* ModulePhysics::CreateFlipper(int flipper_pos_x, int flipper_pos_y, int pivot_pos_x, int pivot_pos_y, int* points, uint size, int anchorA_x, int anchorA_y, int AnchorB_x, int AnchorB_y, float lower_angle, float upper_angle, float density, float restitution, bool ccd, bool isSensor, SDL_Texture* texture)
{

	b2Vec2 circle_pos(pivot_pos_x, pivot_pos_y);
	b2Vec2 flipper_pos(flipper_pos_x, flipper_pos_y);
	float radius = 5;

	//Create the pivot
	b2BodyDef circle_def;
	circle_def.position.Set(PIXEL_TO_METERS(circle_pos.x), PIXEL_TO_METERS(circle_pos.y));
	circle_def.type = b2_staticBody;
	b2Body* circle = world->CreateBody(&circle_def);

	b2CircleShape circle_shape;
	circle_shape.m_radius = PIXEL_TO_METERS(radius);

	b2FixtureDef circle_fixture;
	circle_fixture.shape = &circle_shape;
	circle->CreateFixture(&circle_fixture);

	//Create the flipper
	b2BodyDef flipper_def;
	flipper_def.position.Set(PIXEL_TO_METERS(flipper_pos.x), PIXEL_TO_METERS(flipper_pos.y));
	flipper_def.type = b2_dynamicBody;
	b2Body* flipper = world->CreateBody(&flipper_def);


	b2PolygonShape flipper_shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	flipper_shape.Set(p, size / 2);

	b2FixtureDef box_fixture;
	box_fixture.shape = &flipper_shape;
	box_fixture.density = density;
	box_fixture.restitution = restitution;
	box_fixture.isSensor = isSensor;

	flipper->CreateFixture(&box_fixture);

	delete[] p;

	PhysBody* ret = new PhysBody();
	ret->body = flipper;
	flipper->SetUserData(ret);
	ret->texture = texture;

	//JOINT
	//joint between the flipper and the pivot

	b2RevoluteJointDef revolute_joint_def;
	revolute_joint_def.bodyA = flipper;
	revolute_joint_def.bodyB = circle;
	revolute_joint_def.collideConnected = false;
	revolute_joint_def.enableLimit = true;


	revolute_joint_def.lowerAngle = lower_angle * DEGTORAD;
	revolute_joint_def.upperAngle = upper_angle * DEGTORAD;


	revolute_joint_def.localAnchorA.Set(PIXEL_TO_METERS(anchorA_x), PIXEL_TO_METERS(anchorA_y));
	revolute_joint_def.localAnchorB.Set(PIXEL_TO_METERS(AnchorB_x), PIXEL_TO_METERS(AnchorB_y));

	(b2RevoluteJoint*)world->CreateJoint(&revolute_joint_def);

	bodies.add(ret);
	return ret;

}

PhysBody* ModulePhysics::AddSpring(int x_box, int y_box, SDL_Texture* texture,float restitution)
{

	b2Vec2 box_pos(x_box,y_box);

	//Create the box body for the spring
	b2BodyDef body;
	body.position.Set(PIXEL_TO_METERS(box_pos.x), PIXEL_TO_METERS(box_pos.y));
	body.type = b2_dynamicBody;
	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	int width = 15, height = 25;
	shape.SetAsBox(PIXEL_TO_METERS(width)*0.5f, PIXEL_TO_METERS(height)*0.5f);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	PhysBody* ret = new PhysBody();
	ret->body = b;
	b->SetUserData(ret);
	ret->texture = texture;
	ret->width = width * 0.5f;
	ret->height = height * 0.5f;

	//JOINT
	//Joint between the box body and the ground
	b2Vec2 vec(b->GetPosition());

	b2PrismaticJointDef jointDef;
	b2Vec2 worldAxis(0.0f, -1.0f);

	jointDef.Initialize(ground, b, vec, worldAxis);
	jointDef.lowerTranslation = -1.5f;
	jointDef.upperTranslation = 1.5f;
	jointDef.enableLimit = true;
	jointDef.maxMotorForce = 35.0f;
	jointDef.motorSpeed = 35.0f;
	jointDef.enableMotor = true;


	(b2PrismaticJoint*)world->CreateJoint(&jointDef);
	bodies.add(ret);
	return ret;
}
// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	b2Vec2 mouse_position(0, 0);
	click_body = NULL;

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// MOUSE JOINT	
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				mouse_position.x = PIXEL_TO_METERS(App->input->GetMouseX());
				mouse_position.y = PIXEL_TO_METERS(App->input->GetMouseY());
				if (f->GetShape()->TestPoint(b->GetTransform(), mouse_position) == true)
				{
					click_body = f->GetBody();
					LOG("body_clicked\n");
				}
			}
	
		}
	}
	// creation of the joint
	if (click_body != NULL)
	{
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = click_body;
		def.target = mouse_position;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 20.0f;
		def.maxForce = 100.0f * click_body->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	if (mouse_joint != NULL && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		b2Vec2 posA, posB;
		posA = mouse_joint->GetAnchorA();
		posB.x = PIXEL_TO_METERS(App->input->GetMouseX());
		posB.y = PIXEL_TO_METERS(App->input->GetMouseY());
		mouse_joint->SetTarget(posB);

	}

	if (mouse_joint != NULL && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
			world->DestroyJoint(mouse_joint);
			mouse_joint = NULL;
	}


	return UPDATE_CONTINUE;
}


void ModulePhysics::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

//PhysBody Utilities

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

void PhysBody::Turn(int degrees)
{
	body->ApplyAngularImpulse(DEGTORAD * degrees, true);
}

void PhysBody::Push(float x, float y)
{
	body->ApplyForceToCenter(b2Vec2(x, y), true);
}

double PhysBody::GetAngle()const
{
	return RADTODEG * body->GetAngle();
}

void PhysBody::SetPosition(int x, int y)
{
	body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
}

void PhysBody::SetAngularVelocity(float velocity)
{
	body->SetAngularVelocity(velocity * DEGTORAD);
}

void PhysBody::SetLinearVelocity(int x, int y)
{
	body->SetLinearVelocity(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)));
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	p2List_item<PhysBody*>* item = bodies.getFirst();

	while (item != NULL)
	{
		delete item->data;
		item = item->next;
	}

	bodies.clear();
	// Delete the whole physics world!
	delete world;

	return true;
}


