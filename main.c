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
    while(3 == (fscanf(fp,"%i %i %i", &index, &age, &detected_time))) //3�� �б� 
    {
    	int i;
    	for(i=0;i<5;i++){
			fscanf(fp, "%i", &history_place[i]);					//5���� ��ġ �б� 
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
        	int scan_index; 					//�Է¹��� ȯ�� ��ȣ 
    		char scan_place[MAX_PLACENAME]; 	//�Է¹��� ��� �̸�							 
   	 		int min_age;						//�Է¹��� �ּ� ����
			int max_age; 						//�Է¹��� �ִ� ����
			int root_patient;					//���� �����ڸ� ã�� ���� �Է¹��� ȯ�� ��ȣ
			int p_index;						//���� ȯ�� 
			int spreader;						//������
			int initial_spreader;				//���� ������ 
        	
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT: 									//1�� ����->������ ȯ���� ���� ��� 
            
				printf("Select a patient : "); 					//ȯ�� ���� 
				scanf("%i", &scan_index);
				printf("\n");
				
				if (0 <= scan_index && scan_index <= ifctdb_len()-1){
					ifct_element = ifctdb_getData(scan_index);
					ifctele_printElement(ifct_element);			//������ ȯ�ڿ� ���� ���� ��� 
				}
				else
					printf("[ERROR] You should enter between 0~%i\n", ifctdb_len()-1);		//db ���� ȯ�� ��ȣ�� �Է��ϸ� ���� ��� 
				                                                 
                break;
                
            case MENU_PLACE:									//2�� ����->������ ��ҿ��� ������ Ȯ�ε� ȯ�� ��� 
            	
            	printf("Select a location : ");					//���� ���� 
            	scanf("%s", &scan_place);
            	printf("\n");
            	
            	
            	for(j=0;j<ifctdb_len();j++){					//��� ȯ�� �����͸� �ϳ��� ���� 
                	ifct_element = ifctdb_getData(j);
                	
					if (strcmp(scan_place, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1))) == 0){	//ȯ���� ������ ��ҿ� �Է¹��� ���ڿ��� ������ ��� 
					ifctele_printElement(ifct_element);
					}
					
				}
                
                break;
                
            case MENU_AGE:										//3�� ����->Ư�� ���� ������ ȯ�� ���� ���  
                
                printf("Enter a minimum age : ");
                scanf("%i", &min_age);							//���� �ּڰ� �Է¹��� 
                printf("\nEnter a maximum age : ");
                scanf("%i", &max_age);							//���� �ִ� �Է¹��� 
                printf("\n");
                
                for(i=0;i<ifctdb_len();i++){					//���� �ȿ� ����� ��� ȯ�� �����͸� ���� 
                	ifct_element = ifctdb_getData(i);
					if(min_age <= ifctele_getAge(ifct_element) && ifctele_getAge(ifct_element) <= max_age){
						ifctele_printElement(ifct_element);		//���� �ȿ� ��ٸ� ȯ�� ���� ��� 
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
	int spreader;	//������ 
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
		
		//for(j=2;j<N_HISTORY;j++) //�׳� ��¥ ��ġ�� ������ ����ϴ°� �ƴϰ� ������ų �� �ִ� ���� ��ġ�� ������ ���ؼ� ����ϵ��� �����ؾ� �� 
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
	int index = -1; 		//�ʱⰪ -1
	
	if (time <= infested_time && time > infested_time - N_HISTORY){
		index = N_HISTORY-(infested_time - time) - 1;			//�Է¹��� �������� ��� �迭�� index ��ȯ 
	} 
	
	return index;
}


















