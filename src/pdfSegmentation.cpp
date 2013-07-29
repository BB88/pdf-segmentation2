//============================================================================
// Name        : pdfSegmentation.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>

#include <vector>
#include "tinyxml.h"

#include "bitmap_image.hpp"
#include <stdio.h>


#include <algorithm>
#include <cstdio>
#include <sstream>
#include <math.h>



using namespace std;

/*void figure_found (TiXmlDocument doc){
	TiXmlNode* node = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;
	node = doc.FirstChild( "pages" );
	assert( node );
	todoElement = node->ToElement();
	assert( todoElement  );
	node = todoElement->FirstChildElement();        // This skips the "PDA" comment.
	assert( node );
	itemElement = node->FirstChild("figure")->ToElement();
	const char *c = itemElement->Attribute("name");
	if( c != "name" ){
		cout << "mani"  << endl;
	  }
}*/

bool scan_page (  TiXmlNode* node, bitmap_image image, int i ); // i e' il numero di pagina

bool scan_page (  TiXmlNode* node, bitmap_image image, int i  ){


	TiXmlElement* itemElement = 0;
    int k = 0;
    bool f2 = true;
    image_drawer draw(image);
    while ( f2 ){
    	if ( k == 0 ){
        	itemElement = node-> FirstChildElement("textbox");
        }
        else {
			itemElement = itemElement->NextSiblingElement("textbox");
        }
        if ( itemElement == 0 ) { f2=false; }
        else {

			const char *co = itemElement->Attribute("bbox");
		//	cout << co << endl; 			// stampa le coordinate della bbox
			char * test;
			double coord[4];
			int l = 0;
			test = strtok ((char*)co,",");
			while ( test != NULL ){					// ciclo per separare le coordinate
				coord[l] = atof(test);
				test = strtok (NULL, ",");
				++l;
			}

			/*int a = int(coord[0])+25;
			int b = int(coord[1])+25;
			int c = int(coord[2])+25;
			int d = int(coord[3])+25;*/


			draw.rectangle(int(coord[0])+25,792-int(coord[1]),int(coord[2])+25,792-int(coord[3]));
			++k;
        }
        if ( itemElement->NextSiblingElement("textbox") == 0 ) { f2=false; }
    }
    std::stringstream name_out_ss;
	name_out_ss << "/home/bene/Scrivania/xml_prove/prova-"<<i<<"_draw.bmp";
	image.save_image(name_out_ss.str());
    return true;
}

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	TiXmlDocument doc;
	if(!doc.LoadFile("/home/bene/Scrivania/xml_prove/esempio.xml")){
	cerr << doc.ErrorDesc() << endl;
	return 0;
	}
	TiXmlNode* node = 0;
	node = doc.FirstChildElement( "pages" );
	assert( node );
	int i = 0;
	bool f1 = true;
	while ( f1 ) {

		std::stringstream name_ss;
		name_ss << "/home/bene/Scrivania/xml_prove/prova-"<<i<<".bmp";
		bitmap_image image(name_ss.str());
		if ( i == 0 ) {
			node = node->FirstChildElement("page");
		}
		else {
			node = node->NextSiblingElement("page");
		}
		if ( node == 0 ) { f1 = false; }
		else {
			scan_page ( node, image, i );
			++i;
		}
		if ( node->NextSibling("page") == 0 ){ f1=false; }

	}



	  /*std::cout << "!!!Hello World!!!" << std::endl;
	  string file_name("/home/bene/workspace/pdfSegmentation/prova-0.bmp");
	  bitmap_image image(file_name);
	  image.vertical_flip();
	  image_drawer draw(image);
	  draw.rectangle(int(coord[0]+1),int(coord[1]+1),int(coord[2]+1),int(coord[3]+1));
	  image.save_image("/home/bene/Scrivania/prova-0_draw.bmp");

	  figure_found(doc);*/

    return 0;
}




