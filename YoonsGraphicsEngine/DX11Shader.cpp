#include "DX11Shader.h"
#include "readdata.h"

#include <wrl.h>
using namespace Microsoft::WRL;

DX11Shader::DX11Shader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CreateVertexShader();
	CreatePixelShader();
	CreateConstantBuffer(sizeof(SConstBuffer));
}

DX11Shader::~DX11Shader()
{
	SAFE_RELEASE(m_pConstBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
}

HRESULT DX11Shader::CreateVertexShader()
{
	auto _vertexShaderBytecode = DX::ReadData(L"FrgrttrfPixelShader.cso");

	return m_D3DDevice->CreateVertexShader(
		_vertexShaderBytecode.data(),
		_vertexShaderBytecode.size(),
		nullptr,
		&m_pVertexShader);
}

HRESULT DX11Shader::CreatePixelShader()
{
	auto _pixelShaderBytecode = DX::ReadData(L"DeferredVertexShader.cso");

	return m_D3DDevice->CreatePixelShader(
		_pixelShaderBytecode.data(),
		_pixelShaderBytecode.size(),
		nullptr,
		&m_pPixelShader);
}

/// 특정 크기 만큼의 상수버퍼를 만든다.
HRESULT DX11Shader::CreateConstantBuffer(UINT bufferSize)
{
	ID3D11Buffer* _pConstBuffer = nullptr;
	//m_vConst

	// helper 사용
	CD3D11_BUFFER_DESC _desc;
	_desc.ByteWidth = bufferSize;
	_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//_desc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = m_D3DDevice->CreateBuffer(&_desc, nullptr, &m_pConstBuffer);

	if (FAILED(hr))
	{
		MessageBox(0, L"CreateConstantBuffer Failed.", 0, 0);
	}

	return hr;
}

void DX11Shader::Update()
{
	// 상수 버퍼 업데이트
	m_D3DDeviceContext->UpdateSubresource(m_pConstBuffer, 0, nullptr, &m_ConstBufferStruct, 0, 0);
}

void DX11Shader::Render()
{
	// 쉐이더 셋팅
	m_D3DDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_D3DDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	// 상수 버퍼 셋팅
	m_D3DDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstBuffer);
}
