#pragma once

#include <cstdint>
#include <DirectXMath.h>
#include <vector>
#include "Utilities.h"

struct GeometryVertex
{
	GeometryVertex() {}
	GeometryVertex(
		const DirectX::XMFLOAT3& p,
		const DirectX::XMFLOAT3& n,
		const DirectX::XMFLOAT3& t,
		const DirectX::XMFLOAT2& uv) :
		Position(p),
		Normal(n),
		TangentU(t),
		TexC(uv) {}
	GeometryVertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v) :
		Position(px, py, pz),
		Normal(nx, ny, nz),
		TangentU(tx, ty, tz),
		TexC(u, v) {}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 TangentU;
	DirectX::XMFLOAT2 TexC;
};

struct GeometryObject
{
public:
	std::vector<GeometryVertex> Vertices;
	std::vector<USHORT> Indices;
};

class GeometryGenerator
{
public:
	

	///<summary>
	/// Creates a box centered at the origin with the given dimensions, where each
	/// face has m rows and n columns of vertices.
	///</summary>
	static GeometryObject CreateBox(float width, float height, float depth, UINT numSubdivisions);

	///<summary>
	/// Creates a sphere centered at the origin with the given radius.  The
	/// slices and stacks parameters control the degree of tessellation.
	///</summary>
	static GeometryObject CreateSphere(float radius, UINT sliceCount, UINT stackCount);

	///<summary>
	/// Creates a geosphere centered at the origin with the given radius.  The
	/// depth controls the level of tessellation.
	///</summary>
	static GeometryObject CreateGeosphere(float radius, UINT numSubdivisions);

	///<summary>
	/// Creates a cylinder parallel to the y-axis, and centered about the origin.  
	/// The bottom and top radius can vary to form various cone shapes rather than true
	// cylinders.  The slices and stacks parameters control the degree of tessellation.
	///</summary>
	static GeometryObject CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount);

	///<summary>
	/// Creates an mxn grid in the xz-plane with m rows and n columns, centered
	/// at the origin with the specified width and depth.
	///</summary>
	static GeometryObject CreateGrid(float width, float depth, UINT m, UINT n);

	///<summary>
	/// Creates a quad aligned with the screen.  This is useful for postprocessing and screen effects.
	///</summary>
	static GeometryObject CreateQuad(float x, float y, float w, float h, float depth);

private:
	static void Subdivide(GeometryObject& meshData);
	static GeometryVertex MidPoint(const GeometryVertex& v0, const GeometryVertex& v1);
	static void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, GeometryObject& meshData);
	static void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, GeometryObject& meshData);
};

