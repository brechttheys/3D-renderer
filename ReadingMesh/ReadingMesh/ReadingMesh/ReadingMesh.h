#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <vector> 
#include <map>
using namespace std;

class ReadingMesh
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

	public:
		bool operator==(const Point3I p) {
			return x_value == p.x_value && 
				y_value == p.y_value && 
				z_value == p.z_value;
		}
	};

	struct Triangle3D //a triangle is defined by 3 POINT3D points 
	{
		Point3D p1;
		Point3D p2;
		Point3D p3;
	};

	struct BBox
	{
		double xMin, xMax;
		double yMin, yMax;
		double zMin, zMax;

	public:
		double Width() {
			return xMax - xMin;
		}
		double Height() {
			return yMax - yMin;
		}
		double Depth() {
			return zMax - zMin;
		}
		bool contains(Point3D t) {
			return (t.x_value < xMax && t.x_value > xMin) &&
				(t.y_value < yMax && t.y_value > yMin) &&
				(t.z_value < zMax && t.z_value > zMin);
		}
	};

	ReadingMesh(void); //constructor

	void allocatePlyFile(string in);
	void readHeaderInformation();
	void readVertices(int showLines);
	void readFaces();
	void showTriangles(int showLines);

	void createTopBoundingBox();
	void splitTopBoundingBox(int iterations, vector<BBox>& boxesResult, vector<vector<Point3I>>& facesResult);
	void divideInTwoBoxes(BBox& top, BBox& left, BBox& right);
	void initBoundaries(char direction, BBox& top, BBox& left, BBox& right);
	void decideLeftRight(vector<Point3I>& triangles, BBox& left, BBox& right, vector<Point3I>& leftT, vector<Point3I>& rightT);
	void outputToFile(string path, vector<BBox>& boxesResult, vector<vector<Point3I>>& facesResult);
	void copyPLYFile(ofstream& d);

private:

	std::ifstream fpin;							//the input PLY file
	std::vector<Point3D> verticesList;			//a list of vertices (double values), corners of triangles
	int number_of_vertices;						//the number of vertices in the list
	std::vector<Point3I> facesList;				//a list of faces (integer values, references to verticeslist), triangles
	int number_of_faces;						//the number of faces in the list

	BBox topBoundingBox;						//The top bounding box
	int amtOfBoxes;                             //Total amount of bounding boxes

	Triangle3D fillUpTriangle(Point3I face);	//fills up a triangle (points p1, p2, P3) starting from the face 
												//and then towards the vertices		
	BBox fillUpBBox(Point3D vertex);			//Fills up a bounding box with default values
};
