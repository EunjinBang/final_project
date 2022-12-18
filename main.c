//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient_no, int *detected_time, int *place);
int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int index, age, detected_time;
    int history_place[N_HISTORY]; 
    
    int scan_index; 	//입력받은 환자 번호 
    int scan_place; 	//입력받은 지역 이름 
    int min_age;		//입력받은 최소 나이
	int max_age; 		//입력받은 최대 나이 
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    
    //1-2. loading each patient informations
    while(3 == (fscanf(fp,"%i %i %i", &index, &age, &detected_time))) //3개 읽기 
    {
    	int i;
    	for(i=0;i<5;i++){
			fscanf(fp, "%i", &history_place[i]);		//fscanf를 fscnaf라고 씀... 여기서 엄청 헤맸다 바보야~!!
		}
		
			ifct_element = ifctele_genElement(index, age, detected_time, history_place);
		
			ifctdb_addTail(ifct_element);
	}
	
    //1-3. FILE pointer close
    fclose(fp);
    
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT: 									//1번 선택->지정된 환자의 정보 출력 
            
				printf("Select a patient : "); 					//환자 선택 
				scanf("%i", &scan_index);
				printf("\n");
				
				if (0 <= scan_index && scan_index <= ifctdb_len()-1){
					ifct_element = ifctdb_getData(scan_index);
					ifctele_printElement(ifct_element);			//선택한 환자에 대한 정보 출력 
				}
				else
					printf("[ERROR] You should enter between 0~%i\n", ifctdb_len()-1);		//db 밖의 환자 번호를 입력하면 에러 출력 
				                                                 
                break;
                
            case MENU_PLACE:								//2번 선택->지정된 장소에서 발병된 환자 출력 
            	
            	printf("Select a location : ");				//지역 선택 
            	scanf("%s", &scan_place);
                
                break;
                
            case MENU_AGE:
                
                printf("Enter a minimum age : ");
                scanf("%i", &min_age);
                printf("\nEnter a maximum age : ");
                scanf("%i", &max_age);
                
				//ifct_element를  get_Age함수에 넣은 값이 min_age와 max_age의 사이일 때 printElement 실행 
                
                break;
                
            case MENU_TRACK:
                    
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}
