//============================================================================
// Name        : prova.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <algorithm>
#include <cstdio>
#include <sstream>

#include <iostream>
#include "tinyxml.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <math.h>

#include "bitmap_image.hpp"



using namespace std;



double* get_coord(TiXmlElement* itemElement);
double* get_coord(TiXmlElement* itemElement){

	const char *co = itemElement->Attribute("bbox");
	char * test;
	double coord[4];
	int l=0;
	test = strtok ((char*)co,",");
	while (test != NULL)
		{
			coord[l]=atof(test);
			test= strtok (NULL, ",");
			++l;
		}
	return coord;
}



bool scan_page (  TiXmlNode* node, bitmap_image image, int i );

bool scan_page (  TiXmlNode* node, bitmap_image image, int i  ){


	std::stringstream name_out_ss;
	name_out_ss << "/home/bene/Scrivania/prova-"<<i<<"_draw2.bmp";
	TiXmlElement* itemElement = 0;
	TiXmlElement* lineElement=0;
	TiXmlElement* nextline=0;

	int k=0;
	bool f2=true;
	int a,b,c,d;
	image_drawer draw(image);
	while (f2){

		if (k==0){	itemElement = node-> FirstChildElement("textbox"); }
		else {itemElement = itemElement->NextSiblingElement("textbox");}

		if (itemElement==0) {f2=false;}
		else {

				lineElement=itemElement->FirstChildElement("textline");
				bool f3=true;

				double *coord=get_coord(itemElement);
				a=int(coord[0])+25;
				b=792-int(coord[1]);
				c =int(coord[2])+25;
				d=792-int(coord[3]);
				draw.rectangle(a,b,c,d);

				while (f3){

					nextline=lineElement->NextSiblingElement("textline");
					if (nextline == 0 ) {f3=false;}
					else if(atof(lineElement->FirstChildElement("text")->Attribute("size")) != atof(nextline->FirstChildElement("text")->Attribute("size"))){
						double *co=get_coord(lineElement);
						d=792-int(co[3]);
						draw.rectangle(a,b,c,d);
						b=int(co[2])+25;

					}
					lineElement=nextline;
					//if (nextline->NextSiblingElement("textline")==0){f3=false;}
					//image.save_image(name_out_ss.str());

				}
				++k;
		}

		if (itemElement->NextSibling("textbox")==0){f2=false;}
		}
	image.save_image(name_out_ss.str());
	return true;
}


int main() {



	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	TiXmlDocument doc;
	if(!doc.LoadFile("/home/bene/Scrivania/esempio.xml"))
	{
	    cerr << doc.ErrorDesc() << endl;
	    return 0;
	}

	TiXmlNode* node = 0;


	node = doc.FirstChildElement( "pages" );
	assert( node );


int i=0;
bool f1=true;

while (f1){

		std::stringstream name_ss;
		name_ss << "/home/bene/Scrivania/prova-"<<i<<".bmp";
		bitmap_image image(name_ss.str());

		if (i==0) {	node = node->FirstChildElement("page");}
		else {node = node->NextSiblingElement("page");}

		if (node==0 ) {f1=false;}

		else {

		scan_page (node,image,i);
		++i;
		}
		if  (node->NextSiblingElement("page")==0){f1=false;}
	}
	return 0;
}
