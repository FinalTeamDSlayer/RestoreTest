#pragma once

namespace Engine
{
	template <typename T>			// 클래스 포인터 AddRef
	DWORD Safe_AddRef(T& pInstance)
	{
		DWORD	dwRefCnt = 0;

		if (nullptr != pInstance)
			dwRefCnt = pInstance->AddRef();

		return dwRefCnt;
	}

	template <typename T>			// 클래스 포인터들을 해제
	DWORD Safe_Release(T& pInstance)
	{
		DWORD	dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (dwRefCnt == 0)
				pInstance = nullptr;
		}
		return dwRefCnt;
	}

	template <typename T>			// 원시 자료형, 구조체 포인터 해제
	void Safe_Delete(T& pointer)
	{
		if (nullptr != pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	}

	template <typename T>			// 동적 배열을 삭제하는 용도
	void Safe_Delete_Array(T& pointer)
	{
		if (nullptr != pointer)
		{
			delete[] pointer;
			pointer = nullptr;
		}
	}

	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const wchar_t* pTag)
			: m_pTargetTag(pTag)
		{
		}
		~CTag_Finder() {		}
	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if (false == lstrcmpW(m_pTargetTag, pair.first))
			{
				return true;
			}

			return false;
		}

	private:
		const wchar_t* m_pTargetTag = nullptr;
	};

	class Random
	{
	public:
		static int Generate_Int(const int& iMin, const int& iMax)
		{
			std::random_device rd;
			std::mt19937_64 engine{ rd() };

			std::uniform_int_distribution<int> Random_Int(iMin, iMax);

			return Random_Int(engine);
		}

		static float Generate_Float(const float& fMin, const float& fMax)
		{
			std::random_device rd;
			std::mt19937_64 engine{ rd() };

			std::uniform_real_distribution<float> Random_Float(fMin, fMax);

			return Random_Float(engine);
		}
	};

	class Convert
	{
	public:
		static XMVECTOR ToVector(XMFLOAT2 _vF2)
		{
			XMFLOAT2 vF2 = _vF2;

			return XMLoadFloat2(&vF2);
		}

		static XMVECTOR ToVector(XMFLOAT3 _vF3)
		{
			XMFLOAT3 vF3 = _vF3;

			return XMLoadFloat3(&vF3);
		}

		static XMVECTOR ToVector(XMFLOAT4 _vF4)
		{
			XMFLOAT4 vF4 = _vF4;

			return XMLoadFloat4(&vF4);
		}

		static XMVECTOR ToVector_W1(XMFLOAT3 _vF3)
		{
			XMFLOAT3 vF3 = _vF3;

			return XMVectorSetW(XMLoadFloat3(&vF3), 1.f);
		}

		static XMMATRIX ToMatrix(XMFLOAT4X4 _mat)
		{
			XMFLOAT4X4 mat = _mat;

			return XMLoadFloat4x4(&mat);
		}

		static XMFLOAT2 ToFloat2(FXMVECTOR _vVec)
		{
			XMFLOAT2 vF2;

			XMStoreFloat2(&vF2, _vVec);

			return vF2;
		}

		static XMFLOAT3 ToFloat3(FXMVECTOR _vVec)
		{
			XMFLOAT3 vF3;

			XMStoreFloat3(&vF3, _vVec);

			return vF3;
		}

		static XMFLOAT4 ToFloat4(FXMVECTOR _vVec)
		{
			XMFLOAT4 vF4;

			XMStoreFloat4(&vF4, _vVec);

			return vF4;
		}

		static XMFLOAT4X4 ToFloat4x4(FXMMATRIX _mat)
		{
			XMFLOAT4X4 mat;

			XMStoreFloat4x4(&mat, _mat);

			return mat;
		}

		static XMVECTOR AddToVectorX(FXMVECTOR _vVec, float _fX)
		{
			return XMVectorSetX(_vVec, XMVectorGetX(_vVec) + _fX);
		}

		static XMVECTOR AddToVectorY(FXMVECTOR _vVec, float _fY)
		{
			return XMVectorSetY(_vVec, XMVectorGetY(_vVec) + _fY);
		}

		static XMVECTOR AddToVectorZ(FXMVECTOR _vVec, float _fZ)
		{
			return XMVectorSetZ(_vVec, XMVectorGetZ(_vVec) + _fZ);
		}

		static XMVECTOR AddToVectorW(FXMVECTOR _vVec, float _fW)
		{
			return XMVectorSetW(_vVec, XMVectorGetW(_vVec) + _fW);
		}
	};

	
}
