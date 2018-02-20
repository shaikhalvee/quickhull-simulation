#include<bits/stdc++.h>
#include <windows.h>
#include <glut.h>

using namespace std;

#define MAX 10000000
#define pi (2*acos(0.0))

// for graphical
struct Point
{
	double x,y,z;
};

// for quick hull simulation
struct point
{
    int x, y;
};
typedef vector<point>vp;
vp points;
vp output;

void print(vp P);
vp rightPointsOf(point, point, vp);
int crossProduct(point, point, point);
void quickHull(point, point, vp);


void drawAxes()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0, 0, 0);
        glVertex3f( 150,0,0);
        glVertex3f(-150,0,0);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0,-150,0);
        glVertex3f(0, 150,0);

        glVertex3f(0,0, 150);
        glVertex3f(0,0,-150);
    } glEnd();

}


void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct Point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawPoints(vp P)
{
    for(int i=0;i<P.size();i++){
        glPushMatrix();
        {
            glTranslated(P[i].x, P[i].y, 0);
            drawCircle(0.5,30);
        }
        glPopMatrix();
    }
}

void drawConvexHull(vp P)
{
    for(int i=1;i<P.size();i++){
        glBegin(GL_LINES);
        {
			//glColor3d((double)i/(double)P.size(), 0,0);
			glVertex3d(P[i-1].x,P[i-1].y,0);
			glVertex3d(P[i].x,P[i].y,0);
        }
        glEnd();
    }
}

void quickHullSimulation()
{
    FILE *fp = freopen("input.txt","r",stdin);
    point input;
    int size;
    // while(cin>>size)
    cin >> size;
    for(int i = 0; i<size; i++){
        cin >> input.x >> input.y;
        points.push_back(input);
    }
    sort(points.begin(), points.end(), [](const point& lft, const point& rgt) {return lft.x < rgt.x;});
    print(points);
    vp downHull = rightPointsOf(points[0], points[points.size()-1], points);
    vp upHull = rightPointsOf(points[points.size()-1], points[0], points);
    output.push_back(points[0]);
    quickHull(points[0], points[points.size()-1], downHull);
    output.push_back(points[points.size()-1]);
    quickHull(points[points.size()-1], points[0], upHull);
    output.push_back(points[0]);
    print(output);
}

void drawQuickHull()
{
    drawPoints(points);
    drawConvexHull(output);
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,20,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	quickHullSimulation();
    drawQuickHull();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Quick Hull simulation");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)


	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

// quickHull  main codes
void print(vp P)
{
    cout<<endl;
    for(int i = 0; i<P.size(); i++){
        cout << P[i].x << " " << P[i].y << endl;
    }
}

int crossProduct(point a, point b, point c)
{
    int ac_x = c.x-a.x;
    int ac_y = c.y-a.y;
    int ab_x = b.x-a.x;
    int ab_y = b.y-a.y;
    return ac_x*ab_y - ac_y*ab_x;
}

vp rightPointsOf(point leftP, point rightP, vp P)
{
    vp retPoints;
    for(int i=0;i<P.size();i++){
        if(crossProduct(leftP,rightP,P[i])>0){
            retPoints.push_back(P[i]);
        }
    }
    return retPoints;
}

void quickHull(point leftP, point rightP, vp P)
{
    if(!P.size()){     // left and right are extreme edge
        return;
    }
    //point maxPoint;
    int val = -(MAX), index;
    for(int i=0;i<P.size();i++){    // finding the maximum value of cross product
        if(crossProduct(leftP,rightP,P[i])>val){
            val = crossProduct(leftP,rightP,P[i]);
            //maxPoint = P[i];
            index = i;      // index has max points index
        }
    }
    vp L = rightPointsOf(leftP, P[index], P);
    vp R = rightPointsOf(P[index], rightP, P);
    quickHull(leftP, P[index], L);
    //output.push_back(maxPoint);
    output.push_back(P[index]);
    quickHull(P[index], rightP, R);
}
