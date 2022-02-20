#include "EnemyBehaviour.h"
#include "BehaviourAction.h"
#include "..\CSC8503Common\GameObject.h"
#include "..\CSC8503Common\CollisionDetection.h"
#include "..\CSC8503Common\Debug.h"
using namespace NCL;
using namespace CSC8503;

Vector3 NCL::CSC8503::EnemyBehaviour::GetPointOnGrid(Vector3 pos)
{
	int z = (int)pos.z / 10, x = (int)pos.x / 10;
	if (!m_Obstacles[z * 20 + x])
	{
		return Vector3(x * 10, 2, z * 10);
	}
	if (!m_Obstacles[z * 20 + x + 1])
	{
		return Vector3((x + 1) * 10, 2, z * 10);
	}
	if (!m_Obstacles[(z+1) * 20 + x])
	{
		return Vector3(x * 10, 2, (z + 1) * 10);
	}
	if (!m_Obstacles[(z + 1) * 20 + x+1])
	{
		return Vector3((x+1) * 10, 2, (z + 1) * 10);
	}
	if (!m_Obstacles[(z - 1) * 20 + x])
	{
		return Vector3(x * 10, 2, (z - 1) * 10);
	}
	if (!m_Obstacles[(z - 1) * 20 + x - 1])
	{
		return Vector3((x - 1)* 10, 2, (z - 1) * 10);
	}
	return Vector3((x - 1) * 10, 2, (z) * 10);
	
}

NCL::CSC8503::EnemyBehaviour::EnemyBehaviour(GameObject *obj, GameObject *p, bool * obstacles) : m_Grid("TestGrid1.txt")
{
	enemy = obj;
	player = p;
	m_Obstacles = obstacles;
	delta = 0.0f;
	BehaviourAction* action = new BehaviourAction("Look for player",
		[this](float dt, BehaviourState state)->BehaviourState
		{
			if (delta <= 0)
			{
				delta += 3;
				NavigationPath path;
				Vector3 startPos = GetPointOnGrid(enemy->GetTransform().GetPosition());
				Vector3 endPos = GetPointOnGrid(player->GetTransform().GetPosition());
				bool found = m_Grid.FindPath(startPos, endPos, path);
				if (found)
				{
					Vector3 pos;
					m_Path.clear();
					while (path.PopWaypoint(pos))
					{
						m_Path.insert(m_Path.begin(), pos);
					}
					return Success;
				}
				return Failure;
			}
			delta -= dt;
			return Success;
		}
		);

	BehaviourAction* action2 = new BehaviourAction("Move",
		[this](float dt, BehaviourState state)->BehaviourState
		{
			//for (int i = 1; i < m_Path.size(); ++i) {
			//	Vector3 a = m_Path[i - 1];
			//	Vector3 b = m_Path[i];

			//	Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));

			//}
			Vector3 position = GetPointOnGrid(m_Path.back());
			Vector3 direction = position - enemy->GetTransform().GetPosition();
			while (direction.Length() <= 2 && m_Path.size() > 1)
			{
				m_Path.pop_back();				
				position = GetPointOnGrid(m_Path.back());
				direction = position - enemy->GetTransform().GetPosition();
			}
			if(direction.Length() <= 2)
				return Success;
			float distance = direction.Length();
			direction.Normalise();
			float moveLength = distance < 20 * dt ? 20 * dt - distance : 20 * dt;
			//enemy->GetPhysicsObject()->SetLinearVelocity( direction * moveLength);
			//enemy->GetPhysicsObject()->AddForce(direction * 10);		
			enemy->GetTransform().SetPosition(enemy->GetTransform().GetPosition() + direction * moveLength);
			CollisionDetection::CollisionInfo info;
			if (CollisionDetection::SphereIntersection(*(SphereVolume *)enemy->GetBoundingVolume(), enemy->GetTransform(),
				*(SphereVolume*)player->GetBoundingVolume(), player->GetTransform(), info))
			{
				delta = 0;
				return Success;
			}
			return Ongoing;
		}
	);

	m_Sequence = new BehaviourSequence("Track player");
	m_Sequence->AddChild(action);
	m_Sequence->AddChild(action2);	
}

NCL::CSC8503::EnemyBehaviour::~EnemyBehaviour()
{
	delete m_Sequence;
}

void NCL::CSC8503::EnemyBehaviour::SetEntity(GameObject* obj)
{
	enemy = obj;
}

void NCL::CSC8503::EnemyBehaviour::Update(float dt)
{
	m_Sequence->Execute(dt);
}
