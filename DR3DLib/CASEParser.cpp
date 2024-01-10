#include "CASEParser.h"


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다.
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다.
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련. 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자.
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	/// 1) 다롱이아부지김교수의 파워풀한 재귀함수 한개로 처리 끝!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1차 변환

// CScenedata 복사
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// 값 복사
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// 구현 제거 (예시용)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// 버텍스들은 일단 모두 복사
	/*for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;
	
		pMesh->m_opt_vertex.push_back(_pVertex);
	}*/

	//파싱하면서 바로 넣기로 수정

	// 인덱스는 그냥 복사

	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	// 나머지는 face를 기준으로 한 인덱스로 찾아야 함
	// 이걸 똑바로 안해서 수박바가 뜨는 거엿음
	// face 기준으로 같은 vertex인데 다른 값을 가지는 경우가 존재함.
	// 근데 아래의 코드를 써도 자꾸 수박바 됨. vertex가 복사가 안되는건가 
	unsigned int _faceCount = pMesh->m_meshface.size();
	TRACE("FaceCount : %d", _faceCount);
	
	//
	//for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	//{
	//	TRACE("FaceNum : %d / %d\n", i, _faceCount);

	//	for (int j = 0; j < 3; j++)
	//	{
	//		Vertex* _nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];
	//		Vector3 _nowNormal = pMesh->m_meshface[i]->m_normalvertex[j];
	//		
	//		// face기준으로 버텍스의 노말을 넣어준다.
	//		
	//		//vertex가 normal값을 가지고 있지 않은 경우,
	//		if (_nowVertex->m_isnormalset == false)
	//		{
	//			//normal값을 넣어주고
	//			_nowVertex->m_normal.x = _nowNormal.x;
	//			_nowVertex->m_normal.y = _nowNormal.y;
	//			_nowVertex->m_normal.z = _nowNormal.z;
	//			
	//			//normal값을 가지고 있다로 변경
	//			_nowVertex->m_isnormalset = true;
	//		}
	//		//vertex가 이미 normal값을 가지고 있는 경우,
	//		else
	//		{
	//			//1. normal이 가르키는 vertex찾기
	//			//int _nowVertexIndex = pMesh->m_meshface[i]->m_normalvertexindex;
	//			
	//			//2. vertex 값 복사
	//			Vertex* _newVertex = _nowVertex->CopyVertex();
	//			
	//			//3. new vertex에 normal 넣기
	//			_newVertex->m_normal.x = _nowNormal.x;
	//			_newVertex->m_normal.y = _nowNormal.y;
	//			_newVertex->m_normal.z = _nowNormal.z;

	//			//4. 새로 만든 vertex를 pushback해주기
	//			pMesh->m_opt_vertex.push_back(_newVertex);

	//			//5. 그 외에 필요한 작업 해주기
	//			
	//		}
	//		
	//	}
	//}
	return FALSE;
}

ASEParser::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

//----------------------------------------------------------------
// 재귀 호출됨을 전제로 하는 분기 함수이다.
//
// 일단 읽고, 토큰에 따라 동작한다.
//
// 재귀를 들어가는 조건은 { 을 만날 경우이다.
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다.
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// 지역변수들 선언
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;
	// 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터.
	int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?
	// 이것은 현 토큰이 어떤 종류인가를 판별해준다.
	// 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	//3번 읽을때마다 넣기 위해 위에 전방선언
	Face* _newFace = nullptr;

	/// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
		// 넘어온 토큰에 따라 처리해준다.

		static int iv = 0;
		static int vertexIndex = 0; //0, 1, 2로 넘기기 위한 새로운 변수 출현!

		switch (nowtoken)
		{
			case TOKEND_BLOCK_START:

				Parsing_DivergeRecursiveALL(depth++);
				break;

			case TOKENR_HELPER_CLASS:
				break;

				//--------------------
				// 3DSMAX_ASCIIEXPORT
				//--------------------

			case TOKENR_3DSMAX_ASCIIEXPORT:
				m_data_asciiexport = Parsing_NumberLong();
				break;

				//--------------------
				// COMMENT
				//--------------------

			case TOKENR_COMMENT:
				Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐.
				//AfxMessageBox( m_TokenString, NULL, NULL);		/// 임시로 코멘트를 출력해본다
				break;

				//--------------------
				// SCENE
				//--------------------

			case TOKENR_SCENE:
				//
				break;
			case TOKENR_SCENE_FILENAME:
				m_scenedata.m_filename = Parsing_String();		// 일관성 있는 함수의 사용을 위해 String과 Int도 만들어줬다.
				break;
			case TOKENR_SCENE_FIRSTFRAME:
				m_scenedata.m_firstframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_LASTFRAME:
				m_scenedata.m_lastframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_FRAMESPEED:
				m_scenedata.m_framespeed = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_TICKSPERFRAME:
				m_scenedata.m_ticksperframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_MESHFRAMESTEP:
				m_scenedata.m_meshframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_KEYFRAMESTEP:
				m_scenedata.m_keyframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_BACKGROUND_STATIC:
				m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
				break;
			case TOKENR_SCENE_AMBIENT_STATIC:
				m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
				break;

			case TOKENR_SCENE_ENVMAP:
			{
				// 그냥 안의 내용을 읽어버린다 }가 나올때까지
				while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
				}
			}
			break;

			//--------------------
			// MATERIAL_LIST
			//--------------------


			//--------------------
			// GEOMOBJECT
			//--------------------

			case TOKENR_GROUP:
				//	한 개의 그룹 시작. 이 다음에 이름이 스트링으로 나오기는 하는데.
				break;

			case TOKENR_HELPEROBJECT:
				// 일단 생성하고
				// 오브젝트의 타입 정해줌. 이것에 따라 서로 다른 파싱 모드 발동.
				break;

			case TOKENR_GEOMOBJECT:
				/// 이 토큰을 만났다는건 새로운 메시가 생겼다는 것이다. 
				///지역 변수로 mesh를 하나 선언, 
				/// 그 포인터를 리스트에 넣고, 
				/// m_onemesh에 그 포인터를 복사, 그대로 쓰면 될까?
				/// 라는 역할을 하는 뭔가를 만들어야 된다는 뜻인가?
				
				Create_onemesh_to_list();

				break;

			case TOKENR_NODE_NAME:
				// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
				// 모드에 따라 넣어야 할 곳이 다르다.
				m_OneMesh->m_nodename = Parsing_String();
				break;

			case TOKENR_NODE_PARENT:
				// 현 노드의 부모 노드의 정보.
				// 일단 입력을 하고, 나중에 정리하자.
				m_OneMesh->m_nodeparent = Parsing_String();

				break;

				/// NODE_TM

			case TOKENR_NODE_TM:
				m_parsingmode =	eGeomobject;
				// (NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다.)
				// (Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다)

				/// 게다가,
				// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
				// 아예 이 재귀에서 리턴을 시키고 싶지만. 중간에 읽는것을 무시해야 하기 때문에...
				//if (m_onemesh->m_camera_isloadTarget) {
				//	'}'이 나올때까지 나오는건 무조건 무시! 뭐 이런거 안되나..
				// 재귀구조의 문제점이다....

				break;

			case TOKENR_INHERIT_POS:
				// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
				break;
			case TOKENR_INHERIT_ROT:
				break;
			case TOKENR_INHERIT_SCL:
				break;
			case TOKENR_TM_ROW0:
				break;
			case TOKENR_TM_ROW1:
				break;
			case TOKENR_TM_ROW2:
				break;
			case TOKENR_TM_ROW3:
				break;
			case TOKENR_TM_POS:
				break;
			case TOKENR_TM_ROTAXIS:
				break;
			case TOKENR_TM_ROTANGLE:
				break;
			case TOKENR_TM_SCALE:
				break;
			case TOKENR_TM_SCALEAXIS:
				break;
			case TOKENR_TM_SCALEAXISANG:
				// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
				break;


				/// MESH

			case TOKENR_MESH:
			{
				/// 아래의 함수에서 m_OneMesh가 생긴다.
				///근데 저거 하면 vector error 생김. 예?
				/// 왜냐면 위에서 이미 한번 불러줬기 때문이다. 
				/// 두 번 불러줄 필요가 없어요..
				//Create_onemesh_to_list();
			}
			break;

			case TOKENR_TIMEVALUE:
			{
				///int _timeValue = m_lexer->GetToken(m_TokenString);
				m_OneMesh->m_timevalue = Parsing_NumberInt();
			}
			break;

			case TOKENR_MESH_NUMBONE:
				// 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
				m_OneMesh->m_numbone = Parsing_NumberInt();
				break;
			
			case TOKENR_MESH_NUMSKINWEIGHT: 
				m_OneMesh->m_numskinweight = Parsing_NumberInt();
				break;

			case TOKENR_MESH_NUMVERTEX:
				m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
				break;

			case TOKENR_MESH_NUMFACES:
				m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
				break;

				/// MESH_VERTEX_LIST

			case TOKENR_MESH_VERTEX_LIST:
				//
				// 버텍스의 값들을 집어넣어야 하는데
				// 이미 벡터로 선언이 돼 있으므로 그냥 넣으면 된다.
				break;
			case TOKENR_MESH_VERTEX:
			{
				// 여기서 읽어야 할 데이터의 구조
				// *MESH_VERTEX    0	-0.5000	-0.5000	0.0000
				
				//몇 번째 버텍스인지 읽어옴
				int idx = Parsing_NumberInt();
				
				//새로운 버텍스 구조체를 만들어주기
				Vertex* _newVertex = new Vertex();

				//몇 번째 값, x y z 에 각각 알맞은 값을 넣어줌
				//x y z의 순서가 아니라 x z y의 순으로 읽어야 됨.
				//_newVertex->m_indices = idx;
				_newVertex->m_pos.x = Parsing_NumberFloat();
				_newVertex->m_pos.z = Parsing_NumberFloat();
				_newVertex->m_pos.y = Parsing_NumberFloat();


				//벡터에 넣어주기
				m_OneMesh->m_meshvertex.push_back(_newVertex);
			}
				break;

				/// Bone

			case TOKENR_SKIN_INITTM:
				break;
			case TOKENR_BONE_LIST:
				break;
			case TOKENR_BONE:
			{
				/// 모드 체인지 해 주고, Bone을 소유하고 있다는 것은 이것은 스키닝 오브젝트라는 것이다.
				// 본 하나를 만들어서 임시 포인터 보관, 벡터에 넣고
				// Bone의 넘버를 읽어 주자
			}
			break;
			//이 다음에 본의 이름을 넣어야 한다. 하지만 {를 한 개 더 열었으므로 임시 포인터 변수로서 보관해야겠지.
			case TOKENR_BONE_NAME:
			case TOKENR_BONE_PROPERTY:
				// 이 다음 ABSOLUTE가 나오기는 하는데, 쓸 일이 없다.
				break;
				// 다음에는 TM_ROW0~3이 나오는데 역시 무시됨..

			case TOKENR_MESH_WVERTEXS:
				break;

			case TOKENR_MESH_WEIGHT:
			{
				// 버텍스 하나의 정보를 만들어서 리스트에 넣음
			}
			break;
			case TOKENR_BONE_BLENGING_WEIGHT:
			{
				// 대체 몇 단계를 들어가는거야...
				// 가중치 한개를 만들어서 리스트에 넣는다
				/// 헥 헥....
			}
			break;


			/// MESH_FACE_LIST
			case TOKENR_MESH_FACE_LIST:
				//
				break;
			case TOKENR_MESH_FACE:
			{
				///여기서 face의 index를 따로 읽어도 되지만
				///normal값의 face들을 한번 더 재정의 해준다.
				///따라서 여기서 읽지 말고 아래에서 읽은 face들을 이용해 index버퍼를 만들어줘도 됨.

				////여기서 읽어야 할 데이터의 모양.
				////*MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 1
				//
				////face 저장할 새로운 face구조체 생성
				//Face* face = new Face();

				//// Face의 번호인데...
				//Parsing_String();	// 0: 읽는 용도

				//// A:를 읽고
				//Parsing_String();
				////뒤에 있는 숫자 읽어오기
				//face->m_vertexindex[0] = Parsing_NumberInt();

				//// B:를 읽고
				//Parsing_String();
				////뒤에 있는 숫자 읽어오기
				//face->m_vertexindex[1] = Parsing_NumberInt();
				//
				//// C:를 읽고
				//Parsing_String();
				////뒤에 잇는 숫자 읽어오기
				//face->m_vertexindex[2] = Parsing_NumberInt();

				///// (뒤에 정보가 더 있지만 default에 의해 스킵될 것이다.)
				///// ......

				//// 벡터에 넣어준다.
				//m_OneMesh->m_meshface.push_back(face);
			}
			break;

			///텍스쳐부분!
			case TOKENR_MESH_NUMTVERTEX:
				m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
				break;

				/// MESH_TVERTLIST

			case TOKENR_MESH_TVERTLIST:
			
				break;
			case TOKENR_MESH_TVERT:
			{
				// 버텍스의 인덱스가 나오는데 어차피 순서와 같으므로 버린다.
				// 새로운 TVertex를 만들어서 벡터에 넣는다
				// 일단 여기는 확실함.
				
				// 여기서 uv를 받아와야 함 (w는 지금 사용하지 않음)
				// *MESH_TVERT 0	0.0000	0.0000	0.0000
				int idx = Parsing_NumberInt();

				COneTVertex* _newtVertex = new COneTVertex();
				_newtVertex->m_u = Parsing_NumberFloat();
				_newtVertex->m_v = Parsing_NumberFloat();
				//지금은 쓰지 않음
				//_newtVertex->m_w = Parsing_NumberFloat();

				m_OneMesh->m_mesh_tvertex.push_back(_newtVertex);

			}
			break;
			
			case TOKENR_MESH_TFACE:
			{
				//읽어와야 되는 데이터
				//*MESH_TFACE 0	9	11	10
				
				//새로운 face생성
				//일단 파일 읽는 순서상 여기가 먼저가 맞음.
				_newFace = new Face();
				
				int num = Parsing_NumberInt();

				//여기서 0~2번까지 받아서
				_newFace->m_TFace[0] = Parsing_NumberInt();
				_newFace->m_TFace[1] = Parsing_NumberInt();
				_newFace->m_TFace[2] = Parsing_NumberInt();
				
				//m_meshface 벡터 집어넣기
				m_OneMesh->m_meshface.push_back(_newFace);
				
			}
				break;

			case TOKENR_MESH_FACENORMAL:
			{
				// *MESH_FACENORMAL 0	0.0000	0.0000	-1.0000			
				iv = Parsing_NumberInt();	//0

				//위에서 face를 이미 만들어서 벡터에 넣어뒀으니 우리는 그걸 쓰면 된다.
				//받아서 넣어두자.

				m_OneMesh->m_meshface[iv]->m_normal.x = Parsing_NumberFloat();
				m_OneMesh->m_meshface[iv]->m_normal.z = Parsing_NumberFloat();
				m_OneMesh->m_meshface[iv]->m_normal.y = Parsing_NumberFloat();

			}
				break;

			case TOKENR_MESH_VERTEXNORMAL:
			{
				//여기는 왜 3개나 있어서 날 힘들게 하냐 데이터 이렇게 만들어야 됫냐 진짜

				//*MESH_VERTEXNORMAL 0	0.0000	0.0000	-1.0000
				//*MESH_VERTEXNORMAL 2	0.0000	0.0000 - 1.0000
				//*MESH_VERTEXNORMAL 3	0.0000	0.0000 - 1.0000
				// idx는 그 vertex의 index를 의미 
				//한 facenormal에는 3개의 vertexnormal값을 받아야 함.

				//같은 vertex의 값을 가지더라도 normal의 값이 다를 수 있다. 
				//따라서 vertex split이 필요하다.

				//몇 번째 vertex인지 index를 읽어온다
				int idx = Parsing_NumberInt();	//0 or 2 or 3 이거 vertex split 해줘야 함.

				//모든 행동을 처리하기 전에 vertex의 index를 확인해야 한다.
				if (vertexIndex == 3)
				{
					//vertexIndex가 3이라는 뜻은 face의 normal값을 다 채워뒀다는 뜻.(0,1,2)
					//따라서 새로운 face에 넣어줘야 하니 vertexIndex의 값을 0으로 만들고
					vertexIndex = 0;
					//faces의 갯수를 ++ 해준다.
					//m_OneMesh->m_mesh_numfaces++;
				}

				//vertex split을 위해 새로운 vertex를 만들어준다.
				Vertex* _vertex = new Vertex();

				//미리 저장해둔 vertex의 pos를 받아오기 위해 m_OneMesh에 저장되어있는 vertex를 불러온다.
				_vertex->m_pos.x = m_OneMesh->m_meshvertex[idx]->m_pos.x;
				_vertex->m_pos.y = m_OneMesh->m_meshvertex[idx]->m_pos.y;
				_vertex->m_pos.z = m_OneMesh->m_meshvertex[idx]->m_pos.z;


				//각 x, y ,z의 normal값을 읽어온다.
				_vertex->m_normal.x = Parsing_NumberFloat();
				_vertex->m_normal.z = Parsing_NumberFloat();
				_vertex->m_normal.y = Parsing_NumberFloat();


				/// u, v 넣어주기.
				/// FACENORMAL 에서 받아온 면 번호로 해당 면을 m_meshface 에서 찾고, 거기 있는 텍스쳐 인덱스 값을 순차적으로 받아와 집어 넣는다
				/// 이거 맞냐
				_vertex->u = (m_OneMesh->m_mesh_tvertex[(m_OneMesh->m_meshface[iv])->m_TFace[vertexIndex]])->m_u;
				_vertex->v = m_OneMesh->m_mesh_tvertex[(m_OneMesh->m_meshface[iv])->m_TFace[vertexIndex]]->m_v;

				//vertex vector에 넣어준다.
				m_OneMesh->m_opt_vertex.push_back(_vertex);
				m_OneMesh->m_mesh_numvertex++;


				/// 인덱스  
				/// 파일 순서가 면 -> 해당 면을 구성하는 점들 셋이기 때문에 크기 - 1이 곧 해당 점의 새로운 번호
				(m_OneMesh->m_meshface[iv])->m_vertexindex[vertexIndex] = m_OneMesh->m_opt_vertex.size() - 1;
				vertexIndex++;
			}
				break;

			///가장 아래에 있는 데이터들. 값은 0또는 1을 나타내는듯.
			case TOKENR_PROP_MOTIONBLUR:
			{
				m_OneMesh->m_prop_motionblur = Parsing_NumberInt();
				break;
			}
			case TOKENR_PROP_CASTSHADOW:
			{
				m_OneMesh->m_prop_castshadow = Parsing_NumberInt();
				break;
			}
			case TOKENR_PROP_RECVSHADOW:
			{
				m_OneMesh->m_prop_recvshadow = Parsing_NumberInt();
				break;
			}
			case TOKENR_MATERIAL_REF:
			{
				m_OneMesh->m_material_ref = Parsing_NumberInt();
				break;
			}

			case TOKEND_END:
				// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

				//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
				TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
				return;

				break;

				/// 위의 아무것도 해당하지 않을때
			default:
				// 아무것도 하지 않는다.
				break;

		}	// switch()


		///-----------------------------------------------
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			// 루프를 1000000번이상이나 돌 이유가 없다. (데이터가 100000개가 아닌이상)
			// 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		/// 안전 코드.
		///-----------------------------------------------

	}		// while()

	// 여기까지 왔다면 while()을 벗어났다는 것이고 그 말은
	// 괄호를 닫았다는 것이므로
	// 리턴하자 (재귀함수)

	return;
}


///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3개의 Float를 벡터 하나로
Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
}

///--------------------------------------------------
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// 클래스간 값 복사
	m_MeshList.push_back(m_OneMesh);
}

// 메트리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// 애니메이션데이터 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// 정점 하나를..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}

















