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
			coord[l] = atof(test);
			test = strtok (NULL, ",");
			++l;
		}
	return coord;
}



bool scan_page (  TiXmlNode* node, bitmap_image image, int i, int y );

bool scan_page (  TiXmlNode* node, bitmap_image image, int i, int y ){


	std::stringstream name_out_ss;
	name_out_ss << "../progettoTBD/prova-"<<i<<"_draw2.bmp";
	TiXmlElement* itemElement = 0;
	TiXmlElement* lineElement=0;
	TiXmlElement* nextline=0;

	int k=0;
	bool f2=true;


	image_drawer draw(image);
	while (f2){

		if (k==0){	itemElement = node-> FirstChildElement("textbox"); }
		else {itemElement = itemElement->NextSiblingElement("textbox");}

		if (itemElement==0) {f2=false;}
		else {

				lineElement = itemElement->FirstChildElement("textline");
				bool f3=true;

				double *coord=get_coord(itemElement);
				int a=int(coord[0])+25;
				int b=y-int(coord[1]);
				int c =int(coord[2])+25;
				int	d=y-int(coord[3]);

				draw.rectangle(a,b,c,d);
				if (lineElement!=0){
					while (f3){

						nextline=lineElement->NextSiblingElement("textline");
						if (nextline == 0 ) {f3=false;}
						else {
							int l1=atof(lineElement->FirstChildElement("text")->Attribute("size"));
							int l2=atof(nextline->FirstChildElement("text")->Attribute("size"));
							if(l1!=l2 ){
							double* co=get_coord(nextline);
							d=792-int(co[3]);
							draw.rectangle(a,b,c,d);
							b=int(co[1])+25;

							}
							lineElement=nextline;
							//if (nextline->NextSiblingElement("textline")==0){f3=false;}
						}

				  }
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

	if(!doc.LoadFile("../progettoTBD/xml/salt.xml"))
	{
	    cerr << doc.ErrorDesc() << endl;
	    return 0;
	}

	TiXmlNode* node = 0;


	node = doc.FirstChildElement( "pages" );
	assert( node );

	TiXmlElement* Element = 0;
	Element = node-> FirstChildElement("page");
	double *y_coord=get_coord(Element);
	int y= y_coord[3];

int i=0;
bool f1=true;

while (f1){

		std::stringstream name_ss;

		name_ss <<"../progettoTBD/bmp/salt/salt-"<<i<<".bmp";
		bitmap_image image(name_ss.str());

		if (i==0) {	node = node->FirstChildElement("page");}
		else {node = node->NextSiblingElement("page");}

		if (node==0 ) {f1=false;}

		else {

		scan_page (node,image,i,y);
		++i;
		}
		if  (node->NextSiblingElement("page")==0){f1=false;}
	}
	return 0;
}
