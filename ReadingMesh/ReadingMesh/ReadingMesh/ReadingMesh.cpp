#include "stdafx.h"
#include "ReadingMesh.h"
#include <chrono>


using namespace std;

ReadingMesh::ReadingMesh(void) {} //constructor

void ReadingMesh::allocatePlyFile(string in) {
	fpin = (std::ifstream)(in);
	if (fpin.is_open()) {
		std::cout << "file: " << in << "is opened for input \n";
	}
	else {
		std::cout << "Unable to open input file: " << in << " \n";
		exit(EXIT_FAILURE);
	}
	return;
}

void ReadingMesh::readHeaderInformation() {
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

void ReadingMesh::readVertices(int showLines) {
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
		verticesList.push_back(newVertice); //put it into the vertices list
		if ((showLines == -1) || (verticeCounter < showLines)) { //will we show it on the console or not ?
			std::cout << "VerticeCounter: " << verticeCounter << " (x,y,z): "
				<< newVertice.x_value << " " << newVertice.y_value << " " << newVertice.z_value << " \n";
		}
		verticeCounter++;	//go to the next vertice line
	}
	auto end_reading_vertices = chrono::steady_clock::now();
	std::cout << "Reading vertices took: " << chrono::duration_cast<chrono::seconds>(end_reading_vertices - start_reading_vertices).count() << " seconds\n";
	return;
}

void ReadingMesh::readFaces() {
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
			facesList.push_back(newFace);  //push the new face values to the list containing all the faces
		}
		facesCounter++; //go to the next face line
	}
	auto end_reading_faces = chrono::steady_clock::now();
	std::cout << "Reading faces took: " << chrono::duration_cast<chrono::seconds>(end_reading_faces - start_reading_faces).count() << " seconds\n";
	fpin.close(); //input ply file will not be opended any more, so we can close it 
	return;
}

void ReadingMesh::showTriangles(int showLines) {
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
	std::cout << "Reading faces took: " << chrono::duration_cast<chrono::seconds>(end_showing_triangles - start_showing_triangles).count() << " seconds\n\n\n";
	return;
}

ReadingMesh::Triangle3D ReadingMesh::fillUpTriangle(Point3I getFace) {
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

ReadingMesh::BBox ReadingMesh::fillUpBBox(Point3D vertex) {
	BBox temp;
	temp.xMax = vertex.x_value;
	temp.xMin = vertex.x_value;
	temp.yMax = vertex.y_value;
	temp.yMin = vertex.y_value;
	temp.zMax = vertex.z_value;
	temp.zMin = vertex.z_value;
	return temp;
}

void ReadingMesh::createTopBoundingBox()
{
	Point3D tempP;
	BBox tempB;

	//Init temp Bounding Box for comparisons
	tempB = fillUpBBox(verticesList.at(0));

	//Loop each vertex for min/max sizes of outer bounding box
	for (int i = 0; i < this->verticesList.size(); i++) {
		tempP = verticesList.at(i);
		if (tempP.x_value > tempB.xMax)
			tempB.xMax = tempP.x_value;

		if (tempP.x_value < tempB.xMin)
			tempB.xMin = tempP.x_value;

		if (tempP.y_value > tempB.yMax)
			tempB.yMax = tempP.y_value;

		if (tempP.y_value < tempB.yMin)
			tempB.yMin = tempP.y_value;

		if (tempP.z_value > tempB.zMax)
			tempB.zMax = tempP.z_value;

		if (tempP.z_value < tempB.zMin)
			tempB.zMin = tempP.z_value;
	}

	topBoundingBox = tempB;

	cout << "Outer bounding box found! size: Width: " << topBoundingBox.Width() << ", Height: " << topBoundingBox.Height() << ", Depth: " << topBoundingBox.Depth() << endl;
}

void ReadingMesh::decideLeftRight(vector<Point3I>& faces, BBox& left, BBox& right, vector<Point3I>& leftT, vector<Point3I>& rightT) {
	Point3I tmp = faces.at(0);
	Point3D p1, p2, p3;

	p1 = verticesList.at(tmp.x_value);
	p2 = verticesList.at(tmp.y_value);
	p3 = verticesList.at(tmp.z_value);

	leftT.clear();
	rightT.clear();
	for (int i = 0; i < faces.size(); i++) {
		tmp = faces.at(i);
		p1 = verticesList.at(tmp.x_value);
		p2 = verticesList.at(tmp.y_value);
		p3 = verticesList.at(tmp.z_value);

		if (right.contains(p1) && right.contains(p2) && right.contains(p3)) {
			rightT.push_back(tmp);
		}
		else {
			leftT.push_back(tmp);
		}
	}

}

void ReadingMesh::splitTopBoundingBox(int iterations, vector<BBox>& boxesResult, vector<vector<Point3I>>& facesResult) {
	//Split the already calculated top box into smaller boxes
	//iterations is amount of times the boxes are divided in 2
	//Result is 2^(iterations) vector of boxes
	//And a vector filled with lists of faces linked to the boxes

	auto leftBox = BBox(); //placeholder temp boxes
	auto rightBox = BBox();

	auto leftFaces = vector<Point3I>(); //placeholder faces
	auto rightFaces = vector<Point3I>();

	auto newBoxes = vector<BBox>(); //placeholder lists
	auto newFaces = vector<vector<Point3I>>();

	//Create first two boxes manually, split width in 2 and calc triangles
	initBoundaries('x', topBoundingBox, leftBox, rightBox);
	decideLeftRight(facesList, leftBox, rightBox, leftFaces, rightFaces);

	//Create loopable box list
	auto tmpBBoxList = vector<BBox>();
	tmpBBoxList.push_back(leftBox);
	tmpBBoxList.push_back(rightBox);

	//And list of faceslists
	auto tmpFacesList = vector<vector<Point3I>>();
	tmpFacesList.push_back(leftFaces);
	tmpFacesList.push_back(rightFaces);

	//Iterations - 1 because we already splitted once
	for (int i = 0; i < (iterations - 1); i++) {

		for (int j = 0; j < tmpBBoxList.size(); j++) {
			//Each time calculate new box size
			divideInTwoBoxes(tmpBBoxList.at(j), leftBox, rightBox);
			newBoxes.push_back(leftBox);
			newBoxes.push_back(rightBox);

			//cout << "Checking before" << newFaces.at(0).size()<< endl;
			//And decide if the faces are in left or right box (dead faces are in left box)
			decideLeftRight(tmpFacesList.at(j), leftBox, rightBox, leftFaces, rightFaces);
			newFaces.push_back(leftFaces);
			newFaces.push_back(rightFaces);

			//cout << "Checking after" << endl;

		}

		cout <<"Boxes amount: " << newBoxes.size() << endl;//Should be exactly the same

		tmpBBoxList = newBoxes;    // will loop over new values
		newBoxes.clear();		   // clean current loop results

		tmpFacesList = newFaces;
		newFaces.clear();
	}
	facesResult = tmpFacesList;
	boxesResult = tmpBBoxList;
	amtOfBoxes = tmpBBoxList.size();
}

void ReadingMesh::divideInTwoBoxes(BBox& top, BBox& left, BBox& right) {
	//Based on biggest value will choose to split top box into left and right box

	if (top.Height() > top.Width() && top.Height() > top.Depth()) {
		initBoundaries('y', top, left, right); //Height biggest, split y in 2
	}
	else if (top.Width() > top.Height() && top.Width() > top.Depth()) {
		initBoundaries('x', top, left, right); //Width biggest, split x in 2
	}
	else if (top.Depth() > top.Height() && top.Depth() > top.Width()) {
		initBoundaries('z', top, left, right); //Depth biggest, split z in 2
	}
}

void ReadingMesh::initBoundaries(char direction, BBox& top, BBox& left, BBox& right) {
	//Direction: char x, y or z. Will split based on this char
	//top: top outer box
	//left & right: 2 output boxes

	switch (direction) {
	case 'x':
		left.xMax = ((top.xMax - top.xMin) / 2) + top.xMin;
		left.xMin = top.xMin;
		left.yMax = top.yMax;
		left.yMin = top.yMin;
		left.zMax = top.zMax;
		left.zMin = top.zMin;

		right.xMax = top.xMax;
		right.xMin = ((top.xMax - top.xMin) / 2) + top.xMin;
		right.yMax = top.yMax;
		right.yMin = top.yMin;
		right.zMax = top.zMax;
		right.zMin = top.zMin;
		break;

	case'y': //left = bot, right = top
		left.xMax = top.xMax;
		left.xMin = top.xMin;
		left.yMax = ((top.yMax - top.yMin) / 2) + top.yMin;
		left.yMin = top.yMin;
		left.zMax = top.zMax;
		left.zMin = top.zMin;

		right.xMax = top.xMax;
		right.xMin = top.xMin;
		right.yMax = top.yMax;
		right.yMin = ((top.yMax - top.yMin) / 2) + top.yMin;
		right.zMax = top.zMax;
		right.zMin = top.zMin;
		break;

	case'z': //left = front, right = behind
		left.xMax = top.xMax;
		left.xMin = top.xMin;
		left.yMax = top.yMax;
		left.yMin = top.yMin;
		left.zMax = ((top.zMax - top.zMin) / 2) + top.zMin;
		left.zMin = top.zMin;

		right.xMax = top.xMax;
		right.xMin = top.xMin;
		right.yMax = top.yMax;
		right.yMin = top.yMin;
		right.zMax = top.zMax;
		right.zMin = ((top.zMax - top.zMin) / 2) + top.zMin;
		break;
	default: return;
	}
}

void ReadingMesh::outputToFile(string path, vector<BBox>& boxesResult, vector<vector<Point3I>>& facesResult) {
	fpin = (std::ifstream)(path+".ply");
	ofstream dest(path + ".fly", ios::binary);
	//Output to destination, source is previous file (will include all data of source)

	copyPLYFile(dest);
	fpin.close();
	//Close source after copied 

	for (int i = 0; i < boxesResult.size(); i++) {
		dest << boxesResult.at(i).xMax << " " << boxesResult.at(i).xMin << " " << boxesResult.at(i).yMax << " " << boxesResult.at(i).yMin << " " << boxesResult.at(i).zMax << " " << boxesResult.at(i).zMin << endl;

		string faces = "4 ";
		cout << "outputting " << facesResult.at(i).size() << " faces to the " << i + 1 << "th box!" << endl;
		for (int j = 0; j < facesResult.at(i).size(); j++) {


			for (int faceId = 0; faceId < facesList.size(); faceId++) {
				if (facesList.at(faceId) == facesResult.at(i).at(j)) {
					faces += to_string(faceId) +" ";
				}
			}
		}
		dest << faces << endl;

	}



	dest.close();
}

void ReadingMesh::copyPLYFile(ofstream& d) {
	string line;

	getline(fpin, line);
	d << line << endl;

	//loop untill header opened
	while (line.compare("format ascii 1.0") != 0) {
		getline(fpin, line);
		d << line << endl;
	} 
	//Insert element boxes with boxes count inside header
	d << "element boxes " + amtOfBoxes<< endl;

	//Loop until end of file
	while (!fpin.eof()) {
		getline(fpin, line);
		d << line << endl;
	}
}
int main()
{
	string plyDir = "C:\\Users\\steve\\source\\repos\\RayTracer\\ReadingMesh\\ply_examples\\";
	//different PLY files (unselect the one you want out of comment)
	//string plyFile = "airplane.ply"; //airplane.ply: 1.335 vertices, 2.452 faces
	//string plyFile = "dragon.ply"; //dragon.ply: 437.645 vertices, 871.414 faces
	string plyFile = "airplane"; //happy.ply (happy Buddha) : 543.652 vertices, 1.087.716 faces

	ReadingMesh meshReader;
	meshReader.allocatePlyFile(plyDir + plyFile + ".ply");   //open the PLY file
	meshReader.readHeaderInformation();				//read the header information and extract the number of vertices and the number of faces
	meshReader.readVertices(10);					//read all vertices and store them in a list
	meshReader.readFaces(10);					    //read all faces and store them in a list
	meshReader.showTriangles(20);				    //show the (x,y,z) points of each of the 3 points of a triangle (face)


	auto boxesList = vector<ReadingMesh::BBox>();	//Create values we'll output to file
	auto facesList = vector<vector<ReadingMesh::Point3I>>();

	meshReader.createTopBoundingBox();				//Create the top bounding box
	meshReader.splitTopBoundingBox(4, boxesList, facesList); //Fill in vectors based on amount of iterations
	meshReader.outputToFile(plyDir + plyFile, boxesList, facesList);


	getchar();
	return 0;
}
