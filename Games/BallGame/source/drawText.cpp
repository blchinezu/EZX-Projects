/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"


SDL_Rect TextDstRect;
SDL_Rect TextSrcRect;



int TextIndex;

void Print(int left, int top, char *str)
{
	int pos = 0;
	while (*str)
	{
		DrawChar(left + pos, top, *str++);
		pos += 8;
	}

}


void DrawChar(int left, int top, char letter)
{
TextSrcRect.x = 0;		TextSrcRect.y = 0+216;
TextDstRect.x = left;	TextDstRect.y = top;
TextSrcRect.w = 8;		TextSrcRect.h = 8;
TextDstRect.w = 8;		TextDstRect.h = 8;
if(letter == ' ')				  {TextSrcRect.x = 0;	TextSrcRect.y = 0+216;}
if(letter == 'a' || letter == 'A'){TextSrcRect.x = 8;	TextSrcRect.y = 0+216;}
if(letter == 'b' || letter == 'B'){TextSrcRect.x = 16;	TextSrcRect.y = 0+216;}
if(letter == 'c' || letter == 'C'){TextSrcRect.x = 24;	TextSrcRect.y = 0+216;}
if(letter == 'd' || letter == 'D'){TextSrcRect.x = 32;	TextSrcRect.y = 0+216;}
if(letter == 'e' || letter == 'E'){TextSrcRect.x = 40;	TextSrcRect.y = 0+216;}
if(letter == 'f' || letter == 'F'){TextSrcRect.x = 48;	TextSrcRect.y = 0+216;}
if(letter == 'g' || letter == 'G'){TextSrcRect.x = 56;	TextSrcRect.y = 0+216;}
if(letter == 'h' || letter == 'H'){TextSrcRect.x = 64;	TextSrcRect.y = 0+216;}
if(letter == 'i' || letter == 'I'){TextSrcRect.x = 72;	TextSrcRect.y = 0+216;}
if(letter == 'j' || letter == 'J'){TextSrcRect.x = 80;	TextSrcRect.y = 0+216;}
if(letter == 'k' || letter == 'K'){TextSrcRect.x = 88;	TextSrcRect.y = 0+216;}
if(letter == 'l' || letter == 'L'){TextSrcRect.x = 96;	TextSrcRect.y = 0+216;}
if(letter == 'm' || letter == 'M'){TextSrcRect.x = 0;	TextSrcRect.y = 8+216;}
if(letter == 'n' || letter == 'N'){TextSrcRect.x = 8;	TextSrcRect.y = 8+216;}
if(letter == 'o' || letter == 'O'){TextSrcRect.x = 16;	TextSrcRect.y = 8+216;}
if(letter == 'p' || letter == 'P'){TextSrcRect.x = 24;	TextSrcRect.y = 8+216;}
if(letter == 'q' || letter == 'Q'){TextSrcRect.x = 32;	TextSrcRect.y = 8+216;}
if(letter == 'r' || letter == 'R'){TextSrcRect.x = 40;	TextSrcRect.y = 8+216;}
if(letter == 's' || letter == 'S'){TextSrcRect.x = 48;	TextSrcRect.y = 8+216;}
if(letter == 't' || letter == 'T'){TextSrcRect.x = 56;	TextSrcRect.y = 8+216;}
if(letter == 'u' || letter == 'U'){TextSrcRect.x = 64;	TextSrcRect.y = 8+216;}
if(letter == 'v' || letter == 'V'){TextSrcRect.x = 72;	TextSrcRect.y = 8+216;}
if(letter == 'w' || letter == 'W'){TextSrcRect.x = 80;	TextSrcRect.y = 8+216;}
if(letter == 'x' || letter == 'X'){TextSrcRect.x = 88;	TextSrcRect.y = 8+216;}
if(letter == 'y' || letter == 'Y'){TextSrcRect.x = 96;	TextSrcRect.y = 8+216;}
if(letter == 'z' || letter == 'Z'){TextSrcRect.x = 104;	TextSrcRect.y = 8+216;}

if(letter == '0')				  {TextSrcRect.x = 0;	TextSrcRect.y = 16+216;}
if(letter == '1')				  {TextSrcRect.x = 8;	TextSrcRect.y = 16+216;}
if(letter == '2')				  {TextSrcRect.x = 16;	TextSrcRect.y = 16+216;}
if(letter == '3')				  {TextSrcRect.x = 24;	TextSrcRect.y = 16+216;}
if(letter == '4')				  {TextSrcRect.x = 32;	TextSrcRect.y = 16+216;}
if(letter == '5')				  {TextSrcRect.x = 40;	TextSrcRect.y = 16+216;}
if(letter == '6')				  {TextSrcRect.x = 48;	TextSrcRect.y = 16+216;}
if(letter == '7')				  {TextSrcRect.x = 56;	TextSrcRect.y = 16+216;}
if(letter == '8')				  {TextSrcRect.x = 64;	TextSrcRect.y = 16+216;}
if(letter == '9')			      {TextSrcRect.x = 72;	TextSrcRect.y = 16+216;}
if(letter == '@')			      {TextSrcRect.x = 80;	TextSrcRect.y = 16+216;}
if(letter == '?')			      {TextSrcRect.x = 88;	TextSrcRect.y = 16+216;}
if(letter == '!')			      {TextSrcRect.x = 96;	TextSrcRect.y = 16+216;}
if(letter == '=')			      {TextSrcRect.x = 104;	TextSrcRect.y = 16+216;}
if(letter == ':')			      {TextSrcRect.x = 112;	TextSrcRect.y = 16+216;}
if(letter == '+')			      {TextSrcRect.x = 120;	TextSrcRect.y = 16+216;}
if(letter == ',')			      {TextSrcRect.x = 128;	TextSrcRect.y = 16+216;}
if(letter == '.')			      {TextSrcRect.x = 112;	TextSrcRect.y = 8+216;}
if(letter == '*')			      {TextSrcRect.x = 136;	TextSrcRect.y = 16+216;}
if(letter == '-')			      {TextSrcRect.x = 144;	TextSrcRect.y = 16+216;}
if(letter == '_')			      {TextSrcRect.x = 152;	TextSrcRect.y = 16+216;}
if(letter == '(')			      {TextSrcRect.x = 120;	TextSrcRect.y = 8+216;}
if(letter == ')')			      {TextSrcRect.x = 128;	TextSrcRect.y = 8+216;}
if(letter == '<')			      {TextSrcRect.x = 136;	TextSrcRect.y = 8+216;}
if(letter == '>')			      {TextSrcRect.x = 144;	TextSrcRect.y = 8+216;}
if(letter == '%')			      {TextSrcRect.x = 152;	TextSrcRect.y = 8+216;}

SDL_BlitSurface(SpritesSurface,&TextSrcRect,DisplaySurface,&TextDstRect);
letter = ' ';

}
