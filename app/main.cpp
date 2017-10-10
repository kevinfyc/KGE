#include "core/main_def.h"
#include "core/Application.h"
#include "core/game_object.h"
#include "graphics/camera.h"
#include "graphics/mesh.h"
#include "graphics/material.h"
#include "graphics/mesh_renderer.h"
#include "util/time.h"
#include "core/resource.h"

using namespace kge;

class App : public IApplication
{
public:
	App();
	virtual void Start() override;
	virtual void Update() override;

private:
	WeakRef<Camera> _camera;
	WeakRef<GameObject> _cube;
	float m_rotate_deg;
	WeakRef<GameObject> _gameObject;
};

App::App()
{
	SetName("App Test");
	SetInitSize(800, 600);
}

void App::Start()
{
	auto camera = GameObject::Create("camera")->AddComponent<Camera>();
	camera->GetTransform()->SetWorldPosition(Vector3(0, 0, -6));
	Quaternion rot = Quaternion();
	rot.fromAngleAxis(0, Vector3(0, 0, 0));
	camera->GetTransform()->SetWorldRotation(rot);
	camera->SetCullingMask(1 << 0);

	_camera = camera;

	//auto mesh = Mesh::Create();
	//mesh->vertices.push_back(Vector3(-1, 1, -1));
	//mesh->vertices.push_back(Vector3(-1, -1, -1));
	//mesh->vertices.push_back(Vector3(1, -1, -1));
	//mesh->vertices.push_back(Vector3(1, 1, -1));
	//mesh->vertices.push_back(Vector3(-1, 1, 1));
	//mesh->vertices.push_back(Vector3(-1, -1, 1));
	//mesh->vertices.push_back(Vector3(1, -1, 1));
	//mesh->vertices.push_back(Vector3(1, 1, 1));
	//mesh->uv.push_back(Vector2(0, 0));
	//mesh->uv.push_back(Vector2(0, 1));
	//mesh->uv.push_back(Vector2(1, 1));
	//mesh->uv.push_back(Vector2(1, 0));
	//mesh->uv.push_back(Vector2(1, 0));
	//mesh->uv.push_back(Vector2(1, 1));
	//mesh->uv.push_back(Vector2(0, 1));
	//mesh->uv.push_back(Vector2(0, 0));
	//unsigned short triangles[] = {
	//	0, 1, 2, 0, 2, 3,
	//	3, 2, 6, 3, 6, 7,
	//	7, 6, 5, 7, 5, 4,
	//	4, 5, 1, 4, 1, 0,
	//	4, 0, 3, 4, 3, 7,
	//	1, 5, 6, 1, 6, 2
	//};

	//uint32 count = 36;
	//uint32 old_size = mesh->triangles.size();
	//mesh->triangles.resize(old_size + count);

	//for (uint32 i = 0; i < count; i++)
	//{
	//	mesh->triangles[old_size + i] = triangles[i];
	//}
	//auto mat = Material::Create("Diffuse");
	//mesh->Tick();


	//auto mesh2 = Resource::LoadMesh("Assets/Library/unity default resources.Cube.mesh");
	//if (!mesh2)
	//	return;

	//mesh2->Tick();

	//auto obj = GameObject::Create("mesh");
	//auto renderer = obj->AddComponent<MeshRenderer>();
	//renderer->SetSharedMesh(mesh2);
	//renderer->SetSharedMaterial(mat);

	//_cube = obj;
	m_rotate_deg = 0.1f;


	Resource::LoadGameObjectAsync("Assets/AppMesh/plane.prefab", true);

}

void App::Update()
{
	//Quaternion rot = Quaternion();
	//rot.fromAngleAxis(m_rotate_deg, Vector3(0, 1, 0));
	//_camera.lock()->GetTransform()->SetLocalRotation(rot);
	//m_rotate_deg += 30 * 0.01f;

	Quaternion rot = Quaternion();
	rot.fromAngleAxis(m_rotate_deg * Deg2Rad, Vector3(0, 1, 0));
	//_cube.lock()->GetTransform()->SetLocalRotation(rot);
	//_gameObject.lock()->GetTransform()->SetLocalRotation(rot);
	m_rotate_deg += 30 * Time::GetDeltaTime();
}

KGE_MAIN(App);
