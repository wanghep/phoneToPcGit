#pragma  once 

////////////////////////////UIBC  struct

struct mouse_item
{
	char t;
	short x;
	short y;
};

struct mouse_
{
	char N;
	mouse_item mi[1];
};

struct key_
{
	char reserved;
	short key1;
	short key2;
};
/*struct zoom_
{
short x;
short y;
char integer;
char fraction;
};*/



/*
0  Left Mouse Down/Touch Down
1  Left Mouse Up/Touch Up
2  Mouse Move/Touch Move
3  Key Down
4  Key Up
5  Zoom
6  Vertical Scroll
7  Horizontal Scroll
8  Rotate
9-255  Reserved
*/
//
#define uibcg_MD	 0
#define uibcg_MU	 1
#define uibcg_MM	 2
#define uibcg_KD	 3
#define uibcg_KU	 4
#define uibcg_ZM	 5
#define uibcg_VS	 6
#define uibcg_HS	 7
#define uibcg_RC	 8

struct UIBC_GHEAD{
	char ID;//use uibdg_LD ...
	short length;
};


struct UIBC_G{
	UIBC_GHEAD head;
	union
	{
		mouse_		ms;
		key_			key;
		//zoom_		zoom;
	};
};

struct UIBC_HIDC{
	//
};

#define  gategory_CG 0
#define  gategory_CH 1
struct UIBC_STRUCTHEAD
{
	unsigned short version : 3;
	unsigned short T : 1;
	unsigned short reserved : 8;
	unsigned short gategory : 4;
	unsigned short lenghth;
	unsigned short timestamop;
};
struct UIBC_STRUCT
{
	UIBC_STRUCTHEAD head;
	//unsigned short timestamop;
	union
	{
		UIBC_G ug;
		UIBC_HIDC hc;
	};
};



int wrtiekey(void * pvdata, int key0, int isdown);
int writemouse_touch(void * pvdata, int n, int x, int y, int isdown);
int writemouse_move(void * pvdata, int n, int x, int y);