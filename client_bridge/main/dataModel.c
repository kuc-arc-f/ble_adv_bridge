#include "dataModel.h"

//Struct
struct stParam{
	char adv_name[3+1];
	char val_1[6+1];
	char val_2[6+1];
	char val_3[6+1];
};
struct stParam mParam[3];
const int mMaxDat=3;

const int mOK_CODE =1;
const int mNG_CODE =0;

//
void dataModel_set_advName(int iNum, char *adv_name ){
	strcpy(mParam[iNum].adv_name , adv_name);
}

//
void dataModel_debug_printDat(){
	for(int i= 0; i< mMaxDat; i++){
//		printf("dat.i=%d, name=%s , value=%s \n", i , mParam[i].adv_name,  mParam[i].val_1 );
		printf("dat.i=%d, name=%s , value1=%s,  value2=%s , value3=%s\n", i , mParam[i].adv_name,  mParam[i].val_1, mParam[i].val_2, mParam[i].val_3 );
	}

}

//
void dataModel_init_proc(){
	printf("# init_proc \n");
//	strcpy(mParam[0].adv_name, adv_name1 );
//	strcpy(mParam[1].adv_name, adv_name2 );
//	strcpy(mParam[2].adv_name, adv_name3 );
}
//
void dataModel_set_datByAdvname(char *adv_name, char *value, int field ){
//	printf("val-1=%s\n" , mParam[0].val_1);
	for(int i=0; i< mMaxDat; i++){
		if(strcmp(adv_name, mParam[i].adv_name )== 0){
			if(field==1){
				strcpy(mParam[i].val_1 , value);				
			}else if(field==2){
				strcpy(mParam[i].val_2 , value);
			}else if(field==3){
				strcpy(mParam[i].val_3 , value);
			}
		}
	}
}
//
void dataModel_get_datByAdvname(char *adv_name, int field ,char *cValue){
	static char cRet[3+1];
	for(int i=0; i< mMaxDat; i++){
		if(strcmp(adv_name, mParam[i].adv_name )== 0){
			if(field==1){
//				strcpy(cRet , mParam[i].val_1);
				strcpy(cValue , mParam[i].val_1);
			}else if(field==2){
				strcpy(cValue , mParam[i].val_2);
			}else if(field==3){
				strcpy(cValue , mParam[i].val_3);
			}
		}
	}
}


/*
char *dataModel_get_datByAdvname(char *adv_name, int field ){
	static char cRet[3+1];
	for(int i=0; i< mMaxDat; i++){
		if(strcmp(adv_name, mParam[i].adv_name )== 0){
			if(field==1){
				strcpy(cRet , mParam[i].val_1);
			}else if(field==2){
				strcpy(cRet , mParam[i].val_1);
			}else if(field==3){
				strcpy(cRet , mParam[i].val_1);
			}
		}
	}
	return cRet;
}
*/
int dataModel_recvCount(){
	int ret= mNG_CODE;
	int iCount=0;
	int iCount2=0;
	for(int j=0; j< mMaxDat; j++){
		if(strlen( mParam[j].adv_name ) > 0 ){
			iCount=iCount +1;
		}
	}
	printf("iCount=%d\n",iCount );
	if(iCount < 1){
		return ret;
	}
	for(int i=0; i< iCount; i++){
//		if(strlen( mParam[i].val_1 ) < 1 ){
		if( (strlen( mParam[i].adv_name ) > 0 ) && (strlen( mParam[i].val_1 ) > 0 ) ){
			iCount2= iCount2+1;
			// return ret;
		}
	}
	printf("iCount2=%d\n", iCount2 );
	if(iCount2 > 0){ ret= mOK_CODE;}
	return ret;
}

//
int dataModel_isComplete(){
	int ret= mNG_CODE;
	int iCount=0;
	for(int j=0; j< mMaxDat; j++){
		if(strlen( mParam[j].adv_name ) > 0 ){
			iCount=iCount +1;
		}
	}
	printf("iCount=%d\n",iCount );
//	if(iCount < mMaxDat ){
	if(iCount < 1){
		return ret;
	}
	
	for(int i=0; i< iCount; i++){
//		if(strlen( mParam[i].val_1 ) < 1 ){
		if( (strlen( mParam[i].adv_name ) > 0 ) && (strlen( mParam[i].val_1 ) < 1) ){
			return ret;
		}
	}
	ret =mOK_CODE;
	return ret;
}
//
void dataModel_clear(){
	for(int i=0; i< mMaxDat; i++){
		mParam[i].val_1[0]= '\0';
		mParam[i].val_2[0]= '\0';
		mParam[i].val_3[0]= '\0';
	}
}


