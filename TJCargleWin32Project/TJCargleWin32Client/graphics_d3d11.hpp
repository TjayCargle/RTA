#pragma once
#include "graphics.h"
#include "../DDSTextureLoader.h"
#include "window.h"



#include <d2d1_2.h>
#include <d3d11_2.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
ID3D11Buffer * GPUSideVertexBuffer = nullptr;
ID3D11Buffer * GPUSideIndexBuffer = nullptr;
ID3D11Buffer * GPUSideConstantBuffer = nullptr;
ID3D11SamplerState * m_sampler;
TJMatrix camera;
TJMatrix view;
TJMatrix proj;
TJMVPBuffer myCVPbuffer;
int gridVertCount;
bool AutoAnimate = true;
std::vector<int> animationFrameNum;
std::vector<VERTEX> gridVerts;
std::vector<Mesh *> myMeshes;

std::vector<int> myFrameNumbers;
namespace
{

	const color4f red = { 1.0f, 0.0f, 0.0f, 1.0f };
	const color4f green = { 0.0f, 1.0f, 0.0f, 1.0f };
	const color4f blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	const color4f yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	const color4f cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	const color4f magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	const color4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	const color4f black = { 0.0f, 0.0f, 0.0f, 1.0f };



}




namespace fsgd
{
	class d3d11_device_t : public graphics::device_t
	{
		friend class graphics;

		class d3d11_context_t : public graphics::device_t::context_t
		{
		public:

			ID3D11DeviceContext *devcon;

			ID3D11DeviceContext* operator->() { return devcon; }

			virtual void finalize() {	devcon->Release();}

		};

		window* win;

		void init_depth_stencil_state();
		void init_dev_con_and_swap();
		void init_depth_stencil_buffer();
		void init_depth_stencil_view();
		void init_rasterizer_state();
		void tj_rasterizer_state();
		void init_viewport();
		void init_shaders_and_input_layout();
		void init_default_graphics();



		D3D11_VIEWPORT viewport;

		struct pipeline_state_t
		{
			ID3D11InputLayout *input_layout;
			ID3D11VertexShader *vertex_shader;
			ID3D11PixelShader * pixel_shader;
			ID3D11RenderTargetView *render_target;
			ID3D11Texture2D *depthStencilBuffer;
			ID3D11DepthStencilState *depthStencilState;
			ID3D11DepthStencilView *depthStencilView;
			ID3D11RasterizerState *rasterState;
			ID3D11RasterizerState *tjrasterState;
		}default_pipeline;
		pipeline_state_t texturePipline;
		void set_pipeline(pipeline_state_t* p);

		ID3D11Device *dev;
		IDXGISwapChain *swapchain;
		ID3D11Buffer *default_vert_buffer;
		ID3D11Texture2D	*	m_texture2d;
		ID3D11Texture2D	*	m_texture2d2;
		ID3D11ShaderResourceView *	m_shaderView;
		ID3D11ShaderResourceView *	m_shaderView2;
		ID3D11SamplerState *		m_sampleState;
		d3d11_context_t d3d11_context;
	public:

		void UpdateGrid();
		void UpdateMesh(Mesh * someMesh, int bufferIndex, int frameNumber);
		void UpdateTexture(Mesh * someMesh);
		void UpdateBones(Mesh *someMesh, int bufferIndex, int frameNumber);
		void UpdateJoints(Mesh * someMesh, int bufferIndex, int frameNumber);
		void initialize(window*);
		void finalize();
		context_t* get_context() { return &d3d11_context; }
		const context_t* get_context()const { return &d3d11_context; }

		ID3D11Device* operator->() { return dev; }

	}d3d11_device;

	void d3d11_device_t::initialize(window* w)
	{
		win = w;

		init_dev_con_and_swap();

		init_depth_stencil_buffer();

		init_depth_stencil_state();

		init_depth_stencil_view();

		init_rasterizer_state();
		tj_rasterizer_state();
		init_viewport();

		init_shaders_and_input_layout();

		init_default_graphics();

		set_pipeline(&default_pipeline);

		d3d11_context->RSSetViewports(1, &viewport);
	}

	void d3d11_device_t::set_pipeline(pipeline_state_t* p)
	{
		d3d11_context->OMSetDepthStencilState(p->depthStencilState, 1);
		d3d11_context->OMSetRenderTargets(1, &p->render_target, p->depthStencilView);
		d3d11_context->RSSetState(p->rasterState);
		d3d11_context->VSSetShader(p->vertex_shader, 0, 0);
		d3d11_context->PSSetShader(p->pixel_shader, 0, 0);
		d3d11_context->IASetInputLayout(p->input_layout);
	}

	void d3d11_device_t::init_dev_con_and_swap()
	{
		const uint32_t screenWidth = win->get_properties().width;
		const uint32_t screenHeight = win->get_properties().height;

		DXGI_SWAP_CHAIN_DESC scd;

		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferCount = 1;
		scd.BufferDesc.Width = screenWidth;
		scd.BufferDesc.Height = screenHeight;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = HWND(win->get_native_handle());
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Windowed = TRUE;

		D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_1;

		D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			NULL,
			&feature_level,
			1,
			D3D11_SDK_VERSION,
			&scd,
			&swapchain,
			&dev,
			NULL,
			&d3d11_context.devcon);

		ID3D11Texture2D *pBackBuffer;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		dev->CreateRenderTargetView(pBackBuffer, NULL, &default_pipeline.render_target);
		dev->CreateRenderTargetView(pBackBuffer, NULL, &texturePipline.render_target);
		pBackBuffer->Release();
		pBackBuffer = nullptr;
	}

	void d3d11_device_t::init_depth_stencil_buffer()
	{
		const uint32_t screenWidth = win->get_properties().width;
		const uint32_t screenHeight = win->get_properties().height;

		D3D11_TEXTURE2D_DESC depthBufferDesc;

		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = screenWidth;
		depthBufferDesc.Height = screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		auto result = dev->CreateTexture2D(&depthBufferDesc, NULL, &default_pipeline.depthStencilBuffer);
		auto result2 = dev->CreateTexture2D(&depthBufferDesc, NULL, &texturePipline.depthStencilBuffer);
	}

	void d3d11_device_t::init_depth_stencil_state()
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT result = dev->CreateDepthStencilState(&depthStencilDesc, &default_pipeline.depthStencilState);
		HRESULT result2 = dev->CreateDepthStencilState(&depthStencilDesc, &texturePipline.depthStencilState);
	}

	void d3d11_device_t::init_depth_stencil_view()
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HRESULT result = dev->CreateDepthStencilView(default_pipeline.depthStencilBuffer, &depthStencilViewDesc, &default_pipeline.depthStencilView);
		HRESULT result2 = dev->CreateDepthStencilView(texturePipline.depthStencilBuffer, &depthStencilViewDesc, &texturePipline.depthStencilView);
	}

	void d3d11_device_t::init_rasterizer_state()
	{
		D3D11_RASTERIZER_DESC rasterDesc;

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_FRONT;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		HRESULT result = dev->CreateRasterizerState(&rasterDesc, &default_pipeline.rasterState);
		HRESULT result2 = dev->CreateRasterizerState(&rasterDesc, &texturePipline.rasterState);
	}
	void d3d11_device_t::tj_rasterizer_state()
	{
		D3D11_RASTERIZER_DESC rasterDesc;

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_FRONT;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		HRESULT result = dev->CreateRasterizerState(&rasterDesc, &default_pipeline.tjrasterState);
		HRESULT result2 = dev->CreateRasterizerState(&rasterDesc, &texturePipline.tjrasterState);
	}
	void d3d11_device_t::init_viewport()
	{
		const uint32_t screenWidth = win->get_properties().width;
		const uint32_t screenHeight = win->get_properties().height;

		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = FLOAT(screenWidth);
		viewport.Height = FLOAT(screenHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	void d3d11_device_t::finalize()
	{
		default_vert_buffer->Release();
		default_pipeline.input_layout->Release();
		default_pipeline.vertex_shader->Release();
		default_pipeline.pixel_shader->Release();
		default_pipeline.rasterState->Release();
		default_pipeline.depthStencilView->Release();
		default_pipeline.depthStencilState->Release();
		default_pipeline.depthStencilBuffer->Release();
		default_pipeline.render_target->Release();
		d3d11_context.finalize();
		dev->Release();
		swapchain->Release();
		GPUSideVertexBuffer->Release();
	
	}

	void d3d11_device_t::init_shaders_and_input_layout()
	{
		ID3DBlob *VS, *PS;
		D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_5_0", 0, 0, &VS, 0);
		D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_5_0", 0, 0, &PS, 0);

		d3d11_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &default_pipeline.vertex_shader);
		d3d11_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &default_pipeline.pixel_shader);

		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		d3d11_device->CreateInputLayout(ied, 4, VS->GetBufferPointer(), VS->GetBufferSize(), &default_pipeline.input_layout);

	
		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(TJMVPBuffer), D3D11_BIND_CONSTANT_BUFFER);
	
		dev->CreateBuffer(&constantBufferDesc,nullptr,&GPUSideConstantBuffer);
	}

	void d3d11_device_t::init_default_graphics()
	{
		camera.SetAsIdentiy();
		view.SetAsViewMatrix(camera);
		//proj.SetAsProjectionMatrix(0.1, 10.0, (1280 / 720), 180);
	
		for (float i = -0.5; i < 0.5; i += 0.1)
		{
			TJVertex temp;

			VERTEX StartPoint;
			StartPoint.x = i;
			StartPoint.y = 0;
			StartPoint.z = -0.5;
			StartPoint.color = green;

			VERTEX EndPoint;
			EndPoint.x = i;
			EndPoint.y = 0;
			EndPoint.z = 0.5;
			EndPoint.color = green;

			
			gridVerts.push_back(StartPoint);
			gridVerts.push_back(EndPoint);

			StartPoint.x = -0.5f;
			StartPoint.y = 0;
			StartPoint.z = i;

			EndPoint.x = 0.5;
			EndPoint.y = 0;
			EndPoint.z = i;

			gridVerts.push_back(StartPoint);
			gridVerts.push_back(EndPoint);


		}


		gridVertCount = gridVerts.size();
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX) * gridVerts.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		dev->CreateBuffer(&bd, NULL, &default_vert_buffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d11_context->Map(default_vert_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, gridVerts.data(), sizeof(VERTEX) * gridVerts.size());
		d3d11_context->Unmap(default_vert_buffer, NULL);
	}
	void d3d11_device_t::UpdateGrid()
	{
		int count = 0;
		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirCam, TJMatrix::TJM2Dir(camera));
		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirView, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&myCVPbuffer.dirCam))));
		DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(3.14159265*0.25f,(1280 / 720),	0.01f,100.0f		);	
		DirectX::XMMATRIX orientationMatrix = DirectX::XMMatrixIdentity(); //XMLoadFloat4x4(&orientation);

		DirectX::XMStoreFloat4x4(
			&myCVPbuffer.dirProj,
			XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

		//myCVPbuffer.tjModel = camera;//TJMatrix::CreateIdentiyMatrix();
		//myCVPbuffer.tjView = view.CreateViewMatrix(camera);
	
	
		for (float i = -0.5; i < 0.5; i += 0.1)
		{
			if (count >= gridVerts.size())
				break;

			VERTEX StartPoint;
			StartPoint.x = i;
			StartPoint.y = 0;
			StartPoint.z = -0.5;
			StartPoint.color = green;

			VERTEX EndPoint;
			EndPoint.x = i;
			EndPoint.y = 0;
			EndPoint.z = 0.5;
			EndPoint.color = green;

			gridVerts[count] = StartPoint;
			count++;
			gridVerts[count] = EndPoint;
			count++;

			StartPoint.x = -0.5f;
			StartPoint.y = 0;
			StartPoint.z = i;

			EndPoint.x = 0.5;
			EndPoint.y = 0;
			EndPoint.z = i;

			gridVerts[count] = StartPoint;
			count++;
			gridVerts[count] = EndPoint;
			count++;
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX) * gridVerts.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d11_context->Map(default_vert_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, gridVerts.data(), sizeof(VERTEX) * gridVerts.size());
		d3d11_context->Unmap(default_vert_buffer, NULL);

	}

	void d3d11_device_t::UpdateMesh(Mesh * someMesh, int bufferIndex, int frameNumber)
	{
		int count = 0;
		std::vector<std::vector<VTriangle>> itsTriangles = someMesh->myTriangles;
		std::vector<VERTEX> vertexVector;
	
		//myCVPbuffer.tjModel = camera;
		//myCVPbuffer.tjView = view.CreateViewMatrix(camera);
		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirCam, TJMatrix::TJM2Dir(camera));
		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirView, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&myCVPbuffer.dirCam))));
		DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(3.14159265*0.25f, (1280 / 720), 0.01f, 100.0f);
		DirectX::XMMATRIX orientationMatrix = DirectX::XMMatrixIdentity(); //XMLoadFloat4x4(&orientation);

		DirectX::XMStoreFloat4x4(
			&myCVPbuffer.dirProj,
			XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);
		//myCVPbuffer.tjProj = proj;
		int j = frameNumber;
		if (j >= someMesh->myTriangles.size())
			return;
		//for (int j = 0; j < someMesh->myTriangles.size(); j++)
		{
			for (int i = 0; i < someMesh->myTriangles[j].size(); i++)
			{
				VERTEX temp;
				temp.color = bufferIndex == 0 ? white : yellow;
			

				temp.x =itsTriangles[j][i].a.pos.x;
				temp.y =itsTriangles[j][i].a.pos.y;
				temp.z =itsTriangles[j][i].a.pos.z;
				temp.w =itsTriangles[j][i].a.pos.w;
				temp.u = itsTriangles[j][i].a.u;
				temp.v = itsTriangles[j][i].a.v;
				temp.nx = itsTriangles[j][i].a.normal.x;
				temp.ny = itsTriangles[j][i].a.normal.y;
				temp.nz = itsTriangles[j][i].a.normal.z;
				vertexVector.push_back(temp);

				temp.x = itsTriangles[j][i].b.pos.x;
				temp.y = itsTriangles[j][i].b.pos.y;
				temp.z = itsTriangles[j][i].b.pos.z;
				temp.w = itsTriangles[j][i].b.pos.w;
				temp.u = itsTriangles[j][i].b.u;
				temp.v = itsTriangles[j][i].b.v;
				temp.nx = itsTriangles[j][i].b.normal.x;
				temp.ny = itsTriangles[j][i].b.normal.y;
				temp.nz = itsTriangles[j][i].b.normal.z;
				vertexVector.push_back(temp);

				temp.x =itsTriangles[j][i].c.pos.x;
				temp.y =itsTriangles[j][i].c.pos.y;
				temp.z =itsTriangles[j][i].c.pos.z;
				temp.w =itsTriangles[j][i].c.pos.w;
				temp.u = itsTriangles[j][i].c.u;
				temp.v = itsTriangles[j][i].c.v;
				temp.nx = itsTriangles[j][i].c.normal.x;
				temp.ny = itsTriangles[j][i].c.normal.y;
				temp.nz = itsTriangles[j][i].c.normal.z;
				vertexVector.push_back(temp);

			}
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX) * vertexVector.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		//if (GPUSideVertexBuffer == nullptr)
			dev->CreateBuffer(&bd, NULL, &GPUSideVertexBuffer);
			if (GPUSideVertexBuffer == nullptr)
				return;
		D3D11_MAPPED_SUBRESOURCE ms;
		d3d11_context->Map(GPUSideVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertexVector.data(), sizeof(VERTEX) * vertexVector.size());
		d3d11_context->Unmap(GPUSideVertexBuffer, NULL);

		D3D11_BUFFER_DESC bd2;
		ZeroMemory(&bd2, sizeof(bd2));

		bd2.Usage = D3D11_USAGE_DYNAMIC;
		bd2.ByteWidth = sizeof(unsigned int) * someMesh->indexBuffer.size();
		bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//if (GPUSideIndexBuffer == nullptr)
			dev->CreateBuffer(&bd2, NULL, &GPUSideIndexBuffer);

		D3D11_MAPPED_SUBRESOURCE ms2;
		d3d11_context->Map(GPUSideIndexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms2);
		memcpy(ms2.pData, someMesh->indexBuffer.data(), sizeof(unsigned int) * someMesh->indexBuffer.size());
		d3d11_context->Unmap(GPUSideIndexBuffer, NULL);

	}
	void d3d11_device_t::UpdateTexture(Mesh * someMesh)
	{
		if (someMesh->name == "Teddy" && m_texture2d == nullptr)
		{
			HRESULT result;
			result = CreateDDSTextureFromFile(dev, L"../Teddy_D.dds", (ID3D11Resource**)m_texture2d, &m_shaderView);
		}
		else if (someMesh->name == "Mage" && m_texture2d2 == nullptr)
		{
			HRESULT result;
			result = CreateDDSTextureFromFile(dev, L"../Mage_D.dds", (ID3D11Resource**)m_texture2d2, &m_shaderView2);
		}
	}

	void d3d11_device_t::UpdateBones(Mesh * someMesh, int bufferIndex, int frameNumber)
	{
		int count = 0;
		//myCVPbuffer.tjModel = camera;
		//myCVPbuffer.tjView = view.CreateViewMatrix(camera);
		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirCam, TJMatrix::TJM2Dir(camera));
		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirView, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&myCVPbuffer.dirCam))));
		DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(3.14159265*0.25f, (1280 / 720), 0.01f, 100.0f);
		DirectX::XMMATRIX orientationMatrix = DirectX::XMMatrixIdentity(); //XMLoadFloat4x4(&orientation);

		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirProj,XMMatrixTranspose(perspectiveMatrix * orientationMatrix));
		std::vector<VERTEX> vertexVector;
		//for (int j = 0; j < someMesh->boneVectorSize; j++)
		//{
		if (someMesh->bones.size() < frameNumber || someMesh->bones.size() == 0)
			return;
		int j = frameNumber;
		for (int i = 0; i < someMesh->bones[j].size(); i++)
		{
			if (count >= gridVerts.size())
				break;

			TJVertex temp;
			temp.pos.x = someMesh->bones[j][i].x;
			temp.pos.y = someMesh->bones[j][i].y;
			temp.pos.z = someMesh->bones[j][i].z;
	
			temp.pos.w = 1;
		


			TJVertex tjTemp2;
			int pIndex = someMesh->bones[j][i].parentIndex;
			if (pIndex != -1)
			{

				tjTemp2.pos.x = someMesh->bones[j][pIndex].x;
				tjTemp2.pos.y = someMesh->bones[j][pIndex].y;
				tjTemp2.pos.z = someMesh->bones[j][pIndex].z;
				tjTemp2.pos.w = 1;
		

				VERTEX temp2;
				temp2.color = bufferIndex == 0 ? yellow : white;
				temp2.x = temp.pos.x;
				temp2.y = temp.pos.y;
				temp2.z = temp.pos.z;
				temp2.w = temp.pos.w;
				temp2.nx = temp.normal.x;
				temp2.ny = temp.normal.y;
				temp2.nz = temp.normal.z;
				vertexVector.push_back(temp2);

				temp2.x = tjTemp2.pos.x;
				temp2.y = tjTemp2.pos.y;
				temp2.z = tjTemp2.pos.z;
				temp2.w = tjTemp2.pos.w;
				temp2.nx = tjTemp2.normal.x;
				temp2.ny = tjTemp2.normal.y;
				temp2.nz = tjTemp2.normal.z;
				vertexVector.push_back(temp2);
			}
		}
		//}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX) * vertexVector.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//if (GPUSideVertexBuffer == nullptr)
			dev->CreateBuffer(&bd, NULL, &GPUSideVertexBuffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d11_context->Map(GPUSideVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertexVector.data(), sizeof(VERTEX) * vertexVector.size());
		d3d11_context->Unmap(GPUSideVertexBuffer, NULL);


	}
	void d3d11_device_t::UpdateJoints(Mesh * someMesh, int bufferIndex, int frameNumber)
	{
		int count = 0;
		//myCVPbuffer.tjModel = camera;
		//myCVPbuffer.tjView = view.CreateViewMatrix(camera);
		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirCam, TJMatrix::TJM2Dir(camera));
		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirView, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&myCVPbuffer.dirCam))));
		DirectX::XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(3.14159265*0.25f, (1280 / 720), 0.01f, 100.0f);
		DirectX::XMMATRIX orientationMatrix = DirectX::XMMatrixIdentity(); //XMLoadFloat4x4(&orientation);

		DirectX::XMStoreFloat4x4(&myCVPbuffer.dirProj, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));
		std::vector<VERTEX> vertexVector;
		int j = frameNumber;
		//	for (int j = 0; j < someMesh->boneVectorSize; j++)
		{
			for (int i = 0; i < someMesh->bones[j].size() && j < someMesh->bones.size(); i += 2)
			{
				if (count >= gridVerts.size())
					break;
				//if (i + 1 < someMesh->bones.size())
				{
					TJVertex tjTemp;
					tjTemp.pos.x = someMesh->bones[j][i].x;
					tjTemp.pos.y = someMesh->bones[j][i].y;
					tjTemp.pos.z = someMesh->bones[j][i].z;
					tjTemp.pos.w = 1;
				

					TJVertex tjTempTrans;
					tjTempTrans.pos.x = someMesh->bones[j][i].x + 0.004f;
					tjTempTrans.pos.y = someMesh->bones[j][i].y + 0.005f;
					tjTempTrans.pos.z = someMesh->bones[j][i].z + 0.003f;
					tjTempTrans.pos.w = 1;
			

					VERTEX temp;
					temp.color = red;
					temp.x = tjTemp.pos.x;
					temp.y = tjTemp.pos.y;
					temp.z = tjTemp.pos.z;
					temp.w = tjTemp.pos.w;
					temp.nx = tjTemp.normal.x;
					temp.ny = tjTemp.normal.y;
					temp.nz = tjTemp.normal.z;
					vertexVector.push_back(temp);
					temp.x = tjTempTrans.pos.x;
					vertexVector.push_back(temp);


					temp.color = green;
					temp.x = tjTemp.pos.x;
					temp.y = tjTemp.pos.y;
					temp.z = tjTemp.pos.z;
					temp.w = tjTemp.pos.w;
					temp.nx = tjTemp.normal.x;
					temp.ny = tjTemp.normal.y;
					temp.nz = tjTemp.normal.z;
					vertexVector.push_back(temp);
					temp.y = tjTempTrans.pos.y;
					vertexVector.push_back(temp);


					temp.color = blue;
					temp.x = tjTemp.pos.x;
					temp.y = tjTemp.pos.y;
					temp.z = tjTemp.pos.z;
					temp.w = tjTemp.pos.w;
					temp.nx = tjTemp.normal.x;
					temp.ny = tjTemp.normal.y;
					temp.nz = tjTemp.normal.z;
					vertexVector.push_back(temp);
					temp.x = tjTempTrans.pos.x;
					temp.y = tjTempTrans.pos.y;
					temp.z = tjTempTrans.pos.z;
					vertexVector.push_back(temp);

					//x red y green z blue

				}
			}
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX) * vertexVector.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//if (GPUSideVertexBuffer == nullptr)
			dev->CreateBuffer(&bd, NULL, &GPUSideVertexBuffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d11_context->Map(GPUSideVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertexVector.data(), sizeof(VERTEX) * vertexVector.size());
		d3d11_context->Unmap(GPUSideVertexBuffer, NULL);



	}
}

namespace fsgd
{

	void graphics::initialize(window* win)
	{
		device = &d3d11_device;
		d3d11_device.initialize(win);
	}

	void graphics::finalize()
	{
		d3d11_device.finalize();
	}

	void graphics::render_frame()
	{
		////////////////////////////////////////////////////
		// Update prior to rendering
		////////////////////////////////////////////

		


		auto devcon = d3d11_device.d3d11_context.devcon;
		if (GetAsyncKeyState(VK_NUMPAD0))
		{
			devcon->RSSetState(d3d11_device.default_pipeline.tjrasterState);
		//	devcon->RSSetState(d3d11_device.texturePipline.tjrasterState);
		//	d3d11_device.set_pipeline(&d3d11_device.texturePipline);
		}
		if (GetAsyncKeyState(VK_NUMPAD1))
		{
			devcon->RSSetState(d3d11_device.default_pipeline.rasterState);
		//	devcon->RSSetState(d3d11_device.texturePipline.rasterState);
		//	d3d11_device.set_pipeline(&d3d11_device.default_pipeline);
		}
		
		devcon->ClearRenderTargetView(d3d11_device.default_pipeline.render_target, &black.r);

		devcon->ClearDepthStencilView(d3d11_device.default_pipeline.depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		fsgd::d3d11_device.UpdateGrid();
		//myCVPbuffer.tjModel = TJMatrix::CreateTransposedMatrix(camera);
		//myCVPbuffer.tjView = TJMatrix::CreateTransposedMatrix(view);
		//myCVPbuffer.tjProj = TJMatrix::CreateTransposedMatrix(proj);
		devcon->UpdateSubresource(GPUSideConstantBuffer, 0, NULL, &myCVPbuffer, 0, 0);
		devcon->IASetVertexBuffers(0, 1, &d3d11_device.default_vert_buffer, &stride, &offset);
		devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		devcon->IASetInputLayout(d3d11_device.default_pipeline.input_layout);
		devcon->VSSetShader(d3d11_device.default_pipeline.vertex_shader, nullptr, 0);
		devcon->VSSetConstantBuffers(0, 1, &GPUSideConstantBuffer);
		constexpr size_t alignment = alignof(TJMatrix);
		devcon->Draw(gridVertCount, 0);
	for (int i = 0; i < myMeshes.size(); i++)
	{
	
		fsgd::d3d11_device.UpdateMesh(myMeshes[i], i, myFrameNumbers[i]);

		//myCVPbuffer.dirCam = TJMatrix::CreateTransposedMatrix(camera.CreateIdentiyMatrix());
		//myCVPbuffer.dirCam = TJMatrix::CreateTransposedMatrix(view);
		//myCVPbuffer.tjProj = TJMatrix::CreateTransposedMatrix(proj);

		devcon->IASetVertexBuffers(0, 1, &GPUSideVertexBuffer, &stride, &offset);
		devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devcon->IASetIndexBuffer(GPUSideIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


		ID3D11ShaderResourceView* texViews[] = { nullptr };
		D3D11_SAMPLER_DESC m_sampDesc;
		if (myMeshes[i]->name == "Teddy")
		{
			*texViews = fsgd::d3d11_device.m_shaderView;
			fsgd::d3d11_device.UpdateTexture(myMeshes[i]);
			devcon->PSSetShaderResources(0, 1, texViews);


			ZeroMemory(&m_sampDesc, sizeof(D3D11_SAMPLER_DESC));
			m_sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			m_sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			m_sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			m_sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;


			fsgd::d3d11_device->CreateSamplerState(&m_sampDesc, &m_sampler);

			devcon->PSSetSamplers(0, 1, &m_sampler);
		}
		else if (myMeshes[i]->name == "Mage")
		{
			*texViews = fsgd::d3d11_device.m_shaderView2;
			fsgd::d3d11_device.UpdateTexture(myMeshes[i]);
			devcon->PSSetShaderResources(0, 1, texViews);


			ZeroMemory(&m_sampDesc, sizeof(D3D11_SAMPLER_DESC));
			m_sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			m_sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			m_sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			m_sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;


			fsgd::d3d11_device->CreateSamplerState(&m_sampDesc, &m_sampler);

			devcon->PSSetSamplers(0, 1, &m_sampler);
		}
		devcon->UpdateSubresource(GPUSideConstantBuffer, 0, NULL, &myCVPbuffer, 0, 0);
		devcon->DrawIndexed(myMeshes[i]->indexBuffer.size() * 4, 0, 0);
		*texViews = nullptr;
		m_sampler = nullptr;
		devcon->PSSetShaderResources(0, 1, texViews);
		devcon->PSSetSamplers(0, 1, &m_sampler);
		if (AutoAnimate)
		{
			fsgd::d3d11_device.UpdateBones(myMeshes[i], i, myFrameNumbers[i]);
		}
		else
		{
			if (animationFrameNum[i] >= myMeshes[i]->bones.size())
				animationFrameNum[i] = 0;
			if (animationFrameNum[i] < 0)
				animationFrameNum[i] = myMeshes[i]->bones.size() -1;
			fsgd::d3d11_device.UpdateBones(myMeshes[i], i, animationFrameNum[i]);
		}

		devcon->IASetVertexBuffers(0, 1, &GPUSideVertexBuffer, &stride, &offset);
		devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		devcon->IASetIndexBuffer(GPUSideIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		devcon->UpdateSubresource(GPUSideConstantBuffer, 0, NULL, &myCVPbuffer, 0, 0);
		for (int j = 0; j < myMeshes[i]->boneVectorSize; j++)
			devcon->Draw(myMeshes[i]->bones[j].size() * 2, 0);

	
		if (AutoAnimate)
		{
			fsgd::d3d11_device.UpdateJoints(myMeshes[i], i, myFrameNumbers[i]);
		}
		else
		{
			if (animationFrameNum[i] >= myMeshes[i]->bones.size())
				animationFrameNum[i] = 0;
			if (animationFrameNum[i] < 0)
				animationFrameNum[i] = myMeshes[i]->bones.size() - 1;
			fsgd::d3d11_device.UpdateJoints(myMeshes[i], i, animationFrameNum[i]);
		}
		
		devcon->IASetVertexBuffers(0, 1, &GPUSideVertexBuffer, &stride, &offset);
		devcon->IASetIndexBuffer(GPUSideIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		devcon->UpdateSubresource(GPUSideConstantBuffer, 0, NULL, &myCVPbuffer, 0, 0);
		for (int j = 0; j < myMeshes[i]->boneVectorSize; j++)
			devcon->Draw(myMeshes[i]->bones[j].size() * 6, 0);

		myFrameNumbers[i]++;
		if (myFrameNumbers[i] >= myMeshes[i]->bones.size())
			myFrameNumbers[i] = 1;
	

	
	}

	


		d3d11_device.swapchain->Present(1, 0);
	}
}