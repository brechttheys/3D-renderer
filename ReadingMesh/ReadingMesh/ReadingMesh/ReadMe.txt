========================================================================
    CONSOLE APPLICATION : [ReadingMesh] Project Overview
========================================================================
This is the structure of a typical PLY file :
---------------------------------------------
Header
Vertex List
Face List
(lists of other elements)
---------------------------------------------
Example:

ply
format ascii 1.0
comment author: Greg Turk			  //inventor of the format
comment object: another cube
element vertex 8
property float x
property float y
property float z
property uchar red                    { start of vertex color }
property uchar green
property uchar blue
element face 7
property list uchar int vertex_index  { number of vertices for each face }
element edge 5                        { five edges in object }
property int vertex1                  { index to first vertex of edge }
property int vertex2                  { index to second vertex }
property uchar red                    { start of edge color }
property uchar green
property uchar blue
end_header
0 0 0 255 0 0                         { start of vertex list }
0 0 1 255 0 0
0 1 1 255 0 0
0 1 0 255 0 0
1 0 0 0 0 255
1 0 1 0 0 255
1 1 1 0 0 255
1 1 0 0 0 255
3 0 1 2                           { start of face list, begin with a triangle } //we read only triangles...
3 0 2 3                           { another triangle }
4 7 6 5 4                         { now some quadrilaterals }
4 0 4 5 1
4 1 5 6 2
4 2 6 7 3
4 3 7 4 0
0 1 255 255 255                   { start of edge list, begin with white edge }
1 2 255 255 255
2 3 255 255 255
3 0 255 255 255
2 0 0 0 0                         { end with a single black line }
---------------------------------------------
ply
format ascii 1.0
comment Mars model by Paul Bourke
element vertex 259200  <------------- select this line and select the number of VERTICES:259.200 in total in this case
property float x
property float y
property float z
element face 516960    <------------- select this line and select the number of FACES: 516.960 in total in this case
property list uchar int vertex_indices
end_header             <------------- select this line to select the reading of the vertices after it
15081.5 -3.45644e+06 65.8061
15081 -3.45659e+06 197.422
15078.2 -3.45648e+06 329.009
15075.4 -3.45663e+06 460.597
15071.2 -3.4567e+06 592.148
15065.6 -3.45674e+06 723.653
15059.9 -3.457e+06 855.16
15050.7 -3.45674e+06 986.473

lots of vertices follow

3 0 721 1              <------------- additional indicator to start the reading of the faces
3 721 0 720
3 1 722 2
3 722 1 721
3 2 723 3
3 723 2 722

lots of triangular facets follow
---------------------------------------------
an extract of the "airplane.ply"
ply
format ascii 1.0
element vertex 1335
property float32 x
property float32 y
property float32 z
element face 2452
property list uint8 int32 vertex_indices
end_header
896.994 48.7601 82.2656
906.593 48.7601 80.7452
907.539 55.4902 83.6581
896.994 55.4902 85.3283
896.994 42.8477 77.825
............
806.665 600.301 2.61432
806.665 627.363 5.11482
806.665 654.432 7.51998
806.665 681.537 9.48744
3 0 1 2
3 0 2 3
3 4 5 1
3 4 1 0
3 6 7 5
3 ..........
----etc.....