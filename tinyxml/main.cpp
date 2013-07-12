#include <stdio.h>
#include "tinyxml.h"
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{

	int written=0;
	char toHtml[1000];

	TiXmlDocument doc("app.xml" );
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load xml file. Error='%s'. Exiting.\n", doc.ErrorDesc() );
		exit( 1 );
	}
	TiXmlHandle docH( &doc );

	//解析xml

	TiXmlNode* node = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	itemElement = docH.FirstChildElement( "Production" ).FirstChildElement().ToElement();
	//assert( todoElement  );
	//written += _snprintf(toHtml,sizeof(toHtml),"<!DOCTYPE html><html><head><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/><style type=\"text/css\">input,body {FONT-SIZE:20px;}</style><title>abcd</title></head><body>");
	written += _snprintf(toHtml,sizeof(toHtml),"<!DOCTYPE html><html><head><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/><style type=\"text/css\">input,body {FONT-SIZE:20px;}</style><title>abcd</title></head><body>");
	//node = todoElement->FirstChildElement();
	//assert( node );
	//itemElement = node->ToElement();
	while(itemElement!= NULL){
		int x=0,y=0;
		const char *font,*fontsize,*text;
		itemElement->Attribute( "x" ,&x);
		itemElement->Attribute( "y" ,&y);
		font = itemElement->Attribute( "font");
		fontsize = itemElement->Attribute( "fontsize");
		text = itemElement->GetText();

		if(font == NULL || strlen(font) == 0)
			font = "arial";
		if(fontsize == NULL || strlen(fontsize) == 0)
			fontsize = "20px";
		if(text == NULL)
			text = "";
		printf("font=%s,strlen(font)=%d\n",font,strlen(font) );
		//if(itemElement->Attribute("style")!=NULL&&strcmp(itemElement->Attribute("alt"),"img")==0){
		if(0){
		written += _snprintf(toHtml+written,sizeof(toHtml)-written,"<div style=\"position:absolute;left:%dpx;top:%dpx;\"><img src=\"%s\"/></div>",
			x,y,text
			);}
		else{//written += _snprintf(toHtml+written,sizeof(toHtml)-written,"<div style=\"position:absolute;left:%dpx;top:%dpx;\">%s:%s</div>",
			written += _snprintf(toHtml+written,sizeof(toHtml)-written,"<div style=\"position:absolute;left:%dpx;top:%dpx;font:%s;font-size:%s;\">%s</div>",
			x,y,
			//itemElement->Attribute( "name" ),
			font,
			fontsize,
			text
			);
			}
		itemElement = itemElement->NextSiblingElement();
	}
	written += _snprintf(toHtml+written,sizeof(toHtml)-written,"</body></html>");
	printf(toHtml);
	//FILE* saved  = fopen( "utf8testout.xml", "w" );
	//fwrite(toHtml,sizeof(toHtml),sizeof(toHtml),saved);
	//fclose(saved);
	TiXmlDocument outDoc( "book.html" );
	outDoc.Parse(toHtml);

	if ( outDoc.Error() )
	{
		printf( "create html error in %s: %s\n", outDoc.Value(), outDoc.ErrorDesc() );
		exit( 1 );
	}
	outDoc.SaveFile();

	return 0;
}
