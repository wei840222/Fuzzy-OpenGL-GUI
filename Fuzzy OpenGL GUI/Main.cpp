#include <iostream>
#include <string>
#include <math.h>
#include <GL\glut.h>
#include "g_bmp.h"

using namespace std;

class Point {
public:
	float x, y;
	Point() {
		x = 0;
		y = 0;
	}
	Point(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

class Line {
public:
	float a, b;
};

//X
const int LE = 0;
const int LC = 1;
const int CE = 2;
const int RC = 3;
const int RI = 4;
//FI
const int RB = 0;
const int RU = 1;
const int RV = 2;
const int VE = 3;
const int LV = 4;
const int LU = 5;
const int LB = 6;
//Thida
const int NB = -30;
const int NM = -15;
const int NS = -5;
const int ZE = 0;
const int PS = 5;
const int PM = 15;
const int PB = 30;

const float PI = 3.14159262575;

//Default 5: LE, LC, CE, RC, RI
const int cntIvlXPositionFuzzy = 5;
int ivlXPosition[15];
const int ivlXPositionY[15] = { 20,20,0,0,20,0,0,20,0,0,20,0,0,20,20 };

//Default 7: RB, RU, RV, RC, LV, LU, LB
const int cntIvlAngleFuzzy = 7;
int ivlAngle[21];
const int ivlAngleY[21] = { 0,20,0,0,20,0,0,20,0,0,20,0,0,20,0,0,20,0,0,20,0 };

//Initial value
float x = 0, y = 0, fi = 0, r = 1;

//fuzzyCalculate
int stepCnt = 0;
float fuzzyX[1000], fuzzyY[500], fuzzyThida[500];

//Chart Original Point
Point fuzzyO(0, 0), mxO(150, 45), mfiO(175, 90), mthidaO(200, 0), formO(195, -70), dataO(135, -50);

//Window State
int windowW = 950, windowH = 650;
int windowOX = 10, windowOY = 75;
int lookSizeX = 300, lookSizeY = 200;

//Mouse
bool click = false;
int buttonNum = 99;	//None Button Press ID
Point mousePosition;

//Locus
const int animationDelay = 20;
int animation = 0;
bool animationPlay = true;

//GUI Ctrl Point
Point mxPoint[15], mfiPoint[21], carOrigin, playPause(135, -60), reset(150, -60);
float carOriginFi;


Line lineFuction(float x1, float y1, float x2, float y2) {
	// linerEquation: y = ax + b
	Line l;
	l.a = (y1 - y2) / (x1 - x2); //斜率 
	l.b = y1 - (l.a*x1); //截距 
	return l;
}

int isPointInterval(float x, float bound_1, float bound_2) {
	if (bound_1 <= x && x < bound_2)
		return 1;
	else
		return 0;
}

void calculateFuzzyForXPositionMap(float *mx) {
	//According to Fuzzy membership function for XPosition, calculate row of FAM.
	Line linerEquation;
	float ivlBound_1, ivlBound_2;
	float direct_up[2] = { 0, 1 };
	float direct_down[2] = { 1, 0 };

	//LE
	ivlBound_1 = ivlXPosition[0];
	ivlBound_2 = ivlXPosition[1];
	mx[LE] = 1 * (isPointInterval(x, ivlBound_1, ivlBound_2));

	ivlBound_1 = ivlXPosition[1];
	ivlBound_2 = ivlXPosition[2];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	mx[LE] = mx[LE] + (isPointInterval(x, ivlBound_1, ivlBound_2)) * (linerEquation.a * x + linerEquation.b);

	//LC
	ivlBound_1 = ivlXPosition[3];
	ivlBound_2 = ivlXPosition[4];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	mx[LC] = (isPointInterval(x, ivlBound_1, ivlBound_2)) * (linerEquation.a * x + linerEquation.b);

	ivlBound_1 = ivlXPosition[4];
	ivlBound_2 = ivlXPosition[5];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	mx[LC] = mx[LC] + (isPointInterval(x, ivlBound_1, ivlBound_2)) * (linerEquation.a * x + linerEquation.b);

	//CE
	ivlBound_1 = ivlXPosition[6];
	ivlBound_2 = ivlXPosition[7];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	mx[CE] = (isPointInterval(x, ivlBound_1, ivlBound_2)) * (linerEquation.a * x + linerEquation.b);

	ivlBound_1 = ivlXPosition[7];
	ivlBound_2 = ivlXPosition[8];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	mx[CE] = mx[CE] + (isPointInterval(x, ivlBound_1, ivlBound_2)) * (linerEquation.a * x + linerEquation.b);

	//RC
	ivlBound_1 = ivlXPosition[9];
	ivlBound_2 = ivlXPosition[10];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	mx[RC] = (isPointInterval(x, ivlBound_1, ivlBound_2)) * (linerEquation.a * x + linerEquation.b);

	ivlBound_1 = ivlXPosition[10];
	ivlBound_2 = ivlXPosition[11];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	mx[RC] = mx[RC] + (isPointInterval(x, ivlBound_1, ivlBound_2)) * (linerEquation.a * x + linerEquation.b);

	//RI
	ivlBound_1 = ivlXPosition[12];
	ivlBound_2 = ivlXPosition[13];
	mx[RI] = 1 * (isPointInterval(x, ivlBound_1, ivlBound_2));

	ivlBound_1 = ivlXPosition[14];
	ivlBound_2 = ivlXPosition[12];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	mx[RI] = mx[RI] + (isPointInterval(x, ivlBound_1, ivlBound_2)) * (linerEquation.a * x + linerEquation.b);
}

void calculateFuzzyForAngleMap(float *my) {
	//According to Fuzzy membership function for Angle, calculate column of FAM.   
	Line linerEquation;
	float ivlBound_1, ivlBound_2;
	float direct_up[2] = { 0, 1 };
	float direct_down[2] = { 1, 0 };

	//RB
	ivlBound_1 = ivlAngle[0];
	ivlBound_2 = ivlAngle[1];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	my[RB] = (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	ivlBound_1 = ivlAngle[1];
	ivlBound_2 = ivlAngle[2];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	my[RB] = my[RB] + (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	//RU
	ivlBound_1 = ivlAngle[3];
	ivlBound_2 = ivlAngle[4];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	my[RU] = (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	ivlBound_1 = ivlAngle[4];
	ivlBound_2 = ivlAngle[5];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	my[RU] = my[RU] + (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	//RV
	ivlBound_1 = ivlAngle[6];
	ivlBound_2 = ivlAngle[7];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	my[RV] = (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	ivlBound_1 = ivlAngle[7];
	ivlBound_2 = ivlAngle[8];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	my[RV] = my[RV] + (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	//VE
	ivlBound_1 = ivlAngle[9];
	ivlBound_2 = ivlAngle[10];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	my[VE] = (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	ivlBound_1 = ivlAngle[10];
	ivlBound_2 = ivlAngle[11];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	my[VE] = my[VE] + (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	//LV
	ivlBound_1 = ivlAngle[12];
	ivlBound_2 = ivlAngle[13];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	my[LV] = (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	ivlBound_1 = ivlAngle[13];
	ivlBound_2 = ivlAngle[14];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	my[LV] = my[LV] + (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	//LU
	ivlBound_1 = ivlAngle[15];
	ivlBound_2 = ivlAngle[16];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	my[LU] = (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	ivlBound_1 = ivlAngle[16];
	ivlBound_2 = ivlAngle[17];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	my[LV] = my[LV] + (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	//LB
	ivlBound_1 = ivlAngle[18];
	ivlBound_2 = ivlAngle[19];
	linerEquation = lineFuction(ivlBound_1, direct_up[0], ivlBound_2, direct_up[1]);
	my[LU] = (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);

	ivlBound_1 = ivlAngle[19];
	ivlBound_2 = ivlAngle[20];
	linerEquation = lineFuction(ivlBound_1, direct_down[0], ivlBound_2, direct_down[1]);
	my[LV] = my[LV] + (isPointInterval(fi, ivlBound_1, ivlBound_2)) * (linerEquation.a*fi + linerEquation.b);
}

void fuzzyCalculate() {
	//RB,RU,RV,VE,LV,LU,LB
	float FuzzyAngleMap[7];

	//LE,LC,CE,RC,RI
	float FuzzyXPositionMap[5];

	//Map which provide the angle from FuzzyAngleMap and FuzzyXPositionMap
	float thidaMap[7][5] = {
		{ PS,PM,PM,PB,PB },//{5,15,15,30,30}
		{ NS,PS,PM,PB,PB },//{-5,5,15,30,30}
		{ NM,NS,PS,PM,PB },//{-15,-5,5,15,30}
		{ NM,NM,ZE,PM,PM },//{15,-15-0,15,15}
		{ NB,NM,NS,PS,PM },//{-30,-15,5,5,15}
		{ NB,NB,NM,NS,PS },//{-30,-30,15,-5,5}
		{ NB,NB,NM,NM,NS } //{-30,-30,-15,-15,-5}
	};

	//Reinitialize
	stepCnt = 0;
	x = carOrigin.x;
	y = carOrigin.y;
	fi = carOriginFi;
	fuzzyX[0] = carOrigin.x;
	fuzzyY[0] = carOrigin.y;
	system("CLS");

	while (y < 100) {
		calculateFuzzyForXPositionMap(FuzzyXPositionMap);
		calculateFuzzyForAngleMap(FuzzyAngleMap);

		float weighted = 0;
		float thidaFuzzy = 0;
		float minValue = 0;

		for (int i = 0; i < cntIvlAngleFuzzy; i++) {
			for (int j = 0; j < cntIvlXPositionFuzzy; j++) {
				minValue = fmin(FuzzyAngleMap[i], FuzzyXPositionMap[j]);
				thidaFuzzy += minValue * thidaMap[i][j];
				//Correlation-minimum FAM Inference Procedure
				weighted += minValue;
			}
		}
		//Centroid Defuzzification
		thidaFuzzy /= weighted;
		stepCnt++;
		//Store point data
		fi += thidaFuzzy;
		float rad = (PI / 180) * fi;
		x += r*cos(rad);
		y += r*sin(rad);
		fuzzyX[stepCnt] = x;
		fuzzyY[stepCnt] = y;
		fuzzyThida[stepCnt] = fi;
		cout << "X: " << x << " Y: " << y << " fi: " << fi <<" Step: " << stepCnt << endl;
	}
	cout << "CarOriginX: " << carOrigin.x << " CarOriginY: " << carOrigin.y << " CarOriginfi: " << carOriginFi << endl;
}

void fuzzyReset() {
	int reIvlXPosition[15] = { 0,10,35,30,40,50,45,50,55,50,60,70,65,90,100 };
	int reIvlAngle[21] = { -110,-50,10,-10,30,70,60,75,90,80,90,100,90,120,150,130,160,190,170,230,290 };

	carOrigin.x = 0; carOrigin.y = 0; carOriginFi = 0;
	x = 0; y = 0; fi = 0;

	//fuzzyReset
	for (int i = 0; i < 15; i++) {
		ivlXPosition[i] = reIvlXPosition[i];
	}
	for (int i = 0; i < 21; i++) {
		ivlAngle[i] = reIvlAngle[i];
	}

	//Init GUI Ctrl Point
	for (int i = 0; i < 15; i++) {
		mxPoint[i].x = ivlXPosition[i] + mxO.x;
		mxPoint[i].y = ivlXPositionY[i] + mxO.y;
	}
	for (int i = 0; i < 21; i++) {
		mfiPoint[i].x = ivlAngle[i] * 5 / 18 + mfiO.x;
		mfiPoint[i].y = ivlAngleY[i] + mfiO.y;
	}
}

float distance(Point p, Point q) {
	return sqrt(pow(p.x - q.x, 2) + pow(p.y - q.y, 2));
}

void drawWiredCircle(float x, float y, float radius) {
	int count;
	int sections = 360;

	glBegin(GL_LINE_STRIP);
	for (count = 0; count <= sections; count++) {
		glVertex2f(x + radius*cos(count * 2 * PI / sections), y + radius*sin(count * 2 * PI / sections));
	}
	glEnd();
}

void glPrintf(char *ch, float x, float y) {
	glRasterPos2f(x, y);
	for (int i = 0; i < strlen(ch); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(ch + i));
}

void Chart() {
	glColor3f(0, 0, 0);
	glLineWidth(2);

	//Fuzzy
	glPushMatrix();
	glTranslatef(fuzzyO.x, fuzzyO.y, 0);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(110, 0);
	glVertex2f(0, -60);
	glVertex2f(0, 110);
	glEnd();
	glPrintf("50", 48, -4);
	glPrintf("100", 97, -4);
	glPrintf("X", 111, -1);
	glPrintf("-50", - 8, - 51);
	glPrintf("50", -6, 49);
	glPrintf("100", -8, 99);
	glPrintf("Y", -1, 111);
	glPopMatrix();
	
	//m(x)
	glPushMatrix();
	glTranslatef(mxO.x, mxO.y, 0);
	glBegin(GL_LINES);
	glVertex2f(-20, 0);
	glVertex2f(120, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 30);
	glEnd();
	glPrintf("x", 121, -1);
	glPrintf("m(x)", -3, 32);
	glPrintf("0", -1, -4);
	glPrintf("10", 8, -4);
	glPrintf("20", 18, -4);
	glPrintf("30", 28, -4);
	glPrintf("40", 38, -4);
	glPrintf("50", 48, -4);
	glPrintf("60", 58, -4);
	glPrintf("70", 68, -4);
	glPrintf("80", 78, -4);
	glPrintf("90", 88, -4);
	glPrintf("100", 97, -4);
	glPopMatrix();
		
	//m(fi)
	glPushMatrix();
	glTranslatef(mfiO.x, mfiO.y, 0);
	glBegin(GL_LINES);
	glVertex2f(-45, 0);
	glVertex2f(85, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 30);
	glEnd();
	glPrintf("fi", 86, -1);
	glPrintf("m(fi)", -3, 32);
	glPrintf("-90", -28, -4);
	glPrintf("0", -1, -4);
	glPrintf("90", 23, -4);
	glPrintf("180", 47, -4);
	glPrintf("270", 72, -4);
	glPopMatrix();
	
	//m(thida)
	glPushMatrix();
	glTranslatef(mthidaO.x, mthidaO.y, 0);
	glBegin(GL_LINES);
	glVertex2f(-70, 0);
	glVertex2f(70, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 30);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(-60, 20);
	glVertex2f(-30, 0);
	glVertex2f(-50, 0);
	glVertex2f(-30, 20);
	glVertex2f(-10, 0);
	glVertex2f(-24, 0);
	glVertex2f(-10, 20);
	glVertex2f(0, 0);
	glVertex2f(-10, 0);
	glVertex2f(0, 20);
	glVertex2f(10, 0);
	glVertex2f(0, 0);
	glVertex2f(10, 20);
	glVertex2f(24, 0);
	glVertex2f(10, 0);
	glVertex2f(30, 20);
	glVertex2f(50, 0);
	glVertex2f(30, 0);
	glVertex2f(60, 20);
	glEnd();
	glPrintf("thida", 71, -1);
	glPrintf("m(thida)", -3, 32);
	glPrintf("-30", -63, -4);
	glPrintf("-20", -43, -4);
	glPrintf("-10", -23, -4);
	glPrintf("0", -1, -4);
	glPrintf("10", 18, -4);
	glPrintf("20", 38, -4);
	glPrintf("30", 58, -4);
	glPrintf("NB", -62, 23);
	glPrintf("NM", -32, 23);
	glPrintf("NS", -12, 23);
	glPrintf("ZE", -5, 23);
	glPrintf("PS", 8, 23);
	glPrintf("PM", 28, 23);
	glPrintf("PB", 58, 23);
	glPopMatrix();

	//Form
	GBmp form;
	form.load("form.bmp");
	glRasterPos2f(-form.w / windowH + formO.x, -form.h / windowW + formO.y);
	glDrawPixels(form.w, form.h, GL_RGB, GL_UNSIGNED_BYTE, form.rgb);
}

void Button() {
	//m(x)Points
	for (int i = 0; i <= 12; i += 3) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(mxPoint[i].x, mxPoint[i].y);
		glVertex2f(mxPoint[i + 1].x, mxPoint[i + 1].y);
		glVertex2f(mxPoint[i + 2].x, mxPoint[i + 2].y);
		glEnd();
	}
	glPrintf("LE", mxPoint[1].x - 2, mxPoint[1].y + 3);
	glPrintf("LC", mxPoint[4].x - 2, mxPoint[4].y + 3);
	glPrintf("CE", mxPoint[7].x - 2, mxPoint[7].y + 3);
	glPrintf("RC", mxPoint[10].x - 2, mxPoint[10].y + 3);
	glPrintf("RI", mxPoint[13].x - 2, mxPoint[13].y + 3);

	//m(fi)Points
	for (int i = 0; i <= 18; i += 3) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(mfiPoint[i].x, mfiPoint[i].y);
		glVertex2f(mfiPoint[i + 1].x, mfiPoint[i + 1].y);
		glVertex2f(mfiPoint[i + 2].x, mfiPoint[i + 2].y);
		glEnd();
	}
	glPrintf("RB", mfiPoint[1].x - 2, mfiPoint[1].y + 3);
	glPrintf("RU", mfiPoint[4].x - 2, mfiPoint[4].y + 3);
	glPrintf("RV", mfiPoint[7].x - 2, mfiPoint[7].y + 3);
	glPrintf("VE", mfiPoint[10].x - 2, mfiPoint[10].y + 3);
	glPrintf("LV", mfiPoint[13].x - 2, mfiPoint[13].y + 3);
	glPrintf("LU", mfiPoint[16].x - 2, mfiPoint[16].y + 3);
	glPrintf("LB", mfiPoint[19].x - 2, mfiPoint[19].y + 3);

	//Origin Car
	glPushMatrix();
	glTranslatef(carOrigin.x, carOrigin.y, 0);
	glRotatef(carOriginFi, 0, 0, 1);
	drawWiredCircle(0, 0, 7);
	glBegin(GL_LINE_STRIP);
	glVertex2f(7, 1);
	glVertex2f(9, 0);
	glVertex2f(7, -1);
	glEnd();
	glPopMatrix();

	//AnimationCtrl
	glPushMatrix();
	glTranslatef(playPause.x, playPause.y, 0);
	drawWiredCircle(0, 0, 5);
	if (animationPlay) {
		glBegin(GL_LINES);
		glVertex2f(-1, 2);
		glVertex2f(-1, -2);
		glVertex2f(1, 2);
		glVertex2f(1, -2);
		glEnd();
	}
	else {
		glBegin(GL_LINE_LOOP);
		glVertex2f(-1, 2);
		glVertex2f(2, 0);
		glVertex2f(-1, -2);
		glEnd();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(reset.x, reset.y, 0);
	drawWiredCircle(0, 0, 5);
	drawWiredCircle(0, 0, 2);
	glBegin(GL_LINE_STRIP);
	glVertex2f(1, 1);
	glVertex2f(2, 0);
	glVertex2f(3, 1);
	glEnd();
	glPopMatrix();
}

void Locus() {
	//倒車軌跡
	glColor3f(1, 0, 0);
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	if (animation <= stepCnt) {
		for (int i = 0; i <= animation; i++) {
			glVertex2f(fuzzyX[i], fuzzyY[i]);
		}
		glEnd();
		//小汽車
		glPushMatrix();
		glTranslatef(fuzzyX[animation], fuzzyY[animation], 0);
		glRotatef(fuzzyThida[animation], 0, 0, 1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(10, 5);
		glVertex2f(-10, 5);
		glVertex2f(-10, -5);
		glVertex2f(10, -5);
		glVertex2f(10, 5);
		glEnd();
		glPopMatrix();
	}
	else
		animation = 0;
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);

	Chart();
	Button();
	Locus();

	glutSwapBuffers();
}

void AnimationTimer(int id) {
	if (animationPlay) {
		animation++;
	}
	glutTimerFunc(animationDelay, AnimationTimer, 1);
}

void ChangeSize(int w, int h) {
	windowW = w;
	windowH = h;
	glViewport(0, 0, w, h);
}

void MouseClick(int button, int state, int x, int y) {
	//Look size
	mousePosition.x = (x * lookSizeX / windowW) - windowOX;
	mousePosition.y = lookSizeY - windowOY - (y * lookSizeY / windowH);

	if (button == GLUT_LEFT_BUTTON && state == 0) {
		buttonNum = 99;
		for (int i = 0; i < 36; i++) {
			//mxPoint
			if (i < 15) {
				if (distance(mousePosition, mxPoint[i]) <= 3) {
					buttonNum = i;
					break;
				}
			}
			//mfiPoint
			else if (i < 36)
				if (distance(mousePosition, mfiPoint[i - 15]) <= 3) {
				buttonNum = i;
				break;
				}
		}

		//Origin Car XY
		if (distance(mousePosition, carOrigin) <= 3) {
			buttonNum = 36;
		}

		//Origin Car Fi
		Point carFi(carOrigin.x + 8 * cosf(carOriginFi*PI / 180), carOrigin.y + 8 * sinf(carOriginFi*PI / 180));
		if (distance(mousePosition, carFi) <= 3) {
			buttonNum = 37;
		}
		//Play Pause
		if (distance(mousePosition, playPause) <= 5) {
			if (animationPlay) {
				animationPlay = false;
			}
			else
				animationPlay = true;
		}
		//fuzzyReset
		if (distance(mousePosition, reset) <= 5) {
			fuzzyReset();
			fuzzyCalculate();
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == 1 && buttonNum != 99) {
		fuzzyCalculate();
	}
}

void MouseMotion(int x, int y) {
	//Look size
	mousePosition.x = (x * lookSizeX / windowW) - windowOX;
	mousePosition.y = lookSizeY - windowOY - (y * lookSizeY / windowH);
	if (buttonNum != 99) {
		//mxPoint
		if (buttonNum < 15) {
			mxPoint[buttonNum].x = mousePosition.x;
			ivlXPosition[buttonNum] = mousePosition.x - mxO.x;
		}
		//mfiPoint
		else if (buttonNum < 36) {
			mfiPoint[buttonNum - 15].x = mousePosition.x;
			ivlAngle[buttonNum] = (mousePosition.x - mfiO.x) * 18 / 5;
		}
		else if (buttonNum == 36) {
			carOrigin.x = mousePosition.x;
			carOrigin.y = mousePosition.y;
		}
		else if (buttonNum == 37) {
			if (mousePosition.x >= carOrigin.x) {
				carOriginFi = 180 / PI*atanf((mousePosition.y - carOrigin.y) / (mousePosition.x - carOrigin.x));
			}
		}
	}
	glutPostRedisplay();
}

void CommandIO() {
	cout << "Please key in X (0~100): ";
	cin >> carOrigin.x;

	cout << "Please key in Y (0~100): ";
	cin >> carOrigin.y;

	cout << "Please key in FI (-90~120): ";
	cin >> carOriginFi;
}

void Initialize() {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowW, windowH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Fuzzy OpenGL GUI");
	//Look size
	gluOrtho2D(-windowOX, lookSizeX - windowOX, -windowOY, lookSizeY - windowOY);
	glClearColor(1, 1, 1, 1);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMotion);
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutTimerFunc(animationDelay, AnimationTimer, 1);
}

int main() {
	Initialize();
	fuzzyReset();
	CommandIO();
	fuzzyCalculate();
	glutMainLoop();
	return 0;
}