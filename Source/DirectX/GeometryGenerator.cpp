//***************************************************************************************
// GeometryGenerator.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "GeometryGenerator.h"
#include <algorithm>
#include "DirectXMath.h"

using namespace DirectX;



DirectX::XMVECTOR GeometryGenerator::ComputeNormal(DirectX::XMVECTOR p0, DirectX::XMVECTOR p1, DirectX::XMVECTOR p2)
{

	XMVECTOR u = p1 - p0;

	XMVECTOR v = p2 - p0;


	return XMVector3Normalize(XMVector3Cross(u, v));
};


GeometryGenerator::MeshData GeometryGenerator::CreateBox(float width, float height, float depth, uint32 numSubdivisions)
{
	MeshData meshData;

	//
	// Create the vertices.
	//

	Vertex v[24];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	// Fill in the front face vertex data.
	v[0] = Vertex(-1, -1, -1, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-1, +1, -1, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+1, +1, -1, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+1, -1, -1, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-1, -1, +1, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+1, -1, +1, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+1, +1, +1, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-1, +1, +1, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	// Fill in the top face vertex data.
	v[8]  = Vertex(-1, +1, -1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = Vertex(-1, +1, +1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+1, +1, +1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+1, +1, -1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	
	// Fill in the bottom face vertex data.
	v[12] = Vertex(-1, -1, -1, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+1, -1, -1, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+1, -1, +1, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-1, -1, +1, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-1, -1, +1, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-1, +1, +1, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-1, +1, -1, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-1, -1, -1, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex(+1, -1, -1, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+1, +1, -1, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+1, +1, +1, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+1, -1, +1, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	//inside
	/*
	// Fill in the front face vertex data.
	v[24] = Vertex(-0.75, -0.75, -0.75, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[25] = Vertex(-0.75, +0.75, -0.75, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[26] = Vertex(+0.75, +0.75, -0.75, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[27] = Vertex(+0.75, -0.75, -0.75, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[28] = Vertex(-0.75, -0.75, +0.75, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[29] = Vertex(+0.75, -0.75, +0.75, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[30] = Vertex(+0.75, +0.75, +0.75, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[31] = Vertex(-0.75, +0.75, +0.75, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	// Fill in the top face vertex data.
	//v[32]  = Vertex(-1, +1, -1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	//v[33]  = Vertex(-1, +1, +1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//v[34] = Vertex(+1, +1, +1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//v[35] = Vertex(+1, +1, -1, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	
	// Fill in the bottom face vertex data.
	v[36] = Vertex(-0.75, -0.75, -0.75, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[37] = Vertex(+0.75, -0.75, -0.75, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[38] = Vertex(+0.75, -0.75, +0.75, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[39] = Vertex(-0.75, -0.75, +0.75, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[40] = Vertex(-0.75, -0.75, +0.75, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[41] = Vertex(-0.75, +0.75, +0.75, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[42] = Vertex(-0.75, +0.75, -0.75, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[43] = Vertex(-0.75, -0.75, -0.75, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[44] = Vertex(+0.75, -0.75, -0.75, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[45] = Vertex(+0.75, +0.75, -0.75, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[46] = Vertex(+0.75, +0.75, +0.75, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[47] = Vertex(+0.75, -0.75, +0.75, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	*/

	meshData.Vertices.assign(&v[0], &v[24]);

	//
	// Create the indices.
	//

	uint32 i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 12; i[13] = 13; i[14] = 14;
	i[15] = 15; i[16] = 16; i[17] = 17;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;
	/*
	//inside box

	// Fill in the front face index data
	i[36] = 36;//0
	i[37] = 37;//1
	i[38] = 38;//2
	i[39] = 36;//3
	i[40] = 38;//4
	i[41] = 39;//5

	// Fill in the back face index data
	i[42] = 40;//6
	i[43] = 41;//7
	i[44] = 42;//8
	i[45] = 40;//9
	i[46] = 42;//10
	i[47] = 43;//11
	/*
	// Fill in the top face index data
	i[48] = 48;//12
	i[49] = 49;//13
	i[50] = 50;//14
	i[51] = 51;//15
	i[52] = 52;//16
	i[53] = 53;//17
	
	// Fill in the bottom face index data
	i[54] = 48;//18
	i[55] = 49;//19
	i[56] = 50;//20
	i[57] = 48;//21
	i[58] = 50;//22
	i[59] = 51;//23

	// Fill in the left face index data
	i[60] = 52;//24
	i[61] = 53;//25
	i[62] = 54;//26
	i[63] = 52;//27
	i[64] = 54;//28
	i[65] = 55;//29

	// Fill in the right face index data
	i[66] = 56;//30
	i[67] = 57;//31
	i[68] = 58;//32
	i[69] = 56;//33
	i[70] = 58;//34
	i[71] = 59;//35
	*/




	meshData.Indices32.assign(&i[0], &i[36]);

    // Put a cap on the number of subdivisions.
    numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

    for(uint32 i = 0; i < numSubdivisions; ++i)
        Subdivide(meshData);

    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateSphere(float radius, uint32 sliceCount, uint32 stackCount)
{
    MeshData meshData;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.Vertices.push_back( topVertex );

	float phiStep   = XM_PI/stackCount;
	float thetaStep = 2.0f*XM_PI/sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(uint32 i = 1; i <= stackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
        for(uint32 j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;

			// spherical to cartesian
			v.Position.x = radius*sinf(phi)*cosf(theta);
			v.Position.y = radius*cosf(phi);
			v.Position.z = radius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.TangentU.x = -radius*sinf(phi)*sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = +radius*sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.TangentU);
			XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Position);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexC.x = theta / XM_2PI;
			v.TexC.y = phi / XM_PI;

			meshData.Vertices.push_back( v );
		}
	}

	meshData.Vertices.push_back( bottomVertex );

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

    for(uint32 i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices32.push_back(0);
		meshData.Indices32.push_back(i+1);
		meshData.Indices32.push_back(i);
	}
	
	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
    uint32 baseIndex = 1;
    uint32 ringVertexCount = sliceCount + 1;
	for(uint32 i = 0; i < stackCount-2; ++i)
	{
		for(uint32 j = 0; j < sliceCount; ++j)
		{
			meshData.Indices32.push_back(baseIndex + i*ringVertexCount + j);
			meshData.Indices32.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices32.push_back(baseIndex + (i+1)*ringVertexCount + j);

			meshData.Indices32.push_back(baseIndex + (i+1)*ringVertexCount + j);
			meshData.Indices32.push_back(baseIndex + i*ringVertexCount + j+1);
			meshData.Indices32.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	uint32 southPoleIndex = (uint32)meshData.Vertices.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(uint32 i = 0; i < sliceCount; ++i)
	{
		meshData.Indices32.push_back(southPoleIndex);
		meshData.Indices32.push_back(baseIndex+i);
		meshData.Indices32.push_back(baseIndex+i+1);
	}

    return meshData;
}
 
void GeometryGenerator::Subdivide(MeshData& meshData)
{
	// Save a copy of the input geometry.
	MeshData inputCopy = meshData;


	meshData.Vertices.resize(0);
	meshData.Indices32.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	uint32 numTris = (uint32)inputCopy.Indices32.size()/3;
	for(uint32 i = 0; i < numTris; ++i)
	{
		Vertex v0 = inputCopy.Vertices[ inputCopy.Indices32[i*3+0] ];
		Vertex v1 = inputCopy.Vertices[ inputCopy.Indices32[i*3+1] ];
		Vertex v2 = inputCopy.Vertices[ inputCopy.Indices32[i*3+2] ];

		//
		// Generate the midpoints.
		//

        Vertex m0 = MidPoint(v0, v1);
        Vertex m1 = MidPoint(v1, v2);
        Vertex m2 = MidPoint(v0, v2);

		//
		// Add new geometry.
		//

		meshData.Vertices.push_back(v0); // 0
		meshData.Vertices.push_back(v1); // 1
		meshData.Vertices.push_back(v2); // 2
		meshData.Vertices.push_back(m0); // 3
		meshData.Vertices.push_back(m1); // 4
		meshData.Vertices.push_back(m2); // 5
 
		meshData.Indices32.push_back(i*6+0);
		meshData.Indices32.push_back(i*6+3);
		meshData.Indices32.push_back(i*6+5);

		meshData.Indices32.push_back(i*6+3);
		meshData.Indices32.push_back(i*6+4);
		meshData.Indices32.push_back(i*6+5);

		meshData.Indices32.push_back(i*6+5);
		meshData.Indices32.push_back(i*6+4);
		meshData.Indices32.push_back(i*6+2);

		meshData.Indices32.push_back(i*6+3);
		meshData.Indices32.push_back(i*6+1);
		meshData.Indices32.push_back(i*6+4);
	}

	
}

//Tested and works on OpenGL.  
/*
Mesh* GeometryGenerator::Hexagon2() {
	auto mesh = new Mesh();

	float vertices[] = {

		-1 ,0, 0, //0
		-0.8, 0, -1, //1
		0.8, 0 , -1, //2
		1, 0 , 0, //3
		0.8, 0 , 1, //4
		-0.8, 0, 1, //5 

		-0.6, 1 , 0, //6
		-0.4, 1, -0.6, //7
		0.4, 1, -0.6, //8
		0.6, 1, 0, //9
		0.4, 1, 0.6, //10
		-0.4, 1, 0.6, //1

		-0.6, -1 , 0, //6
		-0.4, -1, -0.6, //7
		0.4, -1, -0.6, //8
		0.6, -1, 0, //9
		0.4, -1, 0.6, //10
		-0.4, -1, 0.6, //11

	};

	GLuint elements[] = {
		//top
		7, 6, 11,
		7,11,10,
		10,8,7,
		8,10,9,

		6,5,11,
		11,6,0,
		0,5,11,
		11,5,4,

		10,5,4,
		10,11,4,
		9,10,4,
		9,10,3,//12

		4,3,10,
		9,3,8,
		9,2,8,



		13, 17,12,
		16,17,13,
		14,16,13,
		15,16,14
	};
	*/
GeometryGenerator::Vertex GeometryGenerator::MidPoint(const Vertex& v0, const Vertex& v1)
{
    XMVECTOR p0 = XMLoadFloat3(&v0.Position);
    XMVECTOR p1 = XMLoadFloat3(&v1.Position);

    XMVECTOR n0 = XMLoadFloat3(&v0.Normal);
    XMVECTOR n1 = XMLoadFloat3(&v1.Normal);

    XMVECTOR tan0 = XMLoadFloat3(&v0.TangentU);
    XMVECTOR tan1 = XMLoadFloat3(&v1.TangentU);

    XMVECTOR tex0 = XMLoadFloat2(&v0.TexC);
    XMVECTOR tex1 = XMLoadFloat2(&v1.TexC);

    // Compute the midpoints of all the attributes.  Vectors need to be normalized
    // since linear interpolating can make them not unit length.  
    XMVECTOR pos = 0.5f*(p0 + p1);
    XMVECTOR normal = XMVector3Normalize(0.5f*(n0 + n1));
    XMVECTOR tangent = XMVector3Normalize(0.5f*(tan0+tan1));
    XMVECTOR tex = 0.5f*(tex0 + tex1);

    Vertex v;
    XMStoreFloat3(&v.Position, pos);
    XMStoreFloat3(&v.Normal, normal);
    XMStoreFloat3(&v.TangentU, tangent);
    XMStoreFloat2(&v.TexC, tex);

    return v;
}

GeometryGenerator::MeshData GeometryGenerator::CreateGeosphere(float radius, uint32 numSubdivisions)
{
    MeshData meshData;

	// Put a cap on the number of subdivisions.
    numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	// Approximate a sphere by tessellating an icosahedron.

	const float X = 0.525731f; 
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] = 
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),  
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),    
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X), 
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),    
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f), 
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

    uint32 k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

    meshData.Vertices.resize(12);
    meshData.Indices32.assign(&k[0], &k[60]);

	for(uint32 i = 0; i < 12; ++i)
		meshData.Vertices[i].Position = pos[i];

	for(uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	// Project vertices onto sphere and scale.
	for(uint32 i = 0; i < meshData.Vertices.size(); ++i)
	{
		// Project onto unit sphere.
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.Vertices[i].Position));

		// Project onto sphere.
		XMVECTOR p = radius*n;

		XMStoreFloat3(&meshData.Vertices[i].Position, p);
		XMStoreFloat3(&meshData.Vertices[i].Normal, n);

		// Derive texture coordinates from spherical coordinates.
        float theta = atan2f(meshData.Vertices[i].Position.z, meshData.Vertices[i].Position.x);

        // Put in [0, 2pi].
        if(theta < 0.0f)
            theta += XM_2PI;

		float phi = acosf(meshData.Vertices[i].Position.y / radius);

		meshData.Vertices[i].TexC.x = theta/XM_2PI;
		meshData.Vertices[i].TexC.y = phi/XM_PI;

		// Partial derivative of P with respect to theta
		meshData.Vertices[i].TangentU.x = -radius*sinf(phi)*sinf(theta);
		meshData.Vertices[i].TangentU.y = 0.0f;
		meshData.Vertices[i].TangentU.z = +radius*sinf(phi)*cosf(theta);

		XMVECTOR T = XMLoadFloat3(&meshData.Vertices[i].TangentU);
		XMStoreFloat3(&meshData.Vertices[i].TangentU, XMVector3Normalize(T));
	}

    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount)
{
    MeshData meshData;

	//
	// Build Stacks.
	// 

	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	uint32 ringCount = stackCount+1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for(uint32 i = 0; i < ringCount; ++i)
	{
		float y = -0.5f*height + i*stackHeight;
		float r = bottomRadius + i*radiusStep;

		// vertices of ring
		float dTheta = 2.0f*XM_PI/sliceCount;
		for(uint32 j = 0; j <= sliceCount; ++j)
		{
			Vertex vertex;

			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);

			vertex.Position = XMFLOAT3(r*c, y, r*s);

			vertex.TexC.x = (float)j/sliceCount;
			vertex.TexC.y = 1.0f - (float)i/stackCount;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
			vertex.TangentU = XMFLOAT3(-s, 0.0f, c);

			float dr = bottomRadius-topRadius;
			XMFLOAT3 bitangent(dr*c, -height, dr*s);

			XMVECTOR T = XMLoadFloat3(&vertex.TangentU);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&vertex.Normal, N);

			meshData.Vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	uint32 ringVertexCount = sliceCount+1;

	// Compute indices for each stack.
	for(uint32 i = 0; i < stackCount; ++i)
	{
		for(uint32 j = 0; j < sliceCount; ++j)
		{
			meshData.Indices32.push_back(i*ringVertexCount + j);
			meshData.Indices32.push_back((i+1)*ringVertexCount + j);
			meshData.Indices32.push_back((i+1)*ringVertexCount + j+1);

			meshData.Indices32.push_back(i*ringVertexCount + j);
			meshData.Indices32.push_back((i+1)*ringVertexCount + j+1);
			meshData.Indices32.push_back(i*ringVertexCount + j+1);
		}
	}

	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);

    return meshData;
}

void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
											uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
	uint32 baseIndex = (uint32)meshData.Vertices.size();

	float y = 0.5f*height;
	float dTheta = 2.0f*XM_PI/sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for(uint32 i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius*cosf(i*dTheta);
		float z = topRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x/height + 0.5f;
		float v = z/height + 0.5f;

		meshData.Vertices.push_back( Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v) );
	}

	// Cap center vertex.
	meshData.Vertices.push_back( Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f) );

	// Index of center vertex.
	uint32 centerIndex = (uint32)meshData.Vertices.size()-1;

	for(uint32 i = 0; i < sliceCount; ++i)
	{
		meshData.Indices32.push_back(centerIndex);
		meshData.Indices32.push_back(baseIndex + i+1);
		meshData.Indices32.push_back(baseIndex + i);
	}
}

void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
											   uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
	// 
	// Build bottom cap.
	//

	uint32 baseIndex = (uint32)meshData.Vertices.size();
	float y = -0.5f*height;

	// vertices of ring
	float dTheta = 2.0f*XM_PI/sliceCount;
	for(uint32 i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius*cosf(i*dTheta);
		float z = bottomRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x/height + 0.5f;
		float v = z/height + 0.5f;

		meshData.Vertices.push_back( Vertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v) );
	}

	// Cap center vertex.
	meshData.Vertices.push_back( Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f) );

	// Cache the index of center vertex.
	uint32 centerIndex = (uint32)meshData.Vertices.size()-1;

	for(uint32 i = 0; i < sliceCount; ++i)
	{
		meshData.Indices32.push_back(centerIndex);
		meshData.Indices32.push_back(baseIndex + i);
		meshData.Indices32.push_back(baseIndex + i+1);
	}
}

GeometryGenerator::MeshData GeometryGenerator::CreateGrid(float width, float depth, uint32 m, uint32 n)
{
    MeshData meshData;

	uint32 vertexCount = m*n;
	uint32 faceCount   = (m-1)*(n-1)*2;

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n-1);
	float dz = depth / (m-1);

	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);

	meshData.Vertices.resize(vertexCount);
	for(uint32 i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for(uint32 j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			meshData.Vertices[i*n+j].Position = XMFLOAT3(x, 0.0f, z);
			meshData.Vertices[i*n+j].Normal   = XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.Vertices[i*n+j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
			meshData.Vertices[i*n+j].TexC.x = j*du;
			meshData.Vertices[i*n+j].TexC.y = i*dv;
		}
	}
 
    //
	// Create the indices.
	//

	meshData.Indices32.resize(faceCount*3); // 3 indices per face

	// Iterate over each quad and compute indices.
	uint32 k = 0;
	for(uint32 i = 0; i < m-1; ++i)
	{
		for(uint32 j = 0; j < n-1; ++j)
		{
			meshData.Indices32[k]   = i*n+j;
			meshData.Indices32[k+1] = i*n+j+1;
			meshData.Indices32[k+2] = (i+1)*n+j;

			meshData.Indices32[k+3] = (i+1)*n+j;
			meshData.Indices32[k+4] = i*n+j+1;
			meshData.Indices32[k+5] = (i+1)*n+j+1;

			k += 6; // next quad
		}
	}

    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateQuad(float x, float y, float w, float h, float depth)
{
    MeshData meshData;

	meshData.Vertices.resize(4);
	meshData.Indices32.resize(6);

	// Position coordinates specified in NDC space.
	meshData.Vertices[0] = Vertex(
        x, y - h, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f);

	meshData.Vertices[1] = Vertex(
		x, y, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f);

	meshData.Vertices[2] = Vertex(
		x+w, y, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f);

	meshData.Vertices[3] = Vertex(
		x+w, y-h, depth,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f);

	meshData.Indices32[0] = 0;
	meshData.Indices32[1] = 1;
	meshData.Indices32[2] = 2;

	meshData.Indices32[3] = 0;
	meshData.Indices32[4] = 2;
	meshData.Indices32[5] = 3;

    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateDiamond(float width, float height, float depth, uint32 numSubdivisions)
{
	MeshData meshData;

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;


	Vertex v[17] =
	{
		Vertex(0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //0  		
		Vertex(w2 * -0.08f, h2 *  0.12f, d2 *  0.16f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //1
		Vertex(w2 *  0.08f, h2 *  0.12f, d2 *  0.16f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //2 //{ Vertex(4.0f, 6.0f, 8.0f)  }, //2
		Vertex(w2 *  0.16f, h2 *  0.12f, d2 *  0.08f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //3 //{ Vertex(8.0f, 6.0f, 4.0f)  }, //3
		Vertex(w2 *  0.16f, h2 *  0.12f, d2 * -0.08f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //4 //{ Vertex(8.0f, 6.0f, -4.0f) }, //4
		Vertex(w2 *  0.08f, h2 *  0.12f, d2 * -0.16f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //5 //{ Vertex(4.0f, 6.0f, -8.0f) }, //5
		Vertex(w2 * -0.08f, h2 *  0.12f, d2 * -0.16f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //6 //{ Vertex(-4.0f, 6.0f, -8.0f)}, //6
		Vertex(w2 * -0.16f, h2 *  0.12f, d2 * -0.08f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //7 //{ Vertex(-8.0f, 6.0f, -4.0f)}, //7
		Vertex(w2 * -0.16f, h2 *  0.12f, d2 *  0.08f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //8 //{ Vertex(-8.0f, 6.0f, 4.0f) }, //8
		Vertex(w2 * -0.08f, h2 *  0.18f, d2 * -0.04f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //9 //{ Vertex(-4.0f, 9.0f, -2.0f)}, //9
		Vertex(w2 * -0.08f, h2 *  0.18f, d2 *  0.04f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //10 //{ Vertex(-4.0f, 9.0f, 2.0f) }, //10
		Vertex(w2 * -0.04f, h2 *  0.18f, d2 *  0.08f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //11 //{ Vertex(-2.0f, 9.0f, 4.0f) }, //11
		Vertex(w2 *  0.04f, h2 *  0.18f, d2 *  0.08f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //12 //{ Vertex(2.0f, 9.0f, 4.0f)  }, //12
		Vertex(w2 *  0.08f, h2 *  0.18f, d2 *  0.04f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //13 //{ Vertex(4.0f, 9.0f, 2.0f)  }, //13
		Vertex(w2 *  0.08f, h2 *  0.18f, d2 * -0.04f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //14 //{ Vertex(4.0f, 9.0f, -2.0f) }, //14
		Vertex(w2 *  0.04f, h2 *  0.18f, d2 * -0.08f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),  //15 //{ Vertex(2.0f, 9.0f, -4.0f) }, //15
		Vertex(w2 * -0.04f, h2 *  0.18f, d2 * -0.08f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)   //16 //{ Vertex(-2.0f, 9.0f, -4.0f)}  //16
	};

	meshData.Vertices.assign(&v[0], &v[17]);

	uint32 i[96] = {
		1, 0, 2, //0
		2, 0, 3, //1
		8,0,1,//2
		//top front
		11,1,12,//3
		12,1,2,//4
		//top left front
		10,8,11,//5
		11,8,1,//6
		//top right front
		12,2,13,//7
		13,2,3,//8
		//mid right
		14,13,3,//9
		14,3,4,//10
		//mid right t down
		4,3,0,//11
		//mid left t down
		7,0,8,//12
		//mid left square
		9,7,8,//13
		10,9,8,//14
		//top left back
		16,6,7,//15
		16,7,9,//16
		//top left t down
		6,0,7,//17
		//top back square
		15,5,6,//18
		15,6,16,//19
		//top back down
		5,0,6,//20
		//top left back square
		14,5,15,//21
		5,14,4,//22
		//top left back t down
		5,4,0,//23
		//top cross 1
		16,11,15,//24
		11,12,15,//25
		10,13,14,//26
		10,14,9,//27
		12,13,16,//28
		16,9,12,//29
		11,14,10,//30
		10,14,15//31
	};

	meshData.Indices32.assign(&i[0], &i[96]);


	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;

}

GeometryGenerator::MeshData GeometryGenerator::CreatePyramid(uint32 numSubdivisions)
{
	MeshData meshData;

	Vertex v[16] =
	{
		//	Vertex(0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //0  		
			//base
			Vertex(0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
			Vertex(0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//1
			Vertex(-0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//2
			Vertex(-0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
			//right
			Vertex(0.0f, 1.f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//4
			Vertex(0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//5
			Vertex(0.5f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
			//front
			Vertex(0.0f, 1.f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//7
			Vertex(0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//8
			Vertex(-0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//9
			//left
			Vertex(0.0f, 1.f, 0.0f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//10
			Vertex(-0.5f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//11
			Vertex(-0.5f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//12
			//back
			Vertex(0.0f, 1.f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//13
			Vertex(0.5f, 0.0f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//14
			Vertex(-0.5f, 0.0f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)//15
	};

	meshData.Vertices.assign(&v[0], &v[16]);

	uint32 i[18] = {
		//bottom
		0,3,1,
		0,2,3,
		//right
		4,5,6,
		//left
		11,10,12,
		//back
		13,14,15,
		//front
		9,8,7
	};

	meshData.Indices32.assign(&i[0], &i[18]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;

}

GeometryGenerator::MeshData GeometryGenerator::CreateRhombo(uint32 numSubdivisions) 
{
	MeshData meshData;

	Vertex v[24] =
	{
		//right top
	Vertex(0.0f, 1.f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
	Vertex(0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//1
	Vertex(0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//2
	//front top
	Vertex(0.0f, 1.f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
	Vertex(0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//4
	Vertex(-0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//5
	//left top
	Vertex(0.0f, 1.f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
	Vertex(-0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//7
	Vertex(-0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//8
	//back top
	Vertex(0.0f, 1.f, 0.0f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//9
	Vertex(0.5f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//10
	Vertex(-0.5f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//11

	//right bottom
	Vertex(0.0f, -1.f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//12
	Vertex(0.5f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//13
	Vertex(0.5f, 0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//14
	//front bottom
	Vertex(0.0f, -1.f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//15
	Vertex(0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//16
	Vertex(-0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//17
	//left bottom
	Vertex(0.0f, -1.f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//18
	Vertex(-0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//19
	Vertex(-0.5f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//20
	//back bottom
	Vertex(0.0f, -1.f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//21
	Vertex(0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//22
	Vertex(-0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)//23
	};

	meshData.Vertices.assign(&v[0], &v[24]);

	uint32 i[24] = {
		//right top
		0,1,2,
		//front top
		3,5,4,
		//top left
		6,8,7,
		//top back
		9,10,11,
		//right bot
		12,14,13,
		//front bot
		15,16,17,
		//left bot
		18,19,20,
		//back bot
		21,23,22
	};

	meshData.Indices32.assign(&i[0], &i[24]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;

}

GeometryGenerator::MeshData GeometryGenerator::CreatePrism(uint32 numSubdivisions)
{
	MeshData meshData;

	Vertex v[8] =
	{
		//middle
	Vertex(-5.f, 0.f, -8.66f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
	Vertex(-10.f, 0.f, 0.f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//1
	Vertex(10.f, 0.f, 0.f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//2
	Vertex(-5.f, 0.f, 8.66f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
	Vertex(5.f, 0.f, 8.66f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//4
	Vertex(5.f, 0.f, -8.66f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//5
	//top
	Vertex(0.f,10.0f, 0.f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
	//bottom
	Vertex(0.f,-10.0f,0.f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//7
	};

	meshData.Vertices.assign(&v[0], &v[8]);

	uint32 i[36] = {
		//top
		6,5,0,
		6,2,5,
		6,4,2,
		6,3,4,
		6,1,3,
		6,0,1,
		//bottom
		7,1,0,
		7,3,1,
		7,4,3,
		7,2,4,
		7,5,2,
		7,0,5
	};

	meshData.Indices32.assign(&i[0], &i[36]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;

}

GeometryGenerator::MeshData GeometryGenerator::CreateBox1(uint32 numSubdivisions) {

	MeshData meshData;

	XMVECTOR v0, v1, v2, back;

	v0 = { -0.65f,-0.65f,0.65f };
	v1 = { 0.65f,-0.65f,0.65f };
	v2 = { 0.65f,0.65f,0.65f };
	back = ComputeNormal(v0, v1, v2); //back face

	XMVECTOR v4, v5, v6, front;

	v4 = { -0.65f,-0.65f,-0.65f };
	v5 = { 0.65f,-0.65f,-0.65f };
	v6 = { 0.65f,0.65f,-0.65f };
	front = ComputeNormal(v4, v5, v6); //front face

	XMVECTOR v8, v9, v10, bottom;

	v8 = { 0.65f,-0.65f,0.65f };
	v9 = { -0.65f,-0.65f,0.65f };
	v10 = { -0.65f,-0.65f,0.65f };
	bottom = ComputeNormal(v8, v9, v10); //bottom face

	XMVECTOR v12, v13, v14, right;

	v12 = { 0.65f,-0.65f,-0.65f };
	v13 = { 0.65f,-0.65f,0.65f };
	v14 = { 0.65f,0.65f,0.65f };
	right = ComputeNormal(v12, v13, v14); //right face

	XMVECTOR v16, v17, v18, left;

	v16 = { -0.65f,-0.65f,-0.65f };
	v17 = { -0.65f,-0.65f,0.65f };
	v18 = { -0.65f,0.65f,0.65f };
	left = ComputeNormal(v16, v17, v18); //left face

	XMVECTOR v20, v21, v22, binner;

	v20 = { -0.60f,-0.65f,0.60f };
	v21 = { 0.60f,-0.65f,0.60f };
	v22 = { 0.60f,0.65f,0.60f };
	binner = ComputeNormal(v20, v21, v22); //back inner face

	XMVECTOR v24, v25, v26, finner;

	v24 = { -0.60f,-0.65f,-0.60f };
	v25 = { 0.60f,-0.65f,-0.60f };
	v26 = { 0.60f,0.65f,-0.60f };
	finner = ComputeNormal(v24, v25, v26); //front inner face

	XMVECTOR v28, v29, v30, rinner;

	v28 = { -0.60f,-0.65f,-0.60f };
	v29 = { 0.60f,-0.65f,-0.60f };
	v30 = { 0.60f,0.65f,-0.60f };
	rinner = ComputeNormal(v28, v29, v30); //right inner face

	XMVECTOR v32, v33, v34, linner;

	v32 = { -0.60f,-0.65f,-0.60f };
	v33 = { -0.60f,-0.65f,0.60f };
	v34 = { -0.60f,0.65f,0.60f };
	linner = ComputeNormal(v32, v33, v34); //right inner face

	Vertex v[36] =
	{
		//Vertex(-1, 0, 0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //0  		 
		//back
		Vertex(-0.65f,-0.65f,0.65f, XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
		Vertex(0.65f,-0.65f,0.65f,XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//1
		Vertex(0.65f,0.65f,0.65f,XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//2
		Vertex(-0.65f,0.65f,0.65f,XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
		//front
		Vertex(-0.65f,-0.65f,-0.65f,XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//4
		Vertex(0.65f,-0.65f,-0.65f, XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//5
		Vertex(0.65f,0.65f,-0.65f, XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
		Vertex(-0.65f,0.65f,-0.65f, XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//7		
		//bottom
		Vertex(0.65f,-0.65f,0.65f, XMVectorGetX(bottom), XMVectorGetY(bottom), XMVectorGetZ(bottom), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//8
		Vertex(-0.65f,-0.65f,0.65f, XMVectorGetX(bottom), XMVectorGetY(bottom), XMVectorGetZ(bottom), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//9
		Vertex(-0.65f,-0.65f,-0.65f, XMVectorGetX(bottom), XMVectorGetY(bottom), XMVectorGetZ(bottom), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//10
		Vertex(0.65f,-0.65f,-0.65f, XMVectorGetX(bottom), XMVectorGetY(bottom), XMVectorGetZ(bottom), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//11
		//right
		Vertex(0.65f,-0.65f,-0.65f, XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//12
		Vertex(0.65f,-0.65f,0.65f, XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//13
		Vertex(0.65f,0.65f,0.65f, XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//14
		Vertex(0.65f,0.65f,-0.65f, XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//15
		//left
		Vertex(-0.65f,-0.65f,-0.65f, XMVectorGetX(left), XMVectorGetY(left), XMVectorGetZ(left), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//16
		Vertex(-0.65f,-0.65f,0.65f, XMVectorGetX(left), XMVectorGetY(left), XMVectorGetZ(left), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//17
		Vertex(-0.65f,0.65f,0.65f, XMVectorGetX(left), XMVectorGetY(left), XMVectorGetZ(left), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//18
		Vertex(-0.65f,0.65f,-0.65f, XMVectorGetX(left), XMVectorGetY(left), XMVectorGetZ(left), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//19

		//back inner
		Vertex(-0.60f,-0.65f,0.60f, XMVectorGetX(binner), XMVectorGetY(binner), XMVectorGetZ(binner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//20 //0
		Vertex(0.60f,-0.65f,0.60f,XMVectorGetX(binner), XMVectorGetY(binner), XMVectorGetZ(binner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//21 //1
		Vertex(0.60f,0.65f,0.60f,XMVectorGetX(binner), XMVectorGetY(binner), XMVectorGetZ(binner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//22 //2
		Vertex(-0.60f,0.65f,0.60f,XMVectorGetX(binner), XMVectorGetY(binner), XMVectorGetZ(binner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//23 //3

		//front inner
		Vertex(-0.60f,-0.65f,-0.60f,XMVectorGetX(finner), XMVectorGetY(finner), XMVectorGetZ(finner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//24 4
		Vertex(0.60f,-0.65f,-0.60f, XMVectorGetX(finner), XMVectorGetY(finner), XMVectorGetZ(finner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//25 5
		Vertex(0.60f,0.65f,-0.60f, XMVectorGetX(finner), XMVectorGetY(finner), XMVectorGetZ(finner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//26  6
		Vertex(-0.60f,0.65f,-0.60f, XMVectorGetX(finner), XMVectorGetY(finner), XMVectorGetZ(finner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//27 7		

		//right inner
		Vertex(0.60f,-0.65f,-0.60f, XMVectorGetX(rinner), XMVectorGetY(rinner), XMVectorGetZ(rinner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//28 12
		Vertex(0.60f,-0.65f,0.60f, XMVectorGetX(rinner), XMVectorGetY(rinner), XMVectorGetZ(rinner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//29 13
		Vertex(0.60f,0.65f,0.60f, XMVectorGetX(rinner), XMVectorGetY(rinner), XMVectorGetZ(rinner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//30 14
		Vertex(0.60f,0.65f,-0.60f, XMVectorGetX(rinner), XMVectorGetY(rinner), XMVectorGetZ(rinner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//31 15

		//left inner
		Vertex(-0.60f,-0.65f,-0.60f, XMVectorGetX(linner), XMVectorGetY(linner), XMVectorGetZ(linner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//32 16
		Vertex(-0.60f,-0.65f,0.60f, XMVectorGetX(linner), XMVectorGetY(linner), XMVectorGetZ(linner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//33 17
		Vertex(-0.60f,0.65f,0.60f, XMVectorGetX(linner), XMVectorGetY(linner), XMVectorGetZ(linner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//34 18
		Vertex(-0.60f,0.65f,-0.60f, XMVectorGetX(linner), XMVectorGetY(linner), XMVectorGetZ(linner), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//35 19
	};

	meshData.Vertices.assign(&v[0], &v[36]);

	uint32 i[84] = {
		//back
		0, 1, 2,
		2, 3, 0,
		//front
		6,4,7,
		6,5,4,
		//bottom
		8,9,10,
		8,10,11,
		//left
		19,16,17,
		17,18,19,
		//right
		12,14,13,
		12, 15,14,

		//bottom up
		8,10,9,
		8,11,10,

		//back top
		3,2,23,
		2,22,23,

		//back inside
		23,21,20,
		23,22,21,

		//front top
		6,7,26,
		7,27,26,

		//front inside
		26, 24,25,
		26, 27, 24,

		//right top
		14,15, 30,
		15, 31,30,

		//right inside
		30,28, 29,
		30, 31, 28,

		//left top
		19, 18, 35,
		18, 34, 35,

		//left inside
		35 , 33 , 32,
		35,34,33


	};
	meshData.Indices32.assign(&i[0], &i[84]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;
}



/*
GeometryGenerator::MeshData GeometryGenerator::CreateBox2(float width, float height, float depth, uint32 numSubdivisions) {

	MeshData meshData;

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	XMVECTOR v0, v1, v2, back;

	v0 = { -0.65f, 0.0, 0.5 }; //from vertex v[0]
	v1 = { 0.5f, 0.0f, -0.5f };
	v2 = { -0.5f, 0.0f, 0.5f, 0.0f };
	back = ComputeNormal(v0, v1, v2); //base face

	XMVECTOR v4, v5, v6, front;

	v4 = { -0.65f, -0.65f, -0.65f };
	v5 = { 0.65f, -0.65f, -0.65f };
	v6 = { 0.65f, 0.65f, -0.65f, 0.0f };
	front = ComputeNormal(v4, v5, v6); //front face

	XMVECTOR v8, v9, v10, bottom;

	v8 = { 0.65f, -0.65f, 0.65f };
	v9 = { 0.65, -0.65, 0.65 };
	v10 = { -0.65, -0.65, -0.65, 0.0f };
	bottom = ComputeNormal(v8, v9, v10); //front face

	XMVECTOR v12, v13, v14, right;

	v12 = { 0.65, -0.65, -0.65 };
	v13 = { 0.65, -0.65, 0.65 };
	v14 = { 0.65, 0.65, 0.65, 0.0f };
	right = ComputeNormal(v12, v13, v14); //right face

	XMVECTOR v16, v17, v18, left;

	v16 = { -0.65f, -0.65, -0.65 };
	v17 = { -0.65f, -0.65, 0.65 };
	v18 = { -0.65f, 0.65, 0.65, 0.0f };
	left = ComputeNormal(v16, v17, v18); //left face


	Vertex v[36] =
	{
		//Vertex(-1, 0, 0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //0  		 
		//back
		//Vertex(w2 * -0.65f,h2 * -0.65f,d2 * 0.65f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
		
		Vertex(-0.65f, -0.65f, 0.65f,XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
		Vertex(0.65f, -0.65f, 0.65f,XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//1
		Vertex(0.65f, 0.65f, 0.65f,XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//2
		Vertex(-0.65f, 0.65f, 0.65f,XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
		//front
		Vertex(-0.65f, -0.65f, -0.65f,XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//4
		Vertex(0.65f, -0.65, -0.65,XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//5
		Vertex(0.65f, 0.65, -0.65,XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
		Vertex(-0.65f, 0.65f, -0.65f, XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//7		
		//bottom
		Vertex(0.65f, -0.65f, 0.65f, XMVectorGetX(bottom), XMVectorGetY(bottom), XMVectorGetZ(bottom), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//8
		Vertex(-0.65f, -0.65, 0.65, XMVectorGetX(bottom), XMVectorGetY(bottom), XMVectorGetZ(bottom), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//9
		Vertex(-0.65f, -0.65, -0.65, XMVectorGetX(bottom), XMVectorGetY(bottom), XMVectorGetZ(bottom), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//10
		Vertex(0.65f, -0.65, -0.65, XMVectorGetX(bottom), XMVectorGetY(bottom), XMVectorGetZ(bottom), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//11
		//right
		Vertex(0.65f, -0.65, -0.65, XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//12
		Vertex(0.65f, -0.65, 0.65, XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//13
		Vertex(0.65f, 0.65, 0.65, XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//14
		Vertex(0.65f, 0.65, -0.65, XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//15
		//left
		Vertex(-0.65f, -0.65, -0.65, XMVectorGetX(left), XMVectorGetY(left), XMVectorGetZ(left), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//16
		Vertex(-0.65f, -0.65, 0.65, XMVectorGetX(left), XMVectorGetY(left), XMVectorGetZ(left), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//17
		Vertex(-0.65f, 0.65, 0.65, XMVectorGetX(left), XMVectorGetY(left), XMVectorGetZ(left), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//18
		Vertex(-0.65f, 0.65, -0.65, XMVectorGetX(left), XMVectorGetY(left), XMVectorGetZ(left), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//19

		//back inner
		Vertex(-0.60f, -0.65f, 0.60f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//20 //0
		Vertex(0.60f, -0.65f, 0.60f,0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//21 //1
		Vertex(0.60f, 0.65f, 0.60f,0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//22 //2
		Vertex(-0.60f, 0.65f, 0.60f,0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//23 //3

		//front inner
		Vertex(-0.60f,-0.65f,-0.60f,0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//24 4
		Vertex(0.60f,-0.65f,-0.60, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//25 5
		Vertex(0.60f,0.65f,-0.60, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//26  6
		Vertex(-0.60f,0.65f,-0.60f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//27 7		

		//right inner
		Vertex(0.60f,-0.65f,-0.60, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//28 12
		Vertex(0.60f,-0.65f,0.60, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//29 13
		Vertex(0.60f,0.65f,0.60, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//30 14
		Vertex(0.60f,0.65f,-0.60, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//31 15

		//left inner
		Vertex(-0.60f,-0.65f,-0.60, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//32 16
		Vertex(-0.60f,-0.65f,0.60, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//33 17
		Vertex(-0.60f,0.65f,0.60, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//34 18
		Vertex(-0.60f,0.65f,-0.60, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//35 19
	};

	meshData.Vertices.assign(&v[0], &v[36]);

	uint32 i[84] = {
		//back
		0, 1, 2,
		2, 3, 0,
		//front
		6,4,7,
		6,5,4,
		//bottom
		8,9,10,
		8,10,11,
		//left
		19,16,17,
		17,18,19,
		//right
		12,14,13,
		12, 15,14,

		//bottom up
		8,10,9,
		8,11,10,

		//back top
		3,2,23,
		2,22,23,

		//back inside
		23,21,20,
		23,22,21,

		//front top
		6,7,26,
		7,27,26,

		//front inside
		26, 24,25,
		26, 27, 24,

		//right top
		14,15, 30,
		15, 31,30,

		//right inside
		30,28, 29,
		30, 31, 28,

		//left top
		19, 18, 35,
		18, 34, 35,

		//left inside
		35 , 33 , 32,
		35,34,33


	};
	meshData.Indices32.assign(&i[0], &i[84]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;
}
*/
GeometryGenerator::MeshData GeometryGenerator::CreateBox3(uint32 numSubdivisions) {

	MeshData meshData;

	XMVECTOR v0, v1, v2, back;

	v0 = { -0.65f, 0.0, 0.5 }; //from vertex v[0]
	v1 = { 0.5f, 0.0f, -0.5f };
	v2 = { -0.5f, 0.0f, 0.5f, 0.0f };
	back = ComputeNormal(v0, v1, v2); //base face

	XMVECTOR v5, v6,  front;

	v5 = { -0.65f, 0.0, 0.5 }; //from vertex v[0]
	v6 = { 0.5f, 0.0f, -0.5f };
	front = ComputeNormal(v5, v6, -v6); //base face

	Vertex v[12] =
	{
		//base
		Vertex(0.f,-0.5f,0.86f, XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
		Vertex(-1,-0.5f,-0.86f,XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//1
		Vertex(1,-0.5f,-0.86f,XMVectorGetX(back), XMVectorGetY(back), XMVectorGetZ(back), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//2
		//front
		Vertex(1.f, 0.5f,-0.86f, XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
		Vertex(-1.f, 0.5f,-0.86f,XMVectorGetX(front), XMVectorGetY(front), XMVectorGetZ(front), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//4
		//right // left
		Vertex(0.f,0.5f,0.86f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//5
		//base inner
		Vertex(0.f,-0.45f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
		Vertex(-1,-0.45f,-0.8f,0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//7
		Vertex(1,-0.45f,-0.8f,0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//8
		//front top
		Vertex(0.9f, 0.5f,-0.80f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//9
		Vertex(-0.9f, 0.5f,-0.80f,0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//10
		//right and left top
		Vertex(0.f,0.5f, 0.8f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//11
	};

	meshData.Vertices.assign(&v[0], &v[12]);

	uint32 i[69] = {
		//bottom
		0, 1,2,
		//front
		4,3,2,
		4,2,1,
		//right
		5,0,3,
		0,2,3,
		//left
		1,0,4,
		5,4,0,

		//bottom up
		6,8,7,
		//front up
		9,3,4,
		9,4,10,
		10,9,3,
		//right top
		11,5,9,
		9,5,3,
		9,3,11,

		//left top
		11,10,5,
		5,10,4,
		11,4,5,
		//front inner
		10,8,9,
		8,10,7,
		//right inner
		9,6,11,
		6,9,8,
		//left inner
		11,7,10,
		11,6,7

	};
	meshData.Indices32.assign(&i[0], &i[69]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateCandy1(float width, float height, float depth, uint32 numSubdivisions) {

	MeshData meshData;


	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	float divider = 4.0f;

	Vertex v[18] =
	{
			Vertex(w2 * -1.0f / divider,h2 * 0.0f, d2 * 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //0  		 
			Vertex(w2 * -0.8f / divider, h2 * 0.0f, d2 * -1.0f / divider, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //1
			Vertex(w2 * 0.8f / divider, h2 * 0.0f, d2 * -1.0f / divider, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //2
			Vertex(w2 * 1.0f / divider, h2 * 0.0f , d2 * 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //3
			Vertex(w2 * 0.8f / divider, h2 * 0.0f , d2 * 1.0f / divider, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //4
			Vertex(w2 * -0.8f / divider, h2 * 0.0f, d2 * 1.0f / divider, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //5 

			Vertex(w2 * -0.6f / divider, h2 * 1.0f / divider, d2 * 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
			Vertex(w2 * -0.4f / divider, h2 * 1.0f / divider, d2 * -0.6f / divider,0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //7
			Vertex(w2 * 0.4f / divider, h2 * 1.0f / divider, d2 * -0.6f / divider,0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //8
			Vertex(w2 * 0.6f / divider, h2 * 1.0f / divider, d2 * 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //9
			Vertex(w2 * 0.4f / divider, h2 * 1.0f / divider, d2 * 0.6f / divider, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //10
			Vertex(w2 * -0.4f / divider, h2 * 1.0f / divider, d2 * 0.6f / divider, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//11

			Vertex(w2 * -0.6f / divider, h2 * -1.0f / divider, d2 * 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//12
			Vertex(w2 * -0.4f / divider, h2 * -1.0f / divider, d2 * -0.6f / divider, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//13
			Vertex(w2 * 0.4f / divider, h2 * -1.0f / divider, d2 * -0.6f / divider, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//14
			Vertex(w2 * 0.6f / divider, h2 * -1.0f  / divider, d2 * 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//15
			Vertex(w2 * 0.4f / divider, h2 * -1.0f / divider, d2 * 0.6f / divider, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//16
			Vertex(w2 * -0.4f / divider, h2 * -1.0f / divider, d2 * 0.6f / divider, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)//17
	};

	meshData.Vertices.assign(&v[0], &v[18]);

	uint32 i[129] = {
		//top
		7, 6, 11,
		7, 11, 10,
		10, 8, 7,
		8, 10, 9,

		6,5,11,
		11,6,0,
		0,5,11,
		11,5,4,

		10,5,4,
		10,11,4,
		9,10,4,
		9,10,3,//12

		4,3,10,
		9,3,8,
		9,2,8,
		3,2,9,//16

		8,2,7,
		8,1,7,
		2,1,8,
		6,7,0,//20

		6,7,1,
		1,0,6,
		17,4,5,
		0,17,5,//24

		0,12,5,
		0,17,5,
		17,5,12,
		17,16,4,//28

		16,15,4,
		16,3,4,
		15,3,4,
		15,14,3,//32//

		3,15,2,
		2,15,14,
		14,13,1,//
		2,14,1,//36

		13,12,0,
		12,0,1,
		1,13,0,

		//bottom
		13,17,12,
		16,17,13,
		14,16,13,
		16,14,15
	};

	meshData.Indices32.assign(&i[0], &i[129]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateCandy2(uint32 numSubdivisions) {

	MeshData meshData;

	XMVECTOR v0, v1, v2, base;

	v0 = { -1.f, -0.5f, 2.5f };
	v1 = { 1.f, -0.5f, 2.5f };
	v2 = { -1.f, -0.5f, -2.5f };

	base = ComputeNormal(v0, v1, v2);

	XMVECTOR v4, v5, v6, row1;

	v4 = { -1.f, 0.f, 2.5f };
	v5 = { 0.f, 0.f, 2.5f };
	v6 = { 1.f, 0.f, 2.5f };

	row1 = ComputeNormal(v4, v5, v6);

	XMVECTOR v7, v8, v9, row2;

	v7 = { -1.f, 0.f, 1.5f };
	v8 = { 0.f, 0.f, 1.5f };
	v9 = { 1.f, 0.f, 1.5f };

	row2 = ComputeNormal(v7, v8, v9);

	XMVECTOR v10, v11, v12, row3;

	v10 = { -1.f, 0.f, 0.5f };
	v11 = { 0.f, 0.f, 0.5f };
	v12 = { 1.f, 0.f, 0.5f };

	row3 = ComputeNormal(v10, v11, v12);

	XMVECTOR v13, v14, v15, row4;

	v13 = { -1.f, 0.f, -0.5f };
	v14 = { 0.f, 0.f, -0.5f };
	v15 = { 1.f, 0.f, -0.5f };

	row4 = ComputeNormal(v13, v14, v15);

	XMVECTOR v16, v17, v18, row5;

	v16 = { -1.f, 0.f, -1.5f };
	v17 = { 0.f, 0.f, -1.5f };
	v18 = { 1.f, 0.f, -1.5f };

	row5 = ComputeNormal(v16, v17, v18);

	XMVECTOR v19, v20, v21, row6;

	v19 = { -1.f, 0.f, -2.5f };
	v20 = { 0.f, 0.f, -2.5f };
	v21 = { 1.f, 0.f, -2.5f };

	row6 = ComputeNormal(v19, v20, v21);

	XMVECTOR v22, v23, v24, row31;

	v22 = { -0.8f, 0.2f, 2.2f };
	v23 = { -0.2f, 0.2f, 2.2f };
	v24 = { 0.2f, 0.2f, 2.2f };

	row31 = ComputeNormal(v22, v23, v24);

	XMVECTOR v26, v27, v28, row32;

	v26 = { -0.8f, 0.2f, 1.8f };
	v27 = { -0.2f, 0.2f, 1.8f };
	v28 = { 0.2f, 0.2f, 1.8f };

	row32 = ComputeNormal(v26, v27, v28);

	XMVECTOR v30, v31, v32, row33;

	v30 = { -0.8f, 0.2f, 1.2f };
	v31 = { -0.2f, 0.2f, 1.2f };
	v32 = { 0.2f, 0.2f, 1.2f };

	row33 = ComputeNormal(v30, v31, v32);

	XMVECTOR v34, v35, v36, row34;

	v34 = { -0.8f, 0.2f, 0.8f };
	v35 = { -0.2f, 0.2f, 0.8f };
	v36 = { 0.2f, 0.2f, 0.8f };

	row34 = ComputeNormal(v34, v35, v36);

	XMVECTOR v38, v39, v40, row35;

	v38 = { -0.8f, 0.2f, 0.2f };
	v39 = { -0.2f, 0.2f, 0.2f };
	v40 = { 0.2f, 0.2f, 0.2f };

	row35 = ComputeNormal(v38, v39, v40);

	XMVECTOR v42, v43, v44, row36;

	v42 = { -0.8f, 0.2f, -0.2f };
	v43 = { -0.2f, 0.2f, -0.2f };
	v44 = { 0.2f, 0.2f, -0.2f };

	row36 = ComputeNormal(v42, v43, v44);

	XMVECTOR v46, v47, v48, row37;

	v46 = { -0.8f, 0.2f, -0.8f };
	v47 = { -0.2f, 0.2f, -0.8f };
	v48 = { 0.2f, 0.2f, -0.8f };

	row37 = ComputeNormal(v46, v47, v48);

	XMVECTOR v50, v51, v52, row38;

	v50 = { -0.8f, 0.2f, -1.2f };
	v51 = { -0.2f, 0.2f, -1.2f };
	v52 = { 0.2f, 0.2f, -1.2f };

	row38 = ComputeNormal(v50, v51, v52);

	XMVECTOR v54, v55, v56, row39;

	v54 = { -0.8f, 0.2f, -1.8f };
	v55 = { -0.2f, 0.2f, -1.8f };
	v56 = { 0.2f, 0.2f, -1.8f };

	row39 = ComputeNormal(v54, v55, v56);

	XMVECTOR v58, v59, v60, row300;

	v58 = { -0.8f, 0.2f, -2.2f };
	v59 = { -0.2f, 0.2f, -2.2f };
	v60 = { 0.2f, 0.2f, -2.2f };

	row300 = ComputeNormal(v58, v59, v50);

	Vertex v[62] =
	{
		//base
			Vertex(-1.f, -0.5f, 2.5f, XMVectorGetX(base), XMVectorGetY(base), XMVectorGetZ(base), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //0  		 
			Vertex(1.f, -0.5f, 2.5f, XMVectorGetX(base), XMVectorGetY(base), XMVectorGetZ(base), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //1
			Vertex(-1.f, -0.5f, -2.5f, XMVectorGetX(base), XMVectorGetY(base), XMVectorGetZ(base), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //2
			Vertex(1.f, -0.5f, -2.5f, XMVectorGetX(base), XMVectorGetY(base), XMVectorGetZ(base), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
		//2nd floor
			//1st row
			Vertex(-1.f, 0.f, 2.5f, XMVectorGetX(row1), XMVectorGetY(row1), XMVectorGetZ(row1), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //4  		 
			Vertex(0.f, 0.f, 2.5f, XMVectorGetX(row1), XMVectorGetY(row1), XMVectorGetZ(row1), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //5
			Vertex(1.f, 0.f, 2.5f, XMVectorGetX(row1), XMVectorGetY(row1), XMVectorGetZ(row1), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //6
			//2nd row
			Vertex(-1.f, 0.f, 1.5f, XMVectorGetX(row2), XMVectorGetY(row2), XMVectorGetZ(row2), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //7	 
			Vertex(0.f, 0.f, 1.5f, XMVectorGetX(row2), XMVectorGetY(row2), XMVectorGetZ(row2), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //8
			Vertex(1.f, 0.f, 1.5f, XMVectorGetX(row2), XMVectorGetY(row2), XMVectorGetZ(row2), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //9
			//3th row
			Vertex(-1.f, 0.f, 0.5f, XMVectorGetX(row3), XMVectorGetY(row3), XMVectorGetZ(row3), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //10  		 
			Vertex(0.f, 0.f, 0.5f, XMVectorGetX(row3), XMVectorGetY(row3), XMVectorGetZ(row3), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //11
			Vertex(1.f, 0.f, 0.5f, XMVectorGetX(row3), XMVectorGetY(row3), XMVectorGetZ(row3), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //12
			//4th row
			Vertex(-1.f, 0.f, -0.5f, XMVectorGetX(row4), XMVectorGetY(row4), XMVectorGetZ(row4), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //13  		 
			Vertex(0.f, 0.f, -0.5f, XMVectorGetX(row4), XMVectorGetY(row4), XMVectorGetZ(row4), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //14
			Vertex(1.f, 0.f, -0.5f, XMVectorGetX(row4), XMVectorGetY(row4), XMVectorGetZ(row4), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //15
			//5th row
			Vertex(-1.f, 0.f, -1.5f, XMVectorGetX(row5), XMVectorGetY(row5), XMVectorGetZ(row5), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //16 
			Vertex(0.f, 0.f, -1.5f, XMVectorGetX(row5), XMVectorGetY(row5), XMVectorGetZ(row5), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //17
			Vertex(1.f, 0.f, -1.5f, XMVectorGetX(row5), XMVectorGetY(row5), XMVectorGetZ(row5), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //18
			//6th row
			Vertex(-1.f, 0.f, -2.5f, XMVectorGetX(row6), XMVectorGetY(row6), XMVectorGetZ(row6), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //19  		 
			Vertex(0.f, 0.f, -2.5f, XMVectorGetX(row6), XMVectorGetY(row6), XMVectorGetZ(row6), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //20
			Vertex(1.f, 0.f, -2.5f, XMVectorGetX(row6), XMVectorGetY(row6), XMVectorGetZ(row6), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //21
		//3th floor
			//1st row
			Vertex(-0.8f, 0.2f, 2.2f, XMVectorGetX(row31), XMVectorGetY(row31), XMVectorGetZ(row31), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //22
			Vertex(-0.2f, 0.2f, 2.2f, XMVectorGetX(row31), XMVectorGetY(row31), XMVectorGetZ(row31), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //23
			Vertex(0.2f, 0.2f, 2.2f, XMVectorGetX(row31), XMVectorGetY(row31), XMVectorGetZ(row31), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //24
			Vertex(0.8f, 0.2f, 2.2f, XMVectorGetX(row31), XMVectorGetY(row31), XMVectorGetZ(row31), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //25
			//2nd row
			Vertex(-0.8f, 0.2f, 1.8f, XMVectorGetX(row32), XMVectorGetY(row32), XMVectorGetZ(row32), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //26
			Vertex(-0.2f, 0.2f, 1.8f, XMVectorGetX(row32), XMVectorGetY(row32), XMVectorGetZ(row32), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //27
			Vertex(0.2f, 0.2f, 1.8f, XMVectorGetX(row32), XMVectorGetY(row32), XMVectorGetZ(row32), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //28
			Vertex(0.8f, 0.2f, 1.8f, XMVectorGetX(row32), XMVectorGetY(row32), XMVectorGetZ(row32), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //29

			//3th row
			Vertex(-0.8f, 0.2f, 1.2f, XMVectorGetX(row33), XMVectorGetY(row33), XMVectorGetZ(row33), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //30
			Vertex(-0.2f, 0.2f, 1.2f, XMVectorGetX(row33), XMVectorGetY(row33), XMVectorGetZ(row33), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //31
			Vertex(0.2f, 0.2f, 1.2f, XMVectorGetX(row33), XMVectorGetY(row33), XMVectorGetZ(row33), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //32
			Vertex(0.8f, 0.2f, 1.2f, XMVectorGetX(row33), XMVectorGetY(row33), XMVectorGetZ(row33), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //33
			//4th row
			Vertex(-0.8f, 0.2f, 0.8f, XMVectorGetX(row34), XMVectorGetY(row34), XMVectorGetZ(row34), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //34
			Vertex(-0.2f, 0.2f, 0.8f, XMVectorGetX(row34), XMVectorGetY(row34), XMVectorGetZ(row34), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //35
			Vertex(0.2f, 0.2f, 0.8f, XMVectorGetX(row34), XMVectorGetY(row34), XMVectorGetZ(row34), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //36
			Vertex(0.8f, 0.2f, 0.8f, XMVectorGetX(row34), XMVectorGetY(row34), XMVectorGetZ(row34), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //37

			//5th row
			Vertex(-0.8f, 0.2f, 0.2f, XMVectorGetX(row35), XMVectorGetY(row35), XMVectorGetZ(row35), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //38
			Vertex(-0.2f, 0.2f, 0.2f, XMVectorGetX(row35), XMVectorGetY(row35), XMVectorGetZ(row35), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //39
			Vertex(0.2f, 0.2f, 0.2f, XMVectorGetX(row35), XMVectorGetY(row35), XMVectorGetZ(row35), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //40
			Vertex(0.8f, 0.2f, 0.2f, XMVectorGetX(row35), XMVectorGetY(row35), XMVectorGetZ(row35), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //41
			//6th row
			Vertex(-0.8f, 0.2f, -0.2f, XMVectorGetX(row36), XMVectorGetY(row36), XMVectorGetZ(row36), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //42
			Vertex(-0.2f, 0.2f, -0.2f, XMVectorGetX(row36), XMVectorGetY(row36), XMVectorGetZ(row36), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //43
			Vertex(0.2f, 0.2f, -0.2f, XMVectorGetX(row36), XMVectorGetY(row36), XMVectorGetZ(row36), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //44
			Vertex(0.8f, 0.2f, -0.2f, XMVectorGetX(row36), XMVectorGetY(row36), XMVectorGetZ(row36), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //45

			//7th row
			Vertex(-0.8f, 0.2f, -0.8f, XMVectorGetX(row37), XMVectorGetY(row37), XMVectorGetZ(row37), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //46
			Vertex(-0.2f, 0.2f, -0.8f, XMVectorGetX(row37), XMVectorGetY(row37), XMVectorGetZ(row37), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //47
			Vertex(0.2f, 0.2f, -0.8f, XMVectorGetX(row37), XMVectorGetY(row37), XMVectorGetZ(row37), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //48
			Vertex(0.8f, 0.2f, -0.8f, XMVectorGetX(row37), XMVectorGetY(row37), XMVectorGetZ(row37), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //49
			//8th row
			Vertex(-0.8f, 0.2f, -1.2f, XMVectorGetX(row38), XMVectorGetY(row38), XMVectorGetZ(row38), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //50
			Vertex(-0.2f, 0.2f, -1.2f, XMVectorGetX(row38), XMVectorGetY(row38), XMVectorGetZ(row38), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //51
			Vertex(0.2f, 0.2f, -1.2f, XMVectorGetX(row38), XMVectorGetY(row38), XMVectorGetZ(row38), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //52
			Vertex(0.8f, 0.2f, -1.2f, XMVectorGetX(row38), XMVectorGetY(row38), XMVectorGetZ(row38), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //53

			//9th row
			Vertex(-0.8f, 0.2f, -1.8f, XMVectorGetX(row39), XMVectorGetY(row39), XMVectorGetZ(row39), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //54
			Vertex(-0.2f, 0.2f, -1.8f, XMVectorGetX(row39), XMVectorGetY(row39), XMVectorGetZ(row39), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //55
			Vertex(0.2f, 0.2f, -1.8f, XMVectorGetX(row39), XMVectorGetY(row39), XMVectorGetZ(row39), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //56
			Vertex(0.8f, 0.2f, -1.8f, XMVectorGetX(row39), XMVectorGetY(row39), XMVectorGetZ(row39), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //57
			//10th row
			Vertex(-0.8f, 0.2f, -2.2f, XMVectorGetX(row300), XMVectorGetY(row300), XMVectorGetZ(row300), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //58
			Vertex(-0.2f, 0.2f, -2.2f, XMVectorGetX(row300), XMVectorGetY(row300), XMVectorGetZ(row300), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //59
			Vertex(0.2f, 0.2f, -2.2f, XMVectorGetX(row300), XMVectorGetY(row300), XMVectorGetZ(row300), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //60
			Vertex(0.8f, 0.2f, -2.2f, XMVectorGetX(row300), XMVectorGetY(row300), XMVectorGetZ(row300), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //61

	};

	meshData.Vertices.assign(&v[0], &v[62]);

	uint32 i[342] = {
		//base
		0,3,1,
		0,2,3,
		//front 2nd
		21,2,19,
		21,3,2,
		//right 2nd
		21,6,3,
		6,1,3,
		//left 2nd
		4,19,0,
		2,0,19,
		//back 2nd
		6,4,1,
		4,0,1,//30

		//square r1c1
		23,26,22,
		23,27,26,
		//square r1c2
		25,28,24,
		25,29,28,
		//square r2c1
		31,34,30,
		31,35,34,

		//square r2c2
		33, 36, 32,
		33,37,36,

		//square r3c1
		39,42,38,
		39, 43, 42,//60

		//square r3c2
		41, 44, 40,
		41,45,44,

		//square r4c1
		47,50,46,
		47,51,50,
		//square r4c2
		49,52,48,
		49,53,52,

		//square r5c1
		55,58,54,
		55,59,58,
		//square r5c2
		57,60,56,
		57,61,60,//90

		//down r5c1
		59,19,58,
		59,20,19,
		//down r5c2
		61,20,60,
		61,21,20,
		//up r5c1
		55,54,17,
		54,16,17,
		//up r5c2
		57,56,18,
		56,17,18,
		//right r5c2
		57,18,61,
		18,21,61,//120
		//right r5c1
		55,17,59,
		17,20,59,
		//left r5c1
		58,19,16,
		16,54,58,
		//left r5c2
		56,60,20,
		56,20,17,

		//down r4c1
		51,16,50,
		51,17,16,
		//down r4c2
		53,17,52,
		53,18,17,//150
		//up r4c1
		14,47,46,
		46,13,14,
		//up r4c2
		15,49,48,
		48,14,15,
		//right r4c1
		14,17,51,
		51,47,14,
		//right r4c2
		15,18,53,
		53,49,15,
		//left r4c1
		46,50,16,
		46,16,13,
		//left r4c2
		48,52,17,
		48,17,14,

		//down r3c1
		43,14,13,
		43,13,42,
		//down r3c2
		45,15,14,
			45,14,44,
			//up r3c1
			11,39,38,
				11,38,10,
				//up r3c2
				12,41,40,
					12,40,11,
					//right r3c1
					11,14,43,
						11,43,39,
						//right r3c2
						12,15,45,
							12,45,41,
							//left r3c1
							42,13,38,
								38,13,10,
								//left r3c2
								44,14,40,
									40,14,11,

									//down r2c1
									35,11,10,
										35,10,34,
										//down r2c2
										37,12,11,
											37,11,36,
											//up r2c1
											8,31,30,
												8,30,7,
												//up r2c2
												9,33,32,
													9,32,8,
													//right r2c1
													8,11,35,
														8,35,31,
														//right r2c2
														9,12,37,
														9,37,33,
														//left r2c1
														34,10,30,
														30,10,7,
														//left r2c2
														36,11,32,
														32,11,8,

														//down r1c1
														27,8,7,
														27,7,26,
														//down r1c2
														29,9,8,
														29,8,28,
														//up r1c1
														22,4,5,
														5,23,22,
														//up r1c2
														6,25,24,
														6,24,5,
														//right r1c1
														5,8,27,
														5,27,23,
														//right r1c2
														6,9,29,
														6,29,25,
														//left r1c1
														26,7,22,
															22,7,4,
															//left r1c2
															28,8,24,
																24,8,5
	};

	meshData.Indices32.assign(&i[0], &i[342]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateCandy3(uint32 numSubdivisions) {

	MeshData meshData;
	XMVECTOR v0, v1, v2, v3;

	v0 = { -0.5f, 0.5f, -1.f }; //from vertex v[0]
	v1 = { 0.5f, 0.5f, -1.f };
	v2 = { -0.5f, -0.5f, -1.f };

	v3 = ComputeNormal(v0, v1, v2);

	Vertex v[20] =
	{
		//front square
		Vertex(-0.5f, 0.5f, -1.6f,  XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //0  		 
		Vertex(0.5f, 0.5f, -1.6f,  XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //1
		Vertex(-0.5f, -0.5f, -1.6f, XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //2
		Vertex(0.5f, -0.5f, -1.6f, XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //3
		//back square
		Vertex(-0.5f, 0.5f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //4 
		Vertex(0.5f, 0.5f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //5
		Vertex(-0.5f, -0.5f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //6
		Vertex(0.5f, -0.5f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //7		

		//front hex
		Vertex(-1.2f, 0.8f, -1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //8
		Vertex(0.0f, 1.2f, -1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //9
		Vertex(1.2f, 0.8f, -1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //10
		Vertex(-1.2f, -0.8f, -1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //11		 
		Vertex(0.0f, -1.2f, -1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //12
		Vertex(1.2f, -0.8f, -1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //13

		//back hex
		Vertex(-1.2f, 0.8f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //14
		Vertex(0.0f, 1.2f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //15
		Vertex(1.2f, 0.8f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //16
		Vertex(-1.2f, -0.8f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //17		 
		Vertex(0.0f, -1.2f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //18
		Vertex(1.2f, -0.8f, 1.6f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f), //19
	};

	meshData.Vertices.assign(&v[0], &v[20]);

	uint32 i[120] = {
		//right inside
		1,7,5,
		1,3,7,
		//top inside
		1,4,0,
		1,5,4,
		//left inside
		4,2,0,
		4,6,2,
		//bott inside
		7,3,2,
		7,2,6,
		//hexa front top
		9,1,0,
		10,1,9,
		9,0,8,
		//hexa front bot
		3,13,12,
		3,12,2,
		2,12,11,
		//hex front right
		1,13,3,
		10,13,1,
		//hex front left
		8,0,2,
		2,11,8,//18

		//hexa back top
		15,4,5,
		14,4,15,
		15,5,16,
		//hexa back bot
		7,6,18,
		17,18,6,
		19,7,18,

		//hex back right
		4,17,6,
		14,17,4,
		//hex back left
		7,19,5,
		5,19,16,//28

		//hex top right
		10,9,15,
		15,16,10,

		//hex top left
		8,15,9,
		8,14,15,

		//hex right
		10,16,13,
		16,19,13,

		//hex left
		14,8,11,
		14,11,17,
		//hex bottom right
		19,12,13,
		19,18,12,
		//hex bottom left
		12,17,11,
		12,18,17
	};

	meshData.Indices32.assign(&i[0], &i[120]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateHexagon(float width, float height, float depth, uint32 numSubdivisions)
{
	MeshData meshData;

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	Vertex v[14] =
	{
		//front
		Vertex(w2 * -5.f / 50,h2 * 1.5f / 50, d2 * 8.66f / 50, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
		Vertex(w2 * -5.f / 50,h2 * -1.5f / 50, d2 * 8.66f / 50, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//1
		Vertex(w2 * 5.f / 50, h2 * 1.5f / 50, d2 * 8.66f / 50, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//2
		Vertex(w2 * 5.f / 50, h2 * -1.5f / 50, d2 * 8.66f / 50, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
		 //left front
		Vertex(w2 * -10.f / 50, h2 * 1.5f / 50, 0.f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//4
		Vertex(w2 * -10.f / 50, h2 * -1.5f / 50, 0.f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//5
	//right front
		Vertex(w2 * 10.f / 50, h2 * 1.5f / 50, 0.f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
		Vertex(w2 * 10.f / 50, h2 * -1.5f / 50, 0.f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//7
	//back
		Vertex(w2 * -5.f / 50, h2 * 1.5f / 50, d2 * -8.66f / 50, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//8
		Vertex(w2 * -5.f / 50,h2 * -1.5f / 50, d2 * -8.66f / 50, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//9
		Vertex(w2 * 5.f / 50, h2 * 1.5f / 50, d2 * -8.66f / 50, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//10
		Vertex(w2 * 5.f / 50, h2 * -1.5f / 50, d2 * -8.66f / 50, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//11
	//top
		Vertex(0.f,h2 * 1.5f / 50,0.f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//12
	 //bottom
		Vertex(0.f,h2 * -1.5f / 50,0.f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)//13
	};

	meshData.Vertices.assign(&v[0], &v[14]);

	uint32 i[72] = {
		//front
		0, 1, 2,
		2, 1, 3,
		//left front
		4,5,0,
		0,5,1,
		//back left
		8,9,4,
		4,9,5,
		//front right
		2,3,6,
		6,3,7,
		//back right
		6,7,10,
		10,7,11,
		//back
		10,11,8,
		8,11,9,
		//top
		12,8,4,
		0,12,4,
		12,0,2,
		2,6,12,
		12,6,10,
		12,10,8,
		//bottom
		13,5,9,
		13,9,11,
		13,11,7,
		13,7,3,
		13,3,1,
		13,1,5
	};

	meshData.Indices32.assign(&i[0], &i[72]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;

}

GeometryGenerator::MeshData GeometryGenerator::CreateTriangleEq(uint32 numSubdivisions)
{
	MeshData meshData;

	Vertex v[24] =
	{
		//front
		//Vertex(-5.f, 1.5f, 8.66f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0//
	Vertex(-0.6f,-0.6f, 1.f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
	Vertex(0.6f,-0.6f, 1.f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//1
	Vertex(0.0f,0.6f, 1.f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//2

	//right top
	Vertex(0.0f,0.6f, 1.f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
	Vertex(0.6f,-0.6f, 1.f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//4
	Vertex(0.0f,0.6f, -1.f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//5

	//right botton
	Vertex(0.6f,-0.6f, 1.f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
	Vertex(0.0f,0.6f, -1.f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//7
	Vertex(0.6f,-0.6f, -1.f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//8

	//back
	Vertex(-0.6f,-0.6f, -1.f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//9
	Vertex(0.6f,-0.6f, -1.f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//10
	Vertex(0.0f, 0.6f, -1.f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//11

	//left top
	Vertex(0.0f, 0.6f, -1.f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//12
	Vertex(-0.6f,-0.6f, -1.f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//13
	Vertex(0.0f, 0.6f, 1.f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//14

	//left bottom
	Vertex(0.f,0.6f, 1.f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//15
	Vertex(-0.6f,-0.6f, 1.f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//16
	Vertex(-0.6f,-0.6f, -1.f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//17

	//bottom right
	Vertex(-0.6f,-0.6f, 1.f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//18
	Vertex(0.6f,-0.6f, 1.f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//19
	Vertex(0.6f,-0.6f, -1.f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//20

	//bottom left
	Vertex(-0.6f,-0.6f, -1.f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//21
	Vertex(0.6f,-0.6f, -1.f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//22
	Vertex(-0.6f,-0.6f, 1.f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)//23
	};

	meshData.Vertices.assign(&v[0], &v[24]);

	uint32 i[24] = {
		//front
		2,0,1,
		//rt
		5,3,4,
		//rb
		7,6,8,
		//back
		11,10,9,
		//lt
		14,12,13,
		//lb
		15,17,16,
		//bol
		20,19,18,
		//bor
		22,23,21
	};

	meshData.Indices32.assign(&i[0], &i[24]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;

}

GeometryGenerator::MeshData GeometryGenerator::CreateTriangleRectSqr(uint32 numSubdivisions)
{
	MeshData meshData;

	Vertex v[24] =
	{
		//Vertex(-5.f, 1.5f, 8.66f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0//
		//front
		Vertex(-0.6f,-0.6f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//0
		Vertex(0.6f,-0.6f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//1
		Vertex(0.6f,0.6f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//2
		//right top
		Vertex(0.6f,0.6f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//3
		Vertex(0.6f,-0.6f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//4
		Vertex(0.6f,0.6f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//5
		//right botton
		Vertex(0.6f,-0.6f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//6
		Vertex(0.6f, 0.6f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//7
		Vertex(0.6f,-0.6f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//8
		//back
		Vertex(-0.6f,-0.6f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//9
		Vertex(0.6f,-0.6f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//10
		Vertex(0.6f,0.6f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//11

		//left top
		Vertex(0.6f,0.6f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//12
		Vertex(-0.6f,-0.6f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//13
		Vertex(0.6f, 0.6f, 0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//14

		//left bottom
		Vertex(0.6f, 0.6f, 0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//15
		Vertex(-0.6f,-0.6f, 0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//16
		Vertex(-0.6f,-0.6f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//17

		//bottom right
		Vertex(-0.6f,-0.6f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//18
		Vertex(0.6f,-0.6f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//19
		Vertex(0.6f,-0.6f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//20

		//bottom left
		Vertex(-0.6f,-0.6f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//21
		Vertex(0.6f,-0.6f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),//22
		Vertex(-0.6f,-0.6f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)//23
	};

	meshData.Vertices.assign(&v[0], &v[24]);

	uint32 i[24] = {
		//front
		2,0,1,
		//rt
		5,3,4,
		//rb
		7,6,8,
		//back
		11,10,9,
		//lt
		14,12,13,
		//lb
		15,17,16,
		//bol
		20,19,18,
		//bor
		22,23,21
	};

	meshData.Indices32.assign(&i[0], &i[24]);

	// Put a cap on the number of subdivisions.
	numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	return meshData;

}


