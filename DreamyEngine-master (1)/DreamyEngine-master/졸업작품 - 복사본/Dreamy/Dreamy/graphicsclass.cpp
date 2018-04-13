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
이름 : Initialize()
용도 : D3D객체, 카메라, 메인화면 객체 초기화
------------------------------------------------------------------------------------------*/
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// D3D객체 생성
	//-------------------------------------------------------------------------------------
	//m_D3D = new D3DClass;
	//
	//if (!m_D3D) { return false; }
	//
	///* Direct3D 객체를 초기화합니다.
	// 이 함수에서 화면의 너비, 높이, 윈도우 핸들, Graphicsclass.h에 정의된 네 개의 전역 변수들을 전달한다.
	// D3DCLASS에서 이 변수들을 사용하여 Direct3D 시스템을 설정한다. */
	//result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	//if (!result) { MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK); return false; }
	D3DInfo info;
	D3D::GetInfo(&info);
	D3D::Get();


	//--------------------------------------------------------------------------------------

	//카메라객체 생성
	//-------------------------------------------------------------------------------------
	//m_Camera = new CameraClass;
   
	//if (!m_Camera) { return false; }
   
	////글꼴의 위치
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
이름 : Loading()
용도 : 실제로 렌더링 되는 애들 초기화
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

	// Default Animation 설정
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

// 모든 그래픽 객체의 종료가 여기에서 일어난다.
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
이름 : Frame()
용도 : 
- SystemClass에서 프레임 마다 호출된다.
- 마우스포인터 움직이는것 할 수 있음
- fps,cpu,frametime재서 text로 출력.
- 물체 돌릴 수 있음
- 각 프레임 마다 구름의 변환.
- 각 프레임 마다 호수의 변환과 호수 표면 텍스처의 3단계 RTT <- 이게 너무 비싼 기능임.
------------------------------------------------------------------------------------------*/
// 매 호출마다 Render함수를 부른다.
bool GraphicsClass::Frame(int fps, int cpu, float frameTime, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int mouseX, int mouseY)
{


	m_horse->Update();

	Camera::Get()->Update();
	return true;
}


/*----------------------------------------------------------------------------------------
이름 : Render()
용도 : 러닝씬과 RTT씬을 그린다.
- 반사를 할 때는 우선 RTT를 먼저하고 러닝씬을 로드해야 혼합된다.
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
이름 : RenderRunningScene()
용도 :
- 1. 카메라의 위치를 토대로 뷰 행렬을 만들기 위해 카메라의 render함수를 호출한다.
- 2. 뷰 행렬을 만들고 복사본을 가져온다.
- 3. d3dclass객체로 부터 월드 행렬과 투영 행렬을 복사해온다.
- 4. modelclass의 render()함수를 호출하여 그래픽 파이프라인에 삼각형 모델을 그리도록 한다.
- 5. 여기서 준비한 정점들로 셰이더를 호출하여 셰이더는 모델 정보와 정점을 배치시키기 위한 세 행렬을 사용하여 정점들을 그려낸다.
- 6. 삼각형이 백버퍼에 그려진다.

- ModelClass, ImageClass, TextClass의 Render()은 정점/인덱스 버퍼를 그래픽스 파이프라인에 묶는 I/A단계 까지만 한다.
- 이 기하구조를 그리는(출력하는)건 ShaderClass의 Render()이다!

- 안개 구현 시 백버퍼를 안개의 색으로 지우는것은 매우 중요한 부분이다.
- FogEnd가 멀어질수록(+될수록) 밝아진다 = 물체의 위치(정점셰이더에서 cameraposition)가 FogENd와 가까워질수록(+) FogFactor가 커져서 텍스처가 가려진다.

안개는 ...나중에 ... 높이맵 했을 때 높이맵에만 셰이더를 씌우는게 나을듯.
------------------------------------------------------------------------------------------*/
bool GraphicsClass::RenderRunningScene(bool Pressed)
{
	
	D3DXMATRIX viewMatrix, projectionMatrix, orthoMatrix;

	//시작
	//-------------------------------------------------------------------------------------
	D3D::Get()->BeginScene();

	//카메라 위치
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



	//버퍼에 그려진 씬을 화면에 표시한다.
	D3D::Get()->EndScene();
	



	return true;
}

