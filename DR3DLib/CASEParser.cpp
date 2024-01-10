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
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// 1) �ٷ��̾ƺ����豳���� �Ŀ�Ǯ�� ����Լ� �Ѱ��� ó�� ��!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// ���� ���� (���ÿ�)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// ���ؽ����� �ϴ� ��� ����
	/*for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;
	
		pMesh->m_opt_vertex.push_back(_pVertex);
	}*/

	//�Ľ��ϸ鼭 �ٷ� �ֱ�� ����

	// �ε����� �׳� ����

	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	// �������� face�� �������� �� �ε����� ã�ƾ� ��
	// �̰� �ȹٷ� ���ؼ� ���ڹٰ� �ߴ� �ſ���
	// face �������� ���� vertex�ε� �ٸ� ���� ������ ��찡 ������.
	// �ٵ� �Ʒ��� �ڵ带 �ᵵ �ڲ� ���ڹ� ��. vertex�� ���簡 �ȵǴ°ǰ� 
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
	//		// face�������� ���ؽ��� �븻�� �־��ش�.
	//		
	//		//vertex�� normal���� ������ ���� ���� ���,
	//		if (_nowVertex->m_isnormalset == false)
	//		{
	//			//normal���� �־��ְ�
	//			_nowVertex->m_normal.x = _nowNormal.x;
	//			_nowVertex->m_normal.y = _nowNormal.y;
	//			_nowVertex->m_normal.z = _nowNormal.z;
	//			
	//			//normal���� ������ �ִٷ� ����
	//			_nowVertex->m_isnormalset = true;
	//		}
	//		//vertex�� �̹� normal���� ������ �ִ� ���,
	//		else
	//		{
	//			//1. normal�� ����Ű�� vertexã��
	//			//int _nowVertexIndex = pMesh->m_meshface[i]->m_normalvertexindex;
	//			
	//			//2. vertex �� ����
	//			Vertex* _newVertex = _nowVertex->CopyVertex();
	//			
	//			//3. new vertex�� normal �ֱ�
	//			_newVertex->m_normal.x = _nowNormal.x;
	//			_newVertex->m_normal.y = _nowNormal.y;
	//			_newVertex->m_normal.z = _nowNormal.z;

	//			//4. ���� ���� vertex�� pushback���ֱ�
	//			pMesh->m_opt_vertex.push_back(_newVertex);

	//			//5. �� �ܿ� �ʿ��� �۾� ���ֱ�
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
// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
//
// �ϴ� �а�, ��ū�� ���� �����Ѵ�.
//
// ��͸� ���� ������ { �� ���� ����̴�.
// �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ���̴�.
//
// ���Ҿ�, ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� � ��������� �����Ѵ�.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// ���������� ����
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;
	// ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
	int listcount = 0;		// �ſ� C����� �ٸ� �ذ�å�� ������?
	// �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
	// �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	//3�� ���������� �ֱ� ���� ���� ���漱��
	Face* _newFace = nullptr;

	/// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
		// �Ѿ�� ��ū�� ���� ó�����ش�.

		static int iv = 0;
		static int vertexIndex = 0; //0, 1, 2�� �ѱ�� ���� ���ο� ���� ����!

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
				Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
				//AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
				break;

				//--------------------
				// SCENE
				//--------------------

			case TOKENR_SCENE:
				//
				break;
			case TOKENR_SCENE_FILENAME:
				m_scenedata.m_filename = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
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
				// �׳� ���� ������ �о������ }�� ���ö�����
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
				//	�� ���� �׷� ����. �� ������ �̸��� ��Ʈ������ ������� �ϴµ�.
				break;

			case TOKENR_HELPEROBJECT:
				// �ϴ� �����ϰ�
				// ������Ʈ�� Ÿ�� ������. �̰Ϳ� ���� ���� �ٸ� �Ľ� ��� �ߵ�.
				break;

			case TOKENR_GEOMOBJECT:
				/// �� ��ū�� �����ٴ°� ���ο� �޽ð� ����ٴ� ���̴�. 
				///���� ������ mesh�� �ϳ� ����, 
				/// �� �����͸� ����Ʈ�� �ְ�, 
				/// m_onemesh�� �� �����͸� ����, �״�� ���� �ɱ�?
				/// ��� ������ �ϴ� ������ ������ �ȴٴ� ���ΰ�?
				
				Create_onemesh_to_list();

				break;

			case TOKENR_NODE_NAME:
				// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
				// ��忡 ���� �־�� �� ���� �ٸ���.
				m_OneMesh->m_nodename = Parsing_String();
				break;

			case TOKENR_NODE_PARENT:
				// �� ����� �θ� ����� ����.
				// �ϴ� �Է��� �ϰ�, ���߿� ��������.
				m_OneMesh->m_nodeparent = Parsing_String();

				break;

				/// NODE_TM

			case TOKENR_NODE_TM:
				m_parsingmode =	eGeomobject;
				// (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
				// (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)

				/// �Դٰ�,
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
				// �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
				//if (m_onemesh->m_camera_isloadTarget) {
				//	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
				// ��ͱ����� �������̴�....

				break;

			case TOKENR_INHERIT_POS:
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
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
				// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
				break;


				/// MESH

			case TOKENR_MESH:
			{
				/// �Ʒ��� �Լ����� m_OneMesh�� �����.
				///�ٵ� ���� �ϸ� vector error ����. ��?
				/// �ֳĸ� ������ �̹� �ѹ� �ҷ���� �����̴�. 
				/// �� �� �ҷ��� �ʿ䰡 �����..
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
				// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
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
				// ���ؽ��� ������ ����־�� �ϴµ�
				// �̹� ���ͷ� ������ �� �����Ƿ� �׳� ������ �ȴ�.
				break;
			case TOKENR_MESH_VERTEX:
			{
				// ���⼭ �о�� �� �������� ����
				// *MESH_VERTEX    0	-0.5000	-0.5000	0.0000
				
				//�� ��° ���ؽ����� �о��
				int idx = Parsing_NumberInt();
				
				//���ο� ���ؽ� ����ü�� ������ֱ�
				Vertex* _newVertex = new Vertex();

				//�� ��° ��, x y z �� ���� �˸��� ���� �־���
				//x y z�� ������ �ƴ϶� x z y�� ������ �о�� ��.
				//_newVertex->m_indices = idx;
				_newVertex->m_pos.x = Parsing_NumberFloat();
				_newVertex->m_pos.z = Parsing_NumberFloat();
				_newVertex->m_pos.y = Parsing_NumberFloat();


				//���Ϳ� �־��ֱ�
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
				/// ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
				// �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
				// Bone�� �ѹ��� �о� ����
			}
			break;
			//�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
			case TOKENR_BONE_NAME:
			case TOKENR_BONE_PROPERTY:
				// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
				break;
				// �������� TM_ROW0~3�� �����µ� ���� ���õ�..

			case TOKENR_MESH_WVERTEXS:
				break;

			case TOKENR_MESH_WEIGHT:
			{
				// ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
			}
			break;
			case TOKENR_BONE_BLENGING_WEIGHT:
			{
				// ��ü �� �ܰ踦 ���°ž�...
				// ����ġ �Ѱ��� ���� ����Ʈ�� �ִ´�
				/// �� ��....
			}
			break;


			/// MESH_FACE_LIST
			case TOKENR_MESH_FACE_LIST:
				//
				break;
			case TOKENR_MESH_FACE:
			{
				///���⼭ face�� index�� ���� �о ������
				///normal���� face���� �ѹ� �� ������ ���ش�.
				///���� ���⼭ ���� ���� �Ʒ����� ���� face���� �̿��� index���۸� ������൵ ��.

				////���⼭ �о�� �� �������� ���.
				////*MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 1
				//
				////face ������ ���ο� face����ü ����
				//Face* face = new Face();

				//// Face�� ��ȣ�ε�...
				//Parsing_String();	// 0: �д� �뵵

				//// A:�� �а�
				//Parsing_String();
				////�ڿ� �ִ� ���� �о����
				//face->m_vertexindex[0] = Parsing_NumberInt();

				//// B:�� �а�
				//Parsing_String();
				////�ڿ� �ִ� ���� �о����
				//face->m_vertexindex[1] = Parsing_NumberInt();
				//
				//// C:�� �а�
				//Parsing_String();
				////�ڿ� �մ� ���� �о����
				//face->m_vertexindex[2] = Parsing_NumberInt();

				///// (�ڿ� ������ �� ������ default�� ���� ��ŵ�� ���̴�.)
				///// ......

				//// ���Ϳ� �־��ش�.
				//m_OneMesh->m_meshface.push_back(face);
			}
			break;

			///�ؽ��ĺκ�!
			case TOKENR_MESH_NUMTVERTEX:
				m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
				break;

				/// MESH_TVERTLIST

			case TOKENR_MESH_TVERTLIST:
			
				break;
			case TOKENR_MESH_TVERT:
			{
				// ���ؽ��� �ε����� �����µ� ������ ������ �����Ƿ� ������.
				// ���ο� TVertex�� ���� ���Ϳ� �ִ´�
				// �ϴ� ����� Ȯ����.
				
				// ���⼭ uv�� �޾ƿ;� �� (w�� ���� ������� ����)
				// *MESH_TVERT 0	0.0000	0.0000	0.0000
				int idx = Parsing_NumberInt();

				COneTVertex* _newtVertex = new COneTVertex();
				_newtVertex->m_u = Parsing_NumberFloat();
				_newtVertex->m_v = Parsing_NumberFloat();
				//������ ���� ����
				//_newtVertex->m_w = Parsing_NumberFloat();

				m_OneMesh->m_mesh_tvertex.push_back(_newtVertex);

			}
			break;
			
			case TOKENR_MESH_TFACE:
			{
				//�о�;� �Ǵ� ������
				//*MESH_TFACE 0	9	11	10
				
				//���ο� face����
				//�ϴ� ���� �д� ������ ���Ⱑ ������ ����.
				_newFace = new Face();
				
				int num = Parsing_NumberInt();

				//���⼭ 0~2������ �޾Ƽ�
				_newFace->m_TFace[0] = Parsing_NumberInt();
				_newFace->m_TFace[1] = Parsing_NumberInt();
				_newFace->m_TFace[2] = Parsing_NumberInt();
				
				//m_meshface ���� ����ֱ�
				m_OneMesh->m_meshface.push_back(_newFace);
				
			}
				break;

			case TOKENR_MESH_FACENORMAL:
			{
				// *MESH_FACENORMAL 0	0.0000	0.0000	-1.0000			
				iv = Parsing_NumberInt();	//0

				//������ face�� �̹� ���� ���Ϳ� �־������ �츮�� �װ� ���� �ȴ�.
				//�޾Ƽ� �־����.

				m_OneMesh->m_meshface[iv]->m_normal.x = Parsing_NumberFloat();
				m_OneMesh->m_meshface[iv]->m_normal.z = Parsing_NumberFloat();
				m_OneMesh->m_meshface[iv]->m_normal.y = Parsing_NumberFloat();

			}
				break;

			case TOKENR_MESH_VERTEXNORMAL:
			{
				//����� �� 3���� �־ �� ����� �ϳ� ������ �̷��� ������ �̳� ��¥

				//*MESH_VERTEXNORMAL 0	0.0000	0.0000	-1.0000
				//*MESH_VERTEXNORMAL 2	0.0000	0.0000 - 1.0000
				//*MESH_VERTEXNORMAL 3	0.0000	0.0000 - 1.0000
				// idx�� �� vertex�� index�� �ǹ� 
				//�� facenormal���� 3���� vertexnormal���� �޾ƾ� ��.

				//���� vertex�� ���� �������� normal�� ���� �ٸ� �� �ִ�. 
				//���� vertex split�� �ʿ��ϴ�.

				//�� ��° vertex���� index�� �о�´�
				int idx = Parsing_NumberInt();	//0 or 2 or 3 �̰� vertex split ����� ��.

				//��� �ൿ�� ó���ϱ� ���� vertex�� index�� Ȯ���ؾ� �Ѵ�.
				if (vertexIndex == 3)
				{
					//vertexIndex�� 3�̶�� ���� face�� normal���� �� ä���״ٴ� ��.(0,1,2)
					//���� ���ο� face�� �־���� �ϴ� vertexIndex�� ���� 0���� �����
					vertexIndex = 0;
					//faces�� ������ ++ ���ش�.
					//m_OneMesh->m_mesh_numfaces++;
				}

				//vertex split�� ���� ���ο� vertex�� ������ش�.
				Vertex* _vertex = new Vertex();

				//�̸� �����ص� vertex�� pos�� �޾ƿ��� ���� m_OneMesh�� ����Ǿ��ִ� vertex�� �ҷ��´�.
				_vertex->m_pos.x = m_OneMesh->m_meshvertex[idx]->m_pos.x;
				_vertex->m_pos.y = m_OneMesh->m_meshvertex[idx]->m_pos.y;
				_vertex->m_pos.z = m_OneMesh->m_meshvertex[idx]->m_pos.z;


				//�� x, y ,z�� normal���� �о�´�.
				_vertex->m_normal.x = Parsing_NumberFloat();
				_vertex->m_normal.z = Parsing_NumberFloat();
				_vertex->m_normal.y = Parsing_NumberFloat();


				/// u, v �־��ֱ�.
				/// FACENORMAL ���� �޾ƿ� �� ��ȣ�� �ش� ���� m_meshface ���� ã��, �ű� �ִ� �ؽ��� �ε��� ���� ���������� �޾ƿ� ���� �ִ´�
				/// �̰� �³�
				_vertex->u = (m_OneMesh->m_mesh_tvertex[(m_OneMesh->m_meshface[iv])->m_TFace[vertexIndex]])->m_u;
				_vertex->v = m_OneMesh->m_mesh_tvertex[(m_OneMesh->m_meshface[iv])->m_TFace[vertexIndex]]->m_v;

				//vertex vector�� �־��ش�.
				m_OneMesh->m_opt_vertex.push_back(_vertex);
				m_OneMesh->m_mesh_numvertex++;


				/// �ε���  
				/// ���� ������ �� -> �ش� ���� �����ϴ� ���� ���̱� ������ ũ�� - 1�� �� �ش� ���� ���ο� ��ȣ
				(m_OneMesh->m_meshface[iv])->m_vertexindex[vertexIndex] = m_OneMesh->m_opt_vertex.size() - 1;
				vertexIndex++;
			}
				break;

			///���� �Ʒ��� �ִ� �����͵�. ���� 0�Ǵ� 1�� ��Ÿ���µ�.
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
				// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

				//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
				TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
				return;

				break;

				/// ���� �ƹ��͵� �ش����� ������
			default:
				// �ƹ��͵� ���� �ʴ´�.
				break;

		}	// switch()


		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	// ������� �Դٸ� while()�� ����ٴ� ���̰� �� ����
	// ��ȣ�� �ݾҴٴ� ���̹Ƿ�
	// �������� (����Լ�)

	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
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
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
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


// 3���� Float�� ���� �ϳ���
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

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// ���� �ϳ���..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}

















