#include "SceneNode.h"
#include "../SceneManager.h"
#include "Vector3.h"

#include <cstdlib>

CSceneNode::CSceneNode(void)
: theModel(NULL)
, theChildren(NULL)
, theTransform(NULL)
, sceneNodeID(-1)
, health(100)
, currentFSM(ESCAPE)
, currentFLOCKING(IDLE)
{
}

CSceneNode::CSceneNode(const int sceneNodeID)
{
	CSceneNode();
	SetSceneNodeID(sceneNodeID);
}


CSceneNode::~CSceneNode(void)
{
	if (theChildren.size() != 0)
	{
		CNode* aChild = NULL;
		for (unsigned i = 0; i < theChildren.size(); i++)
		{
			aChild = theChildren[i];
			delete aChild;
			aChild = NULL;
		}
		theChildren.clear();
	}

	if (theModel != NULL)
	{
		delete theModel;
		theModel = NULL;
	}

	if (theTransform != NULL)
	{
		delete theTransform;
		theTransform = NULL;
	}
}

void CSceneNode::Draw(void)
{
	if (theTransform)
	{
		theTransform->PreRendering();
		if (theModel)
			theModel->Draw();

		if (theChildren.size() != 0)
		{
			CNode* aChild = NULL;
			for (unsigned i = 0; i < theChildren.size(); i++)
			{
				aChild = theChildren[i];
				aChild->Draw();
			}
		}

		theTransform->PostRendering();
	}
}

void CSceneNode::Draw(CSceneManager* theSceneManager)
{
	if (theTransform)
	{
		//theTransform->PreRendering();
		float x, y, z;
		theTransform->GetOffset(x, y, z);
		theSceneManager->PreRendering(theTransform->GetTransform(), false);
		if (theModel)
			theModel->Draw();
		theSceneManager->PostRendering();

		if (theChildren.size() != 0)
		{
			CNode* aChild = NULL;
			for (unsigned i = 0; i < theChildren.size(); i++)
			{
				aChild = theChildren[i];
				((CSceneNode*)aChild)->Draw(theSceneManager);
			}
		}

		theTransform->PostRendering();
	}
}

void CSceneNode::SetModel(CModel* theModel)
{
	this->theModel = theModel;
}

int CSceneNode::SetNode(CTransform* aNewTransform, CModel* aNewModel)
{
	this->theModel = aNewModel;
	this->theTransform = aNewTransform;

	// Set sceneNodeID to 1 as this is the root node
	sceneNodeID = 1;
	return sceneNodeID;
}

void CSceneNode::SetSceneNodeID(const int sceneNodeID)
{
	this->sceneNodeID = sceneNodeID;
}

int CSceneNode::AddChild(CTransform* aNewTransform, CModel* aNewModel)
{
	CSceneNode* aNewNode = new CSceneNode();
	aNewNode->SetNode(aNewTransform, aNewModel);
	aNewNode->SetSceneNodeID(sceneNodeID * 10 + (theChildren.size() + 1));
	theChildren.push_back(aNewNode);

	return aNewNode->GetSceneNodeID();
}

int CSceneNode::AddChild(const int sceneNodeID, CTransform* aNewTransform, CModel* aNewModel)
{
	CSceneNode* aNewNode = new CSceneNode(sceneNodeID);
	aNewNode->SetNode(aNewTransform, aNewModel);
	theChildren.push_back(aNewNode);

	return aNewNode->GetSceneNodeID();
}

int CSceneNode::GetSceneNodeID(void)
{
	return sceneNodeID;
}

CSceneNode* CSceneNode::GetNode(const int sceneNodeID)
{
	if (this->sceneNodeID == sceneNodeID)
		return this;

	CSceneNode* theTarget = NULL;
	if (theChildren.size() != 0)
	{
		CSceneNode* aChild = NULL;
		for (unsigned i = 0; i < theChildren.size(); i++)
		{
			aChild = (CSceneNode*)theChildren[i];

			theTarget = ((CSceneNode*)aChild)->GetNode(sceneNodeID);
			if (theTarget != NULL)
				break;
		}
	}

	return theTarget;
}

void CSceneNode::ApplyTranslate(const float dx, const float dy, const float dz)
{
	if (theTransform)
		theTransform->SetTranslate(dx, dy, dz);
}


void CSceneNode::ApplyRotate(const float angle, const float rx, const float ry, const float rz)
{
	if (theTransform)
	{
		// Then rotate the children
		theTransform->SetRotate(angle, rx, ry, rz);
	}
}


void CSceneNode::ApplyScale(const float sx, const float sy, const float sz)
{
	if (theTransform)
	{
		// Then rotate the children
		theTransform->SetScale(sx, sy, sz);

		/*if (theChildren.size() != 0)
		{
		CNode* aChild = NULL;
		for (unsigned i = 0; i<theChildren.size(); i++)
		{
		aChild = theChildren[i];
		((CSceneNode*)aChild)->ApplyScale(sx, sy, sz);
		}
		}*/

		/*if (this->GetModel()->modelType == CModel::MODEL_TYPE::POLICE_TYPE)
		{
		this->theModel->SetTopLeft(Vector3(sx * 0.25 * this->theModel->GetTopLeft().x, sy * 0.25 * this->theModel->GetTopLeft().y, sz * 0.25 * this->theModel->GetTopLeft().z));
		this->theModel->SetBottomRight(Vector3(sx * 0.25 * this->theModel->GetBottomRight().x, sy * 0.25 * this->theModel->GetBottomRight().y, sz * 0.25 * this->theModel->GetBottomRight().z));
		}
		else
		{
		this->theModel->SetTopLeft(Vector3(sx * 0.25 * this->theModel->GetTopLeft().x, sy * 0.25 * this->theModel->GetTopLeft().y, sz * 0.25 * this->theModel->GetTopLeft().z));
		this->theModel->SetBottomRight(Vector3(sx * 0.25 * this->theModel->GetBottomRight().x, sy * 0.25 * this->theModel->GetBottomRight().y, sz * 0.25 * this->theModel->GetBottomRight().z));
		}*/


	}
}

// Get top left corner of the group
Vector3 CSceneNode::GetTopLeft(void)
{
	if (theTransform == NULL)
		return Vector3(theModel->GetTopLeft().x,
		theModel->GetTopLeft().y,
		theModel->GetTopLeft().z);
	else
	{
		return (theTransform->GetTransform() * theModel->GetTopLeft());
	}
}

// Get bottom right corner of the group
Vector3 CSceneNode::GetBottomRight(void)
{
	if (theTransform == NULL)
		return Vector3(theModel->GetBottomRight().x,
		theModel->GetBottomRight().y,
		theModel->GetBottomRight().z);
	else
		return (theTransform->GetTransform() * theModel->GetBottomRight());
}

void CSceneNode::SetColor(const float red, const float green, const float blue)
{
	theModel->SetColor(red, green, blue);
}

// Return the number of children in this group
int CSceneNode::GetNumOfChild(void)
{
	return (int)theChildren.size();
}

// Get top left corner of a child
bool CSceneNode::GetTopLeft(const int m_iChildIndex, Vector3& Vector3_TopLeft)
{
	Vector3_TopLeft = Vector3(-999, -999, -999);
	CSceneNode* aChild = NULL;
	for (unsigned i = 0; i < theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			Vector3_TopLeft = theTransform->GetTransform() * aChild->GetTopLeft();
			return true;
		}
	}
	return false;
}

// Get bottom right corner of a child
bool CSceneNode::GetBottomRight(const int m_iChildIndex, Vector3& Vector3_BottomRight)
{
	Vector3_BottomRight = Vector3(-999, -999, -999);
	CSceneNode* aChild = NULL;
	for (unsigned i = 0; i < theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			Vector3_BottomRight = theTransform->GetTransform() * aChild->GetBottomRight();
			return true;
		}
	}
	return false;
}

void CSceneNode::SetColorForChild(const int m_iChildIndex, const float red, const float green, const float blue)
{
	CSceneNode* aChild = NULL;
	for (unsigned i = 0; i < theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			aChild->SetColor(red, green, blue);
		}
	}
}

/********************************************************************************
 Check a position for collision with objects in any of the grids
 ********************************************************************************/
bool CSceneNode::CheckForCollision(Vector3 position)
{
	Vector3 ObjectTopLeft = GetTopLeft();
	Vector3 ObjectBottomRight = GetBottomRight();

	if (((ObjectTopLeft.x > position.x) && (ObjectTopLeft.y > position.y) && (ObjectTopLeft.z > position.z)) &&
		((ObjectBottomRight.x < position.x) && (ObjectBottomRight.y < position.y) && (ObjectBottomRight.z < position.z)))
	{
		return true;
	}
	return false;
}

/********************************************************************************
 Check where a line segment between two positions intersects a plane
 ********************************************************************************/
int CSceneNode::GetIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit) {
	if ((fDst1 * fDst2) >= 0.0f) return 0;
	if (fDst1 == fDst2) return 0;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return 1;
}

/********************************************************************************
 Check two positions are within a box region
 ********************************************************************************/
int CSceneNode::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis) {
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return 1;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return 1;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return 1;
	return 0;
}

/********************************************************************************
 Check two positions for collision with objects in any of the grids
 ********************************************************************************/
int CSceneNode::CheckForCollision(Vector3 position_start, Vector3 position_end, Vector3 &Hit)
{
	Vector3 ObjectTopLeft = GetTopLeft();
	Vector3 ObjectBottomRight = GetBottomRight();

	if (position_end.x < ObjectBottomRight.x && position_start.x < ObjectBottomRight.x)
		return false;
	if (position_end.x > ObjectTopLeft.x && position_start.x > ObjectTopLeft.x)
		return false;
	if (position_end.y < ObjectBottomRight.y && position_start.y < ObjectBottomRight.y)
		return false;
	if (position_end.y > ObjectTopLeft.y && position_start.y > ObjectTopLeft.y)
		return false;
	if (position_end.z < ObjectBottomRight.z && position_start.z < ObjectBottomRight.z)
		return false;
	if (position_end.z > ObjectTopLeft.z && position_start.z > ObjectTopLeft.z)
		return false;
	if (position_start.x > ObjectBottomRight.x && position_start.x < ObjectTopLeft.x &&
		position_start.y > ObjectBottomRight.y && position_start.y < ObjectTopLeft.y &&
		position_start.z > ObjectBottomRight.z && position_start.z < ObjectTopLeft.z)
	{
		Hit = position_start;
		return true;
	}
	if ((GetIntersection(position_start.x - ObjectBottomRight.x, position_end.x - ObjectBottomRight.x, position_start, position_end, Hit) && InBox(Hit, ObjectBottomRight, ObjectTopLeft, 1))
		|| (GetIntersection(position_start.y - ObjectBottomRight.y, position_end.y - ObjectBottomRight.y, position_start, position_end, Hit) && InBox(Hit, ObjectBottomRight, ObjectTopLeft, 2))
		|| (GetIntersection(position_start.z - ObjectBottomRight.z, position_end.z - ObjectBottomRight.z, position_start, position_end, Hit) && InBox(Hit, ObjectBottomRight, ObjectTopLeft, 3))
		|| (GetIntersection(position_start.x - ObjectTopLeft.x, position_end.x - ObjectTopLeft.x, position_start, position_end, Hit) && InBox(Hit, ObjectBottomRight, ObjectTopLeft, 1))
		|| (GetIntersection(position_start.y - ObjectTopLeft.y, position_end.y - ObjectTopLeft.y, position_start, position_end, Hit) && InBox(Hit, ObjectBottomRight, ObjectTopLeft, 2))
		|| (GetIntersection(position_start.z - ObjectTopLeft.z, position_end.z - ObjectTopLeft.z, position_start, position_end, Hit) && InBox(Hit, ObjectBottomRight, ObjectTopLeft, 3)))
		return true;

	return false;
}

/********************************************************************************
 Update the SceneNode
 ********************************************************************************/
void CSceneNode::Update(const int resolutionType)
{

	if (theModel)
		theModel->SetResolution(resolutionType);

	if (theChildren.size() != 0)
	{
		CNode* aChild = NULL;
		for (unsigned i = 0; i < theChildren.size(); i++)
		{
			aChild = theChildren[i];
			((CSceneNode*)aChild)->Update(resolutionType);
		}
	}
}

/********************************************************************************
 Update the SceneNode to move towards a position
 This method does not call the children, so it will only work on the
 SceneNode which you call
 ********************************************************************************/
void CSceneNode::Update(const float dt, Vector3 thePosition)
{
	if (health == 0)
	{
		currentFSM = ESCAPE;
	}
	else if (health == 100)
	{
		currentFSM = ATTACK;

		//flocking stuff
		float x, y, z;
		theTransform->GetOffset(x, y, z);

		Vector3 distanceVector = thePosition - Vector3(x, y, z);
		float theDistance = distanceVector.Length();
		// if player in cohesion ring, move towards it
		if (theDistance > 30.f && theDistance < 100.f)
		{
			currentFLOCKING = COHESION;
		}
		// if player in separation ring, move away from it
		else if (theDistance < 200.f || theDistance < 30.f)
		{
			currentFLOCKING = SEPARATION;
		}
		else
			currentFLOCKING = IDLE;
	}

	if (currentFSM == ESCAPE)
	{
		float x, y, z;
		theTransform->GetOffset(x, y, z);

		Vector3 newDirection = thePosition - Vector3(x, y, z);
		if (newDirection.Length() > 0.0f)
		{
			newDirection.Normalize() * 1.0f * dt;

			this->ApplyTranslate(newDirection.x, newDirection.y, newDirection.z);

			if (theChildren.size() != 0)
			{
				CNode* aChild = NULL;
				for (unsigned i = 0; i < theChildren.size(); i++)
				{
					aChild = theChildren[i];
					((CSceneNode*)aChild) ->ApplyTranslate(newDirection.x, newDirection.y, newDirection.z);
				}
			}

			this->SetColor(0.0f, 1.0f, 0.0f);
		}
		if (health < 100)
			health++;
	}
	else if (currentFSM == ATTACK)
	{
		/*float x,y,z;
		theTransform->GetOffset(x,y,z);

		Vector3 newDirection = Vector3(x,y,z) - thePosition;
		if (newDirection.Length() > 0.0f)
		{
		newDirection.Normalize() * 1.0f * dt;

		this->ApplyTranslate( newDirection.x, newDirection.y, newDirection.z );
		this->SetColor( 1.0f, 0.0f, 0.0f);
		}
		health--;*/


		// flocking stuff
		float x, y, z;
		theTransform->GetOffset(x, y, z);

		Vector3 newDirection = Vector3(x, y, z) - thePosition;

		if (currentFLOCKING == COHESION)
		{
			if (newDirection.Length() > 0.0f)
			{
				newDirection.Normalize() * 1.0f * dt;

				this->ApplyTranslate(newDirection.x, newDirection.y, newDirection.z);

				if (theChildren.size() != 0)
				{
					CNode* aChild = NULL;
					for (unsigned i = 0; i < theChildren.size(); i++)
					{
						aChild = theChildren[i];
						((CSceneNode*)aChild)->ApplyTranslate(newDirection.x, newDirection.y, newDirection.z);
					}
				}

				this->SetColor(1.0f, 0.0f, 0.0f);
			}
			health--;
		}
		else if (currentFLOCKING == SEPARATION)
		{
			if (newDirection.Length() > 0.0f)
			{
				newDirection.Normalize() * 1.0f * dt;

				this->ApplyTranslate(-newDirection.x, -newDirection.y, -newDirection.z);

				if (theChildren.size() != 0)
				{
					CNode* aChild = NULL;
					for (unsigned i = 0; i < theChildren.size(); i++)
					{
						aChild = theChildren[i];
						((CSceneNode*)aChild)->ApplyTranslate(-newDirection.x, -newDirection.y, -newDirection.z);
					}
				}
				this->SetColor(1.0f, 0.0f, 0.0f);
			}
		}
		else if (currentFLOCKING == IDLE)
		{
		}

	}
}
