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


int trackInfester(int patient_no);							//�����ڸ� ã�� �Լ� 
int isMet(int patient_no, int entered_patient);				//�����ڿ� ���� ������ reutrn�ϴ� �Լ� 
int convertTimetoIndex(int time, int infested_time);		//������ ��¥ index�� ��ȯ�ϴ� �Լ�  

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
    while(3 == (fscanf(fp,"%i %i %i", &index, &age, &detected_time))) //�ε���, ����, �ð� �б� 
    {
    	int i;
    	for(i=0;i<5;i++){
			fscanf(fp, "%i", &history_place[i]);					//5���� ��ġ �б� 
		}
		
			ifct_element = ifctele_genElement(index, age, detected_time, history_place);
		
			ifctdb_addTail(ifct_element);		//�� ���� ������ ���� 
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
			int pre_spreader;					//���� ������ 
			int initial_spreader;				//���� ������ 
			char* met_place_name;				//�� ȯ�ڰ� ������ ��� �̸� 
			int met_infectedtime;				//�� ȯ�ڰ� ������ ������ ���� 
        	
            case MENU_EXIT:										//0�� ����->���α׷� ����  
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
                
            case MENU_TRACK:									//4�� ����->�����ڸ� ã�� ���� ����, ��Ҹ� ����ϰ� ���������� ���� �����ڸ� ���  
            	
            	printf("Select a patient : ");					//�����ڸ� ã�� ȯ�ڸ� �Է¹���  
            	scanf("%i", &root_patient);
            	p_index = root_patient;
            	
            	if (0 <= p_index && p_index <= ifctdb_len()-1){	//db���� ȯ�ڸ� �Է��ϸ�  
				
            			spreader = trackInfester(p_index);		//trackInfester �Լ��� ���� ���� ȯ���� �����ڸ� ã��  
            			
            			if(spreader != p_index){			//�����ڰ� �����Ѵٸ� (������ ���� �����ڰ� �ƴ϶��)  
            				ifct_element = ifctdb_getData(spreader);
            				met_infectedtime = isMet(p_index, spreader);		//���� ���� ���� ����  
            				met_place_name = ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, convertTimetoIndex(met_infectedtime, ifctele_getinfestedTime(ifct_element))));	//���� ���� ��� ����  
            				printf("Patient %i was insfected with patient %i. (met at %i, %s)\n", p_index, spreader, met_infectedtime, met_place_name);		//���� ȯ���� ������, ���� ����, ��� ���  
            			}	
            			else if(spreader == p_index){		//�����ڰ� �������� �ʴ´ٸ�(������ ���� �����ڶ��) 
            				initial_spreader = spreader;	//���� �����ڸ� ���� ȯ�ڷ� ����  
            			}	
            				
            			while(p_index != spreader){		//���� �����ڰ� ���ö����� �ݺ�  
            				pre_spreader = trackInfester(spreader);				//���� ������ ������� �ݺ�  
            				if(pre_spreader != spreader){
            					ifct_element = ifctdb_getData(pre_spreader);
            					met_infectedtime = isMet(spreader, pre_spreader);
            					met_place_name = ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, convertTimetoIndex(met_infectedtime, ifctele_getinfestedTime(ifct_element))));
            					printf("Patient %i was insfected with patient %i. (met at %i, %s)\n", spreader, pre_spreader, met_infectedtime, met_place_name);
            					spreader = pre_spreader;
            					initial_spreader = pre_spreader;
							}
							else if(pre_spreader == spreader){
								p_index = spreader;					//���� �����ڰ� ���Դٸ� p_index�� spreader���� �����ؼ� while���� ������������ ��  
								initial_spreader = pre_spreader;
							}
						}
						
					printf("The first spreader is number %i patient.\n", initial_spreader);		//���� ������ ���  
				}
				else
					printf("[ERROR] You should enter between 0~%i\n", ifctdb_len()-1);	//db���� ȯ�ڸ� �Է��ϸ� ���� ���  
                    
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}


int trackInfester(int patient_no){		//�����ڸ� ã�� �Լ�
	int i;
	int spreader = -1;		//������ �ʱⰪ -1 
	int max;				//�����ð��� ���� �̸��� �˱����� �� 
	int met_time;			//���� �ð� 
	void *ifct_element;
	
	ifct_element = ifctdb_getData(patient_no);
	max = ifctele_getinfestedTime(ifct_element);		//max ���� �ߺ� Ȯ�νð����� ���� 
	
	for(i=0;i<ifctdb_len();i++){
		if(patient_no != i){
			met_time = isMet(patient_no, i);			//isMet �Լ�(�� ȯ�ڰ� ������������ �����ϴ� �Լ�)�� for ������ ���� ��� ȯ�ڿ� ���� Ȯ�� 
			if(met_time > 0){							//�ѹ��̶� �����ٸ� 
				if(met_time <= max){					
					max = met_time;						//max���� �����ð����� �ٲ㼭 ���ݱ��� ���� ȯ�� �� ���� �̸� �������� Ȯ�� 
					spreader = i;						//�����ڸ� i��° ȯ�ڷ� ���� 
				}
			}
		} 
	}
	
	if(spreader == -1){									//�����ڰ� ���ٸ� 
		spreader = patient_no;							//�ڱ��ڽ��� �����ڷ� ����  
	}
	
	return spreader;
}

int isMet(int patient_no, int entered_patient){		//�����ڿ� ���� ������ reutrn�ϴ� �Լ�
	int i;
	int p_move_time;		//���� ȯ���� ����  
	int p_move_place;		//���� ȯ���� �ش� ������ ���  
	int t_move_time;		//��� ȯ���� �ð� �ε���  
	int t_move_place;		//��� ȯ���� �ش� �ð��� ���  
	int met_time = -1;		//���� ���� (�ʱⰪ -1) 
	void *ifct_element;
	
	for(i=2;i<N_HISTORY;i++){						//���� ȯ���� 0,1,2�Ϥ� ��¥(������ �� �ִ� ��¥)�� ��
		ifct_element = ifctdb_getData(patient_no);
		p_move_time = (ifctele_getinfestedTime(ifct_element) - i);				//���� ȯ���� 0��°, 1��°, 2��°�� ���� 
		p_move_place = ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1-i);	//�� ������ ��� �ε��� 
		 
		ifct_element = ifctdb_getData(entered_patient);
		t_move_time = convertTimetoIndex(p_move_time, ifctele_getinfestedTime(ifct_element));	//��� ȯ���� �� ���������� �ð� �ε��� ����, �� ������ �ε����� ���ٸ� -1�� ����� 
		
		if(t_move_time >= N_HISTORY - 2){														//�ð� �ε����� ��� ȯ���� ���� ���� �ñ⿡ �ش��Ѵٸ�  
			t_move_place = ifctele_getHistPlaceIndex(ifct_element, t_move_time);				//��� ȯ���� �� �ð� �ε��������� ��� �ε��� ���� 
		
			if(p_move_place == t_move_place){	//���� ȯ�ڿ� ��� ȯ���� ��� �ε����� ���ٸ� 
				met_time = p_move_time;			//������� ���� ����
			}
		}
	}
	return met_time;		//���� ���� ���(������ �ʾҴٸ� �ʱⰪ�� -1�� return) 
}

int convertTimetoIndex(int time, int infested_time){	//������ ��¥ index�� ��ȯ�ϴ� �Լ�
	int index = -1; 		//�ε��� �ʱⰪ -1
	
	if (time <= infested_time && time > infested_time - N_HISTORY){
		index = N_HISTORY-(infested_time - time) - 1;			//�Է¹��� ������ �ð� index�� ��ȯ 
	} 
	
	return index;
}

















