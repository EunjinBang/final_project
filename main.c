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


//int trackInfester(int patient_no, int *detected_time, int *place);
int trackInfester(int patient_no);
int isMet(int patient_no, int entered_patient);
int convertTimetoIndex(int time, int infested_time);

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int index, age, detected_time;
    int history_place[N_HISTORY]; 
    
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
			fscanf(fp, "%i", &history_place[i]);					//5가지 위치 읽기 
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
        	int j;
			int i;
        	int scan_index; 					//입력받은 환자 번호 
    		char scan_place[MAX_PLACENAME]; 	//입력받은 장소 이름							 
   	 		int min_age;						//입력받은 최소 나이
			int max_age; 						//입력받은 최대 나이
			int root_patient;					//최초 전파자를 찾기 위해 입력받은 환자 번호
			int p_index;						//현재 환자 
			int spreader;						//전파자
			int initial_spreader;				//최초 전파자 
        	
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
                
            case MENU_PLACE:									//2번 선택->지정된 장소에서 감염이 확인된 환자 출력 
            	
            	printf("Select a location : ");					//지역 선택 
            	scanf("%s", &scan_place);
            	printf("\n");
            	
            	
            	for(j=0;j<ifctdb_len();j++){					//모든 환자 데이터를 하나씩 점검 
                	ifct_element = ifctdb_getData(j);
                	
					if (strcmp(scan_place, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1))) == 0){	//환자의 마지막 장소와 입력받은 문자열이 같으면 출력 
					ifctele_printElement(ifct_element);
					}
					
				}
                
                break;
                
            case MENU_AGE:										//3번 선택->특정 나이 범위의 환자 정보 출력  
                
                printf("Enter a minimum age : ");
                scanf("%i", &min_age);							//나이 최솟값 입력받음 
                printf("\nEnter a maximum age : ");
                scanf("%i", &max_age);							//나이 최댓값 입력받음 
                printf("\n");
                
                for(i=0;i<ifctdb_len();i++){					//범위 안에 드는지 모든 환자 데이터를 점검 
                	ifct_element = ifctdb_getData(i);
					if(min_age <= ifctele_getAge(ifct_element) && ifctele_getAge(ifct_element) <= max_age){
						ifctele_printElement(ifct_element);		//범위 안에 든다면 환자 정보 출력 
					}
				}
				 
                break;
                
            case MENU_TRACK:
            	
            	printf("Select a patient : ");
            	scanf("%i", &root_patient);
            	p_index = root_patient;
            	
            	if (0 <= p_index && p_index <= ifctdb_len()-1){
				
            		while(0 <= p_index){
            			spreader = trackInfester(p_index);
            			if(0 <= spreader){
            				printf("Patient %i was insfected with patient %i\n", p_index, spreader);
            				initial_spreader = spreader;
            				break;
						}
					
						else{
							initial_spreader = p_index;
							p_index = spreader;
						}
					}
					printf("The first spreader is number %i patient.", initial_spreader);
				}
				else
					printf("[ERROR] You should enter between 0~%i\n", ifctdb_len()-1);
                    
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}


int trackInfester(int patient_no){
	int i;
	int spreader;	//전파자 
	int max;
	int met_time;
	void *ifct_element;
	
	ifct_element = ifctdb_getData(patient_no);
	max = ifctele_getinfestedTime(ifct_element);
	
	for(i=0;i<ifctdb_len();i++){
		if(patient_no != i){
			met_time = isMet(patient_no, i);
			if(met_time > 0){
				if(met_time <= max){
					max = met_time;
					spreader = i;
				}
			}
		} 
	}
	return spreader;
}

int isMet(int patient_no, int entered_patient){
	int i;
	int p_move_time;
	int p_move_place;
	int t_move_time;
	int t_move_place;
	int met_time;
	void *ifct_element;
	
	ifct_element = ifctdb_getData(patient_no);
	for(i=2;i<N_HISTORY;i++){
		p_move_time = (ifctele_getinfestedTime(ifct_element) - i);
		p_move_place = ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1-i);
		
		//for(j=2;j<N_HISTORY;j++) //그냥 날짜 위치가 같으면 출력하는게 아니고 감염시킬 수 있는 날에 위치가 같은지 비교해서 출력하도록 수정해야 함 
		ifct_element = ifctdb_getData(entered_patient);
		t_move_time = convertTimetoIndex(p_move_time, ifctele_getinfestedTime(ifct_element));
		t_move_place = ifctele_getHistPlaceIndex(ifct_element, t_move_time);
		
		if(p_move_place == t_move_place){
				met_time = p_move_time;
			}
			
		else{
			met_time = t_move_time;
		}
		
	return met_time;
	}
}

int convertTimetoIndex(int time, int infested_time){
	int index = -1; 		//초기값 -1
	
	if (time <= infested_time && time > infested_time - N_HISTORY){
		index = N_HISTORY-(infested_time - time) - 1;			//입력받은 시점으로 장소 배열의 index 변환 
	} 
	
	return index;
}


















