////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


/*
�̸�: GraphicsClass
�뵵: 
- �� App�� ��� �׷��� ��ҵ��� ĸ��ȭ �Ǿ� �ִ�.
- ������(Billboarding)
 *�� �Ÿ��� �ִ� 3D������ ǥ���ϱ� ���� �ؽ�ó�� ���� �簢���� ����ϴ°�. 
 *�ǽð� �������� �ϱ⿣ �׷��� ī���� ���뼺�� ������ ������, ���� ����� �ֵ��� �������� ������-�ָ� �ִ� �ֵ��� ���� �������� ����ϴ� �����尡 �� �� �ִ�.
 *�����带 �����ϴ� ����鰣�� �������� �簢���� ��� ȸ���ϴ����� ���� �޷��ִ�.
 *�� App���� ������ ��ġ�� ���� ������ �׻� ���ֺ��� �ϴ� ����� ����Ѵ�.


*/


//////////////
// INCLUDES //
//////////////
#pragma once


//#include "D3DClass.h"
#include "CameraClass.h"
#include "ShaderManagerClass.h"
#include "ModelClass.h"
#include "D3D.h"
#include "FBXShader.h"
#include "ModelScene.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
//���� ����ȭ(fps����)
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	bool Loading(int, int, HWND);

	void Shutdown();
	bool Frame(int, int, float, D3DXVECTOR3, D3DXVECTOR3, int, int);// + ī�޶� ȸ��
	bool Render(bool, int);
	void CheckIntersection(int, int, int, int);

	bool RenderMainScene();
	bool RenderLoadingScene();

	//Picking �˻�
	//void TestIntersection(int, int, int, int);
	//bool RaySphereIntersect(D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, float radius);
public:
	bool F1pressed;
	D3DXMATRIX baseViewMatrix; //�۲��� �׻� ���� ��ġ�� ��µǰ�
	D3DXMATRIX minimapMatrix; // �̴ϸ� 


private:

	//RTT����� ����� �� �ְ� �� ������ ������ �Լ��� �����Ѵ�.
	//�������� �� �� �Ͼ���� �и��ߴ�.(ù��°�� �ؽ�ó��, �� ���� �Ϲ� ����ۿ� �׸��°�)
	bool RenderToTexture();
	bool RenderRTTScene();
	bool RenderRunningScene(bool);
	bool RenderRefractionToTexture(bool); // ȣ�� ���� �ؽ�ó
	bool RenderReflectionToTexture(); // ȣ�� �ݻ� �ؽ�ó
	
	void SetEffectVariable(); //����Ʈ ���� ���� ����
	float CalculateBillboarding(D3DXVECTOR3,D3DXVECTOR3);

private:
	bool sibal = true;

	ModelScene* m_horse;
	
	CameraClass* m_Camera;
	//CameraClass* m_minimapCamera;
	ShaderManagerClass* m_Shader;
	ModelClass* m_cube;

private:
	wstring tPosePath;
	wstring idlePath;
	wstring runPath;
	wstring attackPath;


};

