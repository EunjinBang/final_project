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
	int index;							//번호
	int age;							//나이
	int detected_time;					//감염 시점
	place_t history_place[N_HISTORY];	//감염 직전 이동경로 place_t 배열 (N_HISTORY) 
}ifs_ele_t;


void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]){
	int i;
	
	ifs_ele_t*strPtr;
	
	strPtr = (ifs_ele_t*)malloc(sizeof(ifs_ele_t));	//동적 메모리 하나 잡기 

	strPtr->index = index;
	strPtr->age = age;
	strPtr->detected_time = detected_time;
	
	for(i=0;i<N_HISTORY;i++){
		strPtr->history_place[i] = history_place[i];
	}
	
	//free 넣으면 안됨 
	return strPtr;
}

int ifctele_getAge(void* obj){
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	
	return (strPtr->age);
}


char* ifctele_getPlaceName(int placeIndex){
	return countryName[placeIndex];
}


int ifctele_getHistPlaceIndex(void* obj, int index){
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	
	return (strPtr->history_place[index]);
}

unsigned int ifctele_getinfestedTime(void* obj){

}


void ifctele_printElement(void*obj){
	int i;
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	
	printf("number : %i\n", strPtr->index);
	printf("age : %i\n", strPtr->age);
	printf("infected time : %i\n", strPtr->detected_time);
	printf("place : " );
	//for문으로 장소 출력하게 만들어야 함
	for(i=0;i<N_HISTORY;i++){
		printf("%i\t", strPtr->history_place[i]);	//숫자가 아닌 지역 이름이 영어로 나오도록 수정하기 
	}
}

//element.h 파일의 함수 순서에 맞게 수정하기 











