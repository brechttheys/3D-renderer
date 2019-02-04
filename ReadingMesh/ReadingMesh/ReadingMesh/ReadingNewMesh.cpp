#include "stdafx.h"
#include "ReadingNewMesh.h"
#include <chrono>

using namespace std;

ReadingNewMesh::ReadingNewMesh(void) {} //constructor


void ReadingNewMesh::allocatePlyFile(string in) {
	fpin = (std::ifstream)(in);
	if (fpin.is_open()) {
		std::cout << "file: " << in << "is opened for input \n";
	}
	else {
		std::cout << "Unable to open input file: " << in << " \n";
		//exit(EXIT_FAILURE);
	}
	return;
}

void ReadingNewMesh::readHeaderInformation() {
	string line;
	int positionInLine = 0;
	getline(fpin, line);
	std::cout << line << endl;
	while (line.compare("end_header") != 0) //we read up just to the end of the header, vertices start hereafter
	{
		getline(fpin, line);
		std::cout << line << endl;
		positionInLine = line.find("element vertex"); //find the string "element vertex" in the file
		if (positionInLine != -1) { //!= means found (-1 value indicates that the searched element is  not present in string)
			string vertexString = line.substr(positionInLine + 15); //position after the "element vertex" string + blanco
			std::istringstream(vertexString) >> number_of_vertices;	//number of vertices mentioned in the file		
		}
		positionInLine = line.find("element face");
		if (positionInLine != -1) { //means found (-1 value means not present in string)
			string faceString = line.substr(positionInLine + 13); //position after the "element face" string + blanco
			std::istringstream(faceString) >> number_of_faces; //number of faces mentioned in the file			
		}
	}
	std::cout << "===>number of vertices: " << number_of_vertices << " \n";
	std::cout << "===>number of faces:    " << number_of_faces << " \n";
	return;
}

void ReadingNewMesh::readVertices() {
	//vertices are presented as "-0.02025 0.182576 -0.0148439" text lines
	//vertices start immediatly after the end_header line. The file pointer is so automatically at the first vertex element
	//showlines parameter (more for debugging purposes): [0=show NO lines, -1=show all lines, +nn=show nn lines]
	string line;			//the line read from the input file
	std::istringstream iss; //internal string streambuffer to split up the different values within the line
	int verticeCounter = 0; //Counter going up to the total number of vertices
	Point3D newVertice;		//vertices values are real numbers, thats why we use the Point3 Double structure
	std::cout << "Read Vertices\n";
	auto start_reading_vertices = chrono::steady_clock::now();
	while (verticeCounter < number_of_vertices) {
		getline(fpin, line);
		iss.str(line);      //split the line into its different values
		iss >> newVertice.x_value;
		iss >> newVertice.y_value;
		iss >> newVertice.z_value;
		newverticesList.push_back(newVertice); //put it into the vertices list
		verticeCounter++;	//go to the next vertice line
	}
	auto end_reading_vertices = chrono::steady_clock::now();
	std::cout << "Reading vertices took: " << chrono::duration_cast<chrono::seconds>(end_reading_vertices - start_reading_vertices).count() << " seconds\n";
	return;
}

void ReadingNewMesh::readFaces(int showLines) {
	//faces of triangles are presented as "3 95 101 100 "
	//so we just read the faces with a 3 in front of the line
	//showlines: [0=show NO lines, -1=show all lines, +nn=show nn lines]
	string line;
	std::istringstream iss; //internal string streambuffer
	int facesCounter = 0;   //Counter up to the number of faces
	Point3I newFace;        //face values are integers, thats why we use the Point3 Integer structure
	int isItTriangleType = 0;  //a triangle is detected if the first valie of the face record is a 3
	std::cout << "Read Faces\n";
	auto start_reading_faces = chrono::steady_clock::now();
	while (facesCounter < number_of_faces) {
		getline(fpin, line);
		iss.str(line);		//split the line into its different values
		iss >> isItTriangleType;
		if (isItTriangleType == 3) {
			iss >> newFace.x_value;
			iss >> newFace.y_value;
			iss >> newFace.z_value;
			newfacesList.push_back(newFace);  //push the new face values to the list containing all the faces
		}
		if ((showLines == -1) || (facesCounter < showLines)) { //will we show it on the console or not ?
			std::cout << "faceCounter: " << facesCounter << " (x,y,z): " << newFace.x_value << " " << newFace.y_value << " " << newFace.z_value << " \n";
		}
		facesCounter++; //go to the next face line
	}
	auto end_reading_faces = chrono::steady_clock::now();
	std::cout << "Reading faces took: " << chrono::duration_cast<chrono::seconds>(end_reading_faces - start_reading_faces).count() << " seconds\n";
	fpin.close(); //input ply file will not be opended any more, so we can close it 
	return;
}

void ReadingNewMesh::showTriangles(int showLines) {
	int facesCounter = 0; //we start scanning the faces, and for each face we obtain the vertices
	Point3I getFace;
	Triangle3D triangle; //for each face a triangle is constructed, having 3 points with 3 vertices each
	std::cout << "Show Triangles\n";
	auto start_showing_triangles = chrono::steady_clock::now();
	while (facesCounter < (int)facesList.size()) {  //typecasting for "int", otherwise a compiler warning, no error, better cure it...
		getFace = facesList[facesCounter];
		triangle = fillUpTriangle(getFace); //retrieve all vertices for the 3 triangle points	
		if ((showLines == -1) || (facesCounter < showLines)) { //will we show it on the console or not ? (debug reasons?)
			std::cout << "==>Triangle: " << facesCounter << "\n";
			std::cout << "Triangle->p1: " << triangle.p1.x_value << " " << triangle.p1.y_value << " " << triangle.p1.z_value << "\n";
			std::cout << "Triangle->p2: " << triangle.p2.x_value << " " << triangle.p2.y_value << " " << triangle.p2.z_value << "\n";
			std::cout << "Triangle->p3: " << triangle.p3.x_value << " " << triangle.p3.y_value << " " << triangle.p3.z_value << "\n";
		}
		facesCounter++;
	}
	auto end_showing_triangles = chrono::steady_clock::now();
	std::cout << "Reading faces took: " << chrono::duration_cast<chrono::seconds>(end_showing_triangles - start_showing_triangles).count() << " seconds\n";
	return;
}

ReadingNewMesh::Triangle3D ReadingNewMesh::fillUpTriangle(Point3I getFace) {
	Triangle3D triangle;
	//create coordinates of the first triangle point (we use the first value of the face)
	triangle.p1.x_value = verticesList[getFace.x_value].x_value;
	triangle.p1.y_value = verticesList[getFace.x_value].y_value;
	triangle.p1.z_value = verticesList[getFace.x_value].z_value;

	//create coordinates of the second triangle point (we use the second value of the face)
	triangle.p2.x_value = verticesList[getFace.y_value].x_value;
	triangle.p2.y_value = verticesList[getFace.y_value].y_value;
	triangle.p2.z_value = verticesList[getFace.y_value].z_value;

	//create coordinates of the third triangle point (we use the third value of the face)
	triangle.p3.x_value = verticesList[getFace.z_value].x_value;
	triangle.p3.y_value = verticesList[getFace.z_value].y_value;
	triangle.p3.z_value = verticesList[getFace.z_value].z_value;
	return triangle;
}

int main()
{
	string plyDir = "C:\\Users\\bartc\\Source\\Repos\\RayTracer\\ReadingMesh\\ply_examples\\";
	//different PLY files (unselect the one you want out of comment)
	//string plyFile = "airplane.ply"; //airplane.ply: 1.335 vertices, 2.452 faces
	//string plyFile = "dragon.ply"; //dragon.ply: 437.645 vertices, 871.414 faces
	string plyFile = "happy.ply"; //happy.ply (happy Buddha) : 543.652 vertices, 1.087.716 faces

	ReadingNewMesh meshReader;
	meshReader.allocatePlyFile(plyDir + plyFile); //open the PLY file
	meshReader.readHeaderInformation(); //read the header information and extract the number of vertices and the number of faces
	meshReader.readVertices(10);					//read all vertices and store them in a list
	meshReader.readFaces(10);					//read all faces and store them in a list
	meshReader.showTriangles(10);				//show the (x,y,z) points of each of the 3 points of a triangle (face)
	return 0;
}
