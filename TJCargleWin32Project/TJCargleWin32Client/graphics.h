#pragma once
#include "../TJStructs.h"
namespace fsgd
{
	class window;

	class graphics_interface
	{
	protected:
		virtual ~graphics_interface() {}
	public:
	};

	class graphics : public graphics_interface
	{
	public:
		class device_t
		{
		public:

			class context_t
			{
			public:
				virtual void finalize() = 0;
				virtual ~context_t() {}
			};

			virtual ~device_t() {}
			virtual void initialize(window*) = 0;
			virtual void finalize() = 0;
			virtual context_t* get_context() = 0;
			virtual const context_t* get_context()const = 0;
		};

		void initialize(window*);
		void finalize();
		void render_frame();

	private:
		device_t* device;

	};
}

TJMatrix GetCamera();
TJMatrix GetView();
TJMatrix GetProj();
std::vector<Mesh> GetMeshVertex();
void AddMeshToVertexList(Mesh someMesh);
void SetCamera(TJMatrix m);
void SetView(TJMatrix m);
void SetProj(TJMatrix m);
