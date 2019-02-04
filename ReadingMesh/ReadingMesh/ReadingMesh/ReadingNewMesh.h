#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <vector> 
using namespace std;

class ReadingNewMesh
{
public:
	struct Point3D //3 dimensional point in space containing (x,y,z) doubles
	{
		double x_value;
		double y_value;
		double z_value;
	};

	struct Point3I //3 dimensional point containing integers (x,y,z) integers
	{
		int x_value;
		int y_value;
		int z_value;
	};

	struct Triangle3D //a triangle is defined by 3 POINT3D points 
	{
		Point3D p1;
		Point3D p2;
		Point3D p3;
	};

	ReadingNewMesh(void); //constructor
	void allocatePlyFile(string in);
	void readHeaderInformation();
	void readVertices();
	void readFaces(int showLines);
	void showTriangles(int showLines);

private:

	std::ifstream fpin;							//the input PLY file
	std::vector<Point3D> newverticesList;			//a list of vertices (double values)
	int number_of_vertices;						//the number of vertices in the list
	std::vector<Point3I> newfacesList;				//a list of faces (integer values, references to verticeslist)
	int number_of_faces;						//the number of faces in the list

	Triangle3D fillUpTriangle(Point3I face);	//fills up a triangle (points p1, p2, P3) starting from the face 
												//and then towards the vertices		
};