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


void build_doc( );
void build_doc( ){

	// Make xml:
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	TiXmlElement * element = new TiXmlElement( "pages" );
	doc.LinkEndChild( decl );
	doc.LinkEndChild( element );
	doc.SaveFile( "/home/miky/Scrivania/prova_right.xml" );
}

bool add_page( TiXmlElement* page );
bool add_page( TiXmlElement* page ){

		TiXmlDocument doc;
		if(!doc.LoadFile("/home/miky/Scrivania/prova_right.xml")){
			cerr << doc.ErrorDesc() << endl;
			return 0;
		}
		TiXmlElement* node = 0;
		node = doc.FirstChildElement( "pages" );
		assert( node );
		TiXmlElement* new_page = 0;
		new_page = new TiXmlElement( "page" );
		const char *id = page->Attribute("id");
		const char *bbox = page->Attribute("bbox");
		const char *rotate = page->Attribute("rotate");
		new_page->SetAttribute("id",id);
		new_page->SetAttribute("bbox",bbox);
		new_page->SetAttribute("rotate",rotate);
		node->LinkEndChild( new_page );
		doc.SaveFile();
		return true;
}

bool add_rect( int i, int a, int b, int c, int d );
bool add_rect( int i, int a, int b, int c, int d ){

	TiXmlDocument doc;
	if(!doc.LoadFile("/home/miky/Scrivania/prova_right.xml")){
		cerr << doc.ErrorDesc() << endl;
		return 0;
	}
	int j = 0;
	TiXmlNode* node = 0;
	node = doc.FirstChildElement( "pages" );
	assert( node );
	TiXmlElement* elem = 0;
	elem = node-> FirstChildElement("page");
	while ( j < i ){
		elem = elem->NextSiblingElement("page");
		j++;
	}
	TiXmlElement* rect = 0;
	rect = new TiXmlElement( "rect" );
	stringstream strs;
	strs << a<<","<<b<<","<<c<<","<<d;
	string temp_str = strs.str();
	const char *bbox = (char*) temp_str.c_str();
	rect->SetAttribute("bbox",bbox);
	elem->LinkEndChild( rect );
	doc.SaveFile();
	return true;
}

bool scan_page ( int n_page, TiXmlNode* node, bitmap_image image, int i, int y );
bool scan_page ( int n_page, TiXmlNode* node, bitmap_image image, int i, int y ){

	std::stringstream name_out_ss;
	name_out_ss << "../progettoTBD/prova_lettera-"<<i<<".bmp";
	TiXmlElement* itemElement = 0;

	TiXmlElement* lineElement = 0;
	TiXmlElement* nextline = 0;


	int k = 0;
	bool f2 = true;


	image_drawer draw(image);

	while (f2){

		if ( k == 0 ){	itemElement = node-> FirstChildElement("textbox"); }
		else { itemElement = itemElement->NextSiblingElement("textbox"); }

		if ( itemElement == 0 ) { f2 = false; }
		else {
			lineElement = itemElement->FirstChildElement("textline");


				double *coord=get_coord(itemElement);
				int a=int(coord[0])+25;
				int b=y-int(coord[1]);
				int c=int(coord[2])+25;
				int	d=y-int(coord[3]);
				int	last=d;
				int limit = c-((c-a)/5);

		//		draw.rectangle(a,b,c,d);

                //togliere questo per tagliare solo i bbox, con taglia per grandezza e carattere
				bool f3=true;
				bool drawn=false;
				int c_line;

				while (f3)
				{

					nextline=lineElement->NextSiblingElement("textline");
					if (nextline == 0 ) {f3=false;}
					else {
							int l1=atof(lineElement->FirstChildElement("text")->Attribute("size"));
							int l2=atof(nextline->FirstChildElement("text")->Attribute("size"));
							std::string font1 = lineElement->FirstChildElement("text")->Attribute("font");
							std::string font2 = nextline->FirstChildElement("text")->Attribute("font");

							if(l1!=l2 || font1!=font2){
								    double* next_coord=get_coord(nextline);
									d=y-int(next_coord[3]);
									draw.rectangle(a,b,c,d);
									add_rect(n_page, a, b, c, d);
									b=d;
									drawn=true;
							}
//							else{
//							double *line_co=get_coord(lineElement);
//							c_line=int(line_co[2])+25;
//
//							if(c_line<limit)
//								{
//									double* next_coord=get_coord(nextline);
//									d=y-int(next_coord[3]);
//									draw.rectangle(a,b,c,d);
//									b=d;
//								    drawn=true;
//								}
//							}
							lineElement=nextline;
							if (nextline->NextSiblingElement("textline")==0){f3=false;}

							}

			  }
				if(drawn==false){
					draw.rectangle(a,b,c,d);
					add_rect(n_page, a, b, c, d);}
				else{draw.rectangle(a,b,c,last);
				add_rect(n_page, a, b, c, last);}
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
	if(!doc.LoadFile("../progettoTBD/xml/prova.xml")){
		cerr << doc.ErrorDesc() << endl;
	    return 0;
	}
	build_doc();	// nuovo xml
	TiXmlNode* node = 0;
	node = doc.FirstChildElement( "pages" );
	assert( node );
	TiXmlElement* Element = 0;
	Element = node-> FirstChildElement("page");
	add_page(Element);
	int n_page = 0;
	double *y_coord = get_coord(Element);
	int y = y_coord[3];


	int i = 0;
	bool f1 = true;

	while ( f1 ){
		std::stringstream name_ss;
		name_ss <<"../progettoTBD/bmp/prova/prova-"<<i<<".bmp";
		bitmap_image image( name_ss.str() );
		if ( i == 0 ) {	node = node->FirstChildElement("page");	}
		else {
			node = node->NextSiblingElement("page");
			add_page( node->ToElement() );
			n_page ++;
		}
		if ( node == 0 ) { f1 = false; }
		else {
			scan_page ( n_page, node, image, i, y );
			++i;
		}
		if  ( node->NextSiblingElement("page") == 0 ){ f1 = false; }
	}
	return 0;
}
