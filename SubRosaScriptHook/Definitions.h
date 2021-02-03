#pragma once

#define NUMBER_OF_PLAYERS_OFFSET     0XABC1A0
#define PLAYER_POSITION_ARRAY_OFFSET 0X6ACBAC
#define ITEM_ARRAY_OFFSET            0X91559c0
#define VEHICLE_ARRAY_OFFSET         0x8BDF2A0

#define INVISIGUN 0X0
#define AK47 0X1
#define AK47_MAGAZINE 0X2
#define M16 0X3
#define M16_MAGAZINE 0X4
#define OLD_UZI 0X5
#define SHELL 0X6
#define MP5 0X7
#define MP5_MAGAZINE 0X8
#define UZI 0X9
#define UZI_MAGAZINE 0XA
#define HANDGUN 0XB
#define HANDGUN_MAGAZINE 0XC
#define GRENADE 0XD
#define BANDAGE 0XE
#define SUITCASE_CLOSED 0XF
#define SUITCASE_OPEN 0X10
#define CASH 0X11
#define BLACK_BAR 0X12
#define DISK_BLACK 0X13
#define DISK_GREEN 0X14
#define DISK_BLUE 0X15
#define DISK_WHITE 0X16
#define DISK_YELLOW 0X17
#define DISK_RED 0X18
#define PHONE 0X19
#define WALKIE_TALKIE 0X1A
#define KEY 0X1B
#define BLACK_BOARD 0X1C
#define README 0X1D
#define HAMBURGER 0X1E
#define IDK1 0X1F
#define IDK2 0X20
#define PHONE_OLD 0X21
#define PAPER 0X22
#define SOCCERBALL 0X23
#define CUBE 0X25
#define BROWN_BAR 0X26
#define COMPUTER 0X27
#define COMPUTER_DESK 0X28
#define COMPUTER_DESK2 0X29
#define WHITEBOARD 0X2A
#define VASE 0X2B
#define WATERMELON 0X2C


struct vec3 {
    float x;
    float y;
    float z;
};
struct rotation {
    float x = 1.0f;
    float b = 0.0f;
    float c = 0.0f;
    float d = 0.0f;
    float y = 1.0f;
    float f = 0.0f;
    float g = 0.0f;
    float h = 0.0f;
    float z = 1.0f;
    float j = 0.0f;
};


typedef int(__cdecl *SpawnVehicle_p)(int, vec3*, vec3*, rotation*, int); //SpawnVehicle(int Type, vec3* Position, vec3* idk, vec3* rotation, int color)
SpawnVehicle_p SpawnVehicle;

typedef int(__cdecl* SpawnItem_p)(int, vec3*, vec3*, rotation*); //SpawnItem(int Type, vec3* Position, vec3* idk, vec3* rotation)
SpawnItem_p SpawnItem; //0xA39F0

typedef void(__fastcall* HookedLoopFunct_p)(); //Function that serves as our hook for the main thread, not for external use
HookedLoopFunct_p HookedLoopFunct; //0x63CB0