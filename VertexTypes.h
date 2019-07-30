#pragma once

#include <DirectXMath.h>
using namespace DirectX;

// color only vertex and constant buffer structure
struct ColorVertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct ColorConstBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};
static_assert((sizeof(ColorConstBuffer) % 16) == 0, "Constant Buffer size must be 16-byte aligned");


// color and normal vertex and constant buffer structure
struct ColorNormVertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;
};

struct ColorNormConstBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 mLightPos;
	XMFLOAT4 mEyePos;
};
static_assert((sizeof(ColorNormConstBuffer) % 16) == 0, "Constant Buffer size must be 16-byte aligned");


// texture and normal vertex and constant buffer structure
struct TextureVertex
{
	XMFLOAT4 position;
	XMFLOAT4 normal;
	XMFLOAT2 texcoord;
};

struct TextureConstBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 mLightPos;
	XMFLOAT4 mEyePos;
};
static_assert((sizeof(TextureConstBuffer) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

// flat color texture vertex and constant buffer structure
struct FlatTextureVertex
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
};

struct FlatTextureConstBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};
static_assert((sizeof(FlatTextureConstBuffer) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

// explosion texture vertex and constant buffer structure
struct ExplosionTextureVertex
{
	XMFLOAT4 position;
	XMFLOAT4 color;
	XMFLOAT4 normal;
	XMFLOAT2 texcoord;
};

struct ExplosionTextureConstBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 mTimer;
	XMFLOAT4 mUp;
	XMFLOAT4 mSide;
	XMFLOAT4 mOrigin;
};
static_assert((sizeof(ExplosionTextureConstBuffer) % 16) == 0, "Constant Buffer size must be 16-byte aligned");