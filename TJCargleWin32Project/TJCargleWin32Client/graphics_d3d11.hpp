#pragma once
#include "graphics.h"
#include "window.h"



#include <d2d1_2.h>
#include <d3d11_2.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
VERTEX GPUSideVertexBuffer[1024];
TJMatrix camera;
TJMatrix view;
TJMatrix proj;
int gridVertCount;
std::vector<VERTEX> gridVerts;
std::vector<Mesh> myMeshes;
std::vector<ID3D11Buffer *> otherVertexBuffer;
std::vector<ID3D11Buffer *> boneVertexBuffer;
std::vector<ID3D11Buffer *> otherIndexBuffer;
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

			virtual void finalize() { devcon->Release(); }

		};

		window* win;

		void init_depth_stencil_state();
		void init_dev_con_and_swap();
		void init_depth_stencil_buffer();
		void init_depth_stencil_view();
		void init_rasterizer_state();
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
		}default_pipeline;

		void set_pipeline(pipeline_state_t* p);

		ID3D11Device *dev;
		IDXGISwapChain *swapchain;
		ID3D11Buffer *default_vert_buffer;

		d3d11_context_t d3d11_context;
	public:

		void UpdateGrid();
		void UpdateMesh(Mesh someMesh, int bufferIndex);
		void UpdateBones(Mesh someMesh, int bufferIndex);
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
	}

	void d3d11_device_t::init_depth_stencil_view()
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HRESULT result = dev->CreateDepthStencilView(default_pipeline.depthStencilBuffer, &depthStencilViewDesc, &default_pipeline.depthStencilView);
	}

	void d3d11_device_t::init_rasterizer_state()
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

		HRESULT result = dev->CreateRasterizerState(&rasterDesc, &default_pipeline.rasterState);
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
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		d3d11_device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &default_pipeline.input_layout);
	}

	void d3d11_device_t::init_default_graphics()
	{
		camera.SetAsIdentiy();
		view.SetAsViewMatrix();
		proj.SetAsProjectionMatrix(0.1, 10.0, (1280 / 720), 90);
		/*	VERTEX verts[] =
			{
				{ -1.0f, -1.0f, 0.0f, blue },
				{ 1.0f, -1.0f, 0.0f, blue },
				{ -1.0f, 1.0f, 0.0f, blue },

				{ -1.0f, 1.0f, 0.0f, blue },
				{ 1.0f, -1.0f, 0.0f, blue },
				{ 1.0f, 1.0f, 0.0f, blue }
			};*/
		std::vector<VERTEX> verts;
		VERTEX v1 = { -1,-1,0,1, blue };
		VERTEX v2 = { 1,-1,0,1, blue };
		VERTEX v3 = { -1,1,0,1, blue };
		VERTEX v4 = { -1,1,0,1, blue };
		VERTEX v5 = { 1,-1,0,1, blue };
		VERTEX v6 = { 1,1,0,1, blue };
		//	verts.push_back(v1);
		//	verts.push_back(v2);
		//	verts.push_back(v3);
		//	verts.push_back(v4);
		//	verts.push_back(v5);
		//	verts.push_back(v6);

		for (float i = -0.5; i < 0.5; i += 0.1)
		{
			TJVertex temp;

			VERTEX StartPoint;
			StartPoint.x = i;
			StartPoint.y = 0;
			StartPoint.z = -0.5;
			StartPoint.color = green;

			temp.pos.x = StartPoint.x;
			temp.pos.y = StartPoint.y;
			temp.pos.z = StartPoint.z;
			temp.pos.w = 1;

			temp = TJMatrix::Vector_Matrix_Multiply(temp, camera);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, view);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, proj);


			StartPoint.x = temp.pos.x;
			StartPoint.y = temp.pos.y;
			StartPoint.z = temp.pos.z;
			StartPoint.w = temp.pos.w;
			VERTEX EndPoint;
			EndPoint.x = i;
			EndPoint.y = 0;
			EndPoint.z = 0.5;
			EndPoint.color = green;

			temp.pos.x = EndPoint.x;
			temp.pos.y = EndPoint.y;
			temp.pos.z = EndPoint.z;
			temp.pos.w = 1;

			temp = TJMatrix::Vector_Matrix_Multiply(temp, camera);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, view);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, proj);

			EndPoint.x = temp.pos.x;
			EndPoint.y = temp.pos.y;
			EndPoint.z = temp.pos.z;
			EndPoint.w = temp.pos.w;

			gridVerts.push_back(StartPoint);
			gridVerts.push_back(EndPoint);

			StartPoint.x = -0.5f;
			StartPoint.y = 0;
			StartPoint.z = i;

			temp.pos.x = StartPoint.x;
			temp.pos.y = StartPoint.y;
			temp.pos.z = StartPoint.z;
			temp.pos.w = 1;

			temp = TJMatrix::Vector_Matrix_Multiply(temp, camera);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, view);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, proj);

			StartPoint.x = temp.pos.x;
			StartPoint.y = temp.pos.y;
			StartPoint.z = temp.pos.z;
			StartPoint.w = temp.pos.w;

			EndPoint.x = 0.5;
			EndPoint.y = 0;
			EndPoint.z = i;


			temp.pos.x = EndPoint.x;
			temp.pos.y = EndPoint.y;
			temp.pos.z = EndPoint.z;
			temp.pos.w = 1;

			temp = TJMatrix::Vector_Matrix_Multiply(temp, camera);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, view);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, proj);

			EndPoint.x = temp.pos.x;
			EndPoint.y = temp.pos.y;
			EndPoint.z = temp.pos.z;
			EndPoint.w = temp.pos.w;

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
		for (float i = -0.5; i < 0.5; i += 0.1)
		{
			if (count >= gridVerts.size())
				break;
			TJVertex temp;

			VERTEX StartPoint;
			StartPoint.x = i;
			StartPoint.y = 0;
			StartPoint.z = -0.5;
			StartPoint.color = green;

			temp.pos.x = StartPoint.x;
			temp.pos.y = StartPoint.y;
			temp.pos.z = StartPoint.z;
			temp.pos.w = 1;

			temp = TJMatrix::Vector_Matrix_Multiply(temp, camera);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, view);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, proj);


			StartPoint.x = temp.pos.x;
			StartPoint.y = temp.pos.y;
			StartPoint.z = temp.pos.z;
			StartPoint.w = temp.pos.w;
			VERTEX EndPoint;
			EndPoint.x = i;
			EndPoint.y = 0;
			EndPoint.z = 0.5;
			EndPoint.color = green;

			temp.pos.x = EndPoint.x;
			temp.pos.y = EndPoint.y;
			temp.pos.z = EndPoint.z;
			temp.pos.w = 1;

			temp = TJMatrix::Vector_Matrix_Multiply(temp, camera);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, view);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, proj);

			EndPoint.x = temp.pos.x;
			EndPoint.y = temp.pos.y;
			EndPoint.z = temp.pos.z;
			EndPoint.w = temp.pos.w;

			gridVerts[count] = StartPoint;
			count++;
			gridVerts[count] = EndPoint;
			count++;


			StartPoint.x = -0.5f;
			StartPoint.y = 0;
			StartPoint.z = i;

			temp.pos.x = StartPoint.x;
			temp.pos.y = StartPoint.y;
			temp.pos.z = StartPoint.z;
			temp.pos.w = 1;

			temp = TJMatrix::Vector_Matrix_Multiply(temp, camera);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, view);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, proj);

			StartPoint.x = temp.pos.x;
			StartPoint.y = temp.pos.y;
			StartPoint.z = temp.pos.z;
			StartPoint.w = temp.pos.w;

			EndPoint.x = 0.5;
			EndPoint.y = 0;
			EndPoint.z = i;


			temp.pos.x = EndPoint.x;
			temp.pos.y = EndPoint.y;
			temp.pos.z = EndPoint.z;
			temp.pos.w = 1;

			temp = TJMatrix::Vector_Matrix_Multiply(temp, camera);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, view);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, proj);

			EndPoint.x = temp.pos.x;
			EndPoint.y = temp.pos.y;
			EndPoint.z = temp.pos.z;
			EndPoint.w = temp.pos.w;

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

		dev->CreateBuffer(&bd, NULL, &default_vert_buffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d11_context->Map(default_vert_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, gridVerts.data(), sizeof(VERTEX) * gridVerts.size());
		d3d11_context->Unmap(default_vert_buffer, NULL);

	}

	void d3d11_device_t::UpdateMesh(Mesh someMesh, int bufferIndex)
	{
		int count = 0;
		std::vector<VTriangle> itsTriangles = someMesh.myTriangles;
		std::vector<VERTEX> vertexVector;
		for (int i = 0; i < someMesh.myTriangles.size(); i++)
		{
			if (count >= gridVerts.size())
				break;

			someMesh.myTriangles[i].a = TJMatrix::Vector_Matrix_Multiply(someMesh.myTriangles[i].a, camera);
			someMesh.myTriangles[i].a = TJMatrix::Vector_Matrix_Multiply(someMesh.myTriangles[i].a, view);
			someMesh.myTriangles[i].a = TJMatrix::Vector_Matrix_Multiply(someMesh.myTriangles[i].a, proj);

			someMesh.myTriangles[i].b = TJMatrix::Vector_Matrix_Multiply(someMesh.myTriangles[i].b, camera);
			someMesh.myTriangles[i].b = TJMatrix::Vector_Matrix_Multiply(someMesh.myTriangles[i].b, view);
			someMesh.myTriangles[i].b = TJMatrix::Vector_Matrix_Multiply(someMesh.myTriangles[i].b, proj);

			someMesh.myTriangles[i].c = TJMatrix::Vector_Matrix_Multiply(someMesh.myTriangles[i].c, camera);
			someMesh.myTriangles[i].c = TJMatrix::Vector_Matrix_Multiply(someMesh.myTriangles[i].c, view);
			someMesh.myTriangles[i].c = TJMatrix::Vector_Matrix_Multiply(someMesh.myTriangles[i].c, proj);


		}
		for (int i = 0; i < someMesh.myTriangles.size(); i++)
		{
			VERTEX temp;
			temp.color = bufferIndex == 0 ? white : yellow;
			temp.x = someMesh.myTriangles[i].a.pos.x;
			temp.y = someMesh.myTriangles[i].a.pos.y;
			temp.z = someMesh.myTriangles[i].a.pos.z;
			temp.w = someMesh.myTriangles[i].a.pos.w;
			vertexVector.push_back(temp);

			temp.x = someMesh.myTriangles[i].b.pos.x;
			temp.y = someMesh.myTriangles[i].b.pos.y;
			temp.z = someMesh.myTriangles[i].b.pos.z;
			temp.w = someMesh.myTriangles[i].b.pos.w;
			vertexVector.push_back(temp);

			temp.x = someMesh.myTriangles[i].c.pos.x;
			temp.y = someMesh.myTriangles[i].c.pos.y;
			temp.z = someMesh.myTriangles[i].c.pos.z;
			temp.w = someMesh.myTriangles[i].c.pos.w;
			vertexVector.push_back(temp);
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX) * vertexVector.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		dev->CreateBuffer(&bd, NULL, &otherVertexBuffer[bufferIndex]);

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d11_context->Map(otherVertexBuffer[bufferIndex], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertexVector.data(), sizeof(VERTEX) * vertexVector.size());
		d3d11_context->Unmap(otherVertexBuffer[bufferIndex], NULL);

		D3D11_BUFFER_DESC bd2;
		ZeroMemory(&bd2, sizeof(bd2));

		bd2.Usage = D3D11_USAGE_DYNAMIC;
		bd2.ByteWidth = sizeof(unsigned int) * someMesh.indexBuffer.size();
		bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		dev->CreateBuffer(&bd2, NULL, &otherIndexBuffer[bufferIndex]);

		D3D11_MAPPED_SUBRESOURCE ms2;
		d3d11_context->Map(otherIndexBuffer[bufferIndex], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms2);
		memcpy(ms2.pData, someMesh.indexBuffer.data(), sizeof(unsigned int) * someMesh.indexBuffer.size());
		d3d11_context->Unmap(otherIndexBuffer[bufferIndex], NULL);

	}

	void d3d11_device_t::UpdateBones(Mesh someMesh, int bufferIndex)
	{
		int count = 0;

		std::vector<VERTEX> vertexVector;
		for (int i = 0; i < someMesh.bones.size(); i++)
		{
			if (count >= gridVerts.size())
				break;
			TJVertex temp;
			temp.pos.x = someMesh.bones[i].x ;
			temp.pos.y = someMesh.bones[i].y ;
			temp.pos.z = someMesh.bones[i].z ;
			temp.pos.w = someMesh.bones[i].w;
			temp = TJMatrix::Vector_Matrix_Multiply(temp, camera);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, view);
			temp = TJMatrix::Vector_Matrix_Multiply(temp, proj);
			
			VERTEX temp2;
			temp2.color = bufferIndex == 0 ? magenta : cyan;
			temp2.x = temp.pos.x;
			temp2.y = temp.pos.y;
			temp2.z = temp.pos.z;
			temp2.w = temp.pos.w;
			vertexVector.push_back(temp2);
		}
		
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX) * vertexVector.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		dev->CreateBuffer(&bd, NULL, &boneVertexBuffer[bufferIndex]);

		D3D11_MAPPED_SUBRESOURCE ms;
		d3d11_context->Map(boneVertexBuffer[bufferIndex], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertexVector.data(), sizeof(VERTEX) * vertexVector.size());
		d3d11_context->Unmap(boneVertexBuffer[bufferIndex], NULL);

		/*D3D11_BUFFER_DESC bd2;
		ZeroMemory(&bd2, sizeof(bd2));

		bd2.Usage = D3D11_USAGE_DYNAMIC;
		bd2.ByteWidth = sizeof(unsigned int) * someMesh.indexBuffer.size();
		bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		dev->CreateBuffer(&bd2, NULL, &otherIndexBuffer[bufferIndex]);

		D3D11_MAPPED_SUBRESOURCE ms2;
		d3d11_context->Map(otherIndexBuffer[bufferIndex], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms2);
		memcpy(ms2.pData, someMesh.indexBuffer.data(), sizeof(unsigned int) * someMesh.indexBuffer.size());
		d3d11_context->Unmap(otherIndexBuffer[bufferIndex], NULL);*/

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

		fsgd::d3d11_device.UpdateGrid();


		auto devcon = d3d11_device.d3d11_context.devcon;

		devcon->ClearRenderTargetView(d3d11_device.default_pipeline.render_target, &black.r);

		devcon->ClearDepthStencilView(d3d11_device.default_pipeline.depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		devcon->IASetVertexBuffers(0, 1, &d3d11_device.default_vert_buffer, &stride, &offset);

		devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		devcon->Draw(gridVertCount, 0);
		for (int i = 0; i < myMeshes.size(); i++)
		{
			fsgd::d3d11_device.UpdateMesh(myMeshes[i], i);

			devcon->IASetVertexBuffers(0, 1, &otherVertexBuffer[i], &stride, &offset);
			devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			devcon->IASetIndexBuffer(otherIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
			devcon->DrawIndexed(myMeshes[i].indexBuffer.size() * 4, 0, 0);
			//devcon->Draw(;
			fsgd::d3d11_device.UpdateBones(myMeshes[i], i);
			devcon->IASetVertexBuffers(0, 1, &boneVertexBuffer[i], &stride, &offset);
			devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
			devcon->IASetIndexBuffer(boneVertexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
			devcon->Draw(myMeshes[i].bones.size(), 0);
		}
		d3d11_device.swapchain->Present(1, 0);
	}
}