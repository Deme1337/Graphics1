#include "stdafx.h"
#include "BulletCollision\CollisionShapes\btConvexHullShape.h"
#include "BulletCollision\CollisionShapes\btConvexShape.h"
#include "Renderables.h"
#include <glm.hpp>
#include <thread>
#include <future>



glm::vec3 glower(1.0f, 1.0f, 1.0f);

//Geometry

glm::vec3 vCubeVertices[36] =
{
	// Front face
	glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
	// Back face
	glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
	// Left face
	glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
	// Right face
	glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
	// Top face
	glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
	// Bottom face
	glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
};


glm::vec2 vCubeTexCoords[6] = { glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };

glm::vec3 vCubeNormals[6] =
{
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, -1.0f, 0.0f)
};



Renderables::Renderables()
{
}

void Renderables::SetObjectLocation(glm::vec3 loc)
{
	location = loc;
}

void Renderables::SetObjectColor(glm::vec3 col)
{
	color = col;
}

void Renderables::SetObjectSize(glm::vec3 Size)
{
	scaleSize = Size;
}
void Renderables::rotateObject(float speed, glm::vec3 rotAxis)
{
	rotateSpeed = speed;
	rotateAxis = rotAxis;
}

void Renderables::SetObjectTexture(char *TexPath)
{
	TexturePath = TexPath;
	texture.loadTexture2D(TexturePath, true);
	texture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	HasTexture = 1;
}

void Renderables::SetObjectTextureFromData(BYTE* data, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps)
{
	texture.createFromData(data, a_iWidth, a_iHeight, 0, GL_BGR, bGenerateMipMaps);
	texture.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	HasTexture = 1;
}

void Renderables::SkyObject(int val)
{
	if (val == 1)
	{
		PartOfSky = true;
	}
	else
	{
		PartOfSky = false;
	}
}

void Renderables::SetObjectSpecular(float val, float val2)
{
	specular = val;
	specI = val2;
}

void Renderables::MakeGlow()
{
	glowingtex = true;
}

glm::mat4 Renderables::GetModelMatrix()
{
	return mModelMat;
}

void Renderables::SetModelMatrix(glm::mat4 mModelMat)
{
	this->mModelMat = mModelMat;
}

void Renderables::InitializeObject(char* path, float Tiling, std::string matfilepath)
{
	this->ObjectModel.MaterialFilename = matfilepath;
	this->ObjectModel.LoadModelFromFile(path);
	std::string patharr(path);
	Logger::Log("Loaded: " + patharr);
}

void Renderables::InitializeObject(int objectType, float Tiling)
{

}

//Finalize all object model vertexbuffers
void Renderables::FinalizeVBOS()
{
	CAssimpModel::FinalizeVBO();
}

void Renderables::RenderObjectGL()
{

	this->ObjectModel.RenderModel();

	//Oikea renderöinti
	//if (objType == 0)
	//{
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//}
	//else if (objType > 0)
	//{
	//	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	//}
	CAssimpModel::UnBindModelsVAO();
}

glm::vec3 Renderables::GetLocation()
{
	return this->location;
}

glm::vec3 Renderables::GetScale()
{
	return this->scaleSize;
}


void Renderables::RenderObjectInit(CShaderProgram *sProgram, btDynamicsWorld *dyn)
{
	CAssimpModel::BindModelsVAO();
	//dyn->stepSimulation(1 / 60, 10);
	if (HasTexture == 1)
	{
		sProgram->setUniform("hasTex",1);
		texture.bindTexture(0);
		sProgram->setUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		sProgram->setUniform("hasTex", 0);
		sProgram->setUniform("vColor", glm::vec4(color, 1.0f));
	}
	
	mModelMat = glm::mat4(1.0);
	

	if (usesphysics)
	{
		btTransform trans;
		
		rigidbodys->getMotionState()->getWorldTransform(trans);
		//this->SetObjectLocation(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		this->location.x = trans.getOrigin().getX();
		this->location.y = trans.getOrigin().getY();
		this->location.z = trans.getOrigin().getZ();
		rotateObject(trans.getRotation().getAngle(), glm::vec3(trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z()));
		
	}
	
	mModelMat = glm::translate(mModelMat, location);
	mModelMat = glm::rotate(mModelMat, rotateSpeed, rotateAxis);
	
	mModelMat = glm::scale(mModelMat, scaleSize);
	sProgram->setUniform("matActive.fSpecularPower", specular);
	sProgram->setUniform("matActive.fSpecularIntensity", specI);
	sProgram->SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", &mModelMat);
	sProgram->setUniform("shadowMap", 50);
	sProgram->setUniform("bShadowsOn", 1);
}

void Renderables::SetPhysics(btRigidBody* rgbody)
{
	 this->rigidbodys = rgbody;
	 if (usesphysics)
	  {
	  
	   boxCollisionShape = new btBoxShape(btVector3(this->scaleSize.x-5, this->scaleSize.y-5, this->scaleSize.z-5));
	   motionstate = new btDefaultMotionState(btTransform(btQuaternion(this->rotateAxis.x, this->rotateAxis.y,
	  	 this->rotateAxis.z, 1.0f), btVector3(this->location.x, this->location.y, this->location.z)));
	   btScalar mass = 10;
	   btVector3 fallInertia(0, 0,0);
	   boxCollisionShape->calculateLocalInertia(mass, fallInertia);
	   btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionstate, boxCollisionShape,fallInertia);
	  
	   this->rigidbodys = new btRigidBody(rigidBodyCI);
	
	   }

	   else
	   {
		   boxCollisionShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	  
		   motionstate = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1.0f), btVector3(this->scaleSize.x, this->location.y+25, this->scaleSize.z)));
	  
	  	 btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motionstate, boxCollisionShape, btVector3(0, 0, 0));
	  
	  	 this->rigidbodys = new btRigidBody(rigidBodyCI);
		
	   }


	
}



void Renderables::DestroyObject()
{
	//for (int i = 0; i < 2; i++)
	//{
	//	vbos[i]->~VertexBufferObject();
	//}
	
	vbo.releaseVBO();
	if (TexturePath != NULL)
	{
		texture.releaseTexture();
	}
	this->ObjectModel.ReleaseAllModelData();
	//vao->~VertexArrayObject();
}
void Renderables::ReleaseTextureData()
{
	texture.releaseTexture();
}

Renderables::~Renderables()
{

}


