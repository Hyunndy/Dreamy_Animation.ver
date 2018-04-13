////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"




GraphicsClass::GraphicsClass()
{
	//m_D3D = 0;
	//m_Camera = 0;
	m_horse = 0;
	m_Shader = 0;
	m_cube = 0;

}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}
GraphicsClass::~GraphicsClass()
{
}


/*----------------------------------------------------------------------------------------
�̸� : Initialize()
�뵵 : D3D��ü, ī�޶�, ����ȭ�� ��ü �ʱ�ȭ
------------------------------------------------------------------------------------------*/
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// D3D��ü ����
	//-------------------------------------------------------------------------------------
	//m_D3D = new D3DClass;
	//
	//if (!m_D3D) { return false; }
	//
	///* Direct3D ��ü�� �ʱ�ȭ�մϴ�.
	// �� �Լ����� ȭ���� �ʺ�, ����, ������ �ڵ�, Graphicsclass.h�� ���ǵ� �� ���� ���� �������� �����Ѵ�.
	// D3DCLASS���� �� �������� ����Ͽ� Direct3D �ý����� �����Ѵ�. */
	//result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	//if (!result) { MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK); return false; }
	D3DInfo info;
	D3D::GetInfo(&info);
	D3D::Get();


	//--------------------------------------------------------------------------------------

	//ī�޶�ü ����
	//-------------------------------------------------------------------------------------
	//m_Camera = new CameraClass;
   
	//if (!m_Camera) { return false; }
   
	////�۲��� ��ġ
	//m_Camera->SetPosition(D3DXVECTOR3(1.7f, 16, -40));
	//-------------------------------------------------------------------------------------
	m_Shader = new ShaderManagerClass;
	if (!m_Shader) { return false; }
	
	result = m_Shader->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK); return false;
	}





	return true;
}


/*----------------------------------------------------------------------------------------
�̸� : Loading()
�뵵 : ������ ������ �Ǵ� �ֵ� �ʱ�ȭ
------------------------------------------------------------------------------------------*/
bool GraphicsClass::Loading(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//--------------------------------------------------------------------------------------
	FBXShader::CreateBuffer();
    


	wstring filePath = L"../Dreamy/Contents/Models/Sword and Shield Pack/";

	//model = new ModelScene(L"./Contents/Models/Box.fbx");
	//model = new ModelScene(L"./Contents/Models/Rock_01.fbx");

	tPosePath = filePath + L"Horse.fbx";
	m_horse->TexturePath = L"../Dreamy/Data/Horse_D.png";
	idlePath = filePath + L"Horse_Idle.fbx";
	runPath = filePath + L"Horse_Run.fbx";
	attackPath = filePath + L"Horse_Walk.fbx";

	m_horse = new ModelScene();

	m_horse->LoadScene(tPosePath, true, true, true, false);
	m_horse->LoadScene(idlePath, false, false, false, true);
	m_horse->LoadScene(runPath, false, false, false, true);
	m_horse->LoadScene(attackPath, false, false, false, true);

	// Default Animation ����
	m_horse->SetCurrentAnimation(runPath);

	D3DXMATRIX world2;
	D3DXMatrixIdentity(&world2);
	D3DXMatrixTranslation(&world2, -40.0f, 10.0f, 0.0f);

	m_horse->SetWorldTransform(world2);
	

	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------

	m_cube = new ModelClass;
	if (!m_cube) { return false; }

	m_cube->Initialize("../Dreamy/cube.txt", L"../Dreamy/dirt01.dds");
	//--------------------------------------------------------------------------------------




	return true;
}

// ��� �׷��� ��ü�� ���ᰡ ���⿡�� �Ͼ��.
void GraphicsClass::Shutdown()
{

	FBXShader::DeleteBuffer();
	SAFE_DELETE(m_horse);
	
	//if (m_Camera)	{ delete m_Camera; m_Camera = 0;}

	//if (m_D3D){ m_D3D->Shutdown(); delete m_D3D; m_D3D = 0; }
	D3D::Delete();
	return;
}

/*----------------------------------------------------------------------------------------
�̸� : Frame()
�뵵 : 
- SystemClass���� ������ ���� ȣ��ȴ�.
- ���콺������ �����̴°� �� �� ����
- fps,cpu,frametime�缭 text�� ���.
- ��ü ���� �� ����
- �� ������ ���� ������ ��ȯ.
- �� ������ ���� ȣ���� ��ȯ�� ȣ�� ǥ�� �ؽ�ó�� 3�ܰ� RTT <- �̰� �ʹ� ��� �����.
------------------------------------------------------------------------------------------*/
// �� ȣ�⸶�� Render�Լ��� �θ���.
bool GraphicsClass::Frame(int fps, int cpu, float frameTime, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int mouseX, int mouseY)
{


	m_horse->Update();

	Camera::Get()->Update();
	return true;
}


/*----------------------------------------------------------------------------------------
�̸� : Render()
�뵵 : ���׾��� RTT���� �׸���.
- �ݻ縦 �� ���� �켱 RTT�� �����ϰ� ���׾��� �ε��ؾ� ȥ�յȴ�.
------------------------------------------------------------------------------------------*/
bool GraphicsClass::Render( bool Pressed, int frametime)
{
	bool result;


	//-------------------------------------------------------------------------------------
	result = RenderRunningScene(Pressed);
	if (!result) { return false; }
	//-------------------------------------------------------------------------------------
	
	return true;
}

/*----------------------------------------------------------------------------------------
�̸� : RenderRunningScene()
�뵵 :
- 1. ī�޶��� ��ġ�� ���� �� ����� ����� ���� ī�޶��� render�Լ��� ȣ���Ѵ�.
- 2. �� ����� ����� ���纻�� �����´�.
- 3. d3dclass��ü�� ���� ���� ��İ� ���� ����� �����ؿ´�.
- 4. modelclass�� render()�Լ��� ȣ���Ͽ� �׷��� ���������ο� �ﰢ�� ���� �׸����� �Ѵ�.
- 5. ���⼭ �غ��� ������� ���̴��� ȣ���Ͽ� ���̴��� �� ������ ������ ��ġ��Ű�� ���� �� ����� ����Ͽ� �������� �׷�����.
- 6. �ﰢ���� ����ۿ� �׷�����.

- ModelClass, ImageClass, TextClass�� Render()�� ����/�ε��� ���۸� �׷��Ƚ� ���������ο� ���� I/A�ܰ� ������ �Ѵ�.
- �� ���ϱ����� �׸���(����ϴ�)�� ShaderClass�� Render()�̴�!

- �Ȱ� ���� �� ����۸� �Ȱ��� ������ ����°��� �ſ� �߿��� �κ��̴�.
- FogEnd�� �־�������(+�ɼ���) ������� = ��ü�� ��ġ(�������̴����� cameraposition)�� FogENd�� �����������(+) FogFactor�� Ŀ���� �ؽ�ó�� ��������.

�Ȱ��� ...���߿� ... ���̸� ���� �� ���̸ʿ��� ���̴��� ����°� ������.
------------------------------------------------------------------------------------------*/
bool GraphicsClass::RenderRunningScene(bool Pressed)
{
	
	D3DXMATRIX viewMatrix, projectionMatrix, orthoMatrix;

	//����
	//-------------------------------------------------------------------------------------
	D3D::Get()->BeginScene();

	//ī�޶� ��ġ
	//-------------------------------------------------------------------------------------


	m_horse->Render();

	D3DXMATRIX Worldmatrix;
	D3DXMatrixIdentity(&Worldmatrix);
	
	D3DXMatrixTranslation(&Worldmatrix, 0.0f, 10.0f, 0.0f);

    Camera::Get()->GetView(&viewMatrix);
	Camera::Get()->GetProjection(&projectionMatrix);
	//m_Camera->GetViewMatrix(viewMatrix);
	//D3DXMATRIX ProjectionMatrix=m_Camera->GetProjectionMatrix();

	m_cube->Render();
	m_Shader->RenderTextureShader(m_cube->GetIndexCount(), Worldmatrix, viewMatrix, projectionMatrix, m_cube->GetTexture());



	//���ۿ� �׷��� ���� ȭ�鿡 ǥ���Ѵ�.
	D3D::Get()->EndScene();
	



	return true;
}

