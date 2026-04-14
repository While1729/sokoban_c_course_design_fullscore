#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 12
#define HEIGHT 10 
#define MAX_LEVELS 10

#define BLANK 0           // 空地 
#define WALL 1            // 墙 
#define DEST 2            // 目标点 
#define BOX 3             // 箱子 
#define PLAYER 4          // 玩家 
#define BOX_ON_DEST 5     // 箱子在目标点上 
#define OTHER 6           // 地图外的方块 
#define PLAYER_ON_DEST 7  // 玩家在目标点上 

struct Stack
{
    int value[HEIGHT][WIDTH];
    struct Stack *prev;
    struct Stack *next;
};
struct Stack *head;
struct Stack *p = NULL;
struct Stack *prevpoint = NULL;
FILE *fp;
char pass[10] = {'2','0','0','0','0','0','0','0','0','0'};
char restart[10] = {'2','0','0','0','0','0','0','0','0','0'};
int cas = 0;
int next_level = 1;
int exit_menu_flag = 0;
int level_over_flag = 0;
int back_menu_flag = 0;
int restart_flag = 0;
int sound_effect_flag = 1;
int exit_game_flag = IDNO;
int bgm_playing = 0;
int bgm_volume = 80;
int sound_effect_volume = 90;
IMAGE img[9];

HWND hwnd = NULL;
bool isWindowActive;

int map[MAX_LEVELS][HEIGHT][WIDTH] =
{
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,1,1,1,1,1,1,1,1,6,6,
    6,6,1,0,0,0,0,3,2,1,6,6,
    6,6,1,0,2,3,0,0,0,1,6,6,
    6,6,1,0,0,4,0,0,0,1,6,6,
    6,6,1,0,1,0,1,1,0,1,6,6,
    6,6,1,0,1,0,1,1,0,1,6,6,
    6,6,1,2,3,0,0,3,2,1,6,6,
    6,6,1,1,1,1,1,1,1,1,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,

    6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,1,1,1,1,1,1,1,1,6,6,
    6,6,1,2,3,0,0,3,2,1,6,6,
    6,6,1,0,2,3,0,0,0,1,6,6,
    6,6,1,0,0,4,0,0,0,1,6,6,
    6,6,1,0,1,0,1,1,0,1,6,6,
    6,6,1,0,1,0,1,1,0,1,6,6,
    6,6,1,2,3,0,0,3,2,1,6,6,
    6,6,1,1,1,1,1,1,1,1,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,

    6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,1,1,1,1,1,1,1,1,6,6,
    6,6,1,2,3,0,0,3,2,1,6,6,
    6,6,1,0,2,3,0,0,0,1,6,6,
    6,6,1,0,0,4,0,0,0,1,6,6,
    6,6,1,3,1,0,1,1,0,1,6,6,
    6,6,1,2,1,0,1,1,0,1,6,6,
    6,6,1,2,3,0,0,3,2,1,6,6,
    6,6,1,1,1,1,1,1,1,1,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,
    
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,1,1,1,1,1,1,1,6,6,6,
    6,6,1,0,2,2,3,0,1,6,6,6,
    6,6,1,0,1,0,3,0,1,6,6,6,
    6,6,1,0,1,0,1,0,1,6,6,6,
    6,6,1,0,3,4,1,0,1,6,6,6,
    6,6,1,2,3,0,0,0,1,6,6,6,
    6,6,1,2,1,1,1,1,1,6,6,6,
    6,6,1,1,1,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,
    
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,1,1,1,1,6,6,6,6,6,
    6,1,1,1,0,0,1,1,1,1,6,6,
    6,1,0,0,0,0,0,3,0,1,6,6,
    6,1,0,1,0,0,1,3,0,1,6,6,
    6,1,0,2,0,2,1,4,0,1,6,6,
    6,1,1,1,1,1,1,1,1,1,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,
    
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,1,1,1,1,1,6,6,6,6,
    1,1,1,1,0,0,0,1,6,6,6,6,
    1,2,2,1,3,3,0,1,6,6,6,6,
    1,0,2,2,3,0,0,1,1,1,1,1,
    1,0,4,1,0,1,0,1,0,0,0,1,
    1,0,0,3,0,0,0,0,0,3,0,1,
    1,0,0,1,1,1,1,1,1,1,2,1,
    1,1,1,1,6,6,6,6,6,1,1,1,
    6,6,6,6,6,6,6,6,6,6,6,6,
    
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,1,1,1,1,1,1,1,1,6,6,6,
	6,1,0,0,1,1,0,0,1,1,1,6,
	6,1,0,0,0,0,0,3,0,0,1,6,
	6,1,2,2,1,1,4,3,0,0,1,6,
	6,1,2,2,1,1,0,3,0,0,1,6,
	6,1,0,0,0,0,0,3,0,0,1,6,
	6,1,0,0,1,1,0,0,1,1,1,6,
	6,1,1,1,1,1,1,1,1,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,
    
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,1,1,1,1,1,1,1,1,1,1,6,
    6,1,0,0,0,0,0,0,0,0,1,6,
    6,1,0,0,3,0,3,0,3,0,1,6,
    6,1,1,4,1,1,1,1,1,0,1,6,
    6,1,0,0,0,2,0,2,0,2,1,6,
    6,1,0,0,0,0,0,0,0,0,1,6,
    6,1,1,1,1,1,1,1,1,1,1,6,
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,
    
    6,6,6,6,6,6,6,6,6,6,6,6,
    6,1,1,1,1,1,1,1,1,1,1,6,
    6,1,0,0,0,0,0,1,2,0,1,6,
    6,1,0,0,3,0,0,0,2,2,1,6,
    6,1,0,0,0,0,1,0,0,2,1,6,
    6,1,1,1,1,0,1,1,1,1,1,6,
    6,1,0,3,0,3,0,3,0,4,1,6,
    6,1,0,0,0,0,0,0,0,0,1,6,
    6,1,1,1,1,1,1,1,1,1,1,6,
    6,6,6,6,6,6,6,6,6,6,6,6,
    
    6,6,6,6,6,6,6,6,6,6,6,6,
    1,1,1,1,1,1,1,1,1,1,1,6,
    1,0,0,2,1,0,0,1,0,0,1,6,
    1,2,2,2,2,0,3,1,3,0,1,6,
    1,0,0,2,1,0,0,0,0,0,1,6,
    1,1,1,1,1,0,0,1,0,0,1,6,
    1,0,0,3,0,0,3,3,3,0,1,6,
    1,0,0,0,0,4,0,1,0,0,1,6,
    1,1,1,1,1,1,1,1,1,1,1,6,
    6,6,6,6,6,6,6,6,6,6,6,6,
};    //游戏地图 


//LOAD_RESOURCE()函数声明 
void LOAD_RESOURCE();

//声音模块函数声明 
void PLAY_BOX_TO_DEST_SOUND();
void PLAY_LEVEL_COMPLETE_SOUND();
void INIT_BGM(const char* filename);
void SET_BGM_VOLUME(int volume);
void SET_SOUND_EFFECT_VOLUME(int volume);
void DRAW_MUSIC_CONTROL_UI();
void HANDLE_VOLUME_BAR_CLICK(int x, int y);
void PLAY_BGM();
void PAUSE_BGM();
void BEGIN_SOUND_EFFECT();
void STOP_SOUND_EFFECT();
int HANDLE_BUTTON_CLICK(int x, int y);
void CLOSE_BGM();

//主菜单模块函数声明 
void DRAW_MENU_UI();
void MENU_KEY_DOWN();
int CHOICE(int x, int y);

//游戏循环模块函数声明(其中包含控制模块) 
void DRAW_GRAPH();
void INIT_LINK();
void DELETE_LIST(struct Stack *HEAD);
void RECOVER_LEVEL();
void RESET_LEVEL();
void LOAD_LEVEL(int level);
void NEXT_LEVEL();
void RE_START();
int LEVEL_OVER();

//控制模块函数声明(包含在游戏循环模块中) 
int SEARCH_I();
int SEARCH_J();
void MOVE_PLAYER(int di, int dj);
void KEY_DOWN();

//游戏循环模块函数声明(其中包含控制模块) 
void GAME_LOOP();
void CLEAN_UP();


//加载图片资源 
void LOAD_RESOURCE()
{
    loadimage(img+0,"images/0.png");
    loadimage(img+1,"images/1.png");
    loadimage(img+2,"images/2.png");
    loadimage(img+3,"images/3.png");
    loadimage(img+4,"images/4.png");
    loadimage(img+5,"images/5.png");
    loadimage(img+6,"images/6.png");
	loadimage(img+7,"images/background.png");
	loadimage(img+8,"images/choice.png");
}

//----------下面的代码是声音模块----------


//播放箱子推到目标点的音效 
void PLAY_BOX_TO_DEST_SOUND()
{
    //停止之前的相同音效(避免重复播放) 
    mciSendString("close sound_box", NULL, 0, NULL);
    
    FILE* file = fopen("music/SOUND1.mp3", "r");
    if (file != NULL)
    {
        fclose(file);
        char command[256];
        sprintf(command, "open \"music/SOUND1.mp3\" alias sound_box");
        mciSendString(command, NULL, 0, NULL);
        
        //设置音效音量 
        char volume_cmd[50];
        sprintf(volume_cmd, "setaudio sound_box volume to %d",
		        sound_effect_volume * 10);
        mciSendString(volume_cmd, NULL, 0, NULL);
        
        //播放一次(不重复) 
        mciSendString("play sound_box", NULL, 0, NULL);
    }
}
//播放通过关卡的音效 
void PLAY_LEVEL_COMPLETE_SOUND()
{
    // 停止之前的相同音效 
    mciSendString("close sound_complete", NULL, 0, NULL);
    
    FILE* file = fopen("music/SOUND2.mp3", "r");
    if (file != NULL)
    {
        fclose(file);
        char command[256];
        sprintf(command, "open \"music/SOUND2.mp3\" alias sound_complete");
        mciSendString(command, NULL, 0, NULL);
        
        //设置音效音量 
        char volume_cmd[50];
        sprintf(volume_cmd, "setaudio sound_complete volume to %d",
		        sound_effect_volume * 10);
        mciSendString(volume_cmd, NULL, 0, NULL);
        
        //播放一次(不重复) 
        mciSendString("play sound_complete", NULL, 0, NULL);
    }
}
//初始化BGM 
void INIT_BGM(const char* filename)
{
    mciSendString("close bgm", NULL, 0, NULL);
    FILE* file = fopen(filename, "r");
    if (file != NULL)//找到BGM文件 
	{
        fclose(file);
        char command[256];
        sprintf(command, "open \"%s\" type mpegvideo alias bgm", filename);
        mciSendString(command, NULL, 0, NULL);
        mciSendString("play bgm repeat", NULL, 0, NULL);
        bgm_playing = 1;
    }
	else//未找到BGM文件 
	{
        bgm_playing = 0;
    }
}
//设置BGM音量 
void SET_BGM_VOLUME(int volume)
{
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    bgm_volume = volume;
    char command[50];
    sprintf(command, "setaudio bgm volume to %d", volume * 10);
    mciSendString(command, NULL, 0, NULL);
}
//设置音效音量 
void SET_SOUND_EFFECT_VOLUME(int volume)
{
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    sound_effect_volume = volume;
}
//绘制声音设置界面 
void DRAW_MUSIC_CONTROL_UI()
{
    cleardevice();

    //设置背景色 
    setbkcolor(RGB(50, 50, 70));
    cleardevice();
    
    //设置文本背景模式为透明 
	setbkmode(TRANSPARENT);

    //标题 
    settextcolor(WHITE);
    settextstyle(36, 0, _T("宋体"));
    outtextxy(300, 30, _T("音乐设置"));

    //绘制背景音乐控制区域 
    setfillcolor(RGB(70, 70, 90));
    fillrectangle(34, 100, 734, 250);
    
    //绘制音效控制区域 
    setfillcolor(RGB(70, 70, 90));
    fillrectangle(34, 300, 734, 400);

    //BGM标题 
    settextcolor(WHITE);
    settextstyle(24, 0, _T("宋体"));
    outtextxy(54, 110, _T("背景音乐"));

    //BGM播放状态指示器 
    if (bgm_playing)
	{
        setfillcolor(GREEN);
        fillcircle(54, 170, 8);
        settextcolor(WHITE);
        outtextxy(74, 160, _T("播放中"));
    }
    else
	{
        setfillcolor(RED);
        fillcircle(54, 170, 8);
        settextcolor(WHITE);
        outtextxy(74, 160, _T("已停止"));
    }
    //音效播放状态指示器 
    if (sound_effect_flag)
	{
        setfillcolor(GREEN);
        fillcircle(54, 360, 8);
        settextcolor(WHITE);
        outtextxy(74, 350, _T("开启"));
    }
    else
	{
        setfillcolor(RED);
        fillcircle(54, 360, 8);
        settextcolor(WHITE);
        outtextxy(74, 350, _T("关闭"));
    }

    setfillcolor(RGB(100, 100, 100));
    fillrectangle(200, 195, 700, 215);

    //BGM音量条前景(当前音量) 
    int volumeWidth = (700 - 200) * bgm_volume / 100;
    setfillcolor(RGB(100, 200, 100));
    fillrectangle(200, 195, 200 + volumeWidth, 215);

    outtextxy(54, 310, _T("音效"));

    //音效音量条背景 
    setfillcolor(RGB(100, 100, 100));
    fillrectangle(200, 345, 700, 365);

    //音效音量条前景 
    volumeWidth = (700 - 200) * sound_effect_volume / 100;
    setfillcolor(RGB(100, 150, 200));
    fillrectangle(200, 345, 200 + volumeWidth, 365);

    //控制按钮 
    setfillcolor(RGB(80, 120, 200));
    fillrectangle(34 , 450, 134, 500);  //播放BGM按钮 
    fillrectangle(134, 450, 234, 500);  //暂停BGM按钮 
    fillrectangle(234, 450, 334, 500);  //开启音效按钮 
    fillrectangle(334, 450, 434, 500);  //关闭音效按钮 
    fillrectangle(584, 450, 734, 500);  //退出按钮 

    settextcolor(WHITE);
    settextstyle(20, 0, _T("宋体"));
    outtextxy(47, 465, _T("播放BGM"));
    outtextxy(147, 465, _T("暂停BGM"));
    outtextxy(247, 465, _T("开启音效"));
    outtextxy(347, 465, _T("关闭音效"));
    outtextxy(620, 465, _T("退出设置"));

    //操作提示 
    settextstyle(16, 0, _T("宋体"));
    settextcolor(YELLOW);
    outtextxy(34, 520, _T("操作说明:"));
    outtextxy(34, 545, _T("1. 点击音量条可调整音量"));
    outtextxy(34, 570, _T("2. 点击按钮控制播放状态"));
}
//检查并处理音量条点击 
void HANDLE_VOLUME_BAR_CLICK(int x, int y)
{
    if (y >= 195 && y <= 215 && x >= 200 && x <= 700)
	{
        int newVolume = (x - 200) * 100 / (700 - 200);
        SET_BGM_VOLUME(newVolume);
        return;
    }
    
    if (y >= 345 && y <= 365 && x >= 200 && x <= 700)
	{
        int newVolume = (x - 200) * 100 / (700 - 200);
        SET_SOUND_EFFECT_VOLUME(newVolume);
        return;
    }
}
//播放BGM 
void PLAY_BGM()
{
    if (!bgm_playing)
	{
        mciSendString("play bgm repeat", NULL, 0, NULL);
        bgm_playing = 1;
    }
}
//暂停BGM 
void PAUSE_BGM()
{
    if (bgm_playing)
	{
        mciSendString("pause bgm", NULL, 0, NULL);
        bgm_playing = 0;
    }
}
//开启音效 
void BEGIN_SOUND_EFFECT()
{
	sound_effect_flag = 1;
}
//关闭音效 
void STOP_SOUND_EFFECT()
{
	sound_effect_flag = 0;
}
//检查并处理按钮点击 
int HANDLE_BUTTON_CLICK(int x, int y)
{
	//播放BGM 
    if (x >= 34 && x <= 134 && y >= 450 && y <= 500)
	{
        PLAY_BGM();
        return 1;
    }
    //暂停BGM 
    if (x >= 134 && x <= 234 && y >= 450 && y <= 500)
	{
        PAUSE_BGM();
        return 1;
    }
    //开启音效 
    if (x >= 234 && x <= 334 && y >= 450 && y <= 500)
	{
        BEGIN_SOUND_EFFECT();
        return 1;
    }
    //关闭音效 
    if (x >= 334 && x <= 434 && y >= 450 && y <= 500)
	{
        STOP_SOUND_EFFECT();
        return 1;
    }
    //退出音乐设置界面 
    if (x >= 584 && x <= 734 && y >= 450 && y <= 500)
	{
        return -1;
    }

    return 0;
}
//关闭BGM 
void CLOSE_BGM()
{
    mciSendString("close bgm", NULL, 0, NULL);
    bgm_playing = 0;
}
//---------------上面的代码是声音模块---------------


//---------------下面的代码是主菜单模块---------------
void DRAW_MENU_UI()
{
    //设置背景色 
	setbkcolor(RGB(50, 50, 70));
	//设置文本背景模式为透明 
	setbkmode(TRANSPARENT);
	//在背景绘制图片 
	putimage(0, 0, img+7);
	//在选择关卡按钮处绘制图片 
    for(int i=0;i<5;i++)  putimage(96 + i * 128, 64, img+8);
    for(int i=0;i<5;i++)  putimage(96 + i * 128, 192, img+8);
    //设置字体 
	settextstyle(32, 0, "宋体");
	//第一关按钮数字 
	if(pass[0] == '0')
	{
		settextcolor(BLACK);
		outtextxy(120,  80, "1");
	}else if(pass[0] == '1')
	{
		settextcolor(WHITE);
		outtextxy(120,  80, "1");
	}else if(pass[0] == '2')
	{
		settextcolor(BLUE);
		outtextxy(120,  80, "1");
	}
	//第二关按钮数字 
	if(pass[1] == '0')
	{
		settextcolor(BLACK);
        outtextxy(248,  80, "2");
	}else if(pass[1] == '1')
	{
		settextcolor(WHITE);
        outtextxy(248,  80, "2");
	}else if(pass[1] == '2')
	{
		settextcolor(BLUE);
        outtextxy(248,  80, "2");
	}
	//第三关按钮数字 
	if(pass[2] == '0')
	{
		settextcolor(BLACK);
		outtextxy(376,  80, "3");
	}else if(pass[2] == '1')
	{
		settextcolor(WHITE);
		outtextxy(376,  80, "3");
	}else if(pass[2] == '2')
	{
		settextcolor(BLUE);
		outtextxy(376,  80, "3");
	}
	//第四关按钮数字 
	if(pass[3] == '0')
	{
		settextcolor(BLACK);
		outtextxy(504,  80, "4");
	}else if(pass[3] == '1')
	{
		settextcolor(WHITE);
		outtextxy(504,  80, "4");
	}else if(pass[3] == '2')
	{
		settextcolor(BLUE);
		outtextxy(504,  80, "4");
	}
	//第五关按钮数字 
	if(pass[4] == '0')
	{
		settextcolor(BLACK);
		outtextxy(632,  80, "5");
	}else if(pass[4] == '1')
	{
		settextcolor(WHITE);
		outtextxy(632,  80, "5");
	}else if(pass[4] == '2')
	{
		settextcolor(BLUE);
		outtextxy(632,  80, "5");
	}
	//第六关按钮数字 
	if(pass[5] == '0')
	{
		settextcolor(BLACK);
		outtextxy(120, 208, "6");
	}else if(pass[5] == '1')
	{
		settextcolor(WHITE);
		outtextxy(120, 208, "6");
	}else if(pass[5] == '2')
	{
		settextcolor(BLUE);
		outtextxy(120, 208, "6");
	}
	//第七关按钮数字 
	if(pass[6] == '0')
	{
		settextcolor(BLACK);
		outtextxy(248, 208, "7");
	}else if(pass[6] == '1')
	{
		settextcolor(WHITE);
		outtextxy(248, 208, "7");
	}else if(pass[6] == '2')
	{
		settextcolor(BLUE);
		outtextxy(248, 208, "7");
	}
	//第八关按钮数字 
	if(pass[7] == '0')
	{
		settextcolor(BLACK);
		outtextxy(376, 208, "8");
	}else if(pass[7] == '1')
	{
		settextcolor(WHITE);
		outtextxy(376, 208, "8");
	}else if(pass[7] == '2')
	{
		settextcolor(BLUE);
		outtextxy(376, 208, "8");
	}
	//第九关按钮数字 
	if(pass[8] == '0')
	{
		settextcolor(BLACK);
		outtextxy(504, 208, "9");
	}else if(pass[8] == '1')
	{
		settextcolor(WHITE);
		outtextxy(504, 208, "9");
	}else if(pass[8] == '2')
	{
		settextcolor(BLUE);
		outtextxy(504, 208, "9");
	}
	//第十关按钮数字 
	if(pass[9] == '0')
	{
		settextcolor(BLACK);
		outtextxy(624, 208, "10");
	}else if(pass[9] == '1')
	{
		settextcolor(WHITE);
		outtextxy(624, 208, "10");
	}else if(pass[9] == '2')
	{
		settextcolor(BLUE);
		outtextxy(624, 208, "10");
	}
	
	//设置按钮颜色和字体颜色 
	setfillcolor(RGB(70, 70, 90));
    settextcolor(WHITE);
    //重置按钮 
    fillrectangle(480, 320, 673, 384);
    outtextxy(512, 336, _T("重置游戏"));
    //退出按钮 
    fillrectangle(480, 448, 673, 512);
    outtextxy(512, 464, _T("退出游戏"));
	
	//操作提示 
    settextstyle(24, 0, _T("黑体"));
    settextcolor(BLACK);
    outtextxy(34, 450, _T("操作说明:"));
    outtextxy(34, 475, _T("1. WASD控制人物移动"));
    outtextxy(34, 500, _T("2. 空格撤销移动"));
    outtextxy(34, 525, _T("3. R重新开始关卡"));
    outtextxy(34, 550, _T("4. B返回主菜单界面"));
    outtextxy(34, 575, _T("5. M打开音乐控制界面"));
}
// 在主菜单界面按"M"打开音乐设置界面 
void MENU_KEY_DOWN()
{
	//"M"音乐控制界面 
    if(GetAsyncKeyState('M') & 0x8000)
	{
        //音乐控制界面逻辑 
        BeginBatchDraw();

        int exitFlag = 0;
        while(!exitFlag)
		{
            DRAW_MUSIC_CONTROL_UI();
            FlushBatchDraw();

            MOUSEMSG m;
            while(MouseHit())
			{
                m = GetMouseMsg();
                if(m.uMsg == WM_LBUTTONDOWN)
				{
                    HANDLE_VOLUME_BAR_CLICK(m.x, m.y);
                    int result = HANDLE_BUTTON_CLICK(m.x, m.y);
                    if(result == -1)
					{
                        exitFlag = 1;
                    }
                }
            }
            Sleep(10);
        }
    }
}
int CHOICE(int x, int y)
{
	if(x >= 96  && x <= 160 && y >= 64  && y <= 128)  return 1;
	if(x >= 224 && x <= 288 && y >= 64  && y <= 128)  return 2;
	if(x >= 352 && x <= 416 && y >= 64  && y <= 128)  return 3;
	if(x >= 480 && x <= 544 && y >= 64  && y <= 128)  return 4;
	if(x >= 608 && x <= 672 && y >= 64  && y <= 128)  return 5;
	if(x >= 96  && x <= 160 && y >= 192 && y <= 256)  return 6;
	if(x >= 224 && x <= 288 && y >= 192 && y <= 256)  return 7;
	if(x >= 352 && x <= 416 && y >= 192 && y <= 256)  return 8;
	if(x >= 480 && x <= 544 && y >= 192 && y <= 256)  return 9;
	if(x >= 608 && x <= 672 && y >= 192 && y <= 256)  return 10;
	if(x >= 480 && x <= 673 && y >= 320 && y <= 384)  return 11;
	if(x >= 480 && x <= 673 && y >= 448 && y <= 512)  return 12;
	return 0;
} 
//----------上面的代码是主菜单模块----------


//----------下面的代码是游戏循环模块(其中包含控制模块)----------

//在图形化界面显示LOAD_RESOURCE()函数加载的图片 
void DRAW_GRAPH()
{
    cleardevice();
    for (int i = 0; i < HEIGHT; i++)
	{
        for (int j = 0; j < WIDTH; j++)
		{
            int x = j * 64;
            int y = i * 64;
            switch (map[cas][i][j])
			{
                case BLANK: putimage(x, y, img + 0); break;
                case WALL: putimage(x, y, img + 1); break;
                case DEST: putimage(x, y, img + 2); break;
                case BOX: putimage(x, y, img + 3); break;
                case PLAYER: 
                case PLAYER_ON_DEST: putimage(x, y, img + 4); break;
                case BOX_ON_DEST: putimage(x, y, img + 5); break;
                case OTHER: putimage(x, y, img + 6); break; 
            }
        }
    }
}

//初始化链表 
void INIT_LINK()
{
    struct Stack *newNode = (struct Stack *)malloc(sizeof(struct Stack));
    
    if (newNode == NULL)//内存分配失败 
	{
        exit(1);
    }
    
    //复制当前地图到新节点 
    for(int i = 0; i < HEIGHT; i++)
	{
        for(int j = 0; j < WIDTH; j++)
		{
            newNode->value[i][j] = map[cas][i][j];
        }
    }
    
    //初始化节点指针 
    newNode->prev = NULL;
    newNode->next = NULL;
    
    if(head == NULL)
	{
        //第一个节点 
        head = newNode;
        p = head;
        prevpoint = head;
    }
	else
	{
        //添加到链表末尾 
        struct Stack *temp = head;
        while(temp->next != NULL)
		{
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
        
        //更新当前指针 
        p = newNode;
        prevpoint = temp;  //prevpoint指向p的前一个节点 
    }
}
//删除链表 
void DELETE_LIST(struct Stack *HEAD)
{
    struct Stack *current = HEAD;
    struct Stack *next;
    
    while (current != NULL)
	{
        next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
    p = NULL;
    prevpoint = NULL;
}

//将关卡复原(复原map数组里的整型变量) 
void RECOVER_LEVEL()
{
    for(int i=0;i<HEIGHT;i++)
    {
    	for(int j=0;j<WIDTH;j++)
		{
		    map[cas][i][j] = head->value[i][j];
		}	
	}
}
//重置关卡函数 
void RESET_LEVEL()
{
	//恢复初始地图状态 
    RECOVER_LEVEL();
    
    //删除当前链表，重新开始 
    DELETE_LIST(head);
    
    //初始化链表 
    INIT_LINK();
    
    //刷新显示 
    DRAW_GRAPH();
}
//加载指定关卡 
void LOAD_LEVEL(int level)
{
    if (level >= 1 && level <= MAX_LEVELS)
	{
        //删除旧的链表 
        if(head != NULL)
		{
            DELETE_LIST(head);
        }
        
        cas = level - 1;
        next_level = level;
        
        //初始化新链表的第一个节点 
        INIT_LINK();
    }
}
void NEXT_LEVEL()
{
    if (next_level < MAX_LEVELS)
	{
        LOAD_LEVEL(next_level + 1);
        INIT_LINK();
        DRAW_GRAPH();
    }
}
//重置所有关卡的状态为未通关 
void RE_START()
{
	fp = fopen("gamesave/data.txt","w");
	if(fp==NULL)//打开文件失败 
	{
		exit(1);
	}
	for(int i=0;i<10;i++)  fputc(restart[i], fp);
	if(fclose(fp))//不能关闭文件 
	{
		exit(1);
	}
	fp = fopen("gamesave/data.txt","r");
	if(fp==NULL)//打开文件失败 
	{
		exit(1);
	}
	for(int i=0;i<10;i++)  pass[i] = fgetc(fp);
	if(fclose(fp))//不能关闭文件 
	{
		exit(1);
	}
}
//检查关卡是否结束 
int LEVEL_OVER()
{
    for(int i = 0; i < HEIGHT; i++)
	{
        for(int j = 0; j < WIDTH; j++)
		{
            if(map[cas][i][j] == BOX)//还有箱子未推到目标点 
			{
                return 0;
            }
        }
    }
    
    //播放关卡通过音效 
    if(sound_effect_flag)  PLAY_LEVEL_COMPLETE_SOUND();
    
    pass[cas] = '1';
    if(pass[next_level] == '0')  pass[next_level] = '2'; 
    fp = fopen("gamesave/data.txt","w");
    if(fp == NULL)//打开文件失败 
    {
    	exit(1);
	}
	for(int i=0;i<10;i++)  fputc(pass[i], fp);
	if(fclose(fp))//不能关闭文件 
	{
		exit(1);
    }
    if(cas != 9) 
    {
	    int ContinueFlag =
	            MessageBox(hwnd,"恭喜你通过本关，是否进入下一关？",
		                   "成功通关",MB_YESNO);
        if(ContinueFlag == IDYES)  return 1;//本关已通过，进入下一关 
        else if(ContinueFlag == IDNO)  return 2;//本关已通过，返回主菜单 
    }else if(cas == 9)
    {
    	MessageBox(hwnd,"恭喜你完成全部关卡，感谢游玩！",
		           "成功通关",MB_OK);
		return 2; 
	}
}

//----------下面的代码是控制模块(控制模块包含在游戏循环模块中)----------

//查找玩家所在行 
int SEARCH_I()
{
    for(int i = 0; i < HEIGHT; i++)
	{
        for(int j = 0; j < WIDTH; j++)
		{
            if(map[cas][i][j] == PLAYER || map[cas][i][j] == PLAYER_ON_DEST)
                return i;
        }
    }
    return -1;
}
//查找玩家所在列 
int SEARCH_J()
{
    for(int i = 0; i < HEIGHT; i++)
	{
        for(int j = 0; j < WIDTH; j++)
		{
            if(map[cas][i][j] == PLAYER || map[cas][i][j] == PLAYER_ON_DEST)
                return j;
        }
    }
    return -1;
}
void MOVE_PLAYER(int di, int dj)
{
    int i = SEARCH_I();
    int j = SEARCH_J();
    if(map[cas][i+di][j+dj] == BLANK ||
	   map[cas][i+di][j+dj] == DEST)
	{
        if(map[cas][i][j] == PLAYER_ON_DEST)
            map[cas][i][j] = DEST;
        else
            map[cas][i][j] = BLANK;
            
        if(map[cas][i+di][j+dj] == DEST)
            map[cas][i+di][j+dj] = PLAYER_ON_DEST;
        else
            map[cas][i+di][j+dj] = PLAYER;
            
        INIT_LINK();
    }
    else if(map[cas][i+di][j+dj] == BOX ||
	        map[cas][i+di][j+dj] == BOX_ON_DEST)
	{
        if(map[cas][i+2*di][j+2*dj] == BLANK ||
		   map[cas][i+2*di][j+2*dj] == DEST)
		{
            if(map[cas][i][j] == PLAYER_ON_DEST)
                map[cas][i][j] = DEST;
            else
                map[cas][i][j] = BLANK;
                
            if(map[cas][i+di][j+dj] == BOX_ON_DEST)
                map[cas][i+di][j+dj] = PLAYER_ON_DEST;
            else
                map[cas][i+di][j+dj] = PLAYER;
                
            if(map[cas][i+2*di][j+2*dj] == DEST)
            {
			    map[cas][i+2*di][j+2*dj] = BOX_ON_DEST;
			    //播放矿车(箱子)移动到铁轨(目标点)的音效 
			    if(sound_effect_flag)  PLAY_BOX_TO_DEST_SOUND();
		    }
            else
                map[cas][i+2*di][j+2*dj] = BOX;
                
            INIT_LINK();
        }
    }
}
//检测键盘输入，调用movePlayer函数移动人物 
void KEY_DOWN()
{
    if(GetAsyncKeyState('W') & 0x8000 ||
	    GetAsyncKeyState(VK_UP) & 0x8000)
	{
        MOVE_PLAYER(-1, 0);
        Sleep(100);
    }
    else if(GetAsyncKeyState('S') & 0x8000 ||
	        GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
        MOVE_PLAYER(1, 0);
        Sleep(100);
    }
    else if(GetAsyncKeyState('A') & 0x8000 ||
	        GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
        MOVE_PLAYER(0, -1);
        Sleep(100);
    }
    else if(GetAsyncKeyState('D') & 0x8000 ||
	        GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
        MOVE_PLAYER(0, 1);
        Sleep(100);
    }
    //空格键撤销 
    else if(GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        //确保有前一步可以撤销(不是链表头节点) 
        if(p != NULL && p->prev != NULL)
        {
            //移动到前一个节点 
            p = p->prev;
            //恢复地图状态 
            for(int i = 0; i < HEIGHT; i++)
            {
                for(int j = 0; j < WIDTH; j++)
                {
                    map[cas][i][j] = p->value[i][j];
                }
            }
            Sleep(200);
        }
    }
    //"R"重置游戏 
    else if(GetAsyncKeyState('R') & 0x8000)
	{
        RESET_LEVEL();
        Sleep(200);
    }
    //"B"返回主菜单 
    else if(GetAsyncKeyState('B') & 0x8000)
    {
    	back_menu_flag = MessageBox(hwnd,"确认返回主界面？","提示",MB_YESNO);
	}
    //"M"音乐控制界面 
    else if(GetAsyncKeyState('M') & 0x8000)
	{
        //音乐控制界面逻辑 
        
        
        BeginBatchDraw();

        int exitFlag = 0;
        while(!exitFlag)
		{
            DRAW_MUSIC_CONTROL_UI();
            FlushBatchDraw();

            MOUSEMSG m;
            while(MouseHit())
			{
                m = GetMouseMsg();
                if(m.uMsg == WM_LBUTTONDOWN)
				{
                    HANDLE_VOLUME_BAR_CLICK(m.x, m.y);
                    int result = HANDLE_BUTTON_CLICK(m.x, m.y);
                    if(result == -1)
					{
                        exitFlag = 1;
                    }
                }
            }
            Sleep(10);
        }
    }
}

//---------------上面的代码是控制模块(控制模块包含在游戏循环模块中)----------

void GAME_LOOP()
{
    while(1)
	{
		isWindowActive = (GetForegroundWindow() == hwnd);
		if(isWindowActive)  KEY_DOWN();//只有在游戏窗口激活时才能操作人物 
        DRAW_GRAPH();
        FlushBatchDraw();  //执行批量绘制 
        level_over_flag = LEVEL_OVER();
        if(back_menu_flag == IDYES)
        {
        	RECOVER_LEVEL();
        	back_menu_flag = 0;
        	exit_menu_flag = 0;
        	break;
		}
        if(level_over_flag == 1)
		{
			RECOVER_LEVEL();
            NEXT_LEVEL();
        }
        else if(level_over_flag == 2)
		{
			RECOVER_LEVEL();
			exit_menu_flag = 0;
			break;
		} 
        Sleep(16);//控制帧率，约60FPS 
    }
}
void CLEAN_UP()
{
    EndBatchDraw();//结束批量绘制 
    CLOSE_BGM();
    
    //关闭音效 
    mciSendString("close sound_box", NULL, 0, NULL);
    mciSendString("close sound_complete", NULL, 0, NULL);
    
    closegraph();
}
//----------上面的代码是游戏循环模块(其中包括控制模块)----------


int main()
{
	fp = fopen("gamesave/data.txt","r");
	if(fp == NULL)//如果打开文件失败，创建文件 
	{
		fp = fopen("gamesave/data.txt","a+"); 
	    if(fp == NULL)//创建文件失败 
	    {
		    exit(1);
	    }
	    for(int i=0;i<10;i++)  fputc(pass[i], fp);
	    if(fclose(fp))//创建文件成功，不能关闭文件 
	    {
		    exit(1);
	    }
	}
	else//打开文件成功，从文件中读取数据 
	{
		for(int i=0;i<10;i++)  pass[i] = fgetc(fp);
		if(fclose(fp))//不能关闭文件 
		{
			exit(1);
		}
	}
	
	INIT_BGM("music/BGM.mp3");
    SET_BGM_VOLUME(90);
    LOAD_RESOURCE();
	hwnd = initgraph(64 * WIDTH, 64 * HEIGHT);
    BeginBatchDraw();
    
    while(exit_game_flag == IDNO)
    {
        while(!exit_menu_flag)
	    {
            DRAW_MENU_UI();
            FlushBatchDraw();

            MOUSEMSG m;
            while(MouseHit())
		    {
                m = GetMouseMsg();
                if (m.uMsg == WM_LBUTTONDOWN)
			    {
                    int result = CHOICE(m.x, m.y);
                    if     (result == 1)  {if(pass[0]=='1'||
					                          pass[0]=='2')
					                       {LOAD_LEVEL(1);
										    exit_menu_flag = 1;}}
                    else if(result == 2)  {if(pass[1]=='1'||
					                        pass[1]=='2')
					                       {LOAD_LEVEL(2);
										    exit_menu_flag = 1;}}
                    else if(result == 3)  {if(pass[2]=='1'||
					                          pass[2]=='2')
					                       {LOAD_LEVEL(3);
										    exit_menu_flag = 1;}}
                    else if(result == 4)  {if(pass[3]=='1'||
					                          pass[3]=='2')
					                       {LOAD_LEVEL(4);
										    exit_menu_flag = 1;}}
                    else if(result == 5)  {if(pass[4]=='1'||
					                          pass[4]=='2')
					                       {LOAD_LEVEL(5);
										    exit_menu_flag = 1;}}
                    else if(result == 6)  {if(pass[5]=='1'||
					                          pass[5]=='2')
					                       {LOAD_LEVEL(6);
										    exit_menu_flag = 1;}}
                    else if(result == 7)  {if(pass[6]=='1'||
					                          pass[6]=='2')
					                       {LOAD_LEVEL(7);
										    exit_menu_flag = 1;}}
                    else if(result == 8)  {if(pass[7]=='1'||
					                        pass[7]=='2')
					                       {LOAD_LEVEL(8);
										    exit_menu_flag = 1;}}
                    else if(result == 9)  {if(pass[8]=='1'||
					                          pass[8]=='2')
					                       {LOAD_LEVEL(9);
										    exit_menu_flag = 1;}}
                    else if(result == 10) {if(pass[9]=='1'||
					                          pass[9]=='2')
					                       {LOAD_LEVEL(10);
										    exit_menu_flag = 1;}}
                    else if(result == 11) {restart_flag =
					                       MessageBox(hwnd,"确认重置游戏？",
										   "提示",MB_YESNO);
					                       if(restart_flag == IDYES)
										       RE_START();}
					else if(result == 12) {exit_game_flag =
					                       MessageBox(hwnd,"确认退出游戏？",
										   "提示",MB_YESNO);
					                       if(exit_game_flag == IDYES)
										       exit_menu_flag = 1;}
                }
            }
            //在主菜单界面按"M"打开音乐设置界面 
		    isWindowActive = (GetForegroundWindow() == hwnd);
            if(isWindowActive)  MENU_KEY_DOWN();
            
            Sleep(10);
        }
        if(exit_game_flag == IDNO)
        {
		    INIT_LINK();//初始化撤销链表 
            BeginBatchDraw();//开始批量绘制 
            GAME_LOOP();//游戏循环 
            cleardevice();
        }
    }
    CLEAN_UP();
    return 0;
}
