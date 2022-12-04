//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE+1][MAX_PLACENAME] =
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};


typedef struct ifs_ele {
	int ID;//번호
	int age;//나이
	int time;//감염 시점
	int place_t[N_HISTORY];//감염 직전 이동경로 place_t 배열 (N_HISTORY) 
}ifs_ele_t;

int function {
	ifs_ele_t ifs1 = {0, 36, 10, [11, 19, 22, 0, 39]}; //0번 환자 정보 
	ifs_ele_t ifs2 = {1, 51, 12, [2, 0, 1, 24, 26]}; //1번 환자 정보  
	ifs_ele_t ifs3 = {2, 27, 7, [0, 3, 15, 11, 32]}; //2번 환자 정보  
	ifs_ele_t ifs4 = {3, 23, 13, [0, 35, 16, 10, 5]}; //3번 환자 정보  
	ifs_ele_t ifs5 = {4, 44, 8, [12, 17, 11, 29, 0]}; //4번 환자 정보  
};

static ifs_ele_t ifsarray[20];
static int ifs_cnt;

void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]);{
	ifsarray[ifs_cnt].index = index;
	
	ifs_cnt++;
	
	return (void*)&ifsarray[index];
}

int ifctele_getAge(void* obj){
	ifs_ele_t strPtr = (ifs_ele_t *)obj;
	
	return ();
};

void ifsele_printElement(void*obj){
	ifs_ele_t *strPtr = (ifs_ele_t *)obg;
	
	//print elements
};













