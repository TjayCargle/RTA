//TJDev5Library.h 
#pragma once
#ifdef TJCar_DEV5_LIB_EXPORTS
#define TJCarg_DEV5_API __declspec(dllexport)
#else
#define TJCarg_DEV5_API __declspec(dllimport)
#endif

#include "../TJStructs.h"

namespace TJDEV5LIB
{
	
	class Functions
	{
	public:
	
		static TJCarg_DEV5_API double Add(double a, double b);
		static TJCarg_DEV5_API void SetupFbxManager();
		static TJCarg_DEV5_API void setFbxIORoot();
		static TJCarg_DEV5_API bool setupFbxImporter(const char * filename);
		static TJCarg_DEV5_API void importFbxscene(const char * sceneName);
		static TJCarg_DEV5_API Mesh getMeshFromFbx();

	private:

	};
}