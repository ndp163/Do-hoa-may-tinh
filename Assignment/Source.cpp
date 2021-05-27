#include <math.h>
#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include "glut.h"
#include <cmath>

#define PI 3.14159265358979323846
#define RAD 3.141592653589/180
#define NUM 50

using namespace std;

// supportClass
#pragma region
class Point3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
	void set(Point3& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
	}
	Point3() { x = y = z = 0; }
	Point3(float dx, float dy, float dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
};
class Vector3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
	void set(Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	void flip()
	{
		x = -x;
		y = -y;
		z = -z;
	}
	void normalize();
	Vector3() { x = y = z = 0; }
	Vector3(float dx, float dy, float dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
	Vector3(Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
};

void Vector3::normalize()
{
	float temp = sqrt(x * x + y * y + z * z);
	x = x / temp;
	y = y / temp;
	z = z / temp;
}
#pragma endregion

// Mesh
#pragma region
class VertexID
{
public:
	int vertIndex;
};

class Face
{
public:
	int nVerts;
	VertexID* vert;
	Vector3 facenorm;

	Face() {
		nVerts = 0;
		vert = NULL;
	}
	~Face() {
		if (vert != NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh
{
public:
	int numVerts;
	Point3* pt;

	int numFaces;
	Face* face;

	float rotateX, rotateY, rotateZ;

public:
	Mesh() {
		numVerts = 0;
		pt = NULL;
		numFaces = 0;
		face = NULL;
		//
		rotateX = 0;
		rotateY = 0;
		rotateZ = 0;
	}
	~Mesh() {
		if (pt != NULL) {
			delete[] pt;
		}
		if (face != NULL) {
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}

	void DrawWireframe();
	void CreateCuboid(float fSizeX, float fSizeY, float fSizeZ);
	void OVanLoHong(float chieuRong, float chieuDai, float chieuSau, float banKinhChot);
	void hinhBanNguyet(int N, float rong, float cao, float banKinhLon, float banKinhNho);
	void createThanhChuU(float rong, float cao, float sau, float khoangCach);
	void CreateCylinder(int N, float base1Height, float base1Radius);
	void CalculateFacesNorm();
	void Draw();
	void setupMaterial(float ambient[], float diffuse[], float specular[], float shiness);
};
void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;

			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}


// Tìm vector phap tuyen theo phuong phap Newell
void Mesh::CalculateFacesNorm()
{
	for (int f = 0; f < numFaces; f++)
	{
		float mx = 0, my = 0, mz = 0;
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;
			int next = face[f].vert[(v + 1) % face[f].nVerts].vertIndex;
			mx += (pt[iv].y - pt[next].y) * (pt[iv].z + pt[next].z);
			my += (pt[iv].z - pt[next].z) * (pt[iv].x + pt[next].x);
			mz += (pt[iv].x - pt[next].x) * (pt[iv].y + pt[next].y);
		}
		face[f].facenorm.set(mx, my, mz);
		face[f].facenorm.normalize();
	}
}

// To mau cho doi tuong
void Mesh::Draw()
{
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;
			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

// Thiet lap vat lieu cho doi tuong
void Mesh::setupMaterial(float ambient[], float diffuse[], float specular[], float shiness)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiness);
}


// Hinh hop chu nhat
void Mesh::CreateCuboid(float fSizeX, float fSizeY, float fSizeZ)
{
	int i;
	numVerts = 8;
	pt = new Point3[numVerts];

	pt[0].set(-fSizeX / 2, -fSizeY / 2, -fSizeZ / 2);
	pt[1].set(fSizeX / 2, -fSizeY / 2, -fSizeZ / 2);
	pt[2].set(fSizeX / 2, fSizeY / 2, -fSizeZ / 2);
	pt[3].set(-fSizeX / 2, fSizeY / 2, -fSizeZ / 2);
	pt[4].set(-fSizeX / 2, -fSizeY / 2, fSizeZ / 2);
	pt[5].set(fSizeX / 2, -fSizeY / 2, fSizeZ / 2);
	pt[6].set(fSizeX / 2, fSizeY / 2, fSizeZ / 2);
	pt[7].set(-fSizeX / 2, fSizeY / 2, fSizeZ / 2);

	numFaces = 6;
	face = new Face[numFaces];

	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 0;
	face[0].vert[1].vertIndex = 4;
	face[0].vert[2].vertIndex = 7;
	face[0].vert[3].vertIndex = 3;

	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 3;
	face[1].vert[1].vertIndex = 7;
	face[1].vert[2].vertIndex = 6;
	face[1].vert[3].vertIndex = 2;

	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 1;
	face[2].vert[1].vertIndex = 2;
	face[2].vert[2].vertIndex = 6;
	face[2].vert[3].vertIndex = 5;

	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 0;
	face[3].vert[1].vertIndex = 1;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;

	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 6;
	face[4].vert[1].vertIndex = 7;
	face[4].vert[2].vertIndex = 4;
	face[4].vert[3].vertIndex = 5;

	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 2;
	face[5].vert[1].vertIndex = 1;
	face[5].vert[2].vertIndex = 0;
	face[5].vert[3].vertIndex = 3;
}

// Ve hinh tru
void Mesh::CreateCylinder(int N, float base1Height, float base1Radius)
{
	/*
	* N: S? ??nh trên hình tròn
	* base1Height: Chi?u cao c?a hình tr?
	* base1Radius: bán kính c?a hình tròn
	*/
	int i;
	numVerts = 2 * N + 2;
	GLfloat angle = 2 * PI / N;
	pt = new Point3[numVerts];
	// Vong tron tren: [0, N - 1]
	for (i = 0; i < N; i++)
	{
		pt[i].set(base1Radius * sin((double)i * angle), base1Height / 2, -base1Radius * cos((double)i * angle));
	}
	// Vong tron duoi: [N, 2 * N - 1]
	for (i = 0; i < N; i++)
	{
		pt[i + N].set(base1Radius * sin((double)i * angle), -base1Height / 2, -base1Radius * cos((double)i * angle));
	}

	pt[2 * N].set(0, base1Height / 2, 0);
	pt[2 * N + 1].set(0, -base1Height / 2, 0);

	numFaces = 3 * N;
	face = new Face[numFaces];

	for (i = 0; i < numFaces; i++)
	{
		if (i < N - 1)
		{
			face[i].nVerts = 3;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i;
			face[i].vert[1].vertIndex = i + 1;
			face[i].vert[2].vertIndex = 2 * N;
		}
		else if (i == N - 1)
		{
			face[i].nVerts = 3;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i;
			face[i].vert[1].vertIndex = 0;
			face[i].vert[2].vertIndex = 2 * N;
		}
		else if (i < 2 * N - 1)
		{
			face[i].nVerts = 3;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i;
			face[i].vert[1].vertIndex = 2 * N + 1;
			face[i].vert[2].vertIndex = i + 1;
		}
		else if (i == 2 * N - 1)
		{
			face[i].nVerts = 3;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i;
			face[i].vert[1].vertIndex = 2 * N + 1;
			face[i].vert[2].vertIndex = N;
		}
		else if (i < 3 * N - 1)
		{
			face[i].nVerts = 4;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i - 2 * N + 1;
			face[i].vert[1].vertIndex = i - 2 * N;
			face[i].vert[2].vertIndex = i - N;
			face[i].vert[3].vertIndex = i - N + 1;
		}
		else
		{
			face[i].nVerts = 4;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = 0;
			face[i].vert[1].vertIndex = N - 1;
			face[i].vert[2].vertIndex = 2 * N - 1;
			face[i].vert[3].vertIndex = N;
		}
	}
}

// Hinh o van co lo hong
void Mesh::OVanLoHong(float chieuRong, float chieuDai, float chieuSau, float chieuRongCon) {
	// N: So dinh tren mot nua hinh tron
	int N = NUM / 2;
	GLfloat angle = 2 * PI / (2 * (N - 1));

	int i;
	numVerts = N * 8;
	pt = new Point3[numVerts];

	for (i = 0; i < N; i++)
	{
		pt[i].set(-chieuDai / 2 - chieuRong / 2 * sin((double)i * angle), chieuSau / 2, -chieuRong / 2 * cos((double)i * angle));
	}

	for (i = 0; i < N; i++)
	{
		pt[i + N].set(-chieuDai / 2.5, chieuSau / 2, -chieuRongCon * cos((double)i * angle));
	}

	for (i = 0; i < N; i++)
	{
		pt[i + N * 2].set(-chieuDai / 2 - chieuRong / 2 * sin((double)i * angle), -chieuSau / 2, -chieuRong / 2 * cos((double)i * angle));
	}

	for (i = 0; i < N; i++)
	{
		pt[i + N * 3].set(-chieuDai / 2.5, -chieuSau / 2, -chieuRongCon * cos((double)i * angle));
	}

	for (i = 0; i < N; i++)
	{
		pt[i + N * 4].set(chieuDai / 2 + chieuRong / 2 * sin((double)i * angle), chieuSau / 2, -chieuRong / 2 * cos(-(double)i * angle));
	}

	for (i = 0; i < N; i++)
	{
		pt[i + N * 5].set(chieuDai / 3.5, chieuSau / 2, -chieuRongCon * cos(-(double)i * angle));
	}

	for (i = 0; i < N; i++)
	{
		pt[i + N * 6].set(chieuDai / 2 + chieuRong / 2 * sin((double)i * angle), -chieuSau / 2, -chieuRong / 2 * cos(-(double)i * angle));
	}

	for (i = 0; i < N; i++)
	{
		pt[i + N * 7].set(chieuDai / 3.5, -chieuSau / 2, -chieuRongCon * cos(-(double)i * angle));
	}

	numFaces = (N - 1) * 8 + 8;
	face = new Face[numFaces];

	int mau = 0;

	//	Ben trai
	for (i = 0; i < N - 1; i++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];

		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + 1;
		face[i].vert[2].vertIndex = i + N + 1;
		face[i].vert[3].vertIndex = i + N;
	}

	for (i = 0; i < N - 1; i++)
	{
		face[i + (N - 1)].nVerts = 4;
		face[i + (N - 1)].vert = new VertexID[face[i + N - 1].nVerts];

		face[i + (N - 1)].vert[0].vertIndex = i + N * 2 + 1;
		face[i + (N - 1)].vert[1].vertIndex = i + N * 2;
		face[i + (N - 1)].vert[2].vertIndex = i + N * 3;
		face[i + (N - 1)].vert[3].vertIndex = i + N * 3 + 1;
	}

	for (i = 0; i < N - 1; i++)
	{
		face[i + (N - 1) * 2].nVerts = 4;
		face[i + (N - 1) * 2].vert = new VertexID[face[i + (N - 1) * 2].nVerts];

		face[i + (N - 1) * 2].vert[0].vertIndex = i + 1;
		face[i + (N - 1) * 2].vert[1].vertIndex = i;
		face[i + (N - 1) * 2].vert[2].vertIndex = i + N * 2;
		face[i + (N - 1) * 2].vert[3].vertIndex = i + N * 2 + 1;
	}

	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 3].nVerts = 4;
		face[i + (N - 1) * 3].vert = new VertexID[face[i + (N - 1) * 3].nVerts];

		face[i + (N - 1) * 3].vert[0].vertIndex = i + N;
		face[i + (N - 1) * 3].vert[1].vertIndex = i + N + 1;
		face[i + (N - 1) * 3].vert[2].vertIndex = i + N * 3 + 1;
		face[i + (N - 1) * 3].vert[3].vertIndex = i + N * 3;
	}

	// Ben phai
	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 4].nVerts = 4;
		face[i + (N - 1) * 4].vert = new VertexID[face[i + (N - 1) * 4].nVerts];

		face[i + (N - 1) * 4].vert[0].vertIndex = i + N * 4 + 1;
		face[i + (N - 1) * 4].vert[1].vertIndex = i + N * 4;
		face[i + (N - 1) * 4].vert[2].vertIndex = i + N * 5;
		face[i + (N - 1) * 4].vert[3].vertIndex = i + N * 5 + 1;
	}

	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 5].nVerts = 4;
		face[i + (N - 1) * 5].vert = new VertexID[face[i + (N - 1) * 5].nVerts];

		face[i + (N - 1) * 5].vert[0].vertIndex = i + N * 6;
		face[i + (N - 1) * 5].vert[1].vertIndex = i + N * 6 + 1;
		face[i + (N - 1) * 5].vert[2].vertIndex = i + N * 7 + 1;
		face[i + (N - 1) * 5].vert[3].vertIndex = i + N * 7;
	}

	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 6].nVerts = 4;
		face[i + (N - 1) * 6].vert = new VertexID[face[i + (N - 1) * 6].nVerts];

		face[i + (N - 1) * 6].vert[0].vertIndex = i + N * 4;
		face[i + (N - 1) * 6].vert[1].vertIndex = i + N * 4 + 1;
		face[i + (N - 1) * 6].vert[2].vertIndex = i + N * 6 + 1;
		face[i + (N - 1) * 6].vert[3].vertIndex = i + N * 6;

	}

	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 7].nVerts = 4;
		face[i + (N - 1) * 7].vert = new VertexID[face[i + (N - 1) * 7].nVerts];

		face[i + (N - 1) * 7].vert[0].vertIndex = i + N * 5 + 1;
		face[i + (N - 1) * 7].vert[1].vertIndex = i + N * 5;
		face[i + (N - 1) * 7].vert[2].vertIndex = i + N * 7;
		face[i + (N - 1) * 7].vert[3].vertIndex = i + N * 7 + 1;

	}

	i = (N - 1) * 8;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = N;
	face[i].vert[1].vertIndex = N * 3;
	face[i].vert[2].vertIndex = N * 7;
	face[i].vert[3].vertIndex = N * 5;

	i = (N - 1) * 8 + 1;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = 0;
	face[i].vert[1].vertIndex = N;
	face[i].vert[2].vertIndex = N * 5;
	face[i].vert[3].vertIndex = N * 4;

	i = (N - 1) * 8 + 2;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = 0;
	face[i].vert[1].vertIndex = N * 4;
	face[i].vert[2].vertIndex = N * 6;
	face[i].vert[3].vertIndex = N * 2;

	i = (N - 1) * 8 + 3;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = N * 2;
	face[i].vert[1].vertIndex = N * 6;
	face[i].vert[2].vertIndex = N * 7;
	face[i].vert[3].vertIndex = N * 3;

	i = (N - 1) * 8 + 4;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = (N - 1) + N * 0;
	face[i].vert[1].vertIndex = (N - 1) + N * 2;
	face[i].vert[2].vertIndex = (N - 1) + N * 6;
	face[i].vert[3].vertIndex = (N - 1) + N * 4;

	i = (N - 1) * 8 + 5;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = (N - 1) + N * 1;
	face[i].vert[1].vertIndex = (N - 1) + N * 0;
	face[i].vert[2].vertIndex = (N - 1) + N * 4;
	face[i].vert[3].vertIndex = (N - 1) + N * 5;

	i = (N - 1) * 8 + 6;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = (N - 1) + N * 1;
	face[i].vert[1].vertIndex = (N - 1) + N * 5;
	face[i].vert[2].vertIndex = (N - 1) + N * 7;
	face[i].vert[3].vertIndex = (N - 1) + N * 3;

	i = (N - 1) * 8 + 7;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = (N - 1) + N * 2;
	face[i].vert[1].vertIndex = (N - 1) + N * 3;
	face[i].vert[2].vertIndex = (N - 1) + N * 7;
	face[i].vert[3].vertIndex = (N - 1) + N * 6;

}

// Hinh ban nguyet co lo rong
void Mesh::hinhBanNguyet(int N, float rong, float cao, float banKinhLon, float banKinhNho)
{
	// N: so diem tren mot nua hinh tron
	int M = (N - 1) * 2; // M: So diem tren mot hinh tron
	GLfloat angle = 2 * PI / M;

	int i = 0;
	numVerts = M * 2 + N * 2 + N * 2 + 4;
	pt = new Point3[numVerts];

	// Vong tron trong ben trai: [0, M - 1]
	for (int j = 0; j < M; j++)
	{
		pt[i].set(banKinhNho * cos((double)j * angle), cao + banKinhNho * sin((double)j * angle), rong / 2);
		i++;
	}
	// Vong tron trong ben phai: [M, 2M - 1]
	for (int j = 0; j < M; j++)
	{
		pt[i].set(banKinhNho * cos((double)j * angle), cao + banKinhNho * sin((double)j * angle), -rong / 2);
		i++;
	}
	// Nua vong tron ngoai ben trai: [2M, 2M + N - 1]
	for (int j = 0; j < N; j++)
	{
		pt[i].set(banKinhLon * cos((double)j * angle), cao + banKinhLon * sin((double)j * angle), rong / 2);
		i++;
	}
	// Nua vong tron ngoai ben phai: [2M + N, 2M + 2N - 1]
	for (int j = 0; j < N; j++)
	{
		pt[i].set(banKinhLon * cos((double)j * angle), cao + banKinhLon * sin((double)j * angle), -rong / 2);
		i++;
	}
	// Canh trai: [2M + 2N, 2M + 3N - 1]
	for (int j = 0; j < N; j++)
	{
		pt[i].set(banKinhNho * cos((double)j * angle), 0, rong / 2);
		i++;
	}
	// Canh phai: [2M + 3N, 2M + 4N - 1]
	for (int j = 0; j < N; j++)
	{
		pt[i].set(banKinhNho * cos((double)j * angle), 0, -rong / 2);
		i++;
	}
	// 4 goc: [2M + 4N, 2M + 4N + 3]
	pt[i].set(banKinhLon, 0, rong / 2);
	i++;
	pt[i].set(banKinhLon, 0, -rong / 2);
	i++;
	pt[i].set(-banKinhLon, 0, -rong / 2);
	i++;
	pt[i].set(-banKinhLon, 0, rong / 2);
	i++;

	/******************************************************************************************************************/
	numFaces = M + N - 1 + N - 1 + N - 1 + N - 1 + N - 1 + 1 + 1 + 1 + 1 + 1 + 1;
	face = new Face[numFaces];

	i = 0;

	// Mat trong hinh tron
	for (int j = 0; j < (M - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];

		face[i].vert[0].vertIndex = j;
		face[i].vert[1].vertIndex = j + 1;
		face[i].vert[2].vertIndex = j + M + 1;
		face[i].vert[3].vertIndex = j + M;

		i++;
	}
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = (M - 1);
	face[i].vert[1].vertIndex = 0;
	face[i].vert[2].vertIndex = M;
	face[i].vert[3].vertIndex = 2 * M - 1;
	i++;
	// Mat ben trai, canh phia tren hinh tron
	for (int j = 0; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];

		face[i].vert[0].vertIndex = j + 2 * M;
		face[i].vert[1].vertIndex = j + 2 * M + 1;
		face[i].vert[2].vertIndex = j + 1;
		face[i].vert[3].vertIndex = j;

		i++;
	}
	// Mat ngoai nua tren hinh tron
	for (int j = 0; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];

		face[i].vert[0].vertIndex = j + 2 * M + N;
		face[i].vert[1].vertIndex = j + 2 * M + N + 1;
		face[i].vert[2].vertIndex = j + 2 * M + 1;
		face[i].vert[3].vertIndex = j + 2 * M;
		i++;
	}
	// Mat ben phai, nua tren hinh tron
	for (int j = 0; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];

		face[i].vert[0].vertIndex = j + 2 * M + N + 1;
		face[i].vert[1].vertIndex = j + 2 * M + N;
		face[i].vert[2].vertIndex = j + M;
		face[i].vert[3].vertIndex = j + M + 1;
		i++;
	}
	// Mat ben trai, nua duoi hinh tron
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = 0;
	face[i].vert[1].vertIndex = (M - 1);
	face[i].vert[2].vertIndex = 2 * M + 2 * N + 1;
	face[i].vert[3].vertIndex = 2 * M + 2 * N;
	i++;
	for (int j = 1; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];

		face[i].vert[0].vertIndex = M - j;
		face[i].vert[1].vertIndex = M - j - 1;
		face[i].vert[2].vertIndex = j + 2 * M + 2 * N + 1;
		face[i].vert[3].vertIndex = j + 2 * M + 2 * N;
		i++;
	}
	// Mat ben phai, nua duoi hinh tron
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = M;
	face[i].vert[1].vertIndex = 2 * M - 1;
	face[i].vert[2].vertIndex = 2 * M + 3 * N + 1;
	face[i].vert[3].vertIndex = 2 * M + 3 * N;
	i++;
	for (int j = 1; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];

		face[i].vert[0].vertIndex = 2 * M - j;
		face[i].vert[1].vertIndex = 2 * M - j - 1;
		face[i].vert[2].vertIndex = j + 2 * M + 3 * N + 1;
		face[i].vert[3].vertIndex = j + 2 * M + 3 * N;
		i++;
	}
	// Mat cua goc: x > 0, z > 0;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = 2 * M;
	face[i].vert[1].vertIndex = 0;
	face[i].vert[2].vertIndex = 2 * M + 2 * N;
	face[i].vert[3].vertIndex = 2 * M + 4 * N;
	i++;
	// Mat cua goc: x > 0, z < 0;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = M;
	face[i].vert[1].vertIndex = 2 * M + N;
	face[i].vert[2].vertIndex = 2 * M + 4 * N + 1;
	face[i].vert[3].vertIndex = 2 * M + 3 * N;
	i++;
	// Mat cua goc: x < 0, z < 0;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = 2 * M + 2 * N - 1;
	face[i].vert[1].vertIndex = M + (N - 1);
	face[i].vert[2].vertIndex = 2 * M + 4 * N - 1;
	face[i].vert[3].vertIndex = 2 * M + 4 * N + 2;
	i++;
	// Mat cua goc: x < 0, z > 0;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = 0 + (N - 1);
	face[i].vert[1].vertIndex = 2 * M + N - 1;
	face[i].vert[2].vertIndex = 2 * M + 4 * N + 3;
	face[i].vert[3].vertIndex = 2 * M + 3 * N - 1;
	i++;
	// Mat x > 0
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = 2 * M + N;
	face[i].vert[1].vertIndex = 2 * M;
	face[i].vert[2].vertIndex = 2 * M + 4 * N;
	face[i].vert[3].vertIndex = 2 * M + 4 * N + 1;
	i++;
	// Mat x < 0
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];

	face[i].vert[0].vertIndex = 2 * M + N - 1;
	face[i].vert[1].vertIndex = 2 * M + 2 * N - 1;
	face[i].vert[2].vertIndex = 2 * M + 4 * N + 2;
	face[i].vert[3].vertIndex = 2 * M + 4 * N + 3;
	i++;
}

// Thanh chu U
void Mesh::createThanhChuU(float rong, float cao, float sau, float khoangCach) {
	int i;
	numVerts = 24;
	pt = new Point3[numVerts];

	numFaces = 18;
	face = new Face[numFaces];

	// left bar
	pt[0].set(-khoangCach / 2 - rong, -cao / 2, -sau / 2);
	pt[1].set(-khoangCach / 2, -cao / 2, -sau / 2);
	pt[2].set(-khoangCach / 2, cao / 2, -sau / 2);
	pt[3].set(-khoangCach / 2 - rong, cao / 2, -sau / 2);
	pt[4].set(-khoangCach / 2 - rong, -cao / 2, sau / 2);
	pt[5].set(-khoangCach / 2, -cao / 2, sau / 2);
	pt[6].set(-khoangCach / 2, cao / 2, sau / 2);
	pt[7].set(-khoangCach / 2 - rong, cao / 2, sau / 2);

	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 0;
	face[0].vert[1].vertIndex = 4;
	face[0].vert[2].vertIndex = 7;
	face[0].vert[3].vertIndex = 3;

	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 3;
	face[1].vert[1].vertIndex = 7;
	face[1].vert[2].vertIndex = 6;
	face[1].vert[3].vertIndex = 2;

	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 1;
	face[2].vert[1].vertIndex = 2;
	face[2].vert[2].vertIndex = 6;
	face[2].vert[3].vertIndex = 5;

	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 0;
	face[3].vert[1].vertIndex = 1;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;

	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 6;
	face[4].vert[1].vertIndex = 7;
	face[4].vert[2].vertIndex = 4;
	face[4].vert[3].vertIndex = 5;

	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 2;
	face[5].vert[1].vertIndex = 1;
	face[5].vert[2].vertIndex = 0;
	face[5].vert[3].vertIndex = 3;

	// right bar

	pt[8].set(khoangCach / 2 + rong, -cao / 2, -sau / 2);
	pt[9].set(khoangCach / 2, -cao / 2, -sau / 2);
	pt[10].set(khoangCach / 2, cao / 2, -sau / 2);
	pt[11].set(khoangCach / 2 + rong, cao / 2, -sau / 2);
	pt[12].set(khoangCach / 2 + rong, -cao / 2, sau / 2);
	pt[13].set(khoangCach / 2, -cao / 2, sau / 2);
	pt[14].set(khoangCach / 2, cao / 2, sau / 2);
	pt[15].set(khoangCach / 2 + rong, cao / 2, sau / 2);

	face[6].nVerts = 4;
	face[6].vert = new VertexID[face[0].nVerts];
	face[6].vert[0].vertIndex = 8;
	face[6].vert[1].vertIndex = 12;
	face[6].vert[2].vertIndex = 15;
	face[6].vert[3].vertIndex = 11;

	face[7].nVerts = 4;
	face[7].vert = new VertexID[face[1].nVerts];
	face[7].vert[0].vertIndex = 11;
	face[7].vert[1].vertIndex = 15;
	face[7].vert[2].vertIndex = 14;
	face[7].vert[3].vertIndex = 10;

	face[8].nVerts = 4;
	face[8].vert = new VertexID[face[2].nVerts];
	face[8].vert[0].vertIndex = 9;
	face[8].vert[1].vertIndex = 10;
	face[8].vert[2].vertIndex = 14;
	face[8].vert[3].vertIndex = 13;

	face[9].nVerts = 4;
	face[9].vert = new VertexID[face[3].nVerts];
	face[9].vert[0].vertIndex = 8;
	face[9].vert[1].vertIndex = 9;
	face[9].vert[2].vertIndex = 13;
	face[9].vert[3].vertIndex = 12;

	face[10].nVerts = 4;
	face[10].vert = new VertexID[face[4].nVerts];
	face[10].vert[0].vertIndex = 14;
	face[10].vert[1].vertIndex = 15;
	face[10].vert[2].vertIndex = 12;
	face[10].vert[3].vertIndex = 13;

	face[11].nVerts = 4;
	face[11].vert = new VertexID[face[5].nVerts];
	face[11].vert[0].vertIndex = 10;
	face[11].vert[1].vertIndex = 9;
	face[11].vert[2].vertIndex = 8;
	face[11].vert[3].vertIndex = 11;

	// top bar

	pt[16].set(-khoangCach / 2, cao / 2, -sau / 2);
	pt[17].set(khoangCach / 2, cao / 2, -sau / 2);
	pt[18].set(khoangCach / 2, cao / 2 - rong, -sau / 2);
	pt[19].set(-khoangCach / 2, cao / 2 - rong, -sau / 2);
	pt[20].set(-khoangCach / 2, cao / 2, sau / 2);
	pt[21].set(khoangCach / 2, cao / 2, sau / 2);
	pt[22].set(khoangCach / 2, cao / 2 - rong, sau / 2);
	pt[23].set(-khoangCach / 2, cao / 2 - rong, sau / 2);

	face[12].nVerts = 4;
	face[12].vert = new VertexID[face[0].nVerts];
	face[12].vert[0].vertIndex = 16;
	face[12].vert[1].vertIndex = 20;
	face[12].vert[2].vertIndex = 23;
	face[12].vert[3].vertIndex = 19;

	face[13].nVerts = 4;
	face[13].vert = new VertexID[face[1].nVerts];
	face[13].vert[0].vertIndex = 19;
	face[13].vert[1].vertIndex = 23;
	face[13].vert[2].vertIndex = 22;
	face[13].vert[3].vertIndex = 18;

	face[14].nVerts = 4;
	face[14].vert = new VertexID[face[2].nVerts];
	face[14].vert[0].vertIndex = 17;
	face[14].vert[1].vertIndex = 18;
	face[14].vert[2].vertIndex = 22;
	face[14].vert[3].vertIndex = 21;

	face[15].nVerts = 4;
	face[15].vert = new VertexID[face[3].nVerts];
	face[15].vert[0].vertIndex = 16;
	face[15].vert[1].vertIndex = 17;
	face[15].vert[2].vertIndex = 21;
	face[15].vert[3].vertIndex = 20;

	face[16].nVerts = 4;
	face[16].vert = new VertexID[face[4].nVerts];
	face[16].vert[0].vertIndex = 22;
	face[16].vert[1].vertIndex = 23;
	face[16].vert[2].vertIndex = 20;
	face[16].vert[3].vertIndex = 21;

	face[17].nVerts = 4;
	face[17].vert = new VertexID[face[5].nVerts];
	face[17].vert[0].vertIndex = 18;
	face[17].vert[1].vertIndex = 17;
	face[17].vert[2].vertIndex = 16;
	face[17].vert[3].vertIndex = 19;
}

#pragma endregion
// Tham so
#pragma region

bool viewSecond = false;

// Tham so cua cua so
int screenWidth = 800;
int screenHeight = 800;

// Ban quay
float banQuayRadius = .7;
float banQuayHeight = 0.2;
float bqRotateStep = 5;

float thanhTruotRadius = 0.15;
float thanhTruotHeight = 7;

// Box1
float box1X = 5;
float box1Y = 0.3;
float box1Z = 2;

// Box2
float box2X = .9;
float box2Y = 4;
float box2Z = 0.25;

// Box3
float box3X = 5;
float box3Y = 1.5;
float box3Z = 0.25;

// De goi do
float deGoiDoX = 0.3;
float deGoiDoY = 0.2;
float deGoiDoZ = 0.8;

// Goi do
float goiDoRong = deGoiDoX;
float goiDoCao = 0.35;
float goiDoCaoBanKinhLon = 0.8 / 4;
float goiDoCaoBanKinhNho = 0.15;


// Thanh lien ket
float thanhLienKetX = .4;
float thanhLienKetY = 3.3;
float thanhLienKetZ = 0.15;

// Tay quay
float tayQuayX = .2;
float tayQuayY = .6;
float tayQuayZ = 0.15;

// Chot 3
float chot3Height = banQuayHeight + tayQuayZ + thanhLienKetZ + 0.05;
float chot3Radius = 0.15;

// Thanh chu U
float thanhChuURong = 0.1;
float thanhChuUCao = 1.6;
float thanhChuUSau = 0.1;
float thanhChuUKhoangCach = 0.3;

float posXMove = 0;

// Chot 1
float chot1HopX = 0.3;
float chot1HopY = 0.3;
float chot1HopZ = 0.16;
float chot1TruRadius = 0.08;
float chot1TruHeight = 0.2;

// Chot 2
float chot2HopX = 0.2;
float chot2HopY = 0.2;
float chot2HopZ = 0.16;
float chot2TruRadius = 0.08;
float chot2TruHeight = 0.2;
float chot2Move = tayQuayY / 2;
float chot2MoveStep = tayQuayY / 20;


// Mesh
Mesh thanhTruot;
Mesh banQuay;
Mesh box1;
Mesh box2;
Mesh box3;
Mesh deGoiDo1;
Mesh deGoiDo2;
Mesh goiDo1;
Mesh goiDo2;
Mesh thanhLienKet;
Mesh tayQuay;
Mesh chot3;
Mesh thanhChuU;
Mesh chot1Hop;
Mesh chot1Tru;

Mesh chot2Hop;
Mesh chot2Tru;

bool isFirstRun = true;
int arrRandom[999999];

double angle = 0;

bool wireframe = false;
bool turnLight2 = true;

// Tham so cho camera
float camera_angle;
float camera_height;
float camera_dis;
float camera_X, camera_Y, camera_Z;
float lookAt_X, lookAt_Y, lookAt_Z;

#pragma endregion

void mySpecialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		camera_height += 0.5;
		break;
	case GLUT_KEY_DOWN:
		camera_height -= 0.5;
		break;
	case GLUT_KEY_RIGHT:
		camera_angle += 5;
		break;
	case GLUT_KEY_LEFT:
		camera_angle -= 5;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void myKeyboard(unsigned char key, int x, int y)
{
	float fRInc;
	float fAngle;
	switch (key)
	{
	case '1':
		banQuay.rotateZ += bqRotateStep;
		if (banQuay.rotateZ > 360)
			banQuay.rotateZ -= 360;
		break;
	case '2':
		banQuay.rotateZ -= bqRotateStep;
		if (banQuay.rotateZ < 0)
			banQuay.rotateZ += 360;
		break;
	case '3':
		chot2Move += chot2MoveStep;
		if (chot2Move > tayQuayY)
			chot2Move = tayQuayY;
		break;
	case '4':
		chot2Move -= chot2MoveStep;
		if (chot2Move < tayQuayX / 2)
			chot2Move = tayQuayX / 2;
		break;
	case 'w':
	case 'W':
		wireframe = !wireframe;
		break;
	case 'd':
	case 'D':
		turnLight2 = !turnLight2;
		break;
	case '+':
		camera_dis += 0.5;
		break;
	case '-':
		camera_dis -= 0.5;
		break;
	case 'v':
	case 'V':
		viewSecond = !viewSecond;
		break;
	}
	glutPostRedisplay();
}

// Ve vat the
#pragma region
void drawTile1(float x, float z, float len) {
	float R = len / 2;

	glColor4f(202 / 255.0, 207 / 255.0, 251 / 255.0, 1);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(x + len, 0, z);
	for (int i = 0; i <= 90; i++) {
		float x1 = x + (R + len / 10)*cos(i*RAD);
		float z1 = z + (R + len / 10)*sin(i*RAD);
		glVertex3f(x1, 0, z1);

		float x2 = x + (R + len / 10)*cos((270 - i)*RAD) + len;
		float z2 = z + (R + len / 10)*sin((270 - i)*RAD) + len;
		glVertex3f(x2, 0, z2);
	}
	glVertex3f(x, 0, z + len);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x, 0, z);
	for (int i = 0; i <= 90; i++) {
		float x2 = x + (R - len / 10)*cos(i*RAD);
		float z2 = z + (R - len / 10)*sin(i*RAD);
		glVertex3f(x2, 0, z2);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x + len, 0, z + len);
	for (int i = 180; i <= 270; i++) {
		float x2 = x + (R - len / 10)*cos(i*RAD) + len;
		float z2 = z + (R - len / 10)*sin(i*RAD) + len;
		glVertex3f(x2, 0, z2);
	}
	glEnd();

	glColor4f(128 / 255.0, 128 / 255.0, 128 / 255.0, 1);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 90; i++) {
		float x1 = x + (R + len / 10)*cos(i*RAD);
		float z1 = z + (R + len / 10)*sin(i*RAD);
		glVertex3f(x1, 0, z1);

		float x2 = x + (R - len / 10)*cos(i*RAD);
		float z2 = z + (R - len / 10)*sin(i*RAD);
		glVertex3f(x2, 0, z2);
	}
	glEnd();

	glColor4f(128 / 255.0, 128 / 255.0, 128 / 255.0, 1);
	glBegin(GL_QUAD_STRIP);
	for (int i = 180; i <= 270; i++) {
		float x1 = x + (R + len / 10)*cos(i*RAD) + len;
		float z1 = z + (R + len / 10)*sin(i*RAD) + len;
		glVertex3f(x1, 0, z1);

		float x2 = x + (R - len / 10)*cos(i*RAD) + len;
		float z2 = z + (R - len / 10)*sin(i*RAD) + len;
		glVertex3f(x2, 0, z2);
	}
	glEnd();
}
void drawTile2(float x, float z, float len) {
	float R = len / 2;
	glColor4f(202 / 255.0, 207 / 255.0, 251 / 255.0, 1);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(x, 0, z);
	for (int i = 0; i <= 90; i++) {
		float x1 = x + (R + len / 10)*cos((i + 270)*RAD);
		float z1 = z + (R + len / 10)*sin((i + 270)*RAD) + len;
		glVertex3f(x1, 0, z1);

		float x2 = x + (R + len / 10)*cos((180 - i)*RAD) + len;
		float z2 = z + (R + len / 10)*sin((180 - i)*RAD);
		glVertex3f(x2, 0, z2);
	}
	glVertex3f(x + len, 0, z + len);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x + len, 0, z);
	for (int i = 0; i <= 90; i++) {
		float x2 = x + (R - len / 10)*cos((i + 90)*RAD) + len;
		float z2 = z + (R - len / 10)*sin((i + 90)*RAD);
		glVertex3f(x2, 0, z2);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x, 0, z + len);
	for (int i = 0; i <= 90; i++) {
		float x2 = x + (R - len / 10)*cos((i + 270)*RAD);
		float z2 = z + (R - len / 10)*sin((i + 270)*RAD) + len;
		glVertex3f(x2, 0, z2);
	}
	glEnd();

	glColor3f(128 / 255.0, 128 / 255.0, 128 / 255.0);
	glBegin(GL_QUAD_STRIP);
	for (int i = 270; i <= 360; i++) {
		float x1 = x + (R + len / 10)*cos(i*RAD);
		float z1 = z + (R + len / 10)*sin(i*RAD) + len;
		glVertex3f(x1, 0, z1);

		float x2 = x + (R - len / 10)*cos(i*RAD);
		float z2 = z + (R - len / 10)*sin(i*RAD) + len;
		glVertex3f(x2, 0, z2);
	}
	glEnd();

	glColor3f(128 / 255.0, 128 / 255.0, 128 / 255.0);
	glBegin(GL_QUAD_STRIP);
	for (int i = 90; i <= 180; i++) {
		float x1 = x + (R + len / 10)*cos(i*RAD) + len;
		float z1 = z + (R + len / 10)*sin(i*RAD);
		glVertex3f(x1, 0, z1);

		float x2 = x + (R - len / 10)*cos(i*RAD) + len;
		float z2 = z + (R - len / 10)*sin(i*RAD);
		glVertex3f(x2, 0, z2);
	}
	glEnd();
}
void drawTile3(float x, float z, float len) {
	float R = len / 2;
	// background
	glColor4f(202 / 255.0, 207 / 255.0, 251 / 255.0, 1);
	glBegin(GL_POLYGON);
	glVertex3f(x, 0, z);
	glVertex3f(x + R - len / 10, 0, z);
	glVertex3f(x + R - len / 10, 0, z + R - len / 10);
	glVertex3f(x, 0, z + R - len / 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x + R + len / 10, 0, z);
	glVertex3f(x + len, 0, z);
	glVertex3f(x + len, 0, z + R - len / 10);
	glVertex3f(x + R + len / 10, 0, z + R - len / 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x, 0, z + R + len / 10);
	glVertex3f(x, 0, z + len);
	glVertex3f(x + R - len / 10, 0, z + len);
	glVertex3f(x + R - len / 10, 0, z + R + len / 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x + R + len / 10, 0, z + R + len / 10);
	glVertex3f(x + len, 0, z + R + len / 10);
	glVertex3f(x + len, 0, z + len);
	glVertex3f(x + R + len / 10, 0, z + len);
	glEnd();


	//decor
	glColor3f(128 / 255.0, 128 / 255.0, 128 / 255.0);
	glBegin(GL_POLYGON);
	glVertex3f(x + R - len / 10, 0, z);
	glVertex3f(x + R + len / 10, 0, z);
	glVertex3f(x + R + len / 10, 0, len + z);
	glVertex3f(x + R - len / 10, 0, len + z);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x, 0, z + R - len / 10);
	glVertex3f(x, 0, z + R + len / 10);
	glVertex3f(x + len, 0, z + R + len / 10);
	glVertex3f(x + len, 0, z + R - len / 10);
	glEnd();
}
void drawTile4(float x, float z, float len) {
	float R = len / 2;
	float space = len * 3 / 20;
	// background
	glColor4f(202 / 255.0, 207 / 255.0, 251 / 255.0, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {
		float x1 = x + R + space*cos(i*RAD);
		float z1 = z + R + space*sin(i*RAD);
		glVertex3f(x1, 0, z1);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x, 0, z);
	glVertex3f(x, 0, z + R - len / 10);
	for (int i = 196; i <= 254; i++) {
		float x1 = x + R + (space + len / 5)*cos(i*RAD);
		float z1 = z + R + (space + len / 5)*sin(i*RAD);
		glVertex3f(x1, 0, z1);
	}
	glVertex3f(x + R - len / 10, 0, z);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x + len, 0, z + R - len / 10);
	glVertex3f(x + len, 0, z);
	glVertex3f(x + R + len / 10, 0, z);
	for (int i = 286; i <= 344; i++) {
		float x1 = x + R + (space + len / 5)*cos(i*RAD);
		float z1 = z + R + (space + len / 5)*sin(i*RAD);
		glVertex3f(x1, 0, z1);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x + R + len / 10, 0, z + len);
	glVertex3f(x + len, 0, z + len);
	glVertex3f(x + len, 0, z + R + len / 10);
	for (int i = 16; i <= 74; i++) {
		float x1 = x + R + (space + len / 5)*cos(i*RAD);
		float z1 = z + R + (space + len / 5)*sin(i*RAD);
		glVertex3f(x1, 0, z1);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x, 0, z + R + len / 10);
	glVertex3f(x, 0, z + len);
	glVertex3f(x + R - len / 10, 0, z + len);
	for (int i = 106; i <= 164; i++) {
		float x1 = x + R + (space + len / 5)*cos(i*RAD);
		float z1 = z + R + (space + len / 5)*sin(i*RAD);
		glVertex3f(x1, 0, z1);
	}
	glEnd();

	// decor
	glColor3f(128 / 255.0, 128 / 255.0, 128 / 255.0);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++) {
		float x1 = x + R + (space + len / 5)*cos(i*RAD);
		float z1 = z + R + (space + len / 5)*sin(i*RAD);
		glVertex3f(x1, 0, z1);
		x1 = x + R + space*cos(i*RAD);
		z1 = z + R + space*sin(i*RAD);
		glVertex3f(x1, 0, z1);
	}
	glEnd();


	glColor3f(128 / 255.0, 128 / 255.0, 128 / 255.0);
	glBegin(GL_POLYGON);
	glVertex3f(x + R - len / 10, 0, z);
	glVertex3f(x + R + len / 10, 0, z);
	glVertex3f(x + R + len / 10, 0, z + len / 5);
	glVertex3f(x + R - len / 10, 0, z + len / 5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x + R - len / 10, 0, z + len);
	glVertex3f(x + R + len / 10, 0, z + len);
	glVertex3f(x + R + len / 10, 0, z + 4 * len / 5);
	glVertex3f(x + R - len / 10, 0, z + 4 * len / 5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x, 0, z + R - len / 10);
	glVertex3f(x, 0, z + R + len / 10);
	glVertex3f(x + len / 5, 0, z + R + len / 10);
	glVertex3f(x + len / 5, 0, z + R - len / 10);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(x + 4 * len / 5, 0, z + R - len / 10);
	glVertex3f(x + 4 * len / 5, 0, z + R + len / 10);
	glVertex3f(x + len, 0, z + R + len / 10);
	glVertex3f(x + len, 0, z + R - len / 10);
	glEnd();
}

void drawNen(float alpha)
{
	int i = 0;
	float len = 4;
	glDisable(GL_LIGHTING);
	if (isFirstRun) {
		glColor3f(1.0f, 1.0f, 1.0f);
		for (float x = -50; x < 50; x += 4) {
			for (float z = -50; z < 50; z += 4) {
				int r = rand() % 4;
				arrRandom[i++] = r;
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				switch (r)
				{
				case 0:
					drawTile1(x, z, len);
					break;
				case 1:
					drawTile2(x, z, len);
					break;
				case 2:
					drawTile3(x, z, len);
					break;
				case 3:
					drawTile4(x, z, len);
				default:
					break;
				}
			}
		}


		isFirstRun = false;
	}
	else {
		for (float x = -50; x < 50; x += 4) {
			for (float z = -50; z < 50; z += 4) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				switch (arrRandom[i++])
				{
				case 0:
					drawTile1(x, z, len);
					break;
				case 1:
					drawTile2(x, z, len);
					break;
				case 2:
					drawTile3(x, z, len);
					break;
				case 3:
					drawTile4(x, z, len);
				default:
					break;
				}
			}
		}
	}
	glEnable(GL_LIGHTING);

}

void drawAxis()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0); //x
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0); //y
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0); //z
	glVertex3f(0, 0, 4);
	glEnd();
}

void drawBox1()
{
	glPushMatrix();

	glTranslated(0, box1Y / 2, 0);
	glRotatef(thanhTruot.rotateY, 0, 1, 0);

	GLfloat diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	box1.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		box1.DrawWireframe();
	else
		box1.Draw();

	glPopMatrix();
}
void drawBox2()
{
	glPushMatrix();

	glTranslated(0, box1Y + box2Y / 2, 0);
	glRotatef(thanhTruot.rotateY, 0, 1, 0);

	GLfloat diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	box2.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		box2.DrawWireframe();
	else
		box2.Draw();

	glPopMatrix();
}
void drawBox3()
{
	glPushMatrix();

	glTranslated(0, box1Y + box2Y + box3Y / 2, 0);
	glRotatef(thanhTruot.rotateY, 0, 1, 0);

	GLfloat diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	box3.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		box3.DrawWireframe();
	else
		box3.Draw();

	glPopMatrix();
}
void drawDeGoiDo1()
{
	glPushMatrix();

	glTranslated(-box3X / 2.5, box1Y + box2Y + box3Y / 2, (box3Z + deGoiDoY) / 2);

	glRotatef(90, 1, 0, 0);

	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	box1.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		deGoiDo1.DrawWireframe();
	else
		deGoiDo1.Draw();

	glPopMatrix();
}
void drawDeGoiDo2()
{
	glPushMatrix();

	glTranslated(box3X / 2.5, box1Y + box2Y + box3Y / 2, (box3Z + deGoiDoY) / 2);
	glRotatef(90, 1, 0, 0);

	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	deGoiDo2.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		deGoiDo2.DrawWireframe();
	else
		deGoiDo2.Draw();

	glPopMatrix();
}
void drawGoiDo1()
{
	glPushMatrix();

	glTranslated(-box3X / 2.5, box1Y + box2Y + box3Y / 2, box3Z / 2 + deGoiDoY);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);

	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	goiDo1.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		goiDo1.DrawWireframe();
	else
		goiDo1.Draw();

	glPopMatrix();
}
void drawGoiDo2()
{
	glPushMatrix();

	glTranslated(box3X / 2.5, box1Y + box2Y + box3Y / 2, box3Z / 2 + deGoiDoY);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);

	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	goiDo2.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		goiDo2.DrawWireframe();
	else
		goiDo2.Draw();

	glPopMatrix();
}
void drawThanhTruot()
{
	glPushMatrix();

	glTranslated(posXMove, box1Y + box2Y + box3Y / 2, box3Z / 2 + deGoiDoY + goiDoCao);
	glRotatef(90, 0, 0, 1);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 75 / 255.0, 75 / 255.0, 75 / 255.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	thanhTruot.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		thanhTruot.DrawWireframe();
	else
		thanhTruot.Draw();

	glPopMatrix();
}
void drawBanQuay() {
	glPushMatrix();

	glTranslated(0, box1Y + box2Y / 2, box2Z / 2 + banQuayHeight / 2);
	glRotatef(90, 1, 0, 0);
	glRotatef(banQuay.rotateZ, 0, 1, 0);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	banQuay.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		banQuay.DrawWireframe();
	else
		banQuay.Draw();

	glPopMatrix();
}
void drawThanhLienKet()
{
	glPushMatrix();
	float chot3Move = box2Y / 2 - thanhLienKetX / 2 - 0.1;
	float temp1 = chot2Move*sin(banQuay.rotateZ*RAD);
	float temp2 = chot2Move*cos(banQuay.rotateZ*RAD);
	thanhLienKet.rotateZ = atan(temp1 / (temp2 + chot3Move)) * 180 / PI;

	posXMove = -thanhLienKetY*sin(thanhLienKet.rotateZ*RAD);

	glTranslated(0, box1Y + box2Y / 2 - thanhLienKetY / 2, box2Z / 2 + banQuayHeight + tayQuayZ + thanhLienKetZ / 2);
	glRotatef(thanhLienKet.rotateZ, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glTranslatef(thanhLienKetY / 2, 0, 0);



	GLfloat diffuse[] = { 51 / 255.0, 253 / 255.0, 51 / 255.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	thanhLienKet.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		thanhLienKet.DrawWireframe();
	else
		thanhLienKet.Draw();

	glPopMatrix();
}
void drawTayQuay() {
	glPushMatrix();

	glTranslated(0, box1Y + box2Y / 2 + banQuayRadius / 2 - tayQuayY / 2 - tayQuayX / 4, box2Z / 2 + banQuayHeight + tayQuayZ / 2);
	glRotatef(banQuay.rotateZ, 0, 0, 1);
	glTranslatef(0, tayQuayY / 2 + tayQuayX / 4, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);

	GLfloat diffuse[] = { 171 / 255.0, 139 / 255.0, 88 / 255.0, 1.0 };
	GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	tayQuay.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		tayQuay.DrawWireframe();
	else
		tayQuay.Draw();

	glPopMatrix();
}
void drawChot3() {
	glPushMatrix();

	glTranslated(0, box1Y + 0.35, box2Z / 2 + chot3Height / 2);
	glRotatef(90, 1, 0, 0);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 75 / 255.0, 75 / 255.0, 75 / 255.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	chot3.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		chot3.DrawWireframe();
	else
		chot3.Draw();

	glPopMatrix();
}
void drawThanhChuU() {
	glPushMatrix();

	glTranslated(posXMove, box1Y + box2Y + box3Y / 2 - thanhTruotRadius - thanhChuUCao / 2, box3Z / 2 + deGoiDoY + goiDoCao);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 75 / 255.0, 75 / 255.0, 75 / 255.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	thanhChuU.setupMaterial(ambient, diffuse, specular, shininess);

	if (wireframe)
		thanhChuU.DrawWireframe();
	else
		thanhChuU.Draw();

	glPopMatrix();
}
void drawChot1Hop() {
	glPushMatrix();

	glTranslated(posXMove, box1Y + box2Y / 2 + thanhLienKetY * cos(thanhLienKet.rotateZ*RAD) - 1.8, box3Z / 2 + banQuayHeight + tayQuayZ + thanhLienKetZ + chot1HopZ / 2);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	chot1Hop.setupMaterial(ambient, diffuse, specular, shininess);
	if (wireframe) {
		chot1Hop.DrawWireframe();
	}
	else {
		chot1Hop.Draw();
	}

	glPopMatrix();
}
void drawChot1Tru() {
	glPushMatrix();

	glTranslated(posXMove, box1Y + box2Y / 2 + thanhLienKetY * cos(thanhLienKet.rotateZ*RAD) - 1.8, box3Z / 2 + banQuayHeight + tayQuayZ + thanhLienKetZ + chot1TruHeight / 2);
	glRotatef(90, 1, 0, 0);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 75 / 255.0, 75 / 255.0, 75 / 255.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	chot1Tru.setupMaterial(ambient, diffuse, specular, shininess);
	if (wireframe)
		chot1Tru.DrawWireframe();
	else
		chot1Tru.Draw();

	glPopMatrix();
}
void drawChot2Hop() {
	glPushMatrix();

	float posX = chot2Move * cos((banQuay.rotateZ + 90) * RAD);
	float posY = box1Y + box2Y / 2 + chot2Move * sin((banQuay.rotateZ + 90) * RAD);
	float posZ = box3Z / 2 + banQuayHeight + tayQuayZ + chot2HopZ / 2;
	glTranslatef(posX, posY, posZ);
	glRotatef(thanhLienKet.rotateZ, 0, 0, 1);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	chot2Hop.setupMaterial(ambient, diffuse, specular, shininess);
	if (wireframe) {
		chot2Hop.DrawWireframe();
	}
	else {
		chot2Hop.Draw();
	}

	glPopMatrix();
}
void drawChot2Tru() {
	glPushMatrix();
	float posX = chot2Move * cos((banQuay.rotateZ + 90) * RAD);
	float posY = box1Y + box2Y / 2 + chot2Move * sin((banQuay.rotateZ + 90) * RAD);
	float posZ = box3Z / 2 + banQuayHeight + tayQuayZ + chot2TruHeight / 2;
	glTranslatef(posX, posY, posZ);
	glRotatef(90, 1, 0, 0);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 75 / 255.0, 75 / 255.0, 75 / 255.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	chot2Tru.setupMaterial(ambient, diffuse, specular, shininess);
	if (wireframe)
		chot2Tru.DrawWireframe();
	else
		chot2Tru.Draw();

	glPopMatrix();
}
#pragma endregion

void drawAll() {
	drawBox1();
	drawBox2();
	drawBox3();
	drawDeGoiDo1();
	drawDeGoiDo2();
	drawGoiDo1();
	drawGoiDo2();
	drawThanhTruot();
	drawBanQuay();
	drawThanhLienKet();
	drawTayQuay();
	drawChot3();
	drawThanhChuU();
	drawChot1Hop();
	drawChot1Tru();
	drawChot2Hop();
	drawChot2Tru();
}
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GLfloat light_position0[] = { 10.0, 10.0, 10.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	if (turnLight2) {
		glEnable(GL_LIGHT1);
		GLfloat diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat specular1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat position1[] = { -10.0, 10.0, -10.0, 0.0 };

		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
		glLightfv(GL_LIGHT1, GL_POSITION, position1);
	}
	else
		glDisable(GL_LIGHT1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera_X = camera_dis * sinf(camera_angle * PI / 180);
	camera_Y = camera_height;
	camera_Z = camera_dis * cosf(camera_angle * PI / 180);

	if (camera_dis == 0)
	{
		gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, sinf(camera_angle * PI / 180), 0, cosf(camera_angle * PI / 180));
	}
	if (viewSecond) {
		gluLookAt(0, 15, 0, 0, 0, 0, 0, 0, 1);
	}
	else
	{
		gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, 0, 1, 0);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearStencil(0);
	glClearDepth(1.0f);
	// Draw
	drawAll();

	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	drawNen(1.0f);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


	glPushMatrix();
	glScalef(1, -1, 1);
	drawAll();
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawNen(0.7f);
	glDisable(GL_BLEND);

	glFlush();
	glutSwapBuffers();
}


void myInit()
{
	camera_angle = -30;
	camera_height = 5.5;
	camera_dis = 8.5;

	lookAt_X = 0;
	lookAt_Y = 1;
	lookAt_Z = 0;

	float fHalfSize = 4;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	const float ar = (float)screenWidth / (float)screenHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 1.5, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat lmodel_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	GLfloat light_ambient0[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
}

void create(int N)
{
	int M = 2 * (N - 1);
	// Box1
	box1.CreateCuboid(box1X, box1Y, box1Z);
	box1.CalculateFacesNorm();

	//Box2
	box2.CreateCuboid(box2X, box2Y, box2Z);
	box2.CalculateFacesNorm();
	//Box3
	box3.CreateCuboid(box3X, box3Y, box3Z);
	box3.CalculateFacesNorm();
	// De goi do 1
	deGoiDo1.CreateCuboid(deGoiDoX, deGoiDoY, deGoiDoZ);
	deGoiDo1.CalculateFacesNorm();

	// De goi do 2
	deGoiDo2.CreateCuboid(deGoiDoX, deGoiDoY, deGoiDoZ);
	deGoiDo2.CalculateFacesNorm();

	// Goi do 1
	goiDo1.hinhBanNguyet(N, goiDoRong, goiDoCao, goiDoCaoBanKinhLon, goiDoCaoBanKinhNho);
	goiDo1.CalculateFacesNorm();

	// Goi do 2
	goiDo2.hinhBanNguyet(N, goiDoRong, goiDoCao, goiDoCaoBanKinhLon, goiDoCaoBanKinhNho);
	goiDo2.CalculateFacesNorm();

	// Thanh truot
	thanhTruot.CreateCylinder(M, thanhTruotHeight, thanhTruotRadius);
	thanhTruot.CalculateFacesNorm();
	// Bàn quay
	banQuay.CreateCylinder(M, banQuayHeight, banQuayRadius);
	banQuay.CalculateFacesNorm();

	//// Thanh liên k?t
	thanhLienKet.OVanLoHong(thanhLienKetX, thanhLienKetY, thanhLienKetZ, thanhLienKetX / 4);
	thanhLienKet.CalculateFacesNorm();

	// Tay quay
	tayQuay.OVanLoHong(tayQuayX, tayQuayY, tayQuayZ, 0);
	tayQuay.CalculateFacesNorm();

	// Chot 3
	chot3.CreateCylinder(M, chot3Height, chot3Radius);
	chot3.CalculateFacesNorm();

	// Thanh ch? U
	thanhChuU.createThanhChuU(thanhChuURong, thanhChuUCao, thanhChuUSau, thanhChuUKhoangCach);
	thanhChuU.CalculateFacesNorm();

	// Chot 1
	chot1Hop.CreateCuboid(chot1HopX, chot1HopY, chot1HopZ);
	chot1Hop.CalculateFacesNorm();

	chot1Tru.CreateCylinder(M, chot1TruHeight, chot1TruRadius);
	chot1Tru.CalculateFacesNorm();

	// Chot 2
	chot2Hop.CreateCuboid(chot2HopX, chot2HopY, chot2HopZ);
	chot2Hop.CalculateFacesNorm();

	chot2Tru.CreateCylinder(M, chot2TruHeight, chot2TruRadius);
	chot2Tru.CalculateFacesNorm();
}

void print()
{
	cout << "1: Xoay ban quay nguoc chieu" << endl
		<< "2: Xoay ban quay cung chieu kim dong ho" << endl
		<< "3, 4: Dieu chinh vi tri cua chot 2" << endl
		<< "W, w: Chuyen doi qua lai giua che do khung day va to mau" << endl
		<< "V, v: Chuyen doi qua lai giua 2 che do nhin khac nhau" << endl
		<< "D, d: Bat/tat nguon sang thu 2" << endl
		<< "+: Tang khoang cach camera" << endl
		<< "-: Giam khoang cach camera" << endl
		<< "up arrow : Tang chieu cao camera" << endl
		<< "down arrow : Giam chieu cao camera" << endl
		<< "<-: Quay camera theo chieu kim dong ho" << endl
		<< "->: Quay camera nguoc chieu kim dong ho" << endl;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment - Nguyen Dinh Phuc (1813570)");

	print();

	int N = NUM / 2 + 1;		// So dinh tren mot nua hinh tron
	create(N);

	myInit();

	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKeyboard);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}