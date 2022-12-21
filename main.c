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
			int pre_spreader;					//이전 전파자 
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
				
            			spreader = trackInfester(p_index);
            			
            			if(0 <= spreader && spreader != p_index){
            				printf("Patient %i was insfected with patient %i\n", p_index, spreader);
            			}	
            			else if(0 <= spreader && spreader == p_index){
            				initial_spreader = spreader;
            			}	
            				
            				while(p_index != spreader){
            					pre_spreader = trackInfester(spreader);
            					if(0 <= pre_spreader && pre_spreader != spreader){
            						printf("Patient %i was insfected with patient %i\n", spreader, pre_spreader);
            						spreader = pre_spreader;
            						initial_spreader = pre_spreader;
								}
								else if(0 <= pre_spreader && pre_spreader == spreader){
									p_index = spreader;
									initial_spreader = pre_spreader;
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
	int spreader = -1;		//전파자 초기값 -1 
	int max;				//만난시간이 가장 이른지 알기위한 값 
	int met_time;			//만난 시간 
	void *ifct_element;
	
	ifct_element = ifctdb_getData(patient_no);
	max = ifctele_getinfestedTime(ifct_element);		//max 값을 발병 확인시간으로 정함 
	
	for(i=0;i<ifctdb_len();i++){
		if(patient_no != i){
			met_time = isMet(patient_no, i);			//isMet 함수(두 환자가 언제만났는지 리턴하는 함수)를 for 루프를 통해 모든 환자에 대해 확인 
			if(met_time > 0){							//한번이라도 만났다면 
				if(met_time <= max){					
					max = met_time;						//max값을 만난시간으로 바꿔서 지금까지 만난 환자 중 가장 이른 시점인지 확인 
					spreader = i;						//전파자를 i번째 환자로 저장 
				}
			}
		} 
	}
	
	if(spreader == -1){
		spreader = patient_no;
	}
	
	return spreader;
}

int isMet(int patient_no, int entered_patient){
	int i;
	int p_move_time;
	int p_move_place;
	int t_move_time;
	int t_move_place;
	int met_time = -1;
	void *ifct_element;
	
	for(i=2;i<N_HISTORY;i++){						//현재 환자의 0,1,2일쨰 날짜(감염될 수 있는 날짜)와 비교
		ifct_element = ifctdb_getData(patient_no);
		p_move_time = (ifctele_getinfestedTime(ifct_element) - i);				//현재 환자의 0일째, 1일째, 2일째의 시점 
		p_move_place = ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1-i);	//그 시점의 장소 인덱스 
		 
		ifct_element = ifctdb_getData(entered_patient);
		t_move_time = convertTimetoIndex(p_move_time, ifctele_getinfestedTime(ifct_element));	//대상 환자의 위 시점에서의 날짜 인덱스 저장, 위 시점의 인덱스가 없다면 -1이 저장됨 
		
		if(t_move_time >= N_HISTORY - 2){																	//
			t_move_place = ifctele_getHistPlaceIndex(ifct_element, t_move_time);				//대상 환자의 위 날짜 인덱스에서의 장소 인덱스 저장 
		
			if(p_move_place == t_move_place){	//현재 환자와 대상 환자의 장소 인덱스가 같다면 
				met_time = p_move_time;			//결과값에 시점 저장
			}
		}
	}
	return met_time;
}

int convertTimetoIndex(int time, int infested_time){
	int index = -1; 		//초기값 -1
	
	if (time <= infested_time && time > infested_time - N_HISTORY){
		index = N_HISTORY-(infested_time - time) - 1;			//입력받은 시점으로 시간 index 변환 
	} 
	
	return index;
}


/*
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
			}/*
			else
				spreader = patient_no;
			
		} 
	}
	return spreader;
}


int isMet(int patient_no, int entered_patient){
	int i;
	int j;
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
		
		ifct_element = ifctdb_getData(entered_patient);
		if(p_move_time <= ifctele_getinfestedTime(ifct_element) && (ifctele_getinfestedTime(ifct_element)-1) <= p_move_time){ 
			t_move_time = convertTimetoIndex(p_move_time, ifctele_getinfestedTime(ifct_element));
			t_move_place = ifctele_getHistPlaceIndex(ifct_element, t_move_time);
		
			if(p_move_place == t_move_place){
				met_time = p_move_time;
			}
			
			else{
				met_time = t_move_time;
			}
		}
		/*
		else
			met_time = 0;
		
	}
	return met_time;
}

int convertTimetoIndex(int time, int infested_time){
	int index = -1; 		//초기값 -1
	
	if (time <= infested_time && time > infested_time - N_HISTORY){
		index = N_HISTORY-(infested_time - time) - 1;			//입력받은 시점으로 장소 배열의 index 변환 
	} 
	
	return index;
}
*/

















